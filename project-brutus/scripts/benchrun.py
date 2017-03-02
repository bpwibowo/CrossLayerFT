import os, errno
import subprocess
import benchargs
import logging

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

def fpath(s):
    return SCRIPT_DIR + "/../" + s
def frpath(s):
    return "../" + s

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError:
        # assume the path is actually there
        pass

def CreateJobFile(b,data,ext):
    # construct job file path
    job = fpath("exp/jobs/"+b+"."+data+".job")

    # if job file does not exist, create one
    if not os.path.isfile(job):
        # create exp/jobs directory if necessary
        mkdir_p(fpath("exp/jobs"))
        f = open(job, "w")
        f.write(frpath("exp/bench/bin/"+b+ext)+" "+
                benchargs.getargs(b,data,frpath(""))+"\n")
        f.close()

def PrepareReqFile(reqs):
    for f in reqs:
        if os.path.exists(f): continue # skip if exist
        cmd = ["cp","-R",fpath(reqs[f]),"."]
        retcode = subprocess.call(cmd, cwd=SCRIPT_DIR)

class RunPreparator:
    def __init__(self,options):
        self.options = options

    def run(self,b):
        if b != "all":
            print "Jobs generator only accept 'all' as argument"
            return

        # generate job files
        for b in benchargs.Benchmarks:
            print "Generating job file for '%s' for '%s' with extension '%s'"% \
                (b, self.options.data, self.options.ext)
            CreateJobFile(b, self.options.data, self.options.ext)

        # prepare files needed for simulations
        for b in benchargs.Benchmarks:
            print "Preparing files needed for %s" % b
            reqs = benchargs.getreqs(b,self.options.data)
            if reqs: PrepareReqFile(reqs)

class RunCoretime:
    def __init__(self,options):
        self.options = options
        self.tool = fpath("build/bin/coretime")

    def run(self,b):
        # check if benchmark application is listed
        if not b in benchargs.Benchmarks:
            print "Benchmark application %s is not a valid application." % b
            return

        # construct job file path
        job = fpath("exp/jobs/"+b+"."+self.options.data+".job")

        # if job file does not exist, create one
        if not os.path.isfile(job):
            CreateJobFile(b, self.options.data, self.options.ext)

        # prepare simulation files if needed
        reqs = benchargs.getreqs(b,self.options.data)
        if reqs: PrepareReqFile(reqs)

        # construct command file
        cmd = []
        cmd.append(self.tool)

        # - add cfg argument
        cfg_path = fpath("exp/cfg/"+self.options.cfg+".cfg")
        if not os.path.exists(cfg_path):
            print "Error: Config \"" + self.options.cfg + "\" does not exist."
            return
        cmd += ["--cfg", fpath("exp/cfg/single.cfg"), cfg_path]

        # - specify heartbeat
        cmd += ["--heartbeat", "1000000"]

        # - specify warm
        cmd += ["--warm", str(self.options.warm)]

        # - set output path
        output_path = "."
        d_path = self.options.sim_res_data
        if self.options.chkpt > 0:
            # run from checkpoint
            cmd += ["--chkpt", fpath("exp/intermediate/"+b+"/"+
                               self.options.chkpt_dir+"/checkpoint")]
            cmd += ["--chkpt_num", str(self.options.chkpt)]
            # output path depends on checkpoint number
            output_path = fpath(d_path+"/coretime/"+self.options.cfg+"/"+b+\
                                "/chkpt"+str(self.options.chkpt))
        elif self.options.chkpt == 0:
            # assert that checkpoint 0 shouldn't have warm flag
            assert(self.options.warm == 0)

            # run from beginning without warm-up, specify job file
            cmd += ["--job", job]

            # output path depends on checkpoint number
            output_path = fpath(d_path+"/coretime/"+self.options.cfg+"/"+b+\
                                "/chkpt"+str(self.options.chkpt))
        else:
            # run from beginning or crosspoint mode, specify job file
            cmd += ["--job", job]
            if self.options.crspt >= 0:
                output_path = fpath(d_path+"/coretime/"+self.options.cfg+"/"+b+\
                                    "/crspt"+str(self.options.crspt))
                # run crosspoint mode
                cmd += ["--crspt", str(self.options.crspt)]
            else:
                output_path = fpath(d_path+"/coretime/"+self.options.cfg+"/"+b)

        # - set statistics yaml file
        cmd += ["--yaml", output_path+"/results/stats.yaml"]

        # make sure the output path is there
        mkdir_p(output_path+"/results")

        # open stdout redirect file if needed
        stdout_file = None
        if not self.options.verbose:
            stdout_path = output_path+"/results/stdout.log"
            stdout_file = open(stdout_path, "w")
            print "INFO: Quite mode ON. STDOUT will be redirected to %s" % \
                stdout_path

        # pass flags
        for flag in self.options.toolflags.split(): cmd += [flag]

        # print and run command
        print ' '.join(cmd)
        retcode = subprocess.call(cmd, cwd=SCRIPT_DIR, stdout=stdout_file)


