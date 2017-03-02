import os
import subprocess
import sys
import yaml

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CWD = os.getcwd()
CONFIG = "svm/o2wlslm"
S2L = (1024*1024)/(8*1024)
PRF_TRAIN = False
MODE = int(os.getenv('AVFR_MODE', 0)) # 0 is default, up to 11

if MODE == 1:
	AVFR_AFFIX = "wprf_wm_8p"
	WITH_MONTECARLO = True
	WITH_PRF = True
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 8
elif MODE == 2:
	AVFR_AFFIX = "wprf_wm_8p_95"
	WITH_MONTECARLO = True
	WITH_PRF = True
	TARGET_R2 = 0.95
	MIN_PARAM = 1
	MAX_PARAM = 8
elif MODE == 3:
	AVFR_AFFIX = "wprf_wm"
	WITH_MONTECARLO = True
	WITH_PRF = True
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 100
elif MODE == 4:
	AVFR_AFFIX = "woprf_wm_8p"
	WITH_MONTECARLO = True
	WITH_PRF = False
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 8
elif MODE == 5:
	AVFR_AFFIX = "woprf_wm_8p_95"
	WITH_MONTECARLO = True
	WITH_PRF = False
	TARGET_R2 = 0.95
	MIN_PARAM = 1
	MAX_PARAM = 8
elif MODE == 6:
	AVFR_AFFIX = "woprf_wm"
	WITH_MONTECARLO = True
	WITH_PRF = False
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 100
elif MODE == 7: # -------------------------------- without montecarlo
	AVFR_AFFIX = "wprf_wom_8p"
	WITH_MONTECARLO = False
	WITH_PRF = True
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 8
elif MODE == 8:
	AVFR_AFFIX = "wprf_wom_8p_95"
	WITH_MONTECARLO = False
	WITH_PRF = True
	TARGET_R2 = 0.95
	MIN_PARAM = 1
	MAX_PARAM = 8
elif MODE == 9:
	AVFR_AFFIX = "wprf_wom"
	WITH_MONTECARLO = False
	WITH_PRF = True
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 100
elif MODE == 10:
	AVFR_AFFIX = "woprf_wom_8p"
	WITH_MONTECARLO = False
	WITH_PRF = False
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 8
elif MODE == 11:
	AVFR_AFFIX = "woprf_wom_8p_95"
	WITH_MONTECARLO = False
	WITH_PRF = False
	TARGET_R2 = 0.95
	MIN_PARAM = 1
	MAX_PARAM = 8
elif MODE == 100:
	AVFR_AFFIX = "woprf_wm"
	WITH_MONTECARLO = True
	WITH_PRF = False
	TARGET_R2 = 0.95
	MIN_PARAM = 1
	MAX_PARAM = 8
elif MODE == 101:
	AVFR_AFFIX = "woprf_wom"
	WITH_MONTECARLO = False
	WITH_PRF = False
	TARGET_R2 = 0.95
	MIN_PARAM = 1
	MAX_PARAM = 8
else: # ----------------------------------- default
	AVFR_AFFIX = "woprf_wom"
	WITH_MONTECARLO = False
	WITH_PRF = False
	TARGET_R2 = 0.9
	MIN_PARAM = 8
	MAX_PARAM = 100

structures = ['IQ', 'ROB', 'LQ', 'SQ', 'RF', 'InstBuff', 'Sys']

excluded_params = []
if not WITH_PRF and PRF_TRAIN:
	excluded_params += [290]

