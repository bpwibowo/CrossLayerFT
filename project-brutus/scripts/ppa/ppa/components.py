import logging
import math

try:
    # included in standard lib from Python 2.7
    from collections import OrderedDict
    OrderedDict; # workaround for pyflakes issue #13
except ImportError:
    # try importing the backported drop-in replacement
    # it's available on PyPI
    from ordereddict import OrderedDict

import cacti
import power_db

# mm^2
NAND2_AREA = 8e-7

# nW
NAND2_LEAKAGE = 4.962

# power gated constants
POWER_GATED_LEAK_RATIO = 0.05
SVF_SW_ON_PENALTY = 100
SVF_SW_OFF_PENALTY = 100
ECC_SW_ON_PENALTY = 100
ECC_SW_OFF_PENALTY = 100

class Logic():
    def __init__(self, name, knobs):
        '''
        Constructor

        @param name: Name of the logic
        @param knobs: dict of knobs to use in the db query
        '''
        # name of the component. also used as the db name
        self.name               = name

        # copy the required knobs for the db query from knobs to self.knobs
        self.knobs = {}
        for k in power_db.required_knobs(name):
            if k not in knobs:
                logging.error('Required knob %s not set for %s' % (k, name))
                exit()

            self.knobs[k]       = knobs[k]

        # initialize simulation counters
        self.cycles             = 0
        self.cycle_time         = 0
        self.accesses           = 0

        # initialize SVF data
        self.is_svf             = False
        self.cycles_svf         = 0
        self.svf_sw_on          = 0
        self.svf_sw_off         = 0

        # initialize ECC data
        self.is_ecc             = False
        self.cycles_ecc         = 0
        self.ecc_sw_on          = 0
        self.ecc_sw_off         = 0

    def __str__(self):
        s = '\n'
        for (key, value) in self.__dict__.items():
            s += str(key) + ': ' + str(value) + '\n'
        return s

    def get_data(self):
        '''
        '''
        # the user should set tech before calling get_data
        try:
            self.knobs['tech'] = self.tech
        except:
            # use 45 nm as default
            self.knobs['tech'] = 45

        # query power_db for data
        data = power_db.find_one(db_name=self.name,
                                 fields=self.knobs)

        if data is None:
            print 'Error: %s(%s) not found in database %s.' % \
                        (self.name, str(self.knobs), self.name)
            exit()

        # get area data (mm^2) from power_db
        self.area               = float(data.get('area_mm2', 0))

        # get logic delay (ns) 
        self.delay              = float(data.get('delay_ns', 0))

        # get switching power (nW)
        self.switching_power_nw = float(data.get('switching_power_mw', 0) * 1e6)

        # convert mW to nW
        # Leakage power from NANGATE is unreliable. Instead use equation below
        self.leakage_power      = data.get('leakage_power_mw', 0) * 1e6
        # self.leakage_power      = self.area/NAND2_AREA * NAND2_LEAKAGE

    def calculate(self):
        if self.is_svf: logging.info("%s is an SVF structure active cycles = %d, %d, %d svf-on-ff %f %f ecc-on-off %f %f" % (self.name, self.cycles, self.cycles_svf, self.cycles_ecc, self.svf_sw_on, self.svf_sw_off, self.ecc_sw_on, self.ecc_sw_off))
        # calculate total energy counts (nJ)
        self.energy_per_access  = self.switching_power_nw * (self.delay / 1e9)
        self.switching_energy   = self.accesses * self.energy_per_access
        self.leakage_energy     = self.leakage_power * self.cycles * (self.cycle_time / 1e9)
        self.total_energy       = self.switching_energy + self.leakage_energy

        # handle power gating effect
        if self.is_svf:
            active_cycle = float(self.cycles_svf)
            active_cycle += self.svf_sw_on * SVF_SW_ON_PENALTY
            active_cycle += self.svf_sw_off * SVF_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

        if self.is_ecc:
            active_cycle = float(self.cycles_ecc)
            active_cycle += self.ecc_sw_on * ECC_SW_ON_PENALTY
            active_cycle += self.ecc_sw_off * ECC_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

    def dump(self, indent, verbosity=0):
        '''
        print the logic's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area, )
        print pre + 'delay: %.3f ns' % (self.delay,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)

        if verbosity >= 1:
            print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
            print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)
            print pre + 'switching_power: %.3f nW' % (self.switching_power_nw,)
            print pre + 'leakage_power: %.3f nW' % (self.leakage_power,)

        if verbosity >= 2:
            print pre + 'accesses: %d' % (self.accesses,)
            print pre + 'energy_per_access: %.3f nJ' % (self.energy_per_access,)

        print ''


class SRAM():
    def __init__(self, ram, name, words, width, read_ports, write_ports):
        '''
        Constructor

        @param name: Name of the SRAM
        @param words: Number of rows
        @param width: Word width
        @param read_ports: Number of read ports
        @param write_ports: Number of write ports
        '''
        self.name                   = name

        # True for RAM, False for CAM
        self.ram                    = ram

        # all SRAM and CAM data is stored in the same db, unlike logic
        if ram:
            self.db_name            = 'SRAM'
        else:
            self.db_name            = 'CAM'

        # keep the knobs for the db query
        self.knobs                  = {}
        self.knobs['words']         = words
        self.knobs['width']         = width
        self.knobs['read_ports']    = read_ports
        self.knobs['write_ports']   = write_ports

        # simulation counters
        self.cycles             = 0
        self.cycle_time         = 0
        self.read_accesses      = 0
        self.write_accesses     = 0

        # initialize SVF data
        self.is_svf             = False
        self.cycles_svf         = 0
        self.svf_sw_on          = 0
        self.svf_sw_off         = 0

        # initialize ECC data
        self.is_ecc             = False
        self.cycles_ecc         = 0
        self.ecc_sw_on          = 0
        self.ecc_sw_off         = 0

    def __str__(self):
        s = '\n'
        for (key, value) in self.__dict__.items():
            s += str(key) + ': ' + str(value) + '\n'
        return s

    def get_data(self):
        '''
        '''
        import fabmem

        # the user should set tech before calling get_data
        try:
            self.knobs['tech'] = self.tech
        except:
            # use 45 nm as default
            self.knobs['tech'] = 45

        # query power_db for data
        data = power_db.find_one(db_name=self.db_name,
                                 fields=self.knobs)

        # if the query returned nothing, call FabMem
        if data is None:
            if self.ram:
                logging.debug('Calling FabMem for SRAM \'%s\'' % (self.name))
                data = fabmem.generate_ram(words=self.words,
                                           width=self.width,
                                           read_ports=self.read_ports,
                                           write_ports=self.write_ports)
            else:
                logging.debug('Calling FabMem for CAM \'%s\'' % (self.name))
                data = fabmem.generate_cam(words=self.words,
                                           width=self.width,
                                           read_ports=self.read_ports,
                                           write_ports=self.write_ports)

            # add the data to the SRAM or CAM db to avoid calling FabMem for this ram again
            power_db.insert(db_name=self.db_name,
                            # FabMem doesn't report leakage so calculate based on area
                            leakage_power_mw=(float(data['area_mm2'])/NAND2_AREA * (NAND2_LEAKAGE/1e6)),
                            # Add all knobs as part of the row
                            **dict(self.knobs.items() + data.items()))

        # get area data from power_db
        self.area               = float(data.get('area_mm2', 0))

        # get timing data 
        self.delay              = float(data.get('delay_ns', 0))

        # get energy data
        self.energy_per_read    = float(data.get('energy_per_read_nj', 0))
        self.energy_per_write   = float(data.get('energy_per_write_nj', 0))

        # convert mW to nW
        # Leakage power from NANGATE is unreliable. Instead use equation below
        # self.leakage_power = data.get('leakage', 0) * 1e6
        self.leakage_power      = self.area/NAND2_AREA * NAND2_LEAKAGE

    def calculate(self):
        if self.is_svf: logging.info("%s is an SVF structure active cycles = %d, %d, %d svf-on-ff %f %f ecc-on-off %f %f" % (self.name, self.cycles, self.cycles_svf, self.cycles_ecc, self.svf_sw_on, self.svf_sw_off, self.ecc_sw_on, self.ecc_sw_off))
        # calculate total energy counts (nJ)
        self.read_energy        = self.read_accesses * self.energy_per_read
        self.write_energy       = self.write_accesses * self.energy_per_write
        self.switching_energy   = self.read_energy + self.write_energy
        self.leakage_energy     = self.leakage_power * self.cycles * (self.cycle_time / 1e9)
        self.total_energy       = self.switching_energy + self.leakage_energy

        # handle power gating effect
        if self.is_svf:
            active_cycle = float(self.cycles_svf)
            active_cycle += self.svf_sw_on * SVF_SW_ON_PENALTY
            active_cycle += self.svf_sw_off * SVF_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

        if self.is_ecc:
            active_cycle = float(self.cycles_ecc)
            active_cycle += self.ecc_sw_on * ECC_SW_ON_PENALTY
            active_cycle += self.ecc_sw_off * ECC_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

    def dump(self, indent, verbosity=0):
        '''
        print the SRAM's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area, )
        print pre + 'delay: %.3f ns' % (self.delay,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)

        if verbosity >= 1:
            print pre + 'read_energy: %.3f nJ' % (self.read_energy,)
            print pre + 'write_energy: %.3f nJ' % (self.write_energy,)
            print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
            print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)

        if verbosity >= 2:
            print pre + 'read_accesses: %d' % (self.read_accesses,)
            print pre + 'energy_per_read: %.3f nJ' % (self.energy_per_read,)
            print pre + 'write_accesses: %d' % (self.write_accesses,)
            print pre + 'energy_per_write: %.3f nJ' % (self.energy_per_write,)
            print pre + 'leakage_power: %.3f nW' % (self.leakage_power,)

        print ''