class RunSvm:
    def __init__(self,options):
        self.options = options
        self.tool = fpath("build/bin/coretime")

    def run(self,b):
        # check if benchmark application is listed
        if not b in benchargs.Benchmarks:
            print "Benchmark application %s is not a valid application." % b
            return

        # construct job file path
        job = fpath("exp/jobs/"+b+"."+self.options.data+".job")

        # if job file does not exist, create one
        if not os.path.isfile(job):
            CreateJobFile(b, self.options.data, self.options.ext)

        # prepare simulation files if needed
        reqs = benchargs.getreqs(b,self.options.data)
        if reqs: PrepareReqFile(reqs)

        # construct command file
        cmd = []
        cmd.append(self.tool)

        # - add cfg argument
        cfg_path = fpath("exp/cfg/"+self.options.cfg+".cfg")
        if not os.path.exists(cfg_path):
            print "Error: Config \"" + self.options.cfg + "\" does not exist."
            return
        cmd += ["--cfg", fpath("exp/cfg/single.cfg"), cfg_path, fpath("exp/cfg/svm.cfg")]

        # - specify heartbeat
        cmd += ["--heartbeat", "1000000"]

        # - specify warm
        cmd += ["--warm", str(self.options.warm)]

        # - set metadata path
        cmd += ["--svm_metadata_path",
                fpath("exp/bench/bin/"+b+self.options.ext+".metadata")]

        # - set optional flags
        optflgs = ""
        if self.options.svm_small_mcache:
            cmd += ["--svm_small_mcache"]
            optflgs += ".small_mcache"
        if self.options.svm_no_mcache:
            cmd += ["--svm_no_mcache"]
            optflgs += ".no_mcache"
        if self.options.svm_icache:
            cmd += ["--svm_mcache_use_icache"]
            optflgs += ".icache"
        if self.options.svm_small_icache:
            cmd += ["--svm_mcache_use_icache", "--svm_small_mcache"]
            optflgs += ".small_icache"
        if self.options.svm_adaptive_svf:
            cmd += ["--svm_adaptive_svf"]
            if self.options.svm_svf_adaptive_thd is not "": # adaptive mode
                optflgs += ".adaptive_svf"
            else:
                optflgs += ".sampling_svf"

            # check mandatory knob in this mode
            if self.options.svm_svf_rarate == 0:
                print "Error: Please set --svm_svf_rarate to a positive integer"
                return
            cmd += ["--svm_svf_active_size", str(self.options.svm_svf_active_size)]
            cmd += ["--svm_svf_rarate", str(self.options.svm_svf_rarate)]
            optflgs += "." + str(self.options.svm_svf_active_size)
            optflgs += "." + str(self.options.svm_svf_rarate)

        if self.options.svm_svf_adaptive_thd is not "":
            cmd += ["--svm_svf_adaptive_thd", self.options.svm_svf_adaptive_thd]
            optflgs += "." + self.options.svm_svf_adaptive_thd

        if self.options.svm_svf_only:
            cmd += ["--svm_svf_only"]
            optflgs += ".svfonly"

        # - set output path
        output_path = "."
        d_path = self.options.sim_res_data
        if self.options.chkpt > 0:
            # run from checkpoint
            cmd += ["--chkpt", fpath("exp/intermediate/"+b+"/"+
                               self.options.chkpt_dir+"/checkpoint")]
            cmd += ["--chkpt_num", str(self.options.chkpt)]
            # output path depends on checkpoint number
            output_path = fpath(d_path+"/svm"+optflgs+"/"+self.options.cfg+"/"+b+\
                                "/chkpt"+str(self.options.chkpt))
        elif self.options.chkpt == 0:
            # assert that checkpoint 0 shouldn't have warm flag
            assert(self.options.warm == 0)

            # run from beginning without warm-up, specify job file
            cmd += ["--job", job]

            # output path depends on checkpoint number
            output_path = fpath(d_path+"/svm"+optflgs+"/"+self.options.cfg+"/"+b+\
                                "/chkpt"+str(self.options.chkpt))
        else:
            # run from beginning, specify job file
            cmd += ["--job", job]
            output_path = fpath(d_path+"/svm"+optflgs+"/"+self.options.cfg+"/"+b)
        cmd += ["--svm_output_path", output_path]

        # - set statistics yaml file
        cmd += ["--yaml", output_path+"/results/stats.yaml"]

        if self.options.svm_periodic_stat:
            cmd += ["--stat_p_file", output_path+"/results/stats_periodic.log"]
            print "stat_p_file set"

        # - if PVS profile file is available, include it
        pvs_profile_path = fpath("exp/profiles/"+b+"/pvs.yaml")
        if os.path.exists(pvs_profile_path):
            cmd += ["--svm_pvs_profile", pvs_profile_path]

        # make sure the output path is there
        mkdir_p(output_path+"/results")

        # open stdout redirect file if needed
        stdout_file = None
        if not self.options.verbose:
            stdout_path = output_path+"/results/stdout.log"
            stdout_file = open(stdout_path, "w")
            print "INFO: Quite mode ON. STDOUT will be redirected to %s" % \
                stdout_path

        # pass flags
        for flag in self.options.toolflags.split(): cmd += [flag]

        # print and run command
        print ' '.join(cmd)
        retcode = subprocess.call(cmd, cwd=SCRIPT_DIR, stdout=stdout_file)