bench_list_spec2006 = [
	{"app": "401.bzip2", "chkpt": ["chkpt1", "chkpt3", "chkpt4", "chkpt5", "chkpt10"]},
	{"app": "403.gcc", "chkpt": ["chkpt2", "chkpt3", "chkpt5", "chkpt6", "chkpt8"]},
	{"app": "429.mcf", "chkpt": ["chkpt2", "chkpt3", "chkpt4", "chkpt5", "chkpt10", "chkpt11"]},
	{"app": "445.gobmk", "chkpt": ["chkpt1", "chkpt2", "chkpt4", "chkpt8", "chkpt10", "chkpt11"]},
	{"app": "456.hmmer", "chkpt": ["chkpt3", "chkpt4", "chkpt5", "chkpt7", "chkpt8"]},
	{"app": "458.sjeng", "chkpt": ["chkpt1", "chkpt2", "chkpt5", "chkpt8", "chkpt9"]},
	{"app": "462.libquantum", "chkpt": ["chkpt2", "chkpt3", "chkpt4", "chkpt5", "chkpt9", "chkpt11"]},
	{"app": "464.h264ref", "chkpt": ["chkpt2", "chkpt3", "chkpt5", "chkpt6", "chkpt9", "chkpt10"]},
	{"app": "471.omnetpp", "chkpt": ["chkpt2", "chkpt3", "chkpt4"]},
	{"app": "473.astar", "chkpt": ["chkpt2", "chkpt3", "chkpt6", "chkpt7", "chkpt10", "chkpt11"]},
	{"app": "483.xalancbmk", "chkpt": ["chkpt2", "chkpt4", "chkpt6", "chkpt7", "chkpt9"]},
]

bench_list_montecarlo = [
	{"app": "222.montecarlo", "chkpt": ["chkpt0", "chkpt1", "chkpt2", "chkpt5", "chkpt7"]},
]

bench_list = bench_list_spec2006
if WITH_MONTECARLO:
	bench_list += bench_list_montecarlo
	# bench_list = bench_list_montecarlo

