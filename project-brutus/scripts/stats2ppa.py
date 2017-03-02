import math
import sys
import yaml
import copy
import optparse
import os

TOOL = "CactiSRAM"
#TOOL = "SRAM"

METADATA_FULL_SIZE = 32
METADATA_SMALL_SIZE = 4
METADATA_IMPORTANT_SIZE = 1
METADATA_LIVE_VECTOR_SIZE = 28
COUNTER_SIZE = 10 # 14 # 10
VBITS_ACC_SIZE = 14 # 21 # 12
LIVE_VECTOR_FLAG_SIZE = 1
ARCH_REG_DEPTH_LOG = 6

# make ECC structure
def EccOld(st, type, read_accesses, write_accesses):
	ecc = copy.deepcopy(st)

	k = st["width"]
	m = 1
	while (2**m < k + m + 1):
		m += 1
	m = m+1

	ecc["width"] = m
	if type is not None:
		ecc["type"] = type
	ecc["is_ecc"] = True

	if read_accesses is not None:
		ecc["read_accesses"] = read_accesses
	if write_accesses is not None:
		ecc["write_accesses"] = write_accesses

	checker = {}
	checker["type"] = "Custom"
	checker["subtype"] = "ecc_checker_%d" % m
	checker["accesses"] = write_accesses
	checker["count"] = st["write_ports"]
	checker["is_ecc"] = True
	corrector = {}
	corrector["type"] = "Custom"
	corrector["subtype"] = "ecc_corrector_%d" % m
	corrector["accesses"] = read_accesses
	corrector["count"] = st["read_ports"]
	corrector["is_ecc"] = True

	return ecc, checker, corrector

# make Svf structure
def CreateSvf(st, width, read_accesses=None, write_accesses=None, sleepable=True):
	svf = copy.deepcopy(st)
	svf["width"] = width
	svf["is_svf"] = sleepable

	if read_accesses is not None:
		svf["read_accesses"] = read_accesses
	if write_accesses is not None:
		svf["write_accesses"] = write_accesses
	return svf

def CreateSvfCustom(subtype, accesses, count=1):
	custom = {}
	custom["type"] = "Custom"
	custom["subtype"] = subtype
	custom["accesses"] = accesses
	custom["count"] = count
	custom["is_svf"] = True
	return custom

def CreateAvfClCustom(subtype, accesses, count=1):
	custom = {}
	custom["type"] = "Custom"
	custom["subtype"] = subtype
	custom["accesses"] = accesses
	custom["count"] = count
	custom["is_svf"] = True
	return custom

def CreateParityCustom(subtype, read_accesses, write_accesses):
	custom = {}
	custom["type"] = "CustomRAM"
	custom["subtype"] = subtype
	custom["read_accesses"] = read_accesses
	custom["write_accesses"] = write_accesses
	custom["is_ecc"] = True
	return custom

def CreateParityLogicCustom(subtype, read_accesses, write_accesses, count=1):
	custom = {}
	custom["type"] = "Custom"
	custom["subtype"] = subtype
	custom["accesses"] = read_accesses + write_accesses
	custom["count"] = count
	custom["is_ecc"] = True
	return custom

def CreateEccLogicCustom(st, read_accesses=None, write_accesses=None, width=None):
	k = st["width"] if width is None else width
	m = 1
	while (2**m < k + m + 1):
		m += 1
	m = m+1

	if read_accesses is None: read_accesses = st["read_accesses"]
	if write_accesses is None: write_accesses = st["write_accesses"]

	checker = {}
	checker["type"] = "Custom"
	checker["subtype"] = "ecc_checker_%d" % m
	checker["accesses"] = write_accesses
	checker["count"] = st["write_ports"]
	corrector = {}
	corrector["type"] = "Custom"
	corrector["subtype"] = "ecc_corrector_%d" % m
	corrector["accesses"] = read_accesses
	corrector["count"] = st["read_ports"]

	return checker, corrector

def CreateDmrStructure(parent, st_name, read_accesses, write_accesses):
	if options.mode_dmr_st:
		parent[st_name+"_DMR"] = copy.deepcopy(parent[st_name])
		parent[st_name+"_DMR"]["read_accesses"] = read_accesses
		parent[st_name+"_DMR"]["write_accesses"] = write_accesses
		parent[st_name+"_DMR"]["is_ecc"] = True

def CreateTmrStructure(parent, st_name, read_accesses, write_accesses):
	if options.mode_tmr_st:
		parent[st_name+"_TMR1"] = copy.deepcopy(parent[st_name])
		parent[st_name+"_TMR1"]["read_accesses"] = read_accesses
		parent[st_name+"_TMR1"]["write_accesses"] = write_accesses
		parent[st_name+"_TMR1"]["is_ecc"] = True
		parent[st_name+"_TMR2"] = parent[st_name+"_TMR1"] # just a copy from the previous one

# parse input arguments
parser = optparse.OptionParser()
parser.add_option("--stats", dest='input_stats_yaml', help='input stats YAML file from simulation')
parser.add_option("--svm_results", dest='svm_results_yaml', help='input SVM results YAML file from simulation')
parser.add_option("--svm_results_part2", dest='svm_results_part2_yaml', help='input SVM results YAML file part 2 from simulation')
parser.add_option("--template", dest='template_yaml', help='template YAML input file for PPA input')
parser.add_option("--mode_svf", dest='mode_svf', help='enable SVF mode', action='store_true', default=False)
parser.add_option("--mode_avfcl", dest='mode_avfcl', help='enable AVF-CL mode', action='store_true', default=False)
parser.add_option("--mode_parity", dest='mode_parity', help='enable parity mode', action='store_true', default=False)
parser.add_option("--mode_parity_plus", dest='mode_parity_plus', help='enable parity mode with ECC on critical structures', action='store_true', default=False)
parser.add_option("--mode_ecc", dest='mode_ecc', help='enable ECC mode', action='store_true', default=False)
parser.add_option("--mode_ecc_old", dest='mode_ecc_old', help='enable ECC mode, old version', action='store_true', default=False)
parser.add_option("--mode_sampling", dest='mode_sampling', help='enable SVF sampling mode', action='store_true', default=False)
parser.add_option("--mode_adaptive", dest='mode_adaptive', help='enable adaptive mode', action='store_true', default=False)
parser.add_option("--mode_dmr_st", dest='mode_dmr_st', help='enable DMR at structure level', action='store_true', default=False)
parser.add_option("--mode_dmr_core", dest='mode_dmr_core', help='enable DMR at core level', action='store_true', default=False)
parser.add_option("--mode_tmr_st", dest='mode_tmr_st', help='enable TMR at structure level', action='store_true', default=False)
parser.add_option("--mode_tmr_core", dest='mode_tmr_core', help='enable TMR at core level', action='store_true', default=False)
parser.add_option("--mode_dynamic", dest='mode_dynamic', help='enable dynamic protection mode', action='store_true', default=False)
parser.add_option("--ecc_active_file", dest='ecc_active_file', help='file showing ECC activity')
parser.add_option("--use_icache", dest='use_icache', help='metadata use I-cache instead of M-cache', action='store_true', default=False)
parser.add_option("--use_smallmd", dest='use_smallmd', help='use 4-bit of metadata instead of 32-bit', action='store_true', default=False)
parser.add_option("--use_nomd", dest='use_nomd', help='use no metadata', action='store_true', default=False)
parser.add_option("--sampling_cfg", dest='sampling_cfg', help='sampling configuration in ACTIVE/RARATE format')
parser.add_option("--full_ecc", dest='full_ecc', help='enable full ECC mode analysis', action='store_true', default=False)
parser.add_option("--output", dest='output_yaml', help='the output YAML file for PPA input')

(options, args) = parser.parse_args()

if options.mode_sampling:
	options.mode_svf = True
if options.mode_adaptive:
	options.mode_svf = True
	options.mode_ecc_old = True

# load information from stats file
stats = yaml.load(file(options.input_stats_yaml, 'r'))
stats_core0 = stats["stats"]["core_0"]
knobs = stats["knobs"]
knobs_core0 = stats["knobs"]["cores"]["core_0"]