class RunFuncsim:
    def __init__(self,options):
        self.options = options
        self.tool = fpath("build/bin/funcsim")

    def run(self,b):
        # check if benchmark application is listed
        if not b in benchargs.Benchmarks:
            print "Benchmark application %s is not a valid application." % b
            return

        # construct job file path
        job = fpath("exp/jobs/"+b+"."+self.options.data+".job")

        # if job file does not exist, create one
        if not os.path.isfile(job):
            CreateJobFile(b, self.options.data, self.options.ext)

        # prepare simulation files if needed
        reqs = benchargs.getreqs(b,self.options.data)
        if reqs: PrepareReqFile(reqs)

        # construct command file
        cmd = []
        cmd.append(self.tool)

        # - set output path
        output_path = "."
        d_path = self.options.sim_res_data
        if self.options.chkpt > 0:
            # run from checkpoint
            cmd += ["--chkpt", fpath("exp/intermediate/"+b+"/"+
                               self.options.chkpt_dir+"/checkpoint")]
            cmd += ["--chkpt_num", str(self.options.chkpt)]
            # output path depends on checkpoint number
            output_path = fpath(d_path+"/funcsim/"+b+\
                                "/chkpt"+str(self.options.chkpt))
        elif self.options.chkpt == 0:
            # assert that checkpoint 0 shouldn't have warm flag
            assert(self.options.warm == 0)

            # run from beginning without warm-up, specify job file
            cmd += ["--job", job]

            # output path depends on checkpoint number
            output_path = fpath(d_path+"/funcsim/"+b+\
                                "/chkpt"+str(self.options.chkpt))
        else:
            # run from beginning or crosspoint mode, specify job file
            cmd += ["--job", job]
            if self.options.crspt >= 0:
                output_path = fpath(d_path+"/funcsim/"+b+\
                                    "/crspt"+str(self.options.crspt))
                # run crosspoint mode
                cmd += ["--crspt", str(self.options.crspt)]
            else:
                output_path = fpath(d_path+"/funcsim/"+b)

        # - specify heartbeat
        cmd += ["--heartbeat", "100000000"]

        # add enable_pvs_profiling if requested
        if self.options.enable_pvs_profiling:
            cmd += ["--enable_pvs_profiling"]
            cmd += ["--pvs_profiling_output", output_path+"/results/pvs.yaml"]
            cmd += ["--metadata",
                   fpath("exp/bench/bin/"+b+self.options.ext+".metadata")]

        # make sure the output path is there
        mkdir_p(output_path+"/results")

        # open stdout redirect file if needed
        stdout_file = None
        if not self.options.verbose:
            stdout_path = output_path+"/results/stdout.log"
            stdout_file = open(stdout_path, "w")
            print "INFO: Quite mode ON. STDOUT will be redirected to %s" % \
                stdout_path

        # pass flags
        for flag in self.options.toolflags.split(): cmd += [flag]

        # print and run command
        print ' '.join(cmd)
        retcode = subprocess.call(cmd, cwd=SCRIPT_DIR, stdout=stdout_file)