param_list = {
	1: "occIQ",
	2: "occROB",
	3: "occLQ",
	4: "occSQ",
	5: "occRF",
	6: "occIB",
	7: "occFUsimp",
	8: "occFUcomp",
	9: "occFUctrl",
	10: "occFUmem",
	11: "core_0.backend.active_cycles_p",
	12: "core_0.backend.al_reads_p",
	13: "core_0.backend.al_writes_p",
	14: "core_0.backend.idle_cycles_p",
	15: "core_0.backend.insts_dispatched_p",
	16: "core_0.backend.insts_dispatched_fp_p",
	17: "core_0.backend.insts_dispatched_int_p",
	18: "core_0.backend.insts_dispatched_load_p",
	19: "core_0.backend.insts_dispatched_store_p",
	20: "core_0.backend.insts_issued_p",
	21: "core_0.backend.insts_issued_fp_p",
	22: "core_0.backend.insts_issued_int_p",
	23: "core_0.backend.insts_renamed_p",
	24: "core_0.backend.insts_renamed_fp_p",
	25: "core_0.backend.insts_renamed_int_p",
	26: "core_0.backend.insts_retired_p",
	27: "core_0.backend.insts_retired_executed_p",
	28: "core_0.backend.insts_retired_fp_p",
	29: "core_0.backend.insts_retired_int_p",
	30: "core_0.backend.insts_retired_noped_p",
	31: "core_0.backend.insts_writtenback_p",
	32: "core_0.backend.iq_reads_p",
	33: "core_0.backend.iq_wakeup_access_p",
	34: "core_0.backend.iq_writes_p",
	35: "core_0.backend.nukes_all_p",
	36: "core_0.backend.nukes_memory_inconsistent_p",
	37: "core_0.backend.nukes_syscalls_p",
	38: "core_0.backend.rmt_reads_fp_p",
	39: "core_0.backend.rmt_reads_int_p",
	40: "core_0.backend.rmt_writes_fp_p",
	41: "core_0.backend.rmt_writes_int_p",
	42: "core_0.backend.stalls_al_p",
	43: "core_0.backend.stalls_checker_p",
	44: "core_0.backend.stalls_checkpoints_p",
	45: "core_0.backend.stalls_dispatch_p",
	46: "core_0.backend.stalls_dispatch_no_work_p",
	47: "core_0.backend.stalls_free_list_fp_p",
	48: "core_0.backend.stalls_free_list_int_p",
	49: "core_0.backend.stalls_iq_fp_p",
	50: "core_0.backend.stalls_iq_int_p",
	51: "core_0.backend.stalls_lq_p",
	52: "core_0.backend.stalls_rename_p",
	53: "core_0.backend.stalls_rename_no_work_p",
	54: "core_0.backend.stalls_sq_p",
	55: "core_0.backend.svm_nops_retired_p",
	56: "core_0.exec.agen_accesses_p",
	57: "core_0.exec.alu_accesses_p",
	58: "core_0.exec.br_alu_accesses_p",
	59: "core_0.exec.br_misp_p",
	60: "core_0.exec.c_alu_accesses_p",
	61: "core_0.exec.dc.accesses_p",
	62: "core_0.exec.dc.conflicts_p",
	63: "core_0.exec.dc.denom_p",
	64: "core_0.exec.dc.evictions_p",
	65: "core_0.exec.dc.hits_p",
	66: "core_0.exec.dc.mesi_e_to_s_p",
	67: "core_0.exec.dc.mesi_invalidations_p",
	68: "core_0.exec.dc.mesi_m_to_s_p",
	69: "core_0.exec.dc.mesi_misses_p",
	70: "core_0.exec.dc.mesi_s_to_e_p",
	71: "core_0.exec.dc.misses_p",
	72: "core_0.exec.dc.read_accesses_p",
	73: "core_0.exec.dc.read_conflicts_p",
	74: "core_0.exec.dc.read_denom_p",
	75: "core_0.exec.dc.read_hits_p",
	76: "core_0.exec.dc.read_misses_p",
	77: "core_0.exec.dc.read_prefetcher_hits_p",
	78: "core_0.exec.dc.write_accesses_p",
	79: "core_0.exec.dc.write_conflicts_p",
	80: "core_0.exec.dc.write_denom_p",
	81: "core_0.exec.dc.write_hits_p",
	82: "core_0.exec.dc.write_misses_p",
	83: "core_0.exec.dc.write_prefetcher_hits_p",
	84: "core_0.exec.dc.writebacks_p",
	85: "core_0.exec.dtlb.accesses_p",
	86: "core_0.exec.dtlb.conflicts_p",
	87: "core_0.exec.dtlb.denom_p",
	88: "core_0.exec.dtlb.hits_p",
	89: "core_0.exec.dtlb.misses_p",
	90: "core_0.exec.dtlb.read_accesses_p",
	91: "core_0.exec.dtlb.read_conflicts_p",
	92: "core_0.exec.dtlb.read_denom_p",
	93: "core_0.exec.dtlb.read_hits_p",
	94: "core_0.exec.dtlb.read_misses_p",
	95: "core_0.exec.dtlb.write_accesses_p",
	96: "core_0.exec.dtlb.write_conflicts_p",
	97: "core_0.exec.dtlb.write_denom_p",
	98: "core_0.exec.dtlb.write_hits_p",
	99: "core_0.exec.dtlb.write_misses_p",
	100: "core_0.exec.dtlb_accesses_p",
	101: "core_0.exec.fp_prf_reads_p",
	102: "core_0.exec.fp_prf_writes_p",
	103: "core_0.exec.fpu_accesses_p",
	104: "core_0.exec.int_prf_reads_p",
	105: "core_0.exec.int_prf_writes_p",
	106: "core_0.exec.loads_processed_p",
	107: "core_0.exec.loads_speculated_p",
	108: "core_0.exec.loads_violated_p",
	109: "core_0.exec.mul_accesses_p",
	110: "core_0.exec.pf.hits_p",
	111: "core_0.exec.pf.hits_read_p",
	112: "core_0.exec.pf.hits_write_p",
	113: "core_0.exec.pf.requests_p",
	114: "core_0.exec.pf.requests_read_p",
	115: "core_0.exec.pf.requests_write_p",
	116: "core_0.exec.s_alu_accesses_p",
	117: "core_0.exec.stores_committed_p",
	118: "core_0.exec.stores_processed_p",
	119: "core_0.frontend.active_cycles_p",
	120: "core_0.frontend.bp.btb_conflicts_p",
	121: "core_0.frontend.bp.btb_corr_p",
	122: "core_0.frontend.bp.btb_hits_p",
	123: "core_0.frontend.bp.btb_misses_p",
	124: "core_0.frontend.bp.btb_preds_p",
	125: "core_0.frontend.bp.cond_corr_p",
	126: "core_0.frontend.bp.cond_preds_p",
	127: "core_0.frontend.bp.corr_p",
	128: "core_0.frontend.bp.preds_p",
	129: "core_0.frontend.bp.ras_corr_p",
	130: "core_0.frontend.bp.ras_overflows_p",
	131: "core_0.frontend.bp.ras_preds_p",
	132: "core_0.frontend.ic.accesses_p",
	133: "core_0.frontend.ic.conflicts_p",
	134: "core_0.frontend.ic.denom_p",
	135: "core_0.frontend.ic.evictions_p",
	136: "core_0.frontend.ic.hits_p",
	137: "core_0.frontend.ic.mesi_e_to_s_p",
	138: "core_0.frontend.ic.mesi_invalidations_p",
	139: "core_0.frontend.ic.mesi_m_to_s_p",
	140: "core_0.frontend.ic.mesi_misses_p",
	141: "core_0.frontend.ic.mesi_s_to_e_p",
	142: "core_0.frontend.ic.misses_p",
	143: "core_0.frontend.ic.read_accesses_p",
	144: "core_0.frontend.ic.read_conflicts_p",
	145: "core_0.frontend.ic.read_denom_p",
	146: "core_0.frontend.ic.read_hits_p",
	147: "core_0.frontend.ic.read_misses_p",
	148: "core_0.frontend.ic.read_prefetcher_hits_p",
	149: "core_0.frontend.ic.write_accesses_p",
	150: "core_0.frontend.ic.write_conflicts_p",
	151: "core_0.frontend.ic.write_denom_p",
	152: "core_0.frontend.ic.write_hits_p",
	153: "core_0.frontend.ic.write_misses_p",
	154: "core_0.frontend.ic.write_prefetcher_hits_p",
	155: "core_0.frontend.ic.writebacks_p",
	156: "core_0.frontend.ic_accesses_p",
	157: "core_0.frontend.idle_cycles_p",
	158: "core_0.frontend.insts_decoded_p",
	159: "core_0.frontend.insts_fetched_p",
	160: "core_0.frontend.insts_fetched_calls_p",
	161: "core_0.frontend.insts_fetched_cti_p",
	162: "core_0.frontend.insts_fetched_fp_p",
	163: "core_0.frontend.insts_fetched_int_p",
	164: "core_0.frontend.insts_fetched_loads_p",
	165: "core_0.frontend.insts_fetched_stores_p",
	166: "core_0.frontend.insts_split_p",
	167: "core_0.frontend.itlb.accesses_p",
	168: "core_0.frontend.itlb.conflicts_p",
	169: "core_0.frontend.itlb.denom_p",
	170: "core_0.frontend.itlb.hits_p",
	171: "core_0.frontend.itlb.misses_p",
	172: "core_0.frontend.itlb.read_accesses_p",
	173: "core_0.frontend.itlb.read_conflicts_p",
	174: "core_0.frontend.itlb.read_denom_p",
	175: "core_0.frontend.itlb.read_hits_p",
	176: "core_0.frontend.itlb.read_misses_p",
	177: "core_0.frontend.itlb.write_accesses_p",
	178: "core_0.frontend.itlb.write_conflicts_p",
	179: "core_0.frontend.itlb.write_denom_p",
	180: "core_0.frontend.itlb.write_hits_p",
	181: "core_0.frontend.itlb.write_misses_p",
	182: "core_0.frontend.itlb_accesses_p",
	183: "core_0.frontend.nops_inserted_p",
	184: "core_0.frontend.pf.hits_p",
	185: "core_0.frontend.pf.hits_read_p",
	186: "core_0.frontend.pf.hits_write_p",
	187: "core_0.frontend.pf.requests_p",
	188: "core_0.frontend.pf.requests_read_p",
	189: "core_0.frontend.pf.requests_write_p",
	190: "core_0.frontend.recoveries_p",
	191: "core_0.frontend.stalls_ctiq_p",
	192: "core_0.frontend.stalls_fq_p",
	193: "core_0.frontend.svm_nops_injected_p",
	194: "core_0.frontend.svm_nops_not_injected_p",
	195: "core_0.frontend.svm_nops_skipped_p",
	196: "core_0.l2.accesses_p",
	197: "core_0.l2.conflicts_p",
	198: "core_0.l2.denom_p",
	199: "core_0.l2.evictions_p",
	200: "core_0.l2.hits_p",
	201: "core_0.l2.mesi_e_to_s_p",
	202: "core_0.l2.mesi_invalidations_p",
	203: "core_0.l2.mesi_m_to_s_p",
	204: "core_0.l2.mesi_misses_p",
	205: "core_0.l2.mesi_s_to_e_p",
	206: "core_0.l2.misses_p",
	207: "core_0.l2.read_accesses_p",
	208: "core_0.l2.read_conflicts_p",
	209: "core_0.l2.read_denom_p",
	210: "core_0.l2.read_hits_p",
	211: "core_0.l2.read_misses_p",
	212: "core_0.l2.read_prefetcher_hits_p",
	213: "core_0.l2.write_accesses_p",
	214: "core_0.l2.write_conflicts_p",
	215: "core_0.l2.write_denom_p",
	216: "core_0.l2.write_hits_p",
	217: "core_0.l2.write_misses_p",
	218: "core_0.l2.write_prefetcher_hits_p",
	219: "core_0.l2.writebacks_p",
	220: "core_0.pf.hits_p",
	221: "core_0.pf.hits_read_p",
	222: "core_0.pf.hits_write_p",
	223: "core_0.pf.requests_p",
	224: "core_0.pf.requests_read_p",
	225: "core_0.pf.requests_write_p",
	226: "cycles_p",
	227: "global.dc.accesses_p",
	228: "global.dc.hits_p",
	229: "global.dc.misses_p",
	230: "global.ic.accesses_p",
	231: "global.ic.hits_p",
	232: "global.ic.misses_p",
	233: "global.l2.accesses_p",
	234: "global.l2.hits_p",
	235: "global.l2.misses_p",
	236: "llc.accesses_p",
	237: "llc.conflicts_p",
	238: "llc.denom_p",
	239: "llc.evictions_p",
	240: "llc.hits_p",
	241: "llc.mesi_e_to_s_p",
	242: "llc.mesi_invalidations_p",
	243: "llc.mesi_m_to_s_p",
	244: "llc.mesi_misses_p",
	245: "llc.mesi_s_to_e_p",
	246: "llc.misses_p",
	247: "llc.read_accesses_p",
	248: "llc.read_conflicts_p",
	249: "llc.read_denom_p",
	250: "llc.read_hits_p",
	251: "llc.read_misses_p",
	252: "llc.read_prefetcher_hits_p",
	253: "llc.write_accesses_p",
	254: "llc.write_conflicts_p",
	255: "llc.write_denom_p",
	256: "llc.write_hits_p",
	257: "llc.write_misses_p",
	258: "llc.write_prefetcher_hits_p",
	259: "llc.writebacks_p",
	260: "process_0.insts_retired_p",
	261: "process_0.ipc_p",
	262: "process_0.num_threads_p",
	263: "process_0.thread_0.inst_cache_hits_p",
	264: "process_0.thread_0.insts_retired_p",
	265: "ram.accesses_p",
	266: "ram.conflicts_p",
	267: "ram.denom_p",
	268: "ram.evictions_p",
	269: "ram.hits_p",
	270: "ram.mesi_e_to_s_p",
	271: "ram.mesi_invalidations_p",
	272: "ram.mesi_m_to_s_p",
	273: "ram.mesi_misses_p",
	274: "ram.mesi_s_to_e_p",
	275: "ram.misses_p",
	276: "ram.read_accesses_p",
	277: "ram.read_conflicts_p",
	278: "ram.read_denom_p",
	279: "ram.read_hits_p",
	280: "ram.read_misses_p",
	281: "ram.read_prefetcher_hits_p",
	282: "ram.write_accesses_p",
	283: "ram.write_conflicts_p",
	284: "ram.write_denom_p",
	285: "ram.write_hits_p",
	286: "ram.write_misses_p",
	287: "ram.write_prefetcher_hits_p",
	288: "ram.writebacks_p",
	289: "process_0.ipc_p",
	290: "livevectorcount",
}

