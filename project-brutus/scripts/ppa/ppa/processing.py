import logging
from components import Logic, SRAM, CactiSRAM, Cache, Custom, CustomRAM, Stage, Core, Processor


# global Processor instance so dump can print
proc = None
def process(info, perfect_pipelining=False):
    global proc

    proc = process_unit(type='processor', name='processor', info=info)

    # set perfect_pipelining on the cores
    for s in proc.subunits:
        if isinstance(s, Core):
            logging.error('setting perfect pipelining')
            s.perfect_pipelining = perfect_pipelining

    # tell each subunit and their subunits to calculate area, energy and delay
    proc.get_data()
    proc.calculate()

def dump(verbosity):
    global proc

    if not proc:
        logging.error('dump called before processing')
        exit()

    # dump the data
    proc.dump(indent=0, verbosity=verbosity)

def process_subunits(type, info):
    subunits = []

    for (name, sub_info) in info.items():
        subunits.append(process_unit(type=type, name=name, info=sub_info))

    return subunits

def process_unit(type, name, info):
    '''!
    @brief Process info to create a new unit

    @param name the unit's name
    @param info dict containing the unit's info

    @return the processed unit
    '''
    logging.debug('Processing {0} "{1}"'.format(type, name))
    
    # process keys matching any of these
    subunits = ['cores', 'stages', 'components']

    # create a new unit based on type
    if type == 'processor':
        unit = Processor(name=name)
    elif type == 'cores':
        unit = Core(name=name)
    elif type == 'stages':
        unit = Stage(name=name)
    elif type == 'components':
        return process_component(name=name, info=info)

    # iterate through all user-set parameters
    for (key, value) in info.items():

        # process subunits
        if key in subunits:
            unit.add_subunits(process_subunits(key, value))

        # add the list of timing paths to the unit (stages only)
        elif key == 'paths':
            # add each timing path to the unit. each path is a list of component names
            for path in value:
                unit.add_path(path)

        # copy all other user-set parameters
        else:
            unit[key] = value

    return unit

def process_component(name, info):
    '''!
    @brief Process info for a component (Logic, SRAM, CactiSRAM or Cache)

    @param name name of the component
    @param comonent_info dict containing the info for the component

    @return the processed Component
    '''

    if info['type'] == 'Logic':
        # use all user-set parameters minus 'type' and 'accesses' as fields to
        # look up the logic data in the database
        knobs = dict(info)
        del(knobs['type'])
        del(knobs['accesses'])

        comp = Logic(name=name,
                     knobs=knobs)

    elif info['type'] == 'SRAM':
        comp = SRAM(name=name,
                    ram=True,
                    words=info.get('words', None),
                    width=info.get('width', None),
                    read_ports=info.get('read_ports', None),
                    write_ports=info.get('write_ports', None))
                     
    elif info['type'] == 'CAM':
        comp = SRAM(name=name,
                    ram=False,
                    words=info.get('words', None),
                    width=info.get('width', None),
                    read_ports=info.get('read_ports', None),
                    write_ports=info.get('write_ports', None))
                     
    elif info['type'] == 'CactiSRAM':
        comp = CactiSRAM(name=name)

    elif info['type'] == 'Cache':
        comp = Cache(name=name)

    elif info['type'] == 'Custom':
        knobs = dict(info)
        comp = Custom(name=name, knobs=knobs)

    elif info['type'] == 'CustomRAM':
        knobs = dict(info)
        comp = CustomRAM(name=name, knobs=knobs)

    else:
        logging.error("Error, unknown type '%s'", info['type'])
        exit(1)

    # copy the parameters set by the user to the component
    comp.__dict__.update(info)

    logging.debug('Processing %s \'%s\'' % (info['type'], name))

    return comp