# load information from svm results file
s_width = {}
if options.svm_results_yaml:
	svmresults = yaml.load(file(options.svm_results_yaml, 'r'))
	s_width["iq"] = svmresults["structures_list"]["IQ"]/knobs_core0["size_iq_int"]
	s_width["al"] = svmresults["structures_list"]["ROB"]/knobs_core0["size_al"]
	s_width["lq"] = svmresults["structures_list"]["LQ"]/knobs_core0["size_lq"]
	s_width["sq"] = svmresults["structures_list"]["SQ"]/knobs_core0["size_sq"]
	s_width["ibuff"] = svmresults["structures_list"]["InstBuff"]/knobs_core0["size_fq"]

	if options.svm_results_part2_yaml and os.path.exists(options.svm_results_part2_yaml):
		svmresults.update(yaml.load(file(options.svm_results_part2_yaml, 'r')))
else:
	# if svm_results_yaml is not specified, use the following default values
	s_width['iq'] = 213
	s_width['sq'] = 140
	s_width['ibuff'] = 190
	s_width['al'] = 158
	s_width['lq'] = 160

# load template yaml file
data = yaml.load(file(options.template_yaml, 'r'))

# compute SVF sampling activity
svf_sw_on = 0
svf_sw_off = 0
if options.mode_sampling or options.mode_adaptive:
	if options.sampling_cfg is None:
		print "ERROR, --sampling_cfg need to be specified"
		exit(1)
	vals = options.sampling_cfg.split("/")
	active_period = int(vals[0])
	recp_duty_cycle = int(vals[1])
	window_size = active_period * recp_duty_cycle

	cycles = stats["stats"]["cycles"]
	svf_sw_on = math.ceil(float(cycles)/window_size)
	svf_sw_off = math.ceil(float(cycles - active_period)/window_size)

# compute ECC on-off activity
ecc_sw_on = 0
ecc_sw_off = 0
if options.mode_adaptive or options.mode_dynamic:
	if options.ecc_active_file is None:
		print "ERROR, --ecc_active_file need to be specified"
		exit(1)

	line_num = 0
	current_ecc_state = None
	for line in open(options.ecc_active_file):
		line_num += 1

		if line_num == 1: continue # skip first line
		line = int(line) # make integer
		if current_ecc_state == None:
			current_ecc_state = line # first time setting

		if current_ecc_state != line:
			if current_ecc_state == 1: ecc_sw_off += 1
			else: ecc_sw_on += 1

		current_ecc_state = line

	if options.mode_dynamic:
		# in dynamic protection mode, we turn off AVF-CL when protection is enabled
		# and vice versa
		svf_sw_on = ecc_sw_off
		svf_sw_off = ecc_sw_on


# check if _ecc stats available
ecc_suffix = ""
if "cycles_ecc" in stats["stats"] and options.mode_dynamic:
	ecc_suffix = "_ecc"

# update template data
core = data["processor"]["cores"]["core_0"]
core["out_of_order"] = knobs_core0["ooo"]
core["cycles"] = stats["stats"]["cycles"]
core["svf_sw_on"] = svf_sw_on
core["svf_sw_off"] = svf_sw_off
core["ecc_sw_on"] = ecc_sw_on
core["ecc_sw_off"] = ecc_sw_off

# update ECC and SVF cycles
if options.mode_ecc_old or options.mode_parity or options.mode_parity_plus or options.mode_ecc or options.mode_dmr_st or options.mode_tmr_st:
	core["cycles_ecc"] = stats["stats"]["cycles"]
if options.mode_avfcl:
	core["cycles_svf"] = stats["stats"]["cycles_svf"]
if options.mode_svf or options.mode_sampling or options.mode_adaptive or options.mode_dynamic:
	core["cycles_svf"] = stats["stats"]["cycles_svf"]
	core["cycles_ecc"] = stats["stats"]["cycles_ecc"]

fetch_1 = core["stages"]["fetch_1"]["components"]
fetch_1["Fetch1Logic"]["fetch_width"] = knobs_core0["width_frontend"]
fetch_1["Fetch1Logic"]["accesses"] = stats_core0["frontend"]["active_cycles"]
fetch_1["BTB"]["type"] = "CustomRAM"
fetch_1["BTB"]["subtype"] = "BTBMem"
fetch_1["BTB"]["read_ports"] = 1
fetch_1["BTB"]["read_accesses"] = stats_core0["frontend"]["ic_accesses"]
fetch_1["BTB"]["write_accesses"] = stats_core0["frontend"]["bp"]["btb_preds"]
for n in xrange(2,knobs_core0["width_frontend"]+1):
	fetch_1["BTB"+str(n)] = fetch_1["BTB"]
if options.mode_ecc_old and options.full_ecc:
	fetch_1["BTB_ecc"], fetch_1["BTB_ecc_checker"], fetch_1["BTB_ecc_corrector"] = EccOld(fetch_1["BTB"], None,
		stats_core0["frontend"]["ic_accesses"+ecc_suffix],
		stats_core0["frontend"]["bp"]["btb_preds"+ecc_suffix])
fetch_1["BP"]["type"] = "CustomRAM"
fetch_1["BP"]["subtype"] = "BPMem"
fetch_1["BP"]["read_accesses"] = stats_core0["frontend"]["insts_fetched_cti"]
fetch_1["BP"]["write_accesses"] = stats_core0["frontend"]["bp"]["cond_preds"]
if options.mode_ecc_old and options.full_ecc:
	fetch_1["BP_ecc"], fetch_1["BP_ecc_checker"], fetch_1["BP_ecc_corrector"] = EccOld(fetch_1["BP"], None,
		stats_core0["frontend"]["insts_fetched_cti"+ecc_suffix],
		stats_core0["frontend"]["bp"]["cond_preds"+ecc_suffix])
for n in xrange(2,knobs_core0["width_frontend"]+1):
	fetch_1["BP"+str(n)] = fetch_1["BP"]
	if options.mode_ecc_old and options.full_ecc:
		fetch_1["BP"+str(n)+"_ecc"], fetch_1["BP"+str(n)+"_ecc_checker"], fetch_1["BP"+str(n)+"_ecc_corrector"] = EccOld(fetch_1["BP"+str(n)], None,
			stats_core0["frontend"]["insts_fetched_cti"+ecc_suffix],
			stats_core0["frontend"]["bp"]["cond_preds"+ecc_suffix])
fetch_1["RAS"]["type"] = "CustomRAM"
fetch_1["RAS"]["subtype"] = "RASMem"
fetch_1["RAS"]["read_accesses"] = stats_core0["frontend"]["bp"]["ras_preds"]
fetch_1["RAS"]["write_accesses"] = stats_core0["frontend"]["insts_fetched_calls"]
if options.mode_ecc_old and options.full_ecc:
	fetch_1["RAS_ecc"], fetch_1["RAS_ecc_checker"], fetch_1["RAS_ecc_corrector"] = EccOld(fetch_1["RAS"], "SRAM",
		stats_core0["frontend"]["bp"]["ras_preds"+ecc_suffix],
		stats_core0["frontend"]["insts_fetched_calls"+ecc_suffix])
fetch_1["L1ICache"]["sets"] = knobs_core0["ic_sets"]
if (knobs_core0["ic_sets"] < 128):
    print "WARNING: Forcing number of sets of L1ICache to 128 sets due to CACTI limitation."
fetch_1["L1ICache"]["block_size"] = 2**knobs_core0["ic_line_size"]
fetch_1["L1ICache"]["ways"] = knobs_core0["ic_ways"]
fetch_1["L1ICache"]["banks"] = 2
fetch_1["L1ICache"]["read_accesses"] = stats_core0["frontend"]["ic"]["read_accesses"]
fetch_1["L1ICache"]["write_accesses"] = stats_core0["frontend"]["ic"]["write_accesses"]
fetch_1["ITLB"] = {}
fetch_1["ITLB"]["type"] = "Cache"
if (knobs_core0["itlb_sets"] < 128):
    print "WARNING: Forcing number of sets of ITLB to 128 sets due to CACTI limitation."