def generate(mode1M=False):
	n_param = -1
	sys_index = -1
	rf_index = -1
	suffix = "1M" if mode1M else "8K"

	for bench in bench_list:
		app = bench["app"]
		print "Generating train set for " + app

		fouts = []
		line_cnt = 0

		for train in bench_list:
			if train["app"] == app: continue # don't self train!

			for chkpt in train["chkpt"]:
				dirpath = CONFIG+"/"+train["app"]+"/"+chkpt+"/results"
				favf = open(dirpath+"/avf_group_%s.log"%suffix)
				focc = open(dirpath+"/occ_group_%s.log"%suffix)
				fstat = open(dirpath+"/stats_periodic.log")
				fprf = None
				if PRF_TRAIN: fprf = open(dirpath+"/svf_group_%s.prf_nop2.log"%suffix)
				fstat1M = None

				if mode1M:
					fname = dirpath+"/stats_periodic_1M.log"
					if not os.path.isfile(fname):
						fstat1M = open(fname,"w")

				# skip header for AVF and OCC
				headers = favf.readline().strip().split()
				focc.readline()
				if fprf: fprf.readline()

				if len(fouts) == 0:
					for n, header in enumerate(headers):
						if sys_index == -1 and header == "Sys":
							sys_index = n
						if rf_index == -1 and header == "RF":
							rf_index = n
						fname = CONFIG+"/"+app+"/"+"/train_"+AVFR_AFFIX+"_"+header+"_"+suffix+".log"
						fouts.append(open(fname, "w"))

				chkpt_line_cnt = 0

				# loop across all the lines
				while True:
					avf = favf.readline().strip()
					occ = focc.readline().strip()
					stat = fstat.readline().strip()
					rf = None
					if fprf: rf = fprf.readline()

					if (len(avf) and len(occ) and len(stat) and (rf == None or len(rf))):
						avf_vals = avf.split()
						occ_vals = occ.split()
						stat_vals = stat.split()
						rf_vals = None
						if rf: rf_vals = rf.split()

						if mode1M:
							for n in xrange(S2L-1):
								stat_additional = fstat.readline().strip()
								if (len(stat_additional)):
									vals = stat_additional.split()
									for m in xrange(len(stat_vals)):
										stat_vals[m] = float(stat_vals[m])+float(vals[m])
								else:
									# not enough lines to make group of 1M
									stat_vals = None
							if stat_vals is None:
								break

						if n_param == -1:
							n_param = len(stat_vals)
							n_param += (len(occ_vals) - 1) # exclude Sys
							if rf: n_param += 1 # include PRF based on live-vector count metadata

						for n in xrange(len(avf_vals)):
							fouts[n].write(avf_vals[n])
							for m, val in enumerate(occ_vals):
								if m == sys_index: continue # don't dump Sys occupancy
								fouts[n].write(" "+val)
							for stat in stat_vals:
								fouts[n].write(" "+str(stat))
								if fstat1M and n==0: fstat1M.write(" "+str(stat))
							if rf: fouts[n].write(" "+str(rf_vals[rf_index]))
							fouts[n].write("\n");
							if fstat1M and n==0: fstat1M.write("\n")

						chkpt_line_cnt += 1
					else:
						# one or more file reaches end of file
						break

				favf.close()
				focc.close()
				fstat.close()

				line_cnt += chkpt_line_cnt
				print "Processed %s-%s: %d line(s)" % \
					(train["app"],chkpt,chkpt_line_cnt)

		print "#### Summary ####"
		print "# of parameters = %d" % (n_param)
		print "# of lines = %d" % (line_cnt)

