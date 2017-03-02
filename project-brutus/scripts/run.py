#!/usr/bin/python

import sys
import optparse
import benchrun

# Parse arguments
parser = optparse.OptionParser()

parser.add_option("--tool",action="store",dest="tool",default="svm",
                  help="name of tool to use to run the benchmark or native")
parser.add_option("--ext",action="store",dest="ext",default=".mips64",
                  help="extension of benchmark")
parser.add_option("--data",action="store",dest="data",default="ref",
                  help="data set for input [test|train|ref|any]")
parser.add_option("--chkpt",action="store",dest="chkpt",default="-1",type="int",
                  help="run simulation from a specific checkpoint ID")
parser.add_option("--crspt",action="store",dest="crspt",default="-1",type="int",
                  help="run simulation on a specific crosspoint region")
parser.add_option("--warm",action="store",dest="warm",default="0",type="int",
                  help="warm up simulation for N number of instructions")
parser.add_option("--cfg",action="store",dest="cfg",default="o2wlslm",
                  help="brutus coretime configuration file")
parser.add_option("-q", "--quiet",
                  action="store_false", dest="verbose", default=True,
                  help="redirect stdout to file")
parser.add_option("--toolflags",action="store",dest="toolflags",default="",
                  help="flags to be forwarded to the tool")
parser.add_option("--chkpt_dir",action="store",dest="chkpt_dir",
                  default="checkpoints.50M_100M",
                  help="directory name of checkpoints")
parser.add_option("--sim_res_data", dest='sim_res_data', default="data",
                  help='root directory of data used for storing all simulation results')

# - Funcsim specific options
parser.add_option("--enable_pvs_profiling", dest='enable_pvs_profiling', default=False,
                  action="store_true",
                  help='Enable PVS profiling')

# - SVM specific options
parser.add_option("--svm_small_mcache",action="store_true",
                  dest="svm_small_mcache",default=False,
                  help="SVM run with small metadata-cache")
parser.add_option("--svm_no_mcache",action="store_true",
                  dest="svm_no_mcache",default=False,
                  help="SVM run with no metadata-cache")
parser.add_option("--svm_icache",action="store_true",
                  dest="svm_icache",default=False,
                  help="SVM run with metadata stored in i-cache")
parser.add_option("--svm_small_icache",action="store_true",
                  dest="svm_small_icache",default=False,
                  help="SVM run with small metadata and stored in i-cache")
parser.add_option("--svm_adaptive_svf",action="store_true",
                  dest="svm_adaptive_svf",default=False,
                  help="SVM mode with adaptive SVF monitoring")
parser.add_option("--svm_svf_active_size",action="store",
                  dest="svm_svf_active_size",default="1048576",type="int",
                  help="The size of active SVF monitoring in cycles")
parser.add_option("--svm_svf_rarate",action="store",
                  dest="svm_svf_rarate",default="0",type="int",
                  help="SVM reciprocal active rate of SVF monitoring")
parser.add_option("--svm_svf_adaptive_thd",action="store",
                  dest="svm_svf_adaptive_thd",default="",
                  help="SVM adaptive SVF threshold")
parser.add_option("--svm_svf_only",action="store_true",
                  dest="svm_svf_only",default=False,
                  help="SVM enables SVF analysis only")
parser.add_option("--svm_periodic_stat",action="store_true",
                  dest="svm_periodic_stat",default=False,
                  help="SVM enables periodic stat dumping")

# - PPA tool specific options
parser.add_option("--ppa_mode",action="store",dest="ppa_mode",default="default",
                  help="PPA analysis mode (default|avfcl|parity|svf|ecc|sampling|adaptive|dynamic)")
parser.add_option("--ppa_use_icache", dest='ppa_use_icache',
                  help='metadata use I-cache instead of M-cache',
                  action='store_true', default=False)
parser.add_option("--ppa_use_smallmd", dest='ppa_use_smallmd',
                  help='use 4-bit of metadata instead of 32-bit',
                  action='store_true', default=False)
parser.add_option("--ppa_use_nomd", dest='ppa_use_nomd',
                  help='use no metadata', action='store_true', default=False)
parser.add_option("--ppa_sampling_cfg", dest='ppa_sampling_cfg',
                  help='sampling configuration in ACTIVE/RARATE format')
parser.add_option("--ppa_full_ecc", dest='ppa_full_ecc',
                  help='analyze design with full SRAM-ECC protection',
                  action='store_true', default=False)
parser.add_option("--ppa_data", dest='ppa_data', default="data",
                  help='directory of data used for PPA analysis')
parser.add_option("--ppa_config", dest='ppa_config', default="svm",
                  help='directory of configuration used for PPA analysis')

(options,args) = parser.parse_args(sys.argv[1:])

# Prepare selected tool
if options.tool.find("svm") != -1:
    r = benchrun.RunSvm(options)
elif options.tool.find("coretime") != -1:
    r = benchrun.RunCoretime(options)
elif options.tool.find("funcsim") != -1:
    r = benchrun.RunFuncsim(options)
elif options.tool.find("prep") != -1:
    r = benchrun.RunPreparator(options)
elif options.tool.find("ppa") != -1:
    r = benchrun.RunPpa(options)
elif options.tool.find("trace_crosspt") != -1:
    r = benchrun.RunTraceCrosspt(options)
else:
    print "Unsupported tool '%s'" % options.tool
    exit(1)

# Run
if len(args):
    for b in args:
        r.run(b)
else:
    print "Nothing to run."