fetch_1["ITLB"]["sets"] = max(128,knobs_core0["itlb_sets"])
fetch_1["ITLB"]["block_size"] = 8
fetch_1["ITLB"]["ways"] = knobs_core0["itlb_ways"]
fetch_1["ITLB"]["rw_ports"] = 1
fetch_1["ITLB"]["banks"] = 1
fetch_1["ITLB"]["read_accesses"] = stats_core0["frontend"]["itlb"]["read_accesses"]
fetch_1["ITLB"]["write_accesses"] = stats_core0["frontend"]["itlb"]["write_accesses"]
if options.mode_avfcl:
	fetch_1["NopLane_avfcl"] = CreateAvfClCustom("nop_lane", stats_core0["frontend"]["active_cycles_svf"])
if options.mode_svf:
	if options.use_nomd:
		pass # no change on ICache or ITLB needed
	elif options.use_icache:
		fetch_1["L1ICache"]["banks"] *= 2 # double the number of banks
		# adjust ICache and ITLB activities, depending of whether SVF on all the time or not
		if stats_core0["frontend"]["ic"]["read_accesses_svf"] == stats_core0["frontend"]["ic"]["read_accesses"]:
			# SVF is on all the time
			# only one access needed per cycle, make the counters half
			fetch_1["L1ICache"]["read_accesses"] /= 2
			fetch_1["L1ICache"]["write_accesses"] /= 2
			fetch_1["ITLB"]["read_accesses"] /= 2
			fetch_1["ITLB"]["write_accesses"] /= 2

			if options.use_smallmd:
				# add extra access due to bank conflicts
				# 1/4 probability of conflict as much as 1/4 counter occurance
				# thus, additional 1/16
				fetch_1["L1ICache"]["read_accesses"] *= 17/16
				fetch_1["L1ICache"]["write_accesses"] *= 17/16
				fetch_1["ITLB"]["read_accesses"] *= 17/16
				fetch_1["ITLB"]["write_accesses"] *= 17/16
		else:
			# exclude contribution from svf counter
			fetch_1["L1ICache"]["read_accesses"] -= \
				stats_core0["frontend"]["ic"]["read_accesses_svf"]/2
			fetch_1["L1ICache"]["write_accesses"] -= \
				stats_core0["frontend"]["ic"]["write_accesses_svf"]/2
			fetch_1["ITLB"]["read_accesses"] -= \
				stats_core0["frontend"]["itlb"]["read_accesses_svf"]/2
			fetch_1["ITLB"]["write_accesses"] -= \
				stats_core0["frontend"]["itlb"]["write_accesses_svf"]/2

			if options.use_smallmd:
				# add extra access due to bank conflicts
				# 1/4 probability of conflict as much as 1/2 svf counter occurance
				# thus, additional 1/8 svf counter occurance
				fetch_1["L1ICache"]["read_accesses"] += stats_core0["frontend"]["ic"]["read_accesses_svf"]/8
				fetch_1["L1ICache"]["write_accesses"] += stats_core0["frontend"]["ic"]["write_accesses_svf"]/8
				fetch_1["ITLB"]["read_accesses"] += stats_core0["frontend"]["itlb"]["read_accesses_svf"]/8
				fetch_1["ITLB"]["write_accesses"] += stats_core0["frontend"]["itlb"]["write_accesses_svf"]/8
	else:
		fetch_1["MTLB"] = copy.deepcopy(fetch_1["ITLB"])
		fetch_1["MTLB"]["is_svf"] = True
		fetch_1["L1MCache"] = copy.deepcopy(fetch_1["L1ICache"])
		fetch_1["L1MCache"]["is_svf"] = True
		if options.use_smallmd:
			fetch_1["L1MCache"]["block_size"] /= 8

		# update MTLB/MCache activity
		fetch_1["MTLB"]["read_accesses"] = stats_core0["frontend"]["mtlb"]["read_accesses"]
		fetch_1["MTLB"]["write_accesses"] = stats_core0["frontend"]["mtlb"]["write_accesses"]
		fetch_1["L1MCache"]["read_accesses"] = stats_core0["frontend"]["mc"]["read_accesses"]
		fetch_1["L1MCache"]["write_accesses"] = stats_core0["frontend"]["mc"]["write_accesses"]

fetch_2 = core["stages"]["fetch_2"]["components"]
fetch_2["Fetch2Logic"]["fetch_width"] = knobs_core0["width_frontend"]
fetch_2["Fetch2Logic"]["accesses"] = stats_core0["frontend"]["active_cycles"]
fetch_2["CTIQ"]["type"] = "CustomRAM"
fetch_2["CTIQ"]["subtype"] = "CTIQMem"
fetch_2["CTIQ"]["read_accesses"] = stats_core0["frontend"]["bp"]["preds"]
fetch_2["CTIQ"]["write_accesses"] = stats_core0["frontend"]["insts_fetched_cti"]
if options.mode_ecc_old and options.full_ecc:
	fetch_2["CTIQ_ecc"], fetch_2["CTIQ_ecc_checker"], fetch_2["CTIQ_ecc_corrector"] = EccOld(fetch_2["CTIQ"], "SRAM",
		stats_core0["frontend"]["bp"]["preds"+ecc_suffix],
		stats_core0["frontend"]["insts_fetched_cti"+ecc_suffix])

decode = core["stages"]["decode"]["components"]
decode["DecodeLogic"]["fetch_width"] = knobs_core0["width_frontend"]
decode["DecodeLogic"]["accesses"] = stats_core0["frontend"]["active_cycles"]
decode["InstBuff"]["type"] = "CustomRAM"
decode["InstBuff"]["subtype"] = "InstBuffMem"
decode["InstBuff"]["words"] = knobs_core0["size_fq"]
decode["InstBuff"]["width"] = s_width["ibuff"]
decode["InstBuff"]["read_ports"] = knobs_core0["width_frontend"]
decode["InstBuff"]["write_ports"] = knobs_core0["width_frontend"]*2
decode["InstBuff"]["read_accesses"] = stats_core0["backend"]["insts_renamed"]
decode["InstBuff"]["write_accesses"] = stats_core0["frontend"]["insts_decoded"]
if options.mode_avfcl:
	decode["InstBuff_avfcl_counter256"] = CreateAvfClCustom("counter256", core["cycles_svf"])
	decode["InstBuff_avfcl_counter65k"] = CreateAvfClCustom("counter65k", core["cycles_svf"])
	decode["InstBuff_avfcl_ibdur"] = CreateAvfClCustom("ib_dur", stats_core0["frontend"]["active_cycles_svf"])
	decode["InstBuff_avfcl_MetadataDecode"] = CreateAvfClCustom("metadata_decode", stats_core0["frontend"]["active_cycles_svf"])
if options.mode_parity or options.mode_parity_plus:
	decode["InstBuff_parity"] = CreateParityCustom("InstBuffParityMem", \
		stats_core0["backend"]["insts_renamed"+ecc_suffix], \
		stats_core0["frontend"]["insts_decoded"+ecc_suffix])
	decode["InstBuff_paritylogic"] = CreateParityLogicCustom("parity_WIDTH190", \
		stats_core0["backend"]["insts_renamed"+ecc_suffix], \
		stats_core0["frontend"]["insts_decoded"+ecc_suffix])
if options.mode_ecc:
	decode["InstBuff_ecc"] = CreateParityCustom("InstBuffEccMem", \
		stats_core0["backend"]["insts_renamed"+ecc_suffix], \
		stats_core0["frontend"]["insts_decoded"+ecc_suffix])
	decode["InstBuff_ecc_checker"], decode["InstBuff_ecc_corrector"] = CreateEccLogicCustom(decode["InstBuff"])
if options.mode_ecc_old:
	decode["InstBuff_ecc"], decode["InstBuff_ecc_checker"], decode["InstBuff_ecc_corrector"] = EccOld(decode["InstBuff"], "SRAM",
		stats_core0["backend"]["insts_renamed"+ecc_suffix],
		stats_core0["frontend"]["insts_decoded"+ecc_suffix])
	# if decode["InstBuff_ecc"]["words"] < 22:
	# 	decode["InstBuff_ecc"]["words"] = 22
	# decode["InstBuff_ecc"]["read_accesses"] /= 2
	# decode["InstBuff_ecc"]["write_accesses"] /= 2