def train(mode1M=False):
	suffix = "1M" if mode1M else "8K"

	for bench in bench_list:
		for st in structures:
			infile = CWD + "/" + CONFIG + '/' + bench["app"] + '/' + 'train_%s_%s_%s.log' % (AVFR_AFFIX, st, suffix)
			outfile = CWD + "/" + CONFIG + '/' + bench["app"] + '/' + 'avfr_%s_%s_%s.yaml' % (AVFR_AFFIX, st, suffix)
			cmd = ['matlab', '-nojvm', '-nodisplay', '-r']
			cmd.append("avfr('%s', '%s', [%s], %f, %d, %d); exit;" % \
				(infile, outfile, \
				",".join(str(v) for v in excluded_params), TARGET_R2, \
				MIN_PARAM, MAX_PARAM))

			# print and run command
			print ' '.join(cmd)
			retcode = subprocess.call(cmd, cwd=SCRIPT_DIR, stdout=sys.stdout)

			if retcode: # non-zero matlab execution
				sys.exit(retcode)

			avfr_res = yaml.load(open(outfile, "r").read())
			print avfr_res

def summary(mode1M=False):
	suffix = "1M" if mode1M else "8K"

	for bench in bench_list:
		print bench
		for st in structures:
			infile = CWD + "/" + CONFIG + '/' + bench["app"] + '/' + 'avfr_%s_%s_%s.yaml' % (AVFR_AFFIX, st, suffix)

			avfr_res = yaml.load(open(infile, "r").read())

			print "Structure: %s" % st
			print "Params: "
			for idx in avfr_res["params_idx"]:
				print "- %s" % param_list[idx]
			print "Bs: "
			for val in avfr_res["FinalB"]:
				print "- %1.30e" % val
			print "R: %f" % avfr_res["FinalR"]