class CactiSRAM():
    def __init__(self, name):
        '''
        Constructor

        @param name: Name of the SRAM
        '''
        self.name               = name

        # knobs
        self.words              = 0
        self.width              = 8
        self.ways               = 1
        self.block_size         = 1
        self.tag_width          = 'default'
        self.rw_ports           = 0
        self.rd_ports           = 0
        self.wr_ports           = 0
        self.cell_type          = 'itrs-hp'
        self.opt_metric         = 'ED'
        self.tech               = 45

        # simulation counters
        self.cycles             = 0
        self.cycle_time         = 0
        self.read_accesses      = 0
        self.write_accesses     = 0

        # initialize SVF data
        self.is_svf             = False
        self.cycles_svf         = 0
        self.svf_sw_on          = 0
        self.svf_sw_off         = 0

        # initialize ECC data
        self.is_ecc             = False
        self.cycles_ecc         = 0
        self.ecc_sw_on          = 0
        self.ecc_sw_off         = 0

    def __str__(self):
        s = '\n'
        for (key, value) in self.__dict__.items():
            s += str(key) + ': ' + str(value) + '\n'
        return s

    def get_data(self):
        ''' 
        calculates the area, delay and energy using the knobs, simulation counters and Cacti.
        '''
        from math import ceil

        # check for required parameters
        if not self.words:
            logging.error('\'words\' must be defined for %s' % (self.name,))
            exit()

        # create an interface to Cacti
        c = cacti.Cacti()

        # convert width (bits) to block_size (bytes)
        self.block_size = int(ceil(self.width / 8.0))

        # copy all knobs set by the user, overriding defaults
        c.__dict__.update(self.__dict__)

        # set cacti parameters
        c.size      = self.words * self.block_size
        c.cache_type = 'ram'

        c.data_array_cell_type        = self.cell_type
        c.data_array_peripheral_type  = self.cell_type
        c.tag_array_cell_type         = self.cell_type
        c.tag_array_peripheral_type   = self.cell_type

        # call cacti
        data = c.get_data()

        # get area data from cacti
        self.area = data['general']['cache_area']

        # get timing data from cacti
        self.delay = data['general']['access_time']

        # get energy data from cacti
        self.energy_per_read    = data['general']['dyn_read_access_energy']
        self.energy_per_write   = data['general']['dyn_write_access_energy']
        self.leakage_power      = data['general']['leakage_power_per_bank'] * c.banks * 1e6

    def calculate(self):
        if self.is_svf: logging.info("%s is an SVF structure active cycles = %d, %d, %d svf-on-ff %f %f ecc-on-off %f %f" % (self.name, self.cycles, self.cycles_svf, self.cycles_ecc, self.svf_sw_on, self.svf_sw_off, self.ecc_sw_on, self.ecc_sw_off))
        # calculate total energy counts
        self.read_energy        = self.read_accesses * self.energy_per_read
        self.write_energy       = self.write_accesses * self.energy_per_write
        self.switching_energy   = self.read_energy + self.write_energy
        self.leakage_energy     = self.leakage_power * self.cycles * (self.cycle_time / 1e9)
        self.total_energy       = self.switching_energy + self.leakage_energy

        # handle power gating effect
        if self.is_svf:
            active_cycle = float(self.cycles_svf)
            active_cycle += self.svf_sw_on * SVF_SW_ON_PENALTY
            active_cycle += self.svf_sw_off * SVF_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

        if self.is_ecc:
            active_cycle = float(self.cycles_ecc)
            active_cycle += self.ecc_sw_on * ECC_SW_ON_PENALTY
            active_cycle += self.ecc_sw_off * ECC_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy


    def dump(self, indent, verbosity=0):
        '''
        print the SRAM's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area, )
        print pre + 'delay: %.3f ns' % (self.delay,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)

        if verbosity >= 1:
            print pre + 'read_energy: %.3f nJ' % (self.read_energy,)
            print pre + 'write_energy: %.3f nJ' % (self.write_energy,)
            print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
            print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)

        if verbosity >= 2:
            print pre + 'read_accesses: %d' % (self.read_accesses,)
            print pre + 'energy_per_read: %.3f nJ' % (self.energy_per_read,)
            print pre + 'write_accesses: %d' % (self.write_accesses,)
            print pre + 'energy_per_write: %.3f nJ' % (self.energy_per_write,)
            print pre + 'leakage_power: %.3f nW' % (self.leakage_power,)

        print ''


class Cache():
    def __init__(self, name):
        '''
        Constructor

        @param name: Name of the cache
        '''
        self.name               = name

        # required knobs
        self.sets               = 0
        self.block_size         = 1
        self.cell_type          = 'itrs-hp'
        self.tech               = 45

        # see cacti.py for a list of all knobs and default values

        # simulation counters
        self.cycles             = 0
        self.cycle_time         = 0
        self.read_accesses      = 0
        self.write_accesses     = 0

        # initialize SVF data
        self.is_svf             = False
        self.cycles_svf         = 0
        self.svf_sw_on          = 0
        self.svf_sw_off         = 0

        # initialize ECC data
        self.is_ecc             = False
        self.cycles_ecc         = 0
        self.ecc_sw_on          = 0
        self.ecc_sw_off         = 0

    def __str__(self):
        s = '\n'
        for (key, value) in self.__dict__.items():
            s += str(key) + ': ' + str(value) + '\n'
        return s

    def get_data(self):
        ''' 
        calculates the area, delay and energy using the knobs, simulation counters and Cacti.
        '''

        # check for required args
        if not self.sets:
            logging.error('\'sets\' must be defined for \'%s\'' % (self.name,))
            exit()

        # create an interface to Cacti
        c = cacti.Cacti()

        # copy all knobs set by the user
        for (key, value) in self.__dict__.items():
            c.__dict__[key] = value

        # overwrite other cacti parameters
        c.size       = self.sets * self.block_size

        c.tag_width  = 32 - math.log(self.block_size, 2) - math.log(self.ways, 2)
        c.cache_type = 'cache'

        c.data_array_cell_type        = self.cell_type
        c.data_array_peripheral_type  = self.cell_type
        c.tag_array_cell_type         = self.cell_type
        c.tag_array_peripheral_type   = self.cell_type

        # call cacti
        data = c.get_data()

        # get area data from cacti
        self.area = data['general']['cache_area']

        # get timing data from cacti
        self.delay = data['general']['access_time']

        # get energy data from cacti
        self.energy_per_read    = data['general']['dyn_read_access_energy']
        self.energy_per_write   = data['general']['dyn_write_access_energy']
        self.leakage_power      = data['general']['leakage_power_per_bank'] * c.banks * 1e6

    def calculate(self):
        if self.is_svf: logging.info("%s is an SVF structure active cycles = %d, %d, %d svf-on-ff %f %f ecc-on-off %f %f" % (self.name, self.cycles, self.cycles_svf, self.cycles_ecc, self.svf_sw_on, self.svf_sw_off, self.ecc_sw_on, self.ecc_sw_off))
        # calculate total energy counts
        self.read_energy        = self.read_accesses * self.energy_per_read
        self.write_energy       = self.write_accesses * self.energy_per_write
        self.switching_energy   = self.read_energy + self.write_energy
        self.leakage_energy     = self.leakage_power * self.cycles * (self.cycle_time / 1e9)
        self.total_energy       = self.switching_energy + self.leakage_energy

        # handle power gating effect
        if self.is_svf:
            active_cycle = float(self.cycles_svf)
            active_cycle += self.svf_sw_on * SVF_SW_ON_PENALTY
            active_cycle += self.svf_sw_off * SVF_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

        if self.is_ecc:
            active_cycle = float(self.cycles_ecc)
            active_cycle += self.ecc_sw_on * ECC_SW_ON_PENALTY
            active_cycle += self.ecc_sw_off * ECC_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

    def dump(self, indent, verbosity=0):
        '''
        print the SRAM's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area, )
        print pre + 'delay: %.3f ns' % (self.delay,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)

        if verbosity >= 1:
            print pre + 'read_energy: %.3f nJ' % (self.read_energy,)
            print pre + 'write_energy: %.3f nJ' % (self.write_energy,)
            print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
            print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)

        if verbosity >= 2:
            print pre + 'read_accesses: %d' % (self.read_accesses,)
            print pre + 'energy_per_read: %.3f nJ' % (self.energy_per_read,)
            print pre + 'write_accesses: %d' % (self.write_accesses,)
            print pre + 'energy_per_write: %.3f nJ' % (self.energy_per_write,)
            print pre + 'leakage_power: %.3f nW' % (self.leakage_power,)

        print ''