class RunPpa:
    def __init__(self,options):
        self.options = options
        self.tool_stats2ppa = fpath("scripts/stats2ppa.py")
        self.tool_ppa = "./run_ppa.py"

    def run(self,b):
        # check if benchmark application is listed
        if not b in benchargs.Benchmarks:
            print "Benchmark application %s is not a valid application." % b
            return

        # check cfg argument
        cfg_path = fpath("exp/cfg/"+self.options.cfg+".cfg")
        if not os.path.exists(cfg_path):
            print "Error: Config \"" + self.options.cfg + "\" does not exist."
            return

        # construct results path
        res_path = self.options.ppa_data + "/" + self.options.ppa_config + \
            "/" + self.options.cfg + "/" + b + "/"
        res_path_part2 = self.options.ppa_data + "/" + self.options.ppa_config + \
            ".svfonly/" + self.options.cfg + "/" + b + "/"
        if self.options.chkpt >= 0:
            res_path += "chkpt" + str(self.options.chkpt) + "/"
            res_path_part2 += "chkpt" + str(self.options.chkpt) + "/"
        res_path += "results/"
        res_path_part2 += "results/"

        # Step-1 # construct command string to convert stats file into ppa input file
        cmd = []
        cmd += ["python", self.tool_stats2ppa]

        # - add stats.yaml argument
        cmd += ["--stats", fpath(res_path+"stats.yaml")]

        mode = self.options.ppa_mode
        if mode == "svf" or mode == "sampling" or mode == "adaptive":
            # - add svm_results.yaml argument
            cmd += ["--svm_results", fpath(res_path+"svm_results.yaml")]
            cmd += ["--svm_results_part2", fpath(res_path_part2+"svm_results.yaml")]

        # - add ppa.template.yaml argument
        cmd += ["--template", fpath("scripts/ppa.template.yaml")]

        # - add ppa.input.yaml argument
        if self.options.ppa_mode == "default":
            mode = ""
        elif self.options.ppa_mode == "avfcl":
            cmd += ["--mode_avfcl"]
            mode = ".avfcl"
        elif self.options.ppa_mode == "parity":
            cmd += ["--mode_parity"]
            mode = ".parity"
        elif self.options.ppa_mode == "parity_plus":
            cmd += ["--mode_parity_plus"]
            mode = ".parity_plus"
        elif self.options.ppa_mode == "svf":
            cmd += ["--mode_svf"]
            mode = ".svf"
        elif self.options.ppa_mode == "ecc":
            cmd += ["--mode_ecc"]
            mode = ".ecc"
        elif self.options.ppa_mode == "sampling":
            cmd += ["--mode_sampling"]
            mode = ".sampling"
        elif self.options.ppa_mode == "adaptive":
            cmd += ["--mode_adaptive"]
            cmd += ["--ecc_active_file", fpath(res_path+"ecc_8K.log")]
            mode = ".adaptive"
        elif self.options.ppa_mode == "dynamic":
            cmd += ["--mode_avfcl"]
            cmd += ["--mode_parity_plus"]
            cmd += ["--mode_dynamic"]
            cmd += ["--ecc_active_file", fpath(res_path+"ecc_8K.log")]
            mode = ".dynamic"
        elif self.options.ppa_mode == "dynamic_ecc":
            cmd += ["--mode_avfcl"]
            cmd += ["--mode_ecc"]
            cmd += ["--mode_dynamic"]
            cmd += ["--ecc_active_file", fpath(res_path+"ecc_8K.log")]
            mode = ".dynamic_ecc"
        elif self.options.ppa_mode == "dynamic_dmr_st":
            cmd += ["--mode_avfcl"]
            cmd += ["--mode_dmr_st"]
            cmd += ["--mode_dynamic"]
            cmd += ["--ecc_active_file", fpath(res_path+"ecc_8K.log")]
            mode = ".dynamic_dmr_st"
        elif self.options.ppa_mode == "dynamic_tmr_st":
            cmd += ["--mode_avfcl"]
            cmd += ["--mode_tmr_st"]
            cmd += ["--mode_dynamic"]
            cmd += ["--ecc_active_file", fpath(res_path+"ecc_8K.log")]
            mode = ".dynamic_tmr_st"
        elif self.options.ppa_mode == "dmr_st":
            cmd += ["--mode_dmr_st"]
            mode = ".dmr_st"
        elif self.options.ppa_mode == "dmr_core":
            cmd += ["--mode_dmr_core"]
            mode = ".dmr_core"
        elif self.options.ppa_mode == "tmr_st":
            cmd += ["--mode_tmr_st"]
            mode = ".tmr_st"
        elif self.options.ppa_mode == "tmr_core":
            cmd += ["--mode_tmr_core"]
            mode = ".tmr_core"
        else:
            logging.error("Unsupported ppa_mode '%s'." % self.options.ppa_mode)
            exit(1)

        # - add use_icache if requested
        if self.options.ppa_use_icache:
            cmd += ["--use_icache"]
            mode = mode + ".icache"

        # - add use_smallmd if requested
        if self.options.ppa_use_smallmd:
            cmd += ["--use_smallmd"]
            mode = mode + ".smallmd"

        # - add use_nomd if requested
        if self.options.ppa_use_nomd:
            cmd += ["--use_nomd"]
            mode = mode + ".nomd"

        # - add ppa_sampling_cfg if provided
        if self.options.ppa_sampling_cfg is not None:
            cmd += ["--sampling_cfg", self.options.ppa_sampling_cfg]

        # - add full_ecc if requested
        if self.options.ppa_full_ecc:
            cmd += ["--full_ecc"]
            mode = mode + ".fullecc"

        # - add output argument
        cmd += ["--output", fpath(res_path+"ppa.input"+mode+".yaml")]

        # - print and run command
        print ' '.join(cmd)
        retcode = subprocess.call(cmd, cwd=SCRIPT_DIR)

        # Step-2 # construct command string for PPA analysis
        cmd = []
        cmd += ["python", self.tool_ppa]

        # - add input.yaml argument
        cmd += ["-i", fpath(res_path+"ppa.input"+mode+".yaml")]

        # - add verbosity level argument
        cmd += ["-v"]

        # - print and run command
        print ' '.join(cmd)
        retcode = subprocess.call(cmd, cwd=SCRIPT_DIR+"/ppa", \
                    stdout=open(fpath(res_path+"ppa.output"+mode+".yaml"), "w"))