def estimate(mode1M=False):
	suffix = "1M" if mode1M else "8K"
	sys_index = -1
	rf_index = -1

	print "Estimating Q-AVF with group size %s" % ("1M" if mode1M else "8K")

	for bench in bench_list:
		print "  Estimating Q-AVF for %s" % bench["app"]
		# load avfr regression results for each apps
		avfrs = []
		for st in structures:
			favfr = CWD + "/" + CONFIG + '/' + bench["app"] + '/' + 'avfr_%s_%s_%s.yaml' % (AVFR_AFFIX, st, suffix)
			avfr = yaml.load(open(favfr, "r").read())
			avfrs.append(avfr)

		for chkpt in bench["chkpt"]:
			dirpath = CONFIG+"/"+bench["app"]+"/"+chkpt+"/results"
			focc = open(dirpath+"/occ_group_%s.log"%suffix)
			fparam = None
			if mode1M:
				fparam = open(dirpath+"/stats_periodic_1M.log")
			else:
				fparam = open(dirpath+"/stats_periodic.log")
			fprf = None
			if PRF_TRAIN: fprf = open(dirpath+"/svf_group_%s.prf_nop2.log"%suffix)
			favfr = open(dirpath+"/avfr_%s_group_%s.log"%(AVFR_AFFIX, suffix), "w")

			# print header
			for st in structures:
				favfr.write("%s " % st)
			favfr.write("\n");

			# for Occupancy and PRF, skip reading the header line
			headers = focc.readline().strip().split()
			if fprf: fprf.readline()

			if sys_index == -1: # find the Sys column index
				for n, header in enumerate(headers):
					if header == "Sys":
						sys_index = n
					if header == "RF":
						rf_index = n

			# estimate Q-AVF on each group of cycles
			while True:
				occ = focc.readline().strip()
				param = fparam.readline().strip()
				prf = None
				if fprf: prf = fprf.readline().strip()

				if (len(occ) and len(param) and (prf == None or len(prf))):
					occ_vals = occ.split()
					occ_vals.pop(sys_index) # remove sys occupancy
					param_vals = param.split()
					params = occ_vals + param_vals
					if prf: params += [float(prf.split()[rf_index])]
					else: params += [0]
					assert(len(params) == len(param_list))

					for n, st in enumerate(structures):
						val = 0
						for m, b in enumerate(avfrs[n]["FinalB"]):
							if m == 0: val += b
							else:
								idx = avfrs[n]["params_idx"][m-1]-1
								val += b*float(params[idx])
						favfr.write("%f\t" % val)
					favfr.write("\n")
				else:
					break