if options.mode_svf:
	metadata_size = METADATA_FULL_SIZE
	if options.use_smallmd:
		metadata_size = METADATA_SMALL_SIZE
	elif options.use_nomd:
		metadata_size = 0
	decode["InstBuff_svf"] = CreateSvf(decode["InstBuff"], metadata_size + COUNTER_SIZE,
		stats_core0["backend"]["insts_renamed_svf"],
		stats_core0["frontend"]["insts_decoded_svf"])
	decode["InstBuff_svf"]["type"] = "SRAM"
	decode["InstBuff_svfmult"] = CreateSvfCustom("addshift8x10_instbuff",
		stats_core0["backend"]["insts_renamed_svf"], knobs_core0["width_frontend"])
	decode["InstBuff_svfvlt"] = CreateSvfCustom("vlt_instbuff",
		stats_core0["backend"]["insts_renamed_svf"],knobs_core0["width_frontend"])
	decode["InstBuff_svfcounter"] = CreateSvfCustom("counter_instbuff", core["cycles"])
CreateDmrStructure(decode, "InstBuff", \
	stats_core0["backend"]["insts_renamed"+ecc_suffix], \
	stats_core0["frontend"]["insts_decoded"+ecc_suffix])
CreateTmrStructure(decode, "InstBuff", \
	stats_core0["backend"]["insts_renamed"+ecc_suffix], \
	stats_core0["frontend"]["insts_decoded"+ecc_suffix])

rename_int = core["stages"]["rename_int"]["components"]
rename_int["RenameLogic"]["fetch_width"] = knobs_core0["width_frontend"]
rename_int["RenameLogic"]["accesses"] = stats_core0["backend"]["active_cycles"]
rename_int["Freelist"]["type"] = "CustomRAM"
rename_int["Freelist"]["subtype"] = "FreelistMem"
rename_int["Freelist"]["words"] = knobs_core0["size_prf_int"]-32
rename_int["Freelist"]["width"] = int(math.ceil(math.log(knobs_core0["size_prf_int"],2)))
rename_int["Freelist"]["read_ports"] = max(knobs_core0["width_frontend"], knobs_core0["width_retire"])
rename_int["Freelist"]["write_ports"] = rename_int["Freelist"]["read_ports"]
rename_int["Freelist"]["read_accesses"] = stats_core0["backend"]["insts_renamed"]
rename_int["Freelist"]["write_accesses"] = stats_core0["backend"]["insts_retired"]
if options.mode_ecc_old and options.full_ecc:
	rename_int["Freelist_ecc"], rename_int["Freelist_ecc_checker"], rename_int["Freelist_ecc_corrector"] = EccOld(rename_int["Freelist"], None,
		stats_core0["backend"]["insts_renamed"+ecc_suffix],
		stats_core0["backend"]["insts_retired"+ecc_suffix])
rename_int["RMT"]["type"] = "CustomRAM"
rename_int["RMT"]["subtype"] = "RMTMem"
rename_int["RMT"]["words"] = 32
rename_int["RMT"]["width"] = rename_int["Freelist"]["width"]
rename_int["RMT"]["read_ports"] = knobs_core0["width_frontend"]*2
rename_int["RMT"]["write_ports"] = knobs_core0["width_frontend"]
rename_int["RMT"]["read_accesses"] = stats_core0["backend"]["rmt_reads_int"]
rename_int["RMT"]["write_accesses"] = stats_core0["backend"]["rmt_writes_int"]
if options.mode_ecc_old and options.full_ecc:
	rename_int["RMT_ecc"], rename_int["RMT_ecc_checker"], rename_int["RMT_ecc_corrector"] = EccOld(rename_int["RMT"], None,
		stats_core0["backend"]["rmt_reads_int"+ecc_suffix],
		stats_core0["backend"]["rmt_writes_int"+ecc_suffix])
rename_int["AMT"]["type"] = "CustomRAM"
rename_int["AMT"]["subtype"] = "AMTMem"
rename_int["AMT"]["words"] = 32
rename_int["AMT"]["width"] = rename_int["Freelist"]["width"]
rename_int["AMT"]["read_ports"] = knobs_core0["width_retire"]
rename_int["AMT"]["write_ports"] = knobs_core0["width_retire"]
rename_int["AMT"]["read_accesses"] = stats_core0["backend"]["insts_retired"]
rename_int["AMT"]["write_accesses"] = stats_core0["backend"]["insts_retired"]
if options.mode_ecc_old and options.full_ecc:
	rename_int["AMT_ecc"], rename_int["AMT_ecc_checker"], rename_int["AMT_ecc_corrector"] = EccOld(rename_int["AMT"], None,
		stats_core0["backend"]["insts_retired"+ecc_suffix],
		stats_core0["backend"]["insts_retired"+ecc_suffix])
if options.mode_svf: rename_int["AMT_svf"] = CreateSvf(rename_int["AMT"],1)

dispatch = core["stages"]["dispatch"]["components"]
dispatch["DispatchLogic"]["fetch_width"] = knobs_core0["width_frontend"]
dispatch["DispatchLogic"]["issue_width"] = knobs_core0["width_issue"]
dispatch["DispatchLogic"]["accesses"] = stats_core0["backend"]["active_cycles"]
if options.mode_avfcl:
	dispatch["VopDecoder_avfcl"] = CreateAvfClCustom("vop_decoder", stats_core0["backend"]["active_cycles_svf"])

issue = core["stages"]["issue"]["components"]
issue["IssueLogic"]["fetch_width"] = knobs_core0["width_frontend"]
issue["IssueLogic"]["issue_width"] = knobs_core0["width_issue"]
issue["IssueLogic"]["size_iq"] = knobs_core0["size_iq_int"]
issue["IssueLogic"]["accesses"] = stats_core0["backend"]["active_cycles"]
issue["PayloadRAM"]["type"] = "CustomRAM"
issue["PayloadRAM"]["subtype"] = "PayloadRAMMem"
issue["PayloadRAM"]["words"] = knobs_core0["size_iq_int"]
issue["PayloadRAM"]["width"] = s_width["iq"]-2*rename_int["Freelist"]["width"]
issue["PayloadRAM"]["read_ports"] = knobs_core0["width_issue"]
issue["PayloadRAM"]["write_ports"] = knobs_core0["width_frontend"]
issue["PayloadRAM"]["read_accesses"] = stats_core0["backend"]["insts_issued"]
issue["PayloadRAM"]["write_accesses"] = stats_core0["backend"]["iq_writes"]
if options.mode_parity or options.mode_parity_plus:
	issue["PayloadRAM_parity"] = CreateParityCustom("PayloadRAMParityMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_writes"+ecc_suffix])
	issue["PayloadRAM_paritylogic"] = CreateParityLogicCustom("parity_WIDTH197", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_writes"+ecc_suffix])
if options.mode_ecc:
	issue["PayloadRAM_ecc"] = CreateParityCustom("PayloadRAMEccMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_writes"+ecc_suffix])
	issue["PayloadRAM_ecc_checker"], issue["PayloadRAM_ecc_corrector"] = CreateEccLogicCustom(issue["PayloadRAM"])
if options.mode_ecc_old:
	issue["PayloadRAM_ecc"], issue["PayloadRAM_ecc_checker"], issue["PayloadRAM_ecc_corrector"] = EccOld(issue["PayloadRAM"], None,
		stats_core0["backend"]["insts_issued"+ecc_suffix],
		stats_core0["backend"]["iq_writes"+ecc_suffix])
if options.mode_svf:
	issue["PayloadRAM_svf"] = CreateSvf(issue["PayloadRAM"], VBITS_ACC_SIZE + COUNTER_SIZE + METADATA_SMALL_SIZE,
		stats_core0["backend"]["insts_issued_svf"],
		stats_core0["backend"]["iq_writes_svf"])
	issue["PayloadRAM_svfmult"] = CreateSvfCustom("addshift8x10_iq",
		stats_core0["backend"]["insts_issued_svf"], knobs_core0["width_issue"])
	issue["PayloadRAM_svfvlt"] = CreateSvfCustom("vlt_iq",
		stats_core0["backend"]["insts_issued_svf"], knobs_core0["width_issue"])
	issue["PayloadRAM_svfcounter"] = CreateSvfCustom("counter_iq", core["cycles"])
