import logging
import subprocess
import yaml

class Cacti():
    cacti = 'power/cacti65/cacti.noecc'

    def __init__(self, **kwargs):
        # default cacti arguments

        # cache size (bytes)
        self.size                        = 0

        # line size
        self.block_size                  = 64

        # fully assoc: ways=0
        self.ways                        = 4

        # number of ports
        self.rw_ports                    = 0
        self.read_ports                  = 0
        self.write_ports                 = 0
        self.single_ended_read_ports     = 0

        # multiple banks connected using a bus
        self.banks                       = 1

        # to model special structure like branch target buffers, directory, etc.
        # change the tag size parameter
        # if you want cacti to calculate the tagbits, set the tag size to "default"
        self.tag_width                   = 'default'

        # bus width to next level, includes data bits and address bits required by the decoder
        self.output_width                = 8

        # Type of memory - cache (with a tag array) or ram (scratch ram similar to a register file)
        # or main memory (no tag array and every access will happen at a page granularity Ref: CACTI 5.3 report)
        self.cache_type                  = 'cache'

        # technology (nm)
        self.tech                        = 45

        # 300-400 in steps of 10 (K)
        self.temp                        = 350

        # possible values: itrs-hp, itrs-lstp, itrs-lop, lp-dram, comm-dram
        self.data_array_cell_type        = 'itrs-hp'
        self.tag_array_cell_type         = 'itrs-hp'

        # possible values: itrs-hp, itrs-lstp, itrs-lop
        self.data_array_peripheral_type  = 'itrs-hp'
        self.tag_array_peripheral_type   = 'itrs-hp'

        # fast - data and tag access happen in parallel
        # sequential - data array is accessed after accessing the tag array
        # normal - data array lookup and tag access happen in parallel
        #          final data block is broadcasted in data array h-tree
        #          after getting the signal from the tag array
        self.access_mode                 = 'normal'

        # objective weights for UCA (or banks in NUCA)
        # delay, dyn_power, leakage_power, cycle_time, area
        self.delay_weight                = 0
        self.dyn_power_weight            = 50
        self.leakage_weight              = 50
        self.cycle_time_weight           = 0
        self.area_weight                 = 0

        self.delay_weight_nuca           = 0
        self.dyn_power_weight_nuca       = 50
        self.leakage_weight_nuca         = 50
        self.cycle_time_weight_nuca      = 0
        self.area_weight_nuca            = 0

        # Percentage deviation from the minimum value
        # delay, dyn_power, leakage_power, cycle_time, area
        self.delay_dev                   = 100000
        self.dyn_power_dev               = 10
        self.leakage_dev                 = 10
        self.cycle_time_dev              = 100000
        self.area_dev                    = 100000

        self.delay_dev_nuca              = 100000
        self.dyn_power_dev_nuca          = 10
        self.leakage_dev_nuca            = 10
        self.cycle_time_dev_nuca         = 100000
        self.area_dev_nuca               = 100000

        # Set optimize tag to ED or ED^2 to obtain a cache configuration optimized for
        # energy-delay or energy-delay sq. product
        # Note: Optimize tag will disable weight or deviate values mentioned above
        # Set it to NONE to let weight and deviate values determine the
        # appropriate cache configuration
        # possible values: 'ED^2', 'ED', 'NONE'
        self.opt_metric                  = 'ED^2'

        # possible values: 'UCA', 'NUCA'
        self.cache_model                 = 'UCA'

        # following three parameters are meaningful only for main memories
        self.page_size                   = 8192
        self.burst_length                = 8
        self.internal_prefetch_width     = 8

        # In order for CACTI to find the optimal NUCA bank value the following
        # variable should be assigned 0
        self.nuca_banks                  = 0

        # By default CACTI considers both full-swing and low-swing
        # wires to find an optimal configuration. However, it is possible to
        # restrict the search space by changing the signaling from "default" to
        # "fullswing" or "lowswing" type.
        self.wire_signaling              = 'default'

        self.wire_inside_mat             = 'global'
        self.wire_outside_mat            = 'global'

        # possible values: 'conservative', 'aggresive'
        self.interconnect_projection     = 'conservative'

        # only used in NUCA mode
        # Contention in network (which is a function of core count and cache level) is one of
        # the critical factor used for deciding the optimal bank count value
        # core count can be 4, 8, or 16
        self.cores                       = 4

        # only used in NUCA mode
        # possible values: 'L2', 'L3'
        self.cache_level                 = 'L2'

        self.ecc                         = 'true'

        self.print_level                 = 'DETAILED'

        # override attributes from kwargs. too many to list up top
        self.__dict__.update(kwargs)
        del kwargs


    def get_data(self):
        command = [self.cacti]
        command.extend(self.get_args())

        p = subprocess.Popen(command, stdout=subprocess.PIPE)

        data = yaml.load(p.stdout)
        if data is None:
            logging.error('Cacti returned no data')
            exit(0)

        for (key, value) in data.items():
            logging.debug('Cacti info: ' + str(key) + ': ' + str(value) + '\n')

        return data
        

    def get_args(self):
        # pass the necessary arguments to cacti. the arguments have a specific order.
        args = []
        args.append(str(self.size))
        args.append(str(self.block_size))
        args.append(str(self.ways))
        args.append(str(self.rw_ports))
        args.append(str(self.read_ports))
        args.append(str(self.write_ports))
        args.append(str(self.single_ended_read_ports))
        args.append(str(self.banks))
        args.append(str(self.tech))
        args.append(str(self.page_size))
        args.append(str(self.burst_length))
        args.append(str(self.internal_prefetch_width))
        args.append(str(self.output_width))

        # specific_tag, tag_width
        if self.tag_width == 'default':
            args.append(str(0))
            args.append(str(0))
        else:
            args.append(str(1))
            args.append(str(self.tag_width))

        if self.access_mode == 'normal':
            args.append(str(0))
        elif self.access_mode == 'sequential':
            args.append(str(1))
        else: # fast
            args.append(str(2))

        if self.cache_type == 'cache':
            args.append(str(1))
        else: # scratch
            args.append(str(0))

        # main_mem
        if self.cache_type == 'main memory':
            args.append(str(1))
        else:
            args.append(str(0))

        # objective function weights
        args.append(str(self.delay_weight))
        args.append(str(self.dyn_power_weight))
        args.append(str(self.leakage_weight))
        args.append(str(self.area_weight))
        args.append(str(self.cycle_time_weight))

        # objective function deviations
        args.append(str(self.delay_dev))
        args.append(str(self.dyn_power_dev))
        args.append(str(self.leakage_dev))
        args.append(str(self.area_dev))
        args.append(str(self.cycle_time_dev))

        if self.opt_metric == 'ED':
            args.append(str(1))
        elif self.opt_metric == 'ED^2':
            args.append(str(2))
        else: # use weights
            args.append(str(0))

        args.append(str(self.temp))

        # wire type (0: everything, 1: global, 2: 5% delay, 3: 10%, 4: 20%, 5: 30%, 6: low-swing)
        if self.wire_signaling == 'default':
            args.append(str(0))
        else: # ?
            args.append(str(1))

        if self.data_array_cell_type == 'itrs-hp':
            args.append(str(0))
        elif self.data_array_cell_type == 'itrs-lstp':
            args.append(str(1))
        elif self.data_array_cell_type == 'itrs-lop':
            args.append(str(2))
        elif self.data_array_cell_type == 'itrs-dram':
            args.append(str(3))
        else: # comm_dram
            args.append(str(4))

        if self.data_array_peripheral_type == 'itrs-hp':
            args.append(str(0))
        elif self.data_array_peripheral_type == 'itrs-lstp':
            args.append(str(1))
        else: # itrs-lop
            args.append(str(2))

        if self.tag_array_cell_type == 'itrs-hp':
            args.append(str(0))
        elif self.tag_array_cell_type == 'itrs-lstp':
            args.append(str(1))
        elif self.tag_array_cell_type == 'itrs-lop':
            args.append(str(2))
        elif self.tag_array_cell_type == 'itrs-dram':
            args.append(str(3))
        else: # comm_dram
            args.append(str(4))

        if self.tag_array_peripheral_type == 'itrs-hp':
            args.append(str(0))
        elif self.tag_array_peripheral_type == 'itrs-lstp':
            args.append(str(1))
        else: # itrs-lop
            args.append(str(2))

        if self.interconnect_projection == 'aggressive':
            args.append(str(0))
        else: # 'normal' or 'conservative'
            args.append(str(1))

        if self.wire_inside_mat == 'global':
            args.append(str(2))
        elif self.wire_inside_mat == 'local':
            args.append(str(0))
        else:
            args.append(str(1))

        if self.wire_outside_mat == 'global':
            args.append(str(2))
        elif self.wire_outside_mat == 'local':
            args.append(str(0))
        else:
            args.append(str(1))

        if self.cache_model == 'UCA':
            args.append(str(0))
        else: # NUCA
            args.append(str(1))

        args.append(str(self.cores))

        if self.cache_level == 'L2':
            args.append(str(0))
        else: # L3
            args.append(str(1))

        args.append(str(self.nuca_banks))

        # objective function weights
        args.append(str(self.delay_weight_nuca))
        args.append(str(self.dyn_power_weight_nuca))
        args.append(str(self.leakage_weight_nuca))
        args.append(str(self.area_weight_nuca))
        args.append(str(self.cycle_time_weight_nuca))

        # objective function deviations
        args.append(str(self.delay_dev_nuca))
        args.append(str(self.dyn_power_dev_nuca))
        args.append(str(self.leakage_dev_nuca))
        args.append(str(self.area_dev_nuca))
        args.append(str(self.cycle_time_dev_nuca))

        # repeaters_in_htree_segments
        args.append(str(1))

        # print input args
        args.append(str(1))

        return args