def correlate_avfcl():
	suffix = "1M"
	rf_index = -1
	outfile = CWD + "/" + 'corr.out'
	data_all = ""
	open(outfile,"w").close() # clear output file

	for bench in bench_list:
		app = bench["app"]
		print "Correlating AVF-CL for " + app

		data_bench = ""

		for chkpt in bench["chkpt"]:
			dirpath = CONFIG+"/"+app+"/"+chkpt+"/results"
			if not os.path.exists(dirpath): continue
			favf = open(dirpath+"/avf_group_%s.log"%suffix)
			fsvf = open(dirpath+"/svf_group_%s.pm.log"%suffix)
			fprf = open(dirpath+"/svf_group_%s.prf.log"%suffix)
			fnc = open(dirpath+"/avfcl_nc_group_%s.log"%suffix)


			# skip header line
			headers = favf.readline().strip().split()
			fsvf.readline()
			fprf.readline()
			fnc.readline()

			if rf_index == -1:
				for n, header in enumerate(headers):
					if header == "RF":
						rf_index = n
						break

			data = ""

			# loop across all the lines
			while True:
				avf = favf.readline().strip()
				svf = fsvf.readline().strip()
				prf = fprf.readline().strip()
				nc = fnc.readline().strip()

				if (len(avf) and len(svf) and len(prf) and len(nc)):
					avf_vals = avf.split()
					svf_vals = svf.split()
					prf_vals = prf.split()
					nc_vals = nc.split()

					data += "%s %s %s %s;" % \
						(avf_vals[rf_index], svf_vals[rf_index], \
							prf_vals[rf_index], nc_vals[rf_index])
				else:
					break

			assert(len(data) > 0)
			data_bench += data

			cmd = ['matlab', '-nojvm', '-nodisplay', '-r']
			cmd.append("corr('%s', [%s], '%s'); exit;" % (app+"-"+chkpt, data, outfile))

			# print and run command
			print ' '.join(cmd)
			retcode = subprocess.call(cmd, cwd=SCRIPT_DIR, stdout=sys.stdout)

			if retcode: # non-zero matlab execution
				sys.exit(retcode)

		assert(len(data_bench) > 0)
		cmd = ['matlab', '-nojvm', '-nodisplay', '-r']
		cmd.append("corr('%s', [%s], '%s'); exit;" % (app, data_bench, outfile))

		# print and run command
		print ' '.join(cmd)
		retcode = subprocess.call(cmd, cwd=SCRIPT_DIR, stdout=sys.stdout)

		if retcode: # non-zero matlab execution
			sys.exit(retcode)

		data_all += data_bench