CreateDmrStructure(issue, "PayloadRAM",
		stats_core0["backend"]["insts_issued"+ecc_suffix],
		stats_core0["backend"]["iq_writes"+ecc_suffix])
CreateTmrStructure(issue, "PayloadRAM",
		stats_core0["backend"]["insts_issued"+ecc_suffix],
		stats_core0["backend"]["iq_writes"+ecc_suffix])

issue["Src1CAM"]["words"] = knobs_core0["size_iq_int"]
issue["Src1CAM"]["width"] = rename_int["Freelist"]["width"]
issue["Src1CAM"]["read_ports"] = max(knobs_core0["width_issue"], knobs_core0["width_frontend"])
issue["Src1CAM"]["write_ports"] = max(knobs_core0["width_issue"], knobs_core0["width_frontend"])
issue["Src1CAM"]["read_accesses"] = stats_core0["backend"]["insts_issued"]
issue["Src1CAM"]["write_accesses"] = stats_core0["backend"]["iq_wakeup_access"]
issue["Src2CAM"]["words"] = knobs_core0["size_iq_int"]
issue["Src2CAM"]["width"] = rename_int["Freelist"]["width"]
issue["Src2CAM"]["read_ports"] = max(knobs_core0["width_issue"], knobs_core0["width_frontend"])
issue["Src2CAM"]["write_ports"] = max(knobs_core0["width_issue"], knobs_core0["width_frontend"])
issue["Src2CAM"]["read_accesses"] = stats_core0["backend"]["insts_issued"]
issue["Src2CAM"]["write_accesses"] = stats_core0["backend"]["iq_wakeup_access"]
if options.mode_parity or options.mode_parity_plus:
	issue["Src1CAM_parity"] = CreateParityCustom("PayloadRAMParityMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	issue["Src1CAM_paritylogic"] = CreateParityLogicCustom("parity_WIDTH64", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	issue["Src2CAM_parity"] = CreateParityCustom("PayloadRAMParityMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	issue["Src2CAM_paritylogic"] = CreateParityLogicCustom("parity_WIDTH64", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	# for ready bits, assume only cost the memory (lower bound) without parity logic
	issue["Src1Ready_parity"] = CreateParityCustom("PayloadRAMParityMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	issue["Src2Ready_parity"] = CreateParityCustom("PayloadRAMParityMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
if options.mode_ecc:
	issue["Src1CAM_ecc"] = CreateParityCustom("PayloadRAMEccMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	issue["Src2CAM_ecc"] = CreateParityCustom("PayloadRAMEccMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	issue["Src1CAM_ecc_checker"], issue["Src1CAM_ecc_corrector"] = CreateEccLogicCustom(issue["Src1CAM"])
	issue["Src2CAM_ecc_checker"], issue["Src2CAM_ecc_corrector"] = CreateEccLogicCustom(issue["Src2CAM"])
	# for ready bits, assume only cost the memory (lower bound) without ECC logic
	issue["Src1Ready_ecc"] = CreateParityCustom("PayloadRAMParityMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix]) # assume this bit requires only one bit (lower bound)
	issue["Src2Ready_ecc"] = CreateParityCustom("PayloadRAMParityMem", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix]) # assume this bit requires only one bit (lower bound)
if options.mode_ecc_old:
	issue["Src1CAM_ecc"], issue["Src1CAM_ecc_checker"], issue["Src1CAM_ecc_corrector"] = EccOld(issue["Src1CAM"],"SRAM",
		stats_core0["backend"]["insts_issued"+ecc_suffix],
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
	issue["Src2CAM_ecc"], issue["Src2CAM_ecc_checker"], issue["Src2CAM_ecc_corrector"] = EccOld(issue["Src2CAM"],"SRAM",
		stats_core0["backend"]["insts_issued"+ecc_suffix],
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
if options.mode_avfcl:
	issue["IQ_avfcl_counter256"] = CreateAvfClCustom("counter256", core["cycles_svf"])
	issue["IQ_avfcl_counter65k"] = CreateAvfClCustom("counter65k", core["cycles_svf"])
	issue["IQ_avfcl_iqdur"] = CreateAvfClCustom("iq_dur", stats_core0["backend"]["active_cycles_svf"])
CreateDmrStructure(issue, "Src1CAM", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
CreateDmrStructure(issue, "Src2CAM", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
CreateTmrStructure(issue, "Src1CAM", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])
CreateTmrStructure(issue, "Src2CAM", \
		stats_core0["backend"]["insts_issued"+ecc_suffix], \
		stats_core0["backend"]["iq_wakeup_access"+ecc_suffix])

reg_read_int = core["stages"]["reg_read_int"]["components"]
reg_read_int["RegReadLogic"]["issue_width"] = knobs_core0["width_issue"]
reg_read_int["RegReadLogic"]["accesses"] = stats_core0["backend"]["active_cycles"]
reg_read_int["RegFile"]["type"] = "CustomRAM"
reg_read_int["RegFile"]["subtype"] = "RegFileMem"
reg_read_int["RegFile"]["words"] = knobs_core0["size_prf_int"]
reg_read_int["RegFile"]["width"] = 64
reg_read_int["RegFile"]["read_ports"] = knobs_core0["width_issue"]*2
reg_read_int["RegFile"]["write_ports"] = knobs_core0["width_issue"]
reg_read_int["RegFile"]["read_accesses"] = stats_core0["exec"]["int_prf_reads"]
reg_read_int["RegFile"]["write_accesses"] = stats_core0["exec"]["int_prf_writes"]
if options.mode_parity:
	reg_read_int["RegFile_parity"] = CreateParityCustom("RegFileParityMem", \
		stats_core0["exec"]["int_prf_reads"+ecc_suffix], \
		stats_core0["exec"]["int_prf_writes"+ecc_suffix])
	reg_read_int["RegFile_paritylogic"] = CreateParityLogicCustom("parity_WIDTH64", \
		stats_core0["exec"]["int_prf_reads"+ecc_suffix], \
		stats_core0["exec"]["int_prf_writes"+ecc_suffix])
if options.mode_ecc or options.mode_parity_plus: # parity plus requires Ecc protection on PRF
	reg_read_int["RegFile_ecc"] = CreateParityCustom("RegFileEccMem", \
		stats_core0["exec"]["int_prf_reads"+ecc_suffix], \
		stats_core0["exec"]["int_prf_writes"+ecc_suffix])
	reg_read_int["RegFile_ecc_checker"], reg_read_int["RegFile_ecc_corrector"] = CreateEccLogicCustom(reg_read_int["RegFile"])
if options.mode_ecc_old:
	reg_read_int["RegFile_ecc"], reg_read_int["RegFile_ecc_checker"], reg_read_int["RegFile_ecc_corrector"] = EccOld(reg_read_int["RegFile"], None,
		stats_core0["exec"]["int_prf_reads"+ecc_suffix],
		stats_core0["exec"]["int_prf_writes"+ecc_suffix])
CreateDmrStructure(reg_read_int, "RegFile", \
		stats_core0["exec"]["int_prf_reads"+ecc_suffix], \
		stats_core0["exec"]["int_prf_writes"+ecc_suffix])
CreateTmrStructure(reg_read_int, "RegFile", \
		stats_core0["exec"]["int_prf_reads"+ecc_suffix], \
		stats_core0["exec"]["int_prf_writes"+ecc_suffix])

execute = core["stages"]["execute"]["components"]
execute["ExecuteMLogic"]["accesses"] = stats_core0["exec"]["agen_accesses"]
execute["ExecuteCtrlLogic"]["accesses"] = stats_core0["exec"]["br_alu_accesses"]
execute["ExecuteSLogic"]["accesses"] = stats_core0["exec"]["s_alu_accesses"]
execute["ExecuteCLogic"]["accesses"] = stats_core0["exec"]["c_alu_accesses"]

lsu = core["stages"]["lsu"]["components"]
lsu["LSULogic"]["issue_width"] = knobs_core0["width_issue"]
lsu["LSULogic"]["retire_width"] = knobs_core0["width_retire"]
lsu["LSULogic"]["size_lsq"] = max(knobs_core0["size_lq"],knobs_core0["size_sq"])
lsu["LSULogic"]["accesses"] = stats_core0["exec"]["dtlb_accesses"]
lsu["LDQ_CAM"]["words"] = knobs_core0["size_lq"]
lsu["LDQ_CAM"]["width"] = s_width["lq"]
lsu["LDQ_CAM"]["read_ports"] = 1
lsu["LDQ_CAM"]["write_ports"] = 1
lsu["LDQ_CAM"]["read_accesses"] = stats_core0["exec"]["loads_processed"]
lsu["LDQ_CAM"]["write_accesses"] = stats_core0["exec"]["stores_processed"]
if options.mode_svf:
	lsu["LDQ_CAM_svf"] = CreateSvf(lsu["LDQ_CAM"], (VBITS_ACC_SIZE+1) + COUNTER_SIZE + METADATA_IMPORTANT_SIZE,
		stats_core0["exec"]["loads_processed_svf"],
		stats_core0["exec"]["stores_processed_svf"])
	lsu["LDQ_svfmult"] = CreateSvfCustom("addshift8x10_lq",
		stats_core0["exec"]["loads_processed_svf"])
	lsu["LDQ_svfvlt"] = CreateSvfCustom("vlt_lq",
		stats_core0["exec"]["loads_processed_svf"])
	lsu["LDQ_svfcounter"] = CreateSvfCustom("counter_lq", core["cycles"])
if options.mode_avfcl:
	lsu["LQ_avfcl_LdqImportanceBit"] = {}
	lsu["LQ_avfcl_LdqImportanceBit"]["type"] = "CustomRAM"
	lsu["LQ_avfcl_LdqImportanceBit"]["subtype"] = "StqImportanceBitMem"
	lsu["LQ_avfcl_LdqImportanceBit"]["words"] = knobs_core0["size_sq"]
	lsu["LQ_avfcl_LdqImportanceBit"]["width"] = 1
	lsu["LQ_avfcl_LdqImportanceBit"]["read_ports"] = 1
	lsu["LQ_avfcl_LdqImportanceBit"]["write_ports"] = 1
	lsu["LQ_avfcl_LdqImportanceBit"]["read_accesses"] = stats_core0["exec"]["loads_processed_svf"]
	lsu["LQ_avfcl_LdqImportanceBit"]["write_accesses"] = stats_core0["exec"]["dtlb_accesses_svf"]
	lsu["LQ_avfcl_counter64"] = CreateAvfClCustom("counter64", core["cycles_svf"])
	lsu["LQ_avfcl_counter65k"] = CreateAvfClCustom("counter65k", core["cycles_svf"])
	lsu["LQ_avfcl_lqdur"] = CreateAvfClCustom("lq_dur", stats_core0["exec"]["dtlb_accesses_svf"])
	lsu["LQ_avfcl_vlt"] = CreateAvfClCustom("vlt_lq", stats_core0["exec"]["loads_processed_svf"])
	lsu["LQ_avfcl_vlt_dp"] = CreateAvfClCustom("vlt_lq_dp", stats_core0["exec"]["loads_processed_svf"])
	lsu["LQ_avfcl_acc"] = CreateAvfClCustom("avfcl_acc_lq", stats_core0["exec"]["loads_processed_svf"])
if options.mode_parity or options.mode_parity_plus:
	lsu["LDQ_DATA_RAM_parity"] = CreateParityCustom("STQ_DATA_RAMParityMem", \
		stats_core0["exec"]["loads_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
	lsu["LDQ_DATA_RAM_paritylogic"] = CreateParityLogicCustom("parity_WIDTH76", \
		stats_core0["exec"]["loads_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
if options.mode_ecc:
	lsu["LDQ_DATA_RAM_ecc"] = CreateParityCustom("STQ_DATA_RAMEccMem", \
		stats_core0["exec"]["loads_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
	lsu["LDQ_CAM_ecc_checker"], lsu["LDQ_CAM_ecc_corrector"] = CreateEccLogicCustom(lsu["LDQ_CAM"])
if options.mode_ecc_old:
	lsu["LDQ_CAM_ecc"], lsu["LDQ_CAM_ecc_checker"], lsu["LDQ_CAM_ecc_corrector"] = EccOld(lsu["LDQ_CAM"],"SRAM",
		stats_core0["exec"]["loads_processed"+ecc_suffix],
		stats_core0["exec"]["stores_processed"+ecc_suffix])
CreateDmrStructure(lsu, "LDQ_CAM", \
		stats_core0["exec"]["loads_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
CreateTmrStructure(lsu, "LDQ_CAM", \
		stats_core0["exec"]["loads_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])

lsu["STQ_CAM"]["words"] = knobs_core0["size_sq"]
lsu["STQ_CAM"]["width"] = 64
lsu["STQ_CAM"]["read_ports"] = 1
lsu["STQ_CAM"]["write_ports"] = 1
lsu["STQ_CAM"]["read_accesses"] = stats_core0["exec"]["stores_processed"]
lsu["STQ_CAM"]["write_accesses"] = stats_core0["exec"]["loads_processed"]
if options.mode_parity or options.mode_parity_plus:
	lsu["STQ_CAM_parity"] = CreateParityCustom("STQ_DATA_RAMParityMem", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["loads_processed"+ecc_suffix])
	lsu["STQ_CAM_paritylogic"] = CreateParityLogicCustom("parity_WIDTH64", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["loads_processed"+ecc_suffix])
if options.mode_ecc:
	lsu["STQ_CAM_ecc"] = CreateParityCustom("STQ_DATA_RAMEccMem", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["loads_processed"+ecc_suffix])
	lsu["STQ_CAM_ecc_checker"], lsu["STQ_CAM_ecc_corrector"] = CreateEccLogicCustom(lsu["STQ_CAM"])
if options.mode_ecc_old:
	lsu["STQ_CAM_ecc"], lsu["STQ_CAM_ecc_checker"], lsu["STQ_CAM_ecc_corrector"] = EccOld(lsu["STQ_CAM"],"SRAM",
		stats_core0["exec"]["stores_processed"+ecc_suffix],
		stats_core0["exec"]["loads_processed"+ecc_suffix])
CreateDmrStructure(lsu, "STQ_CAM", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["loads_processed"+ecc_suffix])
CreateTmrStructure(lsu, "STQ_CAM", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["loads_processed"+ecc_suffix])
lsu["LAST_ST_CAM"]["words"] = knobs_core0["size_sq"]
lsu["LAST_ST_CAM"]["width"] = int(math.ceil(math.log(knobs_core0["size_sq"],2)))
lsu["LAST_ST_CAM"]["read_ports"] = 1
lsu["LAST_ST_CAM"]["write_ports"] = 1
lsu["LAST_ST_CAM"]["read_accesses"] = stats_core0["exec"]["stores_processed"]
lsu["LAST_ST_CAM"]["write_accesses"] = stats_core0["exec"]["stores_processed"]
if options.mode_parity or options.mode_parity_plus:
	lsu["LAST_ST_CAM_parity"] = CreateParityCustom("STQ_DATA_RAMParityMem", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
	lsu["LAST_ST_CAM_paritylogic"] = CreateParityLogicCustom("parity_WIDTH6", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
if options.mode_ecc:
	lsu["LAST_ST_CAM_ecc"] = CreateParityCustom("STQ_DATA_RAMEccMem", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
	lsu["LAST_ST_CAM_ecc_checker"], lsu["LAST_ST_CAM_ecc_corrector"] = CreateEccLogicCustom(lsu["LAST_ST_CAM"])
if options.mode_ecc_old:
	lsu["LAST_ST_CAM_ecc"], lsu["LAST_ST_CAM_ecc_checker"], lsu["LAST_ST_CAM_ecc_corrector"] = EccOld(lsu["LAST_ST_CAM"],"SRAM",
		stats_core0["exec"]["stores_processed"+ecc_suffix],
		stats_core0["exec"]["stores_processed"+ecc_suffix])
CreateDmrStructure(lsu, "LAST_ST_CAM", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
CreateTmrStructure(lsu, "LAST_ST_CAM", \
		stats_core0["exec"]["stores_processed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
lsu["STQ_DATA_RAM"]["type"] = "CustomRAM"
lsu["STQ_DATA_RAM"]["subtype"] = "STQ_DATA_RAMMem"
lsu["STQ_DATA_RAM"]["words"] = knobs_core0["size_sq"]
lsu["STQ_DATA_RAM"]["width"] = s_width["sq"]-lsu["STQ_CAM"]["width"]
lsu["STQ_DATA_RAM"]["read_ports"] = 1
lsu["STQ_DATA_RAM"]["write_ports"] = 1
lsu["STQ_DATA_RAM"]["read_accesses"] = stats_core0["exec"]["stores_committed"]
lsu["STQ_DATA_RAM"]["write_accesses"] = stats_core0["exec"]["stores_processed"]
if options.mode_avfcl:
	lsu["SQ_avfcl_StqImportanceBit"] = {}
	lsu["SQ_avfcl_StqImportanceBit"]["type"] = "CustomRAM"
	lsu["SQ_avfcl_StqImportanceBit"]["subtype"] = "StqImportanceBitMem"
	lsu["SQ_avfcl_StqImportanceBit"]["words"] = knobs_core0["size_sq"]
	lsu["SQ_avfcl_StqImportanceBit"]["width"] = 1
	lsu["SQ_avfcl_StqImportanceBit"]["read_ports"] = 1
	lsu["SQ_avfcl_StqImportanceBit"]["write_ports"] = 1
	lsu["SQ_avfcl_StqImportanceBit"]["read_accesses"] = stats_core0["exec"]["stores_committed_svf"]
	lsu["SQ_avfcl_StqImportanceBit"]["write_accesses"] = stats_core0["backend"]["al_reads_svf"]
	lsu["SQ_avfcl_counter64"] = CreateAvfClCustom("counter64", core["cycles_svf"])
	lsu["SQ_avfcl_counter65k"] = CreateAvfClCustom("counter65k", core["cycles_svf"])
	lsu["SQ_avfcl_sqdur"] = CreateAvfClCustom("sq_dur", stats_core0["exec"]["stores_committed_svf"])
	lsu["SQ_avfcl_vlt"] = CreateAvfClCustom("vlt_sq", stats_core0["exec"]["stores_committed_svf"])
	lsu["SQ_avfcl_vlt_dp"] = CreateAvfClCustom("vlt_sq_dp", stats_core0["exec"]["stores_committed_svf"])
	lsu["SQ_avfcl_acc"] = CreateAvfClCustom("avfcl_acc_sq", stats_core0["exec"]["stores_committed_svf"])
if options.mode_parity or options.mode_parity_plus:
	lsu["STQ_DATA_RAM_parity"] = CreateParityCustom("STQ_DATA_RAMParityMem", \
		stats_core0["exec"]["stores_committed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
	lsu["STQ_DATA_RAM_paritylogic"] = CreateParityLogicCustom("parity_WIDTH76", \
		stats_core0["exec"]["stores_committed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
if options.mode_ecc:
	lsu["STQ_DATA_RAM_ecc"] = CreateParityCustom("STQ_DATA_RAMEccMem", \
		stats_core0["exec"]["stores_committed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
	lsu["STQ_DATA_RAM_ecc_checker"], lsu["STQ_DATA_RAM_ecc_corrector"] = CreateEccLogicCustom(lsu["STQ_DATA_RAM"])
if options.mode_ecc_old:
	lsu["STQ_DATA_RAM_ecc"], lsu["STQ_DATA_RAM_ecc_checker"], lsu["STQ_DATA_RAM_ecc_corrector"] = EccOld(lsu["STQ_DATA_RAM"],None,
		stats_core0["exec"]["stores_committed"+ecc_suffix],
		stats_core0["exec"]["stores_processed"+ecc_suffix])
if options.mode_svf:
	lsu["STQ_DATA_RAM_svf"] = CreateSvf(lsu["STQ_DATA_RAM"], COUNTER_SIZE + METADATA_IMPORTANT_SIZE,
		stats_core0["exec"]["stores_committed_svf"],
		stats_core0["exec"]["stores_processed_svf"])
	lsu["STQ_DATA_RAM_svfmult"] = CreateSvfCustom("addshift8x10_sq",
		stats_core0["exec"]["stores_committed_svf"])
	lsu["STQ_DATA_RAM_svfvlt"] = CreateSvfCustom("vlt_sq",
		stats_core0["exec"]["stores_committed_svf"])
	lsu["STQ_DATA_RAM_svfcounter"] = CreateSvfCustom("counter_sq", core["cycles"])
CreateDmrStructure(lsu, "STQ_DATA_RAM", \
		stats_core0["exec"]["stores_committed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])
CreateTmrStructure(lsu, "STQ_DATA_RAM", \
		stats_core0["exec"]["stores_committed"+ecc_suffix], \
		stats_core0["exec"]["stores_processed"+ecc_suffix])

if (knobs_core0["dc_sets"] < 128):
	print "WARNING: Forcing number of sets of L1DCache to 128 sets due to CACTI limitation."
lsu["L1DCache"]["sets"] = max(128, knobs_core0["dc_sets"])
lsu["L1DCache"]["block_size"] = 2**knobs_core0["dc_line_size"]
lsu["L1DCache"]["ways"] = knobs_core0["dc_ways"]
lsu["L1DCache"]["read_accesses"] = stats_core0["exec"]["dc"]["read_accesses"]
lsu["L1DCache"]["write_accesses"] = stats_core0["exec"]["dc"]["write_accesses"]
lsu["DTLB"] = {}
lsu["DTLB"]["type"] = "Cache"
if (knobs_core0["dtlb_sets"] < 128):
    print "WARNING: Forcing number of sets of DTLB to 128 sets due to CACTI limitation."
lsu["DTLB"]["sets"] = max(128,knobs_core0["dtlb_sets"])
lsu["DTLB"]["block_size"] = 8
lsu["DTLB"]["ways"] = knobs_core0["dtlb_ways"]
lsu["DTLB"]["rw_ports"] = 1
lsu["DTLB"]["banks"] = 1
lsu["DTLB"]["read_accesses"] = stats_core0["exec"]["dtlb"]["read_accesses"]
lsu["DTLB"]["write_accesses"] = stats_core0["exec"]["dtlb"]["write_accesses"]

retire = core["stages"]["retire"]["components"]
retire["RetireLogic"]["fetch_width"] = knobs_core0["width_frontend"]
retire["RetireLogic"]["issue_width"] = knobs_core0["width_issue"]
retire["RetireLogic"]["retire_width"] = knobs_core0["width_retire"]
retire["RetireLogic"]["accesses"] = stats_core0["backend"]["active_cycles"]
retire["ActiveList"]["type"] = "CustomRAM"
retire["ActiveList"]["subtype"] = "ActiveListMem"
retire["ActiveList"]["words"] = knobs_core0["size_al"]
retire["ActiveList"]["width"] = s_width["al"]-76
retire["ActiveList"]["read_ports"] = knobs_core0["width_retire"]
retire["ActiveList"]["write_ports"] = knobs_core0["width_frontend"]
retire["ActiveList"]["read_accesses"] = stats_core0["backend"]["al_reads"]
retire["ActiveList"]["write_accesses"] = stats_core0["backend"]["al_writes"]
if options.mode_avfcl:
	retire["ActiveList_avfcl"] = {}
	retire["ActiveList_avfcl"]["type"] = "CustomRAM"
	retire["ActiveList_avfcl"]["subtype"] = "ActiveListAvfClMem"
	retire["ActiveList_avfcl"]["words"] = knobs_core0["size_al"]
	retire["ActiveList_avfcl"]["width"] = 4
	retire["ActiveList_avfcl"]["read_ports"] = knobs_core0["width_retire"]
	retire["ActiveList_avfcl"]["write_ports"] = knobs_core0["width_frontend"]
	retire["ActiveList_avfcl"]["read_accesses"] = stats_core0["backend"]["al_reads_svf"]
	retire["ActiveList_avfcl"]["write_accesses"] = stats_core0["backend"]["al_writes_svf"]
if options.mode_parity or options.mode_parity_plus:
	retire["ActiveList_parity"] = CreateParityCustom("ActiveListParityMem", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["al_writes"+ecc_suffix])
	retire["ActiveList_paritylogic"] = CreateParityLogicCustom("parity_WIDTH82", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["al_writes"+ecc_suffix])
if options.mode_ecc:
	# this part contains PC of retiring instruction
	retire["ActiveList_ecc"] = CreateParityCustom("ActiveListEccMem", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["al_writes"+ecc_suffix])
	retire["ActiveList_ecc_checker"], retire["ActiveList_ecc_corrector"] = CreateEccLogicCustom(retire["ActiveList"])
if options.mode_ecc_old:
	retire["ActiveList_ecc"], retire["ActiveList_ecc_checker"], retire["ActiveList_ecc_corrector"] = EccOld(retire["ActiveList"], None,
		stats_core0["backend"]["al_reads"+ecc_suffix],
		stats_core0["backend"]["al_writes"+ecc_suffix])
CreateDmrStructure(retire, "ActiveList", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["al_writes"+ecc_suffix])
CreateTmrStructure(retire, "ActiveList", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["al_writes"+ecc_suffix])
retire["ActiveListWb"] = {}
retire["ActiveListWb"]["type"] = "CustomRAM"
retire["ActiveListWb"]["subtype"] = "ActiveListWbMem"
retire["ActiveListWb"]["words"] = knobs_core0["size_al"]
retire["ActiveListWb"]["width"] = 76
retire["ActiveListWb"]["read_ports"] = knobs_core0["width_retire"]
retire["ActiveListWb"]["write_ports"] = knobs_core0["width_frontend"]
retire["ActiveListWb"]["read_accesses"] = stats_core0["backend"]["al_reads"]
retire["ActiveListWb"]["write_accesses"] = stats_core0["backend"]["insts_writtenback"]
if options.mode_avfcl:
	retire["ActiveListWb_avfcl"] = {}
	retire["ActiveListWb_avfcl"]["type"] = "CustomRAM"
	retire["ActiveListWb_avfcl"]["subtype"] = "ActiveListAvfClWbMem"
	retire["ActiveListWb_avfcl"]["words"] = knobs_core0["size_al"]
	retire["ActiveListWb_avfcl"]["width"] = 1
	retire["ActiveListWb_avfcl"]["read_ports"] = knobs_core0["width_retire"]
	retire["ActiveListWb_avfcl"]["write_ports"] = knobs_core0["width_frontend"]
	retire["ActiveListWb_avfcl"]["read_accesses"] = stats_core0["backend"]["al_reads_svf"]
	retire["ActiveListWb_avfcl"]["write_accesses"] = stats_core0["backend"]["insts_writtenback_svf"]
	retire["ROB_avfcl_counter64"] = CreateAvfClCustom("counter256", core["cycles_svf"])
	retire["ROB_avfcl_counter65k"] = CreateAvfClCustom("counter65k", core["cycles_svf"])
	retire["ROB_avfcl_robdur"] = CreateAvfClCustom("rob_dur", stats_core0["backend"]["al_reads_svf"])
	retire["ROB_avfcl_acc"] = CreateAvfClCustom("avfcl_acc_rob", stats_core0["backend"]["al_reads_svf"])
	retire["IQ_avfcl_acc"] = CreateAvfClCustom("avfcl_acc_iq", stats_core0["backend"]["al_reads_svf"])
	retire["IB_avfcl_acc"] = CreateAvfClCustom("avfcl_acc_ib", stats_core0["backend"]["al_reads_svf"])
	retire["ROB_IQ_IB_avfcl_vlt"] = CreateAvfClCustom("vlt", stats_core0["backend"]["al_reads_svf"], knobs_core0["width_retire"])
if options.mode_parity or options.mode_parity_plus:
	retire["ActiveListWb_parity"] = CreateParityCustom("ActiveListWbParityMem", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["insts_writtenback"+ecc_suffix])
	retire["ActiveListWb_paritylogic"] = CreateParityLogicCustom("parity_WIDTH76", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["insts_writtenback"+ecc_suffix])
if options.mode_ecc:
	retire["ActiveListWb_ecc"] = CreateParityCustom("ActiveListWbEccMem", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["insts_writtenback"+ecc_suffix])
	retire["ActiveListWb_ecc_checker"], retire["ActiveListWb_ecc_corrector"] = CreateEccLogicCustom(retire["ActiveListWb"])
CreateDmrStructure(retire, "ActiveListWb", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["insts_writtenback"+ecc_suffix])
CreateTmrStructure(retire, "ActiveListWb", \
		stats_core0["backend"]["al_reads"+ecc_suffix], \
		stats_core0["backend"]["insts_writtenback"+ecc_suffix])

if options.mode_svf:
	live_vector_indicator_size = 0 if options.use_smallmd or options.use_nomd else LIVE_VECTOR_FLAG_SIZE
	retire["ActiveList_dp_svf"] = CreateSvf(retire["ActiveList"],live_vector_indicator_size+COUNTER_SIZE,
		stats_core0["backend"]["al_reads_svf"],
		stats_core0["backend"]["al_writes_svf"])
	retire["ActiveList_wb_svf"] = CreateSvf(retire["ActiveList"],(VBITS_ACC_SIZE+1)+METADATA_SMALL_SIZE+COUNTER_SIZE*2+ARCH_REG_DEPTH_LOG*2,
		stats_core0["backend"]["al_reads_svf"],
		stats_core0["backend"]["al_writes_svf"])
	retire["ActiveList_svfmult"] = CreateSvfCustom("addshift8x10_rob",
		stats_core0["backend"]["al_reads_svf"], knobs_core0["width_retire"])
	retire["ActiveList_svfvlt"] = CreateSvfCustom("vlt_rob",
		stats_core0["backend"]["al_reads_svf"], knobs_core0["width_retire"])
	retire["ActiveList_svfcounter"] = CreateSvfCustom("counter_rob", core["cycles"])
	retire["SPU"] = CreateSvfCustom("spu", core["cycles"])

	# ------------ Please reenable later
	# retire["CalleeStack_svf"] = CreateSvf(fetch_1["RAS"], 9,
	# 	svmresults["return_cnt"], svmresults["call_cnt"], sleepable=False);
	# retire["CalleeStack_svf"]["type"] = "SRAM"
	if live_vector_indicator_size > 0:
		retire["LiveVectorFifo_svf"] = CreateSvf(retire["ActiveList"], 28,
			svmresults["livevectorfifo_pop_cnt"],
			svmresults["livevectorfifo_push_cnt"])
		retire["LiveVectorFifo_svf"]["type"] = "SRAM"
		retire["LiveVectorFifo_svf"]["read_ports"] = 1
		retire["LiveVectorFifo_svf"]["write_ports"] = 1

l2 = core["components"]["L2"]
l2["sets"] = knobs_core0["l2_sets"]
l2["block_size"] = 2**knobs_core0["l2_line_size"]
l2["ways"] = knobs_core0["l2_ways"]
l2["read_accesses"] = stats_core0["l2"]["read_accesses"]
l2["write_accesses"] = stats_core0["l2"]["write_accesses"]

l3 = data["processor"]["components"]["L3"]
l3["sets"] = knobs["processor"]["llc_sets"]
l3["block_size"] = 2**knobs["processor"]["llc_line_size"]
l3["ways"] = knobs["processor"]["llc_ways"]
l3["read_accesses"] = stats["stats"]["llc"]["read_accesses"]
l3["write_accesses"] = stats["stats"]["llc"]["write_accesses"]

# dump updated data into output yaml file
yaml.dump(data, file(options.output_yaml, 'w'))