class Custom():
    def __init__(self, name, knobs):
        '''
        Constructor

        @param name: Name of the logic
        @param knobs: dict of knobs to use in the db query
        '''
        # name of the component. also used as the db name
        self.name               = name

        # initialize simulation counters
        self.cycles             = 0
        self.cycle_time         = 0
        self.accesses           = 0

        # initialize SVF data
        self.is_svf             = False
        self.cycles_svf         = 0
        self.svf_sw_on          = 0
        self.svf_sw_off         = 0

        # initialize ECC data
        self.is_ecc             = False
        self.cycles_ecc         = 0
        self.ecc_sw_on          = 0
        self.ecc_sw_off         = 0

        # number of units, for area and static power computation
        self.count              = 1

        # default type is an empty string
        self.subtype            = knobs["subtype"]

    def __str__(self):
        s = '\n'
        for (key, value) in self.__dict__.items():
            s += str(key) + ': ' + str(value) + '\n'
        return s

    def get_data(self):
        '''
        '''
        db = {
            ### SVF logics
            # "addshift8x10_instbuff" : {"area_mm2" : 0.00051205, "delay_ns" : 0, "energy_per_access" : 0.0001799684, "leakage_power_mw" : 0.0205},
            # "addshift8x10_iq" : {"area_mm2" : 0.000550886, "delay_ns" : 0, "energy_per_access" : 0.000166538, "leakage_power_mw" : 0.0195},
            # "addshift8x10_lq" : {"area_mm2" : 0.00054929, "delay_ns" : 0, "energy_per_access" : 0.0011744186, "leakage_power_mw" : 0.0192},
            # "addshift8x10_rob" : {"area_mm2" : 0.00054929, "delay_ns" : 0, "energy_per_access" : 0.0001611658, "leakage_power_mw" : 0.0194},
            # "addshift8x10_sq" : {"area_mm2" : 0.000549822, "delay_ns" : 0, "energy_per_access" : 0.0017881669, "leakage_power_mw" : 0.0192},
            # "vlt_instbuff" : {"area_mm2" : 0.000217588, "delay_ns" : 0, "energy_per_access" : 0.0001077796, "leakage_power_mw" : 0.0117},
            # "vlt_iq" : {"area_mm2" : 0.000208544, "delay_ns" : 0, "energy_per_access" : 9.70352214350469E-005, "leakage_power_mw" : 0.0108},
            # "vlt_lq" : {"area_mm2" : 0.000083524, "delay_ns" : 0, "energy_per_access" : 0.000504104, "leakage_power_mw" : 0.00353},
            # "vlt_rob" : {"area_mm2" : 0.000146832, "delay_ns" : 0, "energy_per_access" : 8.89769331497835E-005, "leakage_power_mw" : 0.00698},
            # "vlt_sq" : {"area_mm2" : 0.000076342, "delay_ns" : 0, "energy_per_access" : 0.0007083994, "leakage_power_mw" : 0.00314},
            # "spu" : {"area_mm2" : 0.001026228, "delay_ns" : 0, "energy_per_access" : 0.0001063, "leakage_power_mw" : 0.0352},
            # "counter_instbuff" : {"area_mm2" : 0.000078736, "delay_ns" : 0, "energy_per_access" : 0.000002743, "leakage_power_mw" : 0.00272},
            # "counter_iq" : {"area_mm2" : 0.0000798, "delay_ns" : 0, "energy_per_access" : 0.000004227, "leakage_power_mw" : 0.00272},
            # "counter_lq" : {"area_mm2" : 0.0000798, "delay_ns" : 0, "energy_per_access" : 0.000004227, "leakage_power_mw" : 0.00272},
            # "counter_rob" : {"area_mm2" : 0.0000798, "delay_ns" : 0, "energy_per_access" : 0.000004227, "leakage_power_mw" : 0.00272},
            # "counter_sq" : {"area_mm2" : 0.0000798, "delay_ns" : 0, "energy_per_access" : 0.000004227, "leakage_power_mw" : 0.00272},
            ### ECC logics
            # "ecc_checker_10" : {"area_mm2" : 0.00185242390, "delay_ns" : 0, "energy_per_access" : 0.00366815504, "leakage_power_mw" : 0.07440000},
            # "ecc_checker_3" : {"area_mm2" : 0.00000239400, "delay_ns" : 0, "energy_per_access" : 0.00000754898, "leakage_power_mw" : 0.00016900},
            # "ecc_checker_4" : {"area_mm2" : 0.00001117200, "delay_ns" : 0, "energy_per_access" : 0.00002015257, "leakage_power_mw" : 0.00043900},
            # "ecc_checker_5" : {"area_mm2" : 0.00003990000, "delay_ns" : 0, "energy_per_access" : 0.00007853430, "leakage_power_mw" : 0.00170000},
            # "ecc_checker_6" : {"area_mm2" : 0.00009256800, "delay_ns" : 0, "energy_per_access" : 0.00018130061, "leakage_power_mw" : 0.00382000},
            # "ecc_checker_7" : {"area_mm2" : 0.00020854400, "delay_ns" : 0, "energy_per_access" : 0.00041237334, "leakage_power_mw" : 0.00865000},
            # "ecc_checker_8" : {"area_mm2" : 0.00043890000, "delay_ns" : 0, "energy_per_access" : 0.00087126293, "leakage_power_mw" : 0.01790000},
            # "ecc_checker_9" : {"area_mm2" : 0.00090493200, "delay_ns" : 0, "energy_per_access" : 0.00177982096, "leakage_power_mw" : 0.03670000},
            # "ecc_corrector_10" : {"area_mm2" : 0.00658802200, "delay_ns" : 0, "energy_per_access" : 0.00602818081, "leakage_power_mw" : 0.41000000},
            # "ecc_corrector_3" : {"area_mm2" : 0.00001064000, "delay_ns" : 0, "energy_per_access" : 0.00000295784, "leakage_power_mw" : 0.00044300},
            # "ecc_corrector_4" : {"area_mm2" : 0.00004495400, "delay_ns" : 0, "energy_per_access" : 0.00004268158, "leakage_power_mw" : 0.00192000},
            # "ecc_corrector_5" : {"area_mm2" : 0.00013459600, "delay_ns" : 0, "energy_per_access" : 0.00016615071, "leakage_power_mw" : 0.00630000},
            # "ecc_corrector_6" : {"area_mm2" : 0.00028940800, "delay_ns" : 0, "energy_per_access" : 0.00029438497, "leakage_power_mw" : 0.01300000},
            # "ecc_corrector_7" : {"area_mm2" : 0.00064957200, "delay_ns" : 0, "energy_per_access" : 0.00072651158, "leakage_power_mw" : 0.02810000},
            # "ecc_corrector_8" : {"area_mm2" : 0.00148162000, "delay_ns" : 0, "energy_per_access" : 0.00143071961, "leakage_power_mw" : 0.08250000},
            # "ecc_corrector_9" : {"area_mm2" : 0.00314837600, "delay_ns" : 0, "energy_per_access" : 0.00296029779, "leakage_power_mw" : 0.19100000},
            # "ecc_checker_10" : {"area_mm2" : 0.0018524239, "delay_ns" : 0, "energy_per_access" : 0.00133932636235673, "leakage_power_mw" : 0.0744},
            "ecc_checker_3" : {"area_mm2" : 0.000002394, "delay_ns" : 0, "energy_per_access" : 0.00000192484066155708, "leakage_power_mw" : 0.000169},
            "ecc_checker_4" : {"area_mm2" : 0.000011172, "delay_ns" : 0, "energy_per_access" : 0.00000602272278225806, "leakage_power_mw" : 0.000439},
            "ecc_checker_5" : {"area_mm2" : 0.0000399, "delay_ns" : 0, "energy_per_access" : 0.0000294503627569528, "leakage_power_mw" : 0.0017},
            "ecc_checker_6" : {"area_mm2" : 0.000092568, "delay_ns" : 0, "energy_per_access" : 0.0000671743303121853, "leakage_power_mw" : 0.00382},
            "ecc_checker_7" : {"area_mm2" : 0.000208544, "delay_ns" : 0, "energy_per_access" : 0.00015113683044704, "leakage_power_mw" : 0.00865},
            "ecc_checker_8" : {"area_mm2" : 0.0004389, "delay_ns" : 0, "energy_per_access" : 0.000319096297041658, "leakage_power_mw" : 0.0179},
            "ecc_checker_9" : {"area_mm2" : 0.000904932, "delay_ns" : 0, "energy_per_access" : 0.000649934620800644, "leakage_power_mw" : 0.0367},
            "ecc_corrector_10" : {"area_mm2" : 0.006588022, "delay_ns" : 0, "energy_per_access" : 0.00225931818181818, "leakage_power_mw" : 0.41},
            "ecc_corrector_3" : {"area_mm2" : 0.00001064, "delay_ns" : 0, "energy_per_access" : 0.000000121280600927606, "leakage_power_mw" : 0.000443},
            "ecc_corrector_4" : {"area_mm2" : 0.000044954, "delay_ns" : 0, "energy_per_access" : 0.0000155296815158234, "leakage_power_mw" : 0.00192},
            "ecc_corrector_5" : {"area_mm2" : 0.000134596, "delay_ns" : 0, "energy_per_access" : 0.0000559844751158573, "leakage_power_mw" : 0.0063},
            "ecc_corrector_6" : {"area_mm2" : 0.000289408, "delay_ns" : 0, "energy_per_access" : 0.000103134871071716, "leakage_power_mw" : 0.013},
            "ecc_corrector_7" : {"area_mm2" : 0.000649572, "delay_ns" : 0, "energy_per_access" : 0.000271190960338232, "leakage_power_mw" : 0.0281},
            "ecc_corrector_8" : {"area_mm2" : 0.00148162, "delay_ns" : 0, "energy_per_access" : 0.000502252616747182, "leakage_power_mw" : 0.0825},
            "ecc_corrector_9" : {"area_mm2" : 0.003148376, "delay_ns" : 0, "energy_per_access" : 0.00103010362173038, "leakage_power_mw" : 0.191},

            ### AVFCL logics
            "counter64" : {"area_mm2" : 0.000072086, "delay_ns" : 0, "energy_per_access" : 0.000003, "leakage_power_mw" : 0.00362},
            "counter65k" : {"area_mm2" : 0.000072086, "delay_ns" : 0, "energy_per_access" : 0.000000106666666666667, "leakage_power_mw" : 0.005133},
            "vlt_lq_dp" : {"area_mm2" : 0.000004256, "delay_ns" : 0, "energy_per_access" : 0, "leakage_power_mw" : 0.000416},
            "counter256" : {"area_mm2" : 0.000072086, "delay_ns" : 0, "energy_per_access" : 0.00000303333333333333, "leakage_power_mw" : 0.00437},
            # "metadata_decode" : {"area_mm2" : 0.008694742, "delay_ns" : 0, "energy_per_access" : 0.00101316666666667, "leakage_power_mw" : 0.47935},
            # "metadata_decode" : {"area_mm2" : 0.0630188582, "delay_ns" : 0, "energy_per_access" : 0.00545333333333334, "leakage_power_mw" : 2.711},
            # "metadata_decode" : {"area_mm2" : 0.0060437859, "delay_ns" : 0, "energy_per_access" : 0.0006568, "leakage_power_mw" : 0.20232},
            "metadata_decode" : {"area_mm2" : 0.0140924139, "delay_ns" : 0, "energy_per_access" : 0.00119366666666667, "leakage_power_mw" : 0.6903},
            "vlt" : {"area_mm2" : 0.000141512, "delay_ns" : 0, "energy_per_access" : 0.0000536, "leakage_power_mw" : 0.00867},
            "avfcl_acc_sq" : {"area_mm2" : 0.001022238, "delay_ns" : 0, "energy_per_access" : 0.000756586666666667, "leakage_power_mw" : 0.033808},
            "avfcl_acc_ib" : {"area_mm2" : 0.00203091, "delay_ns" : 0, "energy_per_access" : 0.000572468333333334, "leakage_power_mw" : 0.0843865},
            "ib_dur" : {"area_mm2" : 0.000427994, "delay_ns" : 0, "energy_per_access" : 0.0000772229333333333, "leakage_power_mw" : 0.01660104},
            "rob_dur" : {"area_mm2" : 0.000800926, "delay_ns" : 0, "energy_per_access" : 0.0000909666666666667, "leakage_power_mw" : 0.0311},
            "avfcl_acc_iq" : {"area_mm2" : 0.003216472, "delay_ns" : 0, "energy_per_access" : 0.002288705, "leakage_power_mw" : 0.1391295},
            "nop_lane" : {"area_mm2" : 0.000179816, "delay_ns" : 0, "energy_per_access" : 0.00000082, "leakage_power_mw" : 0.00666},
            "vlt_lq" : {"area_mm2" : 0.000024738, "delay_ns" : 0, "energy_per_access" : 0.0000241333333333333, "leakage_power_mw" : 0.00134},
            "vlt_sq_dp" : {"area_mm2" : 0.000004256, "delay_ns" : 0, "energy_per_access" : 0, "leakage_power_mw" : 0.000391},
            "sq_dur" : {"area_mm2" : 0.000462042, "delay_ns" : 0, "energy_per_access" : 0.0000591, "leakage_power_mw" : 0.019},
            "avfcl_acc_prf" : {"area_mm2" : 0.00023009, "delay_ns" : 0, "energy_per_access" : 0.0000197, "leakage_power_mw" : 0.01145},
            "lq_dur" : {"area_mm2" : 0.000462042, "delay_ns" : 0, "energy_per_access" : 0.0000591666666666667, "leakage_power_mw" : 0.019},
            "vlt_sq" : {"area_mm2" : 0.00001862, "delay_ns" : 0, "energy_per_access" : 0.000014, "leakage_power_mw" : 0.000938},
            "iq_dur" : {"area_mm2" : 0.000487046, "delay_ns" : 0, "energy_per_access" : 0.0000740333333333333, "leakage_power_mw" : 0.0198},
            "avfcl_acc_rob" : {"area_mm2" : 0.004735332, "delay_ns" : 0, "energy_per_access" : 0.003436725, "leakage_power_mw" : 0.2053275},
            "avfcl_acc_lq" : {"area_mm2" : 0.001022238, "delay_ns" : 0, "energy_per_access" : 0.000756586666666667, "leakage_power_mw" : 0.033808},
            "vop_decoder" : {"area_mm2" : 0.000334096, "delay_ns" : 0, "energy_per_access" : 0.000144666666666667, "leakage_power_mw" : 0.017},

            #### parities
            "parity_WIDTH64" : {"area_mm2" : 0.000100548, "delay_ns" : 0, "energy_per_access" : 0.0000533666666666667, "leakage_power_mw" : 0.00414},
            "parity_WIDTH6" : {"area_mm2" : 0.00000798, "delay_ns" : 0, "energy_per_access" : 0.00000385333333333333, "leakage_power_mw" : 0.000335},
            "parity_WIDTH8" : {"area_mm2" : 0.000011172, "delay_ns" : 0, "energy_per_access" : 0.00000556666666666667, "leakage_power_mw" : 0.000478},
            "parity_WIDTH190" : {"area_mm2" : 0.000301644, "delay_ns" : 0, "energy_per_access" : 0.000160666666666667, "leakage_power_mw" : 0.0124},
            "parity_WIDTH197" : {"area_mm2" : 0.000312816, "delay_ns" : 0, "energy_per_access" : 0.000166666666666667, "leakage_power_mw" : 0.0128},
            "parity_WIDTH76" : {"area_mm2" : 0.0001197, "delay_ns" : 0, "energy_per_access" : 0.0000634666666666667, "leakage_power_mw" : 0.00493},
            "parity_WIDTH82" : {"area_mm2" : 0.000129276, "delay_ns" : 0, "energy_per_access" : 0.0000688, "leakage_power_mw" : 0.00532},
        }

        if self.subtype not in db:
            logging.error("ERROR! custom subtype '%s' is not supported." % self.subtype)
        data = db[self.subtype]

        # get area data (mm^2) from power_db
        self.area               = float(data.get('area_mm2', 0) * self.count)
        logging.info("custom st '%s' count = %d" % (self.name, self.count))

        # get logic delay (ns) 
        self.delay              = float(data.get('delay_ns', 0))

        # get energy per access (nJ)
        self.energy_per_access  = float(data.get('energy_per_access', 0))

        # convert mW to nW
        # Leakage power from NANGATE is unreliable. Instead use equation below
        self.leakage_power      = data.get('leakage_power_mw', 0) * 1e6
        # self.leakage_power      = self.area/NAND2_AREA * NAND2_LEAKAGE

    def calculate(self):
        if self.is_svf: logging.info("%s is an SVF structure active cycles = %d, %d, %d svf-on-ff %f %f ecc-on-off %f %f" % (self.name, self.cycles, self.cycles_svf, self.cycles_ecc, self.svf_sw_on, self.svf_sw_off, self.ecc_sw_on, self.ecc_sw_off))
        # calculate total energy counts (nJ)
        self.switching_energy   = self.accesses * self.energy_per_access
        self.leakage_energy     = self.leakage_power * self.cycles * (self.cycle_time / 1e9) * self.count
        self.total_energy       = self.switching_energy + self.leakage_energy

        # handle power gating effect
        if self.is_svf:
            active_cycle = float(self.cycles_svf)
            active_cycle += self.svf_sw_on * SVF_SW_ON_PENALTY
            active_cycle += self.svf_sw_off * SVF_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

        if self.is_ecc:
            active_cycle = float(self.cycles_ecc)
            active_cycle += self.ecc_sw_on * ECC_SW_ON_PENALTY
            active_cycle += self.ecc_sw_off * ECC_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy

    def dump(self, indent, verbosity=0):
        '''
        print the logic's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area, )
        print pre + 'delay: %.3f ns' % (self.delay,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)

        if verbosity >= 1:
            print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
            print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)
            print pre + 'leakage_power: %.3f nW' % (self.leakage_power,)

        if verbosity >= 2:
            print pre + 'accesses: %d' % (self.accesses,)
            print pre + 'energy_per_access: %.3f nJ' % (self.energy_per_access,)

        print ''


class CustomRAM():
    def __init__(self, name, knobs):
        '''
        Constructor

        @param name: Name of the RAM
        @param knobs: dict of knobs to use in the db query
        '''
        # name of the component. also used as the db name
        self.name               = name

        # initialize simulation counters
        self.cycles             = 0
        self.cycle_time         = 0
        self.read_accesses      = 0
        self.write_accesses     = 0

        # initialize SVF data
        self.is_svf             = False
        self.cycles_svf         = 0
        self.svf_sw_on          = 0
        self.svf_sw_off         = 0

        # initialize ECC data
        self.is_ecc             = False
        self.cycles_ecc         = 0
        self.ecc_sw_on          = 0
        self.ecc_sw_off         = 0

        # number of units, for area and static power computation
        self.count              = 1

        # default type is an empty string
        self.subtype            = knobs["subtype"]

    def __str__(self):
        s = '\n'
        for (key, value) in self.__dict__.items():
            s += str(key) + ': ' + str(value) + '\n'
        return s

    def get_data(self):
        '''
        '''
        db = {
            "STQ_DATA_RAMMem" : { "energy_per_read": 0.00298353909465021, "energy_per_write": 0.00447154471544716, "internal_energy_per_cycle": 0.0189, "leakage_power_mw": 1.3, "area_mm2": 0.0341687634, "delay_ns": 0 },
            "ActiveListMem" : { "energy_per_read": 0.0123259412068076, "energy_per_write": 0.0181726907630522, "internal_energy_per_cycle": 0.102, "leakage_power_mw": 10.7, "area_mm2": 0.2310920191, "delay_ns": 0 },
            "ActiveListWbMem" : { "energy_per_read": 0.011397627643115, "energy_per_write": 0.0224181360201511, "internal_energy_per_cycle": 0.109, "leakage_power_mw": 13.5, "area_mm2": 0.272625527, "delay_ns": 0 },
            "ActiveListAvfClWbMem" : { "energy_per_read": 0.00018824136152656, "energy_per_write": 0.000574307304785894, "internal_energy_per_cycle": 0.00195, "leakage_power_mw": 0.25, "area_mm2": 0.005470822, "delay_ns": 0 },
            "StqImportanceBitMem" : { "energy_per_read": 0.0000716049382716049, "energy_per_write": 0.000133333333333333, "internal_energy_per_cycle": 0.000265, "leakage_power_mw": 0.0265, "area_mm2": 0.000583338, "delay_ns": 0 },
            "ActiveListAvfClMem" : { "energy_per_read": 0.000639504899432697, "energy_per_write": 0.00229919678714859, "internal_energy_per_cycle": 0.00603, "leakage_power_mw": 0.666, "area_mm2": 0.0134000158, "delay_ns": 0 },
            "AMTMem" : { "energy_per_read": 0.000165033522434244, "energy_per_write": 0.000649874055415617, "internal_energy_per_cycle": 0.00162, "leakage_power_mw": 0.208, "area_mm2": 0.004025644, "delay_ns": 0 },
            "RegFileMem" : { "energy_per_read": 0.00894123606889564, "energy_per_write": 0.0191435768261965, "internal_energy_per_cycle": 0.106, "leakage_power_mw": 15, "area_mm2": 0.4976873203, "delay_ns": 0 },
            "RASMem" : { "energy_per_read": 0.000606995884773663, "energy_per_write": 0.00132520325203252, "internal_energy_per_cycle": 0.00384, "leakage_power_mw": 0.282, "area_mm2": 0.46191112, "delay_ns": 0 },
            "CTIQMem" : { "energy_per_read": 0.000820987654320988, "energy_per_write": 0.0013150406504065, "internal_energy_per_cycle": 0.0042, "leakage_power_mw": 0.285, "area_mm2": 0.0072328059, "delay_ns": 0 },
            "BTBMem" : { "energy_per_read": 0.0334677419354839, "energy_per_write": 0.0485772357723577, "internal_energy_per_cycle": 0.433, "leakage_power_mw": 23.1, "area_mm2": 0.0272894718, "delay_ns": 0 },
            "RMTMem" : { "energy_per_read": 0.00016761217122228, "energy_per_write": 0.000711847389558233, "internal_energy_per_cycle": 0.00144, "leakage_power_mw": 0.165, "area_mm2": 0.0274655637, "delay_ns": 0 },
            "PayloadRAMMem" : { "energy_per_read": 0.0075296544610624, "energy_per_write": 0.01214859437751, "internal_energy_per_cycle": 0.0566, "leakage_power_mw": 5.88, "area_mm2": 0.1380406982, "delay_ns": 0 },
            "InstBuffMem" : { "energy_per_read": 0.00233870967741936, "energy_per_write": 0.0055919395465995, "internal_energy_per_cycle": 0.0185, "leakage_power_mw": 1.73, "area_mm2": 0.0075631779, "delay_ns": 0 },
            "BPMem" : { "energy_per_read": 0.00110493827160494, "energy_per_write": 0.00177032520325203, "internal_energy_per_cycle": 0.0134, "leakage_power_mw": 0.715, "area_mm2": 0.003312764, "delay_ns": 0 },
            "FreelistMem" : { "energy_per_read": 0.000160907684373388, "energy_per_write": 0.00368261964735516, "internal_energy_per_cycle": 0.00886, "leakage_power_mw": 1.33, "area_mm2": 0.2962942072, "delay_ns": 0 },
            "ActiveListWbParityMem" : { "energy_per_read": 0.000132542547705003, "energy_per_write": 0.000534005037783375, "internal_energy_per_cycle": 0.00205, "leakage_power_mw": 0.246, "area_mm2": 0.005626964, "delay_ns": 0 },
            "ActiveListParityMem" : { "energy_per_read": 0.000193914388860237, "energy_per_write": 0.000520080321285141, "internal_energy_per_cycle": 0.00159, "leakage_power_mw": 0.19, "area_mm2": 0.003818164, "delay_ns": 0 },
            "STQ_DATA_RAMParityMem" : { "energy_per_read": 0.0000588477366255144, "energy_per_write": 0.00012520325203252, "internal_energy_per_cycle": 0.000285, "leakage_power_mw": 0.0274, "area_mm2": 0.000624302, "delay_ns": 0 },
            "RegFileParityMem" : { "energy_per_read": 0.000176038500506586, "energy_per_write": 0.000523929471032746, "internal_energy_per_cycle": 0.00211, "leakage_power_mw": 0.282, "area_mm2": 0.006563018, "delay_ns": 0 },
            "PayloadRAMParityMem" : { "energy_per_read": 0.000074780814853017, "energy_per_write": 0.000179718875502008, "internal_energy_per_cycle": 0.000443, "leakage_power_mw": 0.0514, "area_mm2": 0.001009736, "delay_ns": 0 },
            "InstBuffParityMem" : { "energy_per_read": 0.0000179435483870968, "energy_per_write": 0.0000942065491183879, "internal_energy_per_cycle": 0.000178, "leakage_power_mw": 0.0196, "area_mm2": 0.000400596, "delay_ns": 0 },
            "ActiveListWbEccMem" : { "energy_per_read": 0.0012429087158329, "energy_per_write": 0.00400503778337532, "internal_energy_per_cycle": 0.0123, "leakage_power_mw": 1.62, "area_mm2": 0.0263073996, "delay_ns": 0 },
            "InstBuffEccMem" : { "energy_per_read": 0.0000634347601856627, "energy_per_write": 0.000579317269076305, "internal_energy_per_cycle": 0.000921, "leakage_power_mw": 0.0897, "area_mm2": 0.0318564257, "delay_ns": 0 },
            "RegFileEccMem" : { "energy_per_read": 0.00944444444444445, "energy_per_write": 0.016219512195122, "internal_energy_per_cycle": 0.0141, "leakage_power_mw": 2.08, "area_mm2": 0.00188461, "delay_ns": 0 },
            "ActiveListEccMem" : { "energy_per_read": 0.000605369807497467, "energy_per_write": 0.00215617128463476, "internal_energy_per_cycle": 0.0121, "leakage_power_mw": 1.29, "area_mm2": 0.0072213679, "delay_ns": 0 },
            "STQ_DATA_RAMEccMem" : { "energy_per_read": 0.0000995358432181537, "energy_per_write": 0.00025, "internal_energy_per_cycle": 0.00202, "leakage_power_mw": 0.14, "area_mm2": 0.0400925837, "delay_ns": 0 },
            "PayloadRAMEccMem" : { "energy_per_read": 0.000821572580645161, "energy_per_write": 0.000614609571788413, "internal_energy_per_cycle": 0.00314, "leakage_power_mw": 0.326, "area_mm2": 0.0036856959, "delay_ns": 0 },
            }
        if self.subtype not in db:
            logging.error("ERROR! custom RAM type '%s' is not supported." % self.type)
        data = db[self.subtype]

        # get area data (mm^2) from power_db
        self.area               = float(data.get('area_mm2', 0) * self.count)

        # get logic delay (ns) 
        self.delay              = float(data.get('delay_ns', 0))

        # convert mW to nW
        # Leakage power from NANGATE is unreliable. Instead use equation below
        self.leakage_power      = data.get('leakage_power_mw', 0) * 1e6
        # self.leakage_power      = self.area/NAND2_AREA * NAND2_LEAKAGE
        self.internal_energy_per_cycle = data.get('internal_energy_per_cycle', 0)
        self.energy_per_read    = data.get('energy_per_read', 0)
        self.energy_per_write   = data.get('energy_per_write', 0)

    def calculate(self):
        if self.is_svf: logging.info("%s is an SVF structure active cycles = %d, %d, %d svf-on-ff %f %f ecc-on-off %f %f" % (self.name, self.cycles, self.cycles_svf, self.cycles_ecc, self.svf_sw_on, self.svf_sw_off, self.ecc_sw_on, self.ecc_sw_off))
        # calculate total energy counts (nJ)
        self.read_energy        = self.read_accesses * self.energy_per_read
        self.write_energy       = self.write_accesses * self.energy_per_write
        self.switching_energy   = self.read_energy + self.write_energy
        # self.internal_energy    = self.internal_energy_per_cycle * self.cycles
        self.leakage_energy     = self.leakage_power * self.cycles * (self.cycle_time / 1e9) * self.count
        self.total_energy       = self.switching_energy + self.leakage_energy #+ self.internal_energy

        # handle power gating effect
        if self.is_svf:
            active_cycle = float(self.cycles_svf)
            active_cycle += self.svf_sw_on * SVF_SW_ON_PENALTY
            active_cycle += self.svf_sw_off * SVF_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy
            # internal_energy = POWER_GATED_LEAK_RATIO * self.internal_energy
            # internal_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles)
            # self.internal_energy = internal_energy

        if self.is_ecc:
            active_cycle = float(self.cycles_ecc)
            active_cycle += self.ecc_sw_on * ECC_SW_ON_PENALTY
            active_cycle += self.ecc_sw_off * ECC_SW_OFF_PENALTY
            leakage_energy = POWER_GATED_LEAK_RATIO * self.leakage_energy
            leakage_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles) * self.leakage_energy
            self.leakage_energy = leakage_energy
            # internal_energy = POWER_GATED_LEAK_RATIO * self.internal_energy
            # internal_energy += (1-POWER_GATED_LEAK_RATIO) * (active_cycle/self.cycles)
            # self.internal_energy = internal_energy

    def dump(self, indent, verbosity=0):
        '''
        print the logic's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area, )
        print pre + 'delay: %.3f ns' % (self.delay,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)

        if verbosity >= 1:
            print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
            print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)
            print pre + 'leakage_power: %.3f nW' % (self.leakage_power,)
            # print pre + 'internal_energy: %.3f nJ' % (self.internal_energy,)

        if verbosity >= 2:
            print pre + 'read_accesses: %d' % (self.read_accesses,)
            print pre + 'write_accesses: %d' % (self.write_accesses,)
            print pre + 'energy_per_read: %.3f nJ' % (self.energy_per_read,)
            print pre + 'energy_per_write: %.3f nJ' % (self.energy_per_write,)
            # print pre + 'internal_energy_per_cycle: %.3f nJ' % (self.internal_energy_per_cycle,)

        print ''