def dynamic(mode1M=False, threshold=0.1):
	suffix = "1M" if mode1M else "8K"
	sys_idx = -1

	for bench in bench_list:
		app = bench["app"]
		for chkpt in bench["chkpt"]:
			dirpath = CONFIG+"/"+app+"/"+chkpt+"/results"
			if not os.path.exists(dirpath): continue
			favf = open(dirpath+"/avf_group_%s.log"%suffix)
			favfr = open(dirpath+"/avfr_%s_group_%s.log"%(AVFR_AFFIX, suffix))
			fdyn = open(dirpath+"/avfr_%s_avf_%f_group_%s.log"%(AVFR_AFFIX, threshold, suffix), "w")

			# skip header line
			headers = favf.readline().strip().split()
			favfr.readline()

			for n, header in enumerate(headers):
				if header == "Sys" and sys_idx == -1: sys_idx = n
				if n > 0: fdyn.write("\t")
				fdyn.write(header)
			fdyn.write("\n")

			protected = True # initially, assume protection is needed

			# loop across all the lines
			while True:
				avf = favf.readline().strip()
				avfr = favfr.readline().strip()

				if (len(avf) and len(avfr)):
					avf_vals = avf.split()
					avfr_vals = avfr.split()

					for n, val in enumerate(avf_vals):
						if n > 0: fdyn.write("\t")
						if protected: fdyn.write("0.0")
						else: fdyn.write("%s" % val)
					fdyn.write("\n")

					# decide whether protection is needed or not
					protected = float(avfr_vals[sys_idx]) > threshold
				else:
					break
			fdyn.close()

for opt in sys.argv[1:]:
	if opt == "generate": generate()
	elif opt == "generate_1M": generate(True)
	elif opt == "train": train()
	elif opt == "train_1M": train(True)
	elif opt == "summary": summary()
	elif opt == "summary_1M": summary(True)
	elif opt == "estimate": estimate()
	elif opt == "estimate_1M": estimate(True)
	elif opt == "corr": correlate_avfcl()
	elif opt == "dynamic_1M_0.1": dynamic(True, 0.1)
	elif opt == "dynamic_1M_0.05": dynamic(True, 0.05)
	elif opt == "dynamic_1M_0.01": dynamic(True, 0.01)
	else: print "Unknown command '%s'" % opt