class RunTraceCrosspt:
    def __init__(self,options):
        self.options = options
        self.tool = fpath("build/bin/funcsim")

    def run(self,b):
        # check if benchmark application is listed
        if not b in benchargs.Benchmarks:
            print "Benchmark application %s is not a valid application." % b
            return

        # construct job file path
        job = fpath("exp/jobs/"+b+"."+self.options.data+".job")

        # if job file does not exist, create one
        if not os.path.isfile(job):
            CreateJobFile(b, self.options.data, self.options.ext)

        # prepare simulation files if needed
        reqs = benchargs.getreqs(b,self.options.data)
        if reqs: PrepareReqFile(reqs)

        # construct command file
        cmd = []
        cmd.append(self.tool)

        # - specify heartbeat
        cmd += ["--heartbeat", "1000000000"]

        # - run from beginning, specify job file
        cmd += ["--job", job]
        d_path = self.options.sim_res_data
        output_path = fpath(d_path+"/trace_crosspt/"+b)

        # - set trace_crosspt flag
        cmd += ["--trace_crosspt"]

        # - set simstdout file
        cmd += ["--simstdout", output_path+"/results/simstdout.log"]

        # make sure the output path is there
        mkdir_p(output_path+"/results")

        # open stdout redirect file if needed
        stdout_path = output_path+"/results/stdout.log"
        stdout_file = open(stdout_path, "w")

        # print and run command
        print ' '.join(cmd)
        retcode = subprocess.call(cmd, cwd=SCRIPT_DIR, stdout=stdout_file)