class Unit():
    def __init__(self, name):
        '''
        Constructor

        @param name: name of the stage
        '''
        self.name       = name

        # dict of subunits comprising this unit
        self.subunits   = OrderedDict()
        
        self.cycles     = 0
        # ns
        self.cycle_time = 0
        # nm
        self.tech       = 45

        self.cycles_svf = 0
        self.svf_sw_on  = 0
        self.svf_sw_off = 0
        self.cycles_ecc = 0
        self.ecc_sw_on  = 0
        self.ecc_sw_off = 0

    def __getitem__(self, key):
        return self.__dict__[key]

    def __setitem__(self, key, value):
        self.__dict__[key] = value

    def add_subunit(self, subunit):
        '''
        Add a sub-unit to this unit

        @param subunit: subunit to add
        '''
        self.subunits[subunit.name] = subunit

    def add_subunits(self, subunits):
        '''
        Add multiple sub-units to this unit

        @param subunits: subunits to add
        '''
        for subunit in subunits:
            self.subunits[subunit.name] = subunit

    def get_data(self):
        # mm^2
        self.area              = 0

        # set common values then call calculate() for each subunit
        for c in self.subunits.values():
            c.tech       = self.tech
            c.get_data()

            # area is accumulated over all subunits
            self.area          += c.area

    def calculate(self):
        # nJ
        self.switching_energy  = 0
        # nJ
        self.leakage_energy    = 0
        # nJ
        self.total_energy      = 0

        # set common values then call calculate() for each subunit
        for c in self.subunits.values():
            c.cycles     = self.cycles
            c.cycles_svf = self.cycles_svf
            c.svf_sw_on  = self.svf_sw_on
            c.svf_sw_off = self.svf_sw_off
            c.cycles_ecc = self.cycles_ecc
            c.ecc_sw_on  = self.ecc_sw_on
            c.ecc_sw_off = self.ecc_sw_off
            c.cycle_time = self.cycle_time
            c.calculate()

            # energy is accumulated over all subunits
            self.switching_energy     += c.switching_energy
            self.leakage_energy       += c.leakage_energy

        # calculate        self.leakage_energy       += c.leakage_energy

        # calculate total energy (nJ)
        self.total_energy = self.switching_energy + self.leakage_energy

    def dump(self, indent, verbosity=0):
        '''
        print the unit's data then each subunit's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area,)
        print pre + 'cycle_time: %.3f ns' % (self.cycle_time,)
        print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
        print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)
        print ''

        for c in self.subunits.values():
            c.dump(indent=indent, verbosity=verbosity)


class Stage(Unit):
    def __init__(self, name):
        '''
        Constructor

        @param name: name of the stage
        '''
        Unit.__init__(self, name)

        self.delay_paths = []

    def get_data(self):
        Unit.get_data(self)

        # ns
        self.max_delay  = 0
        path_num        = 0

        # find the critical path
        for (i, path) in enumerate(self.delay_paths):
            tmp_delay = 0
            # sum the delay of each component in the path
            for c in path:
                tmp_delay += self.subunits[c].delay

            # save the delay if it's greater than the current max_delay
            if tmp_delay > self.max_delay:
                self.max_delay  = tmp_delay
                path_num        = i

        # the core expects cycle_time to be set. save max_delay for printing
        self.cycle_time         = self.max_delay

        logging.debug('Critical path for \'%s\': %s' % (self.name, ' '.join(self.delay_paths[path_num])))

    def add_path(self, path):
        '''
        adds a new possible critical path

        @param path: list of component names in the path
        '''
        self.delay_paths.append(path)

    def dump(self, indent, verbosity=0):
        '''
        print the unit's data then each subunit's data

        @param indent: integer specifying how many spaces of indentation to start at.
        @param verbosity: (0=totals only, 1=adds data used from cacti, 2=everything)
        '''
        pre = ' ' * indent

        print pre + str(self.name) + ': '
        indent += 4
        pre = ' ' * indent

        print pre + 'area: %f mm^2' % (self.area,)
        print pre + 'cycle_time: %.3f ns' % (self.cycle_time,)
        print pre + 'switching_energy: %.3f nJ' % (self.switching_energy,)
        print pre + 'leakage_energy: %.3f nJ' % (self.leakage_energy,)
        print pre + 'total_energy: %.3f nJ' % (self.total_energy,)

        if verbosity >= 1:
            print pre + 'max_delay: %.3f ns' % (self.max_delay,)
            print pre + 'slack: %.3f ns' % (self.cycle_time - self.max_delay,)
            
        print ''

        for c in self.subunits.values():
            c.dump(indent=indent, verbosity=verbosity)


class Core(Unit):
    def __init__(self, name):
        # call the base class's __init__
        Unit.__init__(self, name)

        # if set, divide the fetch-to-execute delay by the number of stages
        self.perfect_pipelining = True

        # list of stages in the fetch-to-execute path
        self.fetch_to_exe_stages = []

        # allow the depth to be set arbitrarily. if not set, 
        # len(fetch_to_exe_stages) will be used
        self.fetch_to_exe_depth = None

    def get_data(self):

        # check for required parameters
        if not self.cycles:
            logging.error('\'cycles\' must be defined for \'%s\'' % (self.name,))
            exit()

        Unit.get_data(self)

        # ns
        self.cycle_time = 0

        # perfect_pipelining assumes all fetch-to-execute stages are perfectly balanced
        if self.perfect_pipelining:

            # find the total fetch-to-execute delay
            for stage in self.fetch_to_exe_stages:
                self.cycle_time += self.subunits[stage].cycle_time
            logging.info('performing perfect pipelining. total delay: ' + str(self.cycle_time))

            # divide the delay by the number of stages
            if self.fetch_to_exe_depth is None:
                self.cycle_time /= len(self.fetch_to_exe_stages)
            else:
                self.cycle_time /= self.fetch_to_exe_depth

        else: 
            # find the max delay of all fetch-to-execute stages
            for stage in self.fetch_to_exe_stages:
                if self.subunits[stage].cycle_time > self.cycle_time:
                    self.cycle_time = self.subunits[stage].cycle_time


class Processor(Unit):

    def calculate(self):
        # nJ
        self.switching_energy  = 0
        # nJ
        self.leakage_energy    = 0
        # nJ
        self.total_energy = 0

        for c in self.subunits.values():
            c.calculate()

            self.switching_energy     += c.switching_energy
            self.leakage_energy       += c.leakage_energy
            
        self.total_energy = self.switching_energy + self.leakage_energy

