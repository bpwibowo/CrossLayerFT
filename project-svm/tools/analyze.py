import logging
import sys
from svm import Svm

def analyze_perf_base(log=logging.INFO):
	svm = Svm(log)

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", "AVF-CL")
	svm.Load("svm.nofirstlane/o2wlslm", "AVF-CL.nfl")
	svm.Load("svm.nonop/o2wlslm", "AVF-CL.nonop")

	svm.Process(vf=False)

	svm.Plot(mode="bargraph", type="perf_norm",
		ylabel="Normalized execution time",
		items=[
			svm.BarItem("AVF-CL.nfl", color='salmon'),
			svm.BarItem("AVF-CL", color='red'),
			svm.BarItem("AVF-CL.nonop", color='grey'),
			],
		filename="norm_exec_time_base.pdf",
		geomean=True, yrange=[0.9, 1.15], size=[7, 4])

def analyze_vf_mae_base(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "avfcl", "avfcl_nm", "avfe", "avfhp", "occ", "avfcl_nc", "svf.prf", "svf.prf_nop2", \
		"avfr_woprf_wom_8p_95", "avfr_woprf_wom_8p", "avfr_woprf_wom",\
		"avfr_wprf_wom_8p_95", "avfr_wprf_wom_8p", "avfr_wprf_wom"]
	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	ignored_apps=["222.montecarlo"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods},
		ignored_apps=ignored_apps)

	svm.Process(vf=False)

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
		ylabel="Mean Absolute Error",
		items=[
			svm.BarItem("svm/o2wlslm", "AVF-CL", "avfcl", "red"),
			svm.BarItem("svm/o2wlslm", "AVF-HP", "avfhp", 'green'),
			svm.BarItem("svm/o2wlslm", "AVF-R", "avfr_woprf_wom", 'cornflowerblue'),
			svm.BarItem("svm/o2wlslm", "AVF-CL.nm", "avfcl_nm", 'grey'),
			],
		arithmean=True, structures=structures, size=[9,4],
		legend_loc="outside",
		filename="vf_svm_base_mae_$structure$.pdf")

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors_ratio",
		ylabel="Mean Absolute Error Ratio",
		items=[
			svm.BarItem("svm/o2wlslm", "AVF-CL", "avfcl", "red"),
			svm.BarItem("svm/o2wlslm", "AVF-HP", "avfhp", 'green'),
			svm.BarItem("svm/o2wlslm", "AVF-R", "avfr_woprf_wom", 'cornflowerblue'),
			svm.BarItem("svm/o2wlslm", "AVF-CL.nm", "avfcl_nm", 'grey'),
			],
		arithmean=True, structures=structures, size=[9,4],
		legend_loc="outside",
		filename="vf_svm_base_maer_$structure$.pdf")

def analyze_vf_ts_base(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "avfcl", "avfhp", "avfr_woprf_wom"]
	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process(vf=False)

	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	structure_aliases = ["iq", "rob", "lq", "sq", "rf", "ib", "combined"]
	aliases = {"avf": "AVF", "avfcl": "AVF-CL", "occ": "Occupancy",
		"avfe": "AVF-HP.old", "avfhp": "AVF-HP", "svf.prf": "AVF-CL.prf", "svf_nc": "SVF-NC", "avfr_woprf_wom": "AVF-R", "avfr_woprf_wm": "AVF-R"}
	colors = {"avf": "black", "avfcl": "red", "occ": "purple", "svf_nc": "blue", "avfr_woprf_wom": "green", "avfr_woprf_wm": "green",
		"avfe": "cyan", "avfhp": "blue", "svf.prf": "cornflowerblue"}
	linestyles = {"avf": "-", "avfcl": "--", "occ": "-.", "avfhp": "-",
		"avfe": "-", "svf.prf": ":", "avfcl": "-", "svf_nc": "-", "avfr_woprf_wom": "-", "avfr_woprf_wm": "-"}
	linewidths = {"avf": 5, "avfcl": 2, "occ": 2, "avfe": 2, "avfhp": 2, "svf.prf": 2, "avfcl": 2, "svf_nc": 2, "avfr_woprf_wom": 2, "avfr_woprf_wm": 2}

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		structure_aliases=structure_aliases, filename="ts_vf",
		linestyles=linestyles, linewidths=linewidths)

def analyze_vf_mae_montecarlo(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "avfcl", "avfcl_nm", "avfe", "avfhp", "occ", "avfcl_nc", "svf.prf", "svf.prf_nop2",\
		"avfr_woprf_wm_8p_95", "avfr_woprf_wm_8p", "avfr_woprf_wm",\
		"avfr_wprf_wm_8p_95", "avfr_wprf_wm_8p", "avfr_wprf_wm"]
	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	ignored_apps=["401.bzip2", "403.gcc", "429.mcf", "445.gobmk", "456.hmmer", "458.sjeng",
		"462.libquantum", "464.h264ref", "471.omnetpp", "473.astar", "483.xalancbmk"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods},
		ignored_apps=ignored_apps)
	svm.Load("svm.nosif/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods},
		ignored_apps=ignored_apps)

	svm.Process(vf=False)

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
		ylabel="Mean Absolute Error",
		items=[
			svm.BarItem("svm/o2wlslm", "AVF-CL", "avfcl", "red"),
			svm.BarItem("svm/o2wlslm", "AVF-HP", "avfhp", 'green'),
			svm.BarItem("svm/o2wlslm", "AVF-R", "avfr_woprf_wm", 'cornflowerblue'),
			svm.BarItem("svm/o2wlslm", "AVF-CL.nm", "avfcl_nm", 'grey'),
			],
		arithmean=True, structures=structures, size=[9,4],
		legend_loc="outside",
		filename="vf_svm_montecarlo_mae_$structure$.pdf")

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
		ylabel="Mean Absolute Error",
		items=[
			svm.BarItem("svm.nosif/o2wlslm", "AVF-CL", "avfcl", "red"),
			svm.BarItem("svm.nosif/o2wlslm", "AVF-HP", "avfhp", 'green'),
			svm.BarItem("svm.nosif/o2wlslm", "AVF-R", "avfr_woprf_wm", 'cornflowerblue'),
			svm.BarItem("svm.nosif/o2wlslm", "AVF-CL.nm", "avfcl_nm", 'grey'),
			],
		arithmean=True, structures=structures, size=[9,4],
		legend_loc="outside",
		filename="vf_svm_montecarlo_mae_nosif_$structure$.pdf")

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors_ratio",
		ylabel="Mean Absolute Error Ratio",
		items=[
			svm.BarItem("svm/o2wlslm", "AVF-CL", "avfcl", "red"),
			svm.BarItem("svm/o2wlslm", "AVF-HP", "avfhp", 'green'),
			svm.BarItem("svm/o2wlslm", "AVF-R", "avfr_woprf_wm", 'cornflowerblue'),
			svm.BarItem("svm/o2wlslm", "AVF-CL.nm", "avfcl_nm", 'grey'),
			],
		arithmean=True, structures=structures, size=[9,4],
		legend_loc="outside",
		filename="vf_svm_montecarlo_maer_$structure$.pdf")

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors_ratio",
		ylabel="Mean Absolute Error Ratio",
		items=[
			svm.BarItem("svm.nosif/o2wlslm", "AVF-CL", "avfcl", "red"),
			svm.BarItem("svm.nosif/o2wlslm", "AVF-HP", "avfhp", 'green'),
			svm.BarItem("svm.nosif/o2wlslm", "AVF-R", "avfr_woprf_wm", 'cornflowerblue'),
			svm.BarItem("svm.nosif/o2wlslm", "AVF-CL.nm", "avfcl_nm", 'grey'),
			],
		arithmean=True, structures=structures, size=[9,4],
		legend_loc="outside",
		filename="vf_svm_montecarlo_maer_nosif_$structure$.pdf")

	# svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
	# 	ylabel="Mean Absolute Error",
	# 	items=[
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-CL(1)", "avfcl", "red"),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-CL.prf(2)", "svf.prf", 'salmon'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-CL.nc(3)","avfcl_nc","orange"),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-HP(4)", "avfhp", 'green'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-HP.self(5)", "avfe", 'limegreen'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-R-woprf(6)", "avfr_woprf_wm", 'cornflowerblue'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-R-woprf_8p(7)", "avfr_woprf_wm_8p", 'blue'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-R-woprf_1_8p_95(8)", "avfr_woprf_wm_8p_95", 'darkblue'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-R-wprf(9)", "avfr_wprf_wm", 'sandybrown'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-R-wprf_8p(10)", "avfr_wprf_wm_8p", 'goldenrod'),
	# 		svm.BarItem("svm.montecarlo.nosif/o2wlslm", "AVF-CL-mbits", "avfcl", 'saddlebrown'),
	# 		svm.BarItem("svm.prf2.nosif/o2wlslm", "AVF-CL.prf2", "svf.prf_nop2", 'black'),
	# 		],
	# 	arithmean=True, structures=structures, size=[9,4],
	# 	legend_loc="outside",
	# 	filename="vf_svm_montecarlo_mae_$structure$_nomark.pdf")

def analyze_vf_ts_montecarlo(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "avfcl", "avfhp", "avfr_woprf_wm"]
	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	ignores = ['401.bzip2', '403.gcc', '429.mcf', '445.gobmk', '456.hmmer', '458.sjeng', \
		'462.libquantum', '464.h264ref', '471.omnetpp', '473.astar', '483.xalancbmk']

	svm.Load("coretime/o2wlslm", "baseline", ignored_apps=ignores)
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods},
		ignored_apps=ignores)
	svm.Load("svm.nosif/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods},
		ignored_apps=ignores)

	svm.Process(vf=False)

	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	structure_aliases = ["iq", "rob", "lq", "sq", "rf", "ib", "combined"]
	aliases = {"avf": "AVF", "avfcl": "AVF-CL", "occ": "Occupancy",
		"avfe": "AVF-HP.old", "avfhp": "AVF-HP", "svf.prf": "AVF-CL.prf", "avfcl_nc": "AVF-CL.nc", "avfr_woprf_wm_8p_95": "avfr_woprf_8p_95", "avfr_woprf_wm": "AVF-R"}
	colors = {"avf": "black", "avfcl": "red", "occ": "purple", "avfcl_nc": "blue", "avfr_woprf_wm_8p_95": "orange", "avfr_woprf_wm": "green",
		"avfe": "cyan", "avfhp": "blue", "svf.prf": "cornflowerblue"}
	linestyles = {"avf": "-", "avfcl": "--", "occ": "-.", "avfhp": "-",
		"avfe": "-", "svf.prf": ":", "avfcl_nc": "-", "avfr_woprf_wm_8p_95": "-", "avfr_woprf_wm": "-"}
	linewidths = {"avf": 5, "avfcl": 2, "occ": 2, "avfe": 2, "avfhp": 2, "svf.prf": 2, "avfcl_nc": 2, "avfr_woprf_wm_8p_95": 2, "avfr_woprf_wm": 2}

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm/o2wlslm"},
			{"config": "svm.nosif/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		structure_aliases=structure_aliases, filename="ts_vf_montecarlo",
		linestyles=linestyles, linewidths=linewidths)

def analyze_power_base(log=logging.INFO):
	svm = Svm(log)

	svm.LoadPower("coretime/o2wlslm", "ppa.output.yaml", "baseline")
	svm.LoadPower("coretime/o2wlslm", "ppa.output.parity.yaml", "parity")
	svm.LoadPower("coretime/o2wlslm", "ppa.output.parity_plus.yaml", "parity_plus")
	svm.LoadPower("coretime/o2wlslm", "ppa.output.ecc.yaml", "ecc")
	svm.LoadPower("svm/o2wlslm", "ppa.output.avfcl.yaml", "AVF-CL.base")

	svm.ProcessPower()

	svm.Plot(mode="bargraph", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=[
			svm.BarItem("AVF-CL.base", "AVF-CL(1)", color='red'),
			svm.BarItem("parity", "parity(2)", color='blue'),
			svm.BarItem("parity_plus", "parity_plus(3)", color='darkblue'),
			svm.BarItem("ecc", "ecc(4)", color='green'),
			],
		filename="norm_energy_base.pdf",
		size=[7, 4], legend_loc=None, legend_cols=6)

def analyze_perf_dynamic(log=logging.INFO):
	svm = Svm(log)
	ignored_apps = ["222.montecarlo"]

	svm.Load("coretime/o2wlslm", "baseline", ignored_apps=ignored_apps)
	svm.Load("svm/o2wlslm", "AVF-CL", ignored_apps=ignored_apps)
	svm.Load("svm.dynamic.0.1.svfonly/o2wlslm", "AVF-CL.10", ignored_apps=ignored_apps)
	svm.Load("svm.dynamic.0.05.svfonly/o2wlslm", "AVF-CL.5", ignored_apps=ignored_apps)
	svm.Load("svm.dynamic.0.01.svfonly/o2wlslm", "AVF-CL.1", ignored_apps=ignored_apps)

	svm.Process(vf=False)

	svm.Plot(mode="bargraph", type="perf_norm",
		ylabel="Normalized execution time",
		items=[
			svm.BarItem("AVF-CL", color='red'),
			svm.BarItem("AVF-CL.10", color='yellow'),
			svm.BarItem("AVF-CL.5", color='orange'),
			svm.BarItem("AVF-CL.1", color='brown'),
			],
		filename="norm_exec_time_dynamic.pdf",
		geomean=True, yrange=[0.9, 1.15], size=[7, 4])

def analyze_power_dynamic(log=logging.INFO):
	svm = Svm(log)
	ignored_apps = ["222.montecarlo"]

	svm.LoadPower("svm.dynamic.0.1.svfonly/o2wlslm", "ppa.output.dynamic.yaml", "AVF-CL.dynamic.10", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.05.svfonly/o2wlslm", "ppa.output.dynamic.yaml", "AVF-CL.dynamic.5", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.01.svfonly/o2wlslm", "ppa.output.dynamic.yaml", "AVF-CL.dynamic.1", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.yaml", "baseline", ignored_apps=ignored_apps)
	#svm.LoadPower("coretime/o2wlslm", "ppa.output.parity.yaml", "parity", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.parity_plus.yaml", "parity_plus", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.ecc.yaml", "ecc", ignored_apps=ignored_apps)
	svm.LoadPower("svm/o2wlslm", "ppa.output.avfcl.yaml", "AVF-CL.base", ignored_apps=ignored_apps)

	svm.ProcessPower()

	svm.Plot(mode="bargraph", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=[
			svm.BarItem("AVF-CL.base", "AVF-CL(1)", color='red'),
			#svm.BarItem("parity", "parity(2)", color='blue'),
			svm.BarItem("parity_plus", "parity_plus(2)", color='darkblue'),
			svm.BarItem("ecc", "ecc(3)", color='green'),
			svm.BarItem("AVF-CL.dynamic.10", "dynamic.10(4)", color='yellow'),
			svm.BarItem("AVF-CL.dynamic.5", "dynamic.5(5)", color='orange'),
			svm.BarItem("AVF-CL.dynamic.1", "dynamic.1(6)", color='brown'),
			],
		filename="norm_energy_dynamic.pdf",
		size=[7, 4], legend_loc=None, legend_cols=6)

def analyze_power_dynamic_mr(log=logging.INFO):
	svm = Svm(log)
	ignored_apps = ["222.montecarlo"]

	svm.LoadPower("svm.dynamic.0.1/o2wlslm", "ppa.output.dynamic.yaml", "AVF-CL.dynamic.10", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.05/o2wlslm", "ppa.output.dynamic.yaml", "AVF-CL.dynamic.5", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.01/o2wlslm", "ppa.output.dynamic.yaml", "AVF-CL.dynamic.1", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.1/o2wlslm", "ppa.output.dynamic_ecc.yaml", "AVF-CL.dynamic_ecc.10", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.05/o2wlslm", "ppa.output.dynamic_ecc.yaml", "AVF-CL.dynamic_ecc.5", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.01/o2wlslm", "ppa.output.dynamic_ecc.yaml", "AVF-CL.dynamic_ecc.1", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.1/o2wlslm", "ppa.output.dynamic_dmr_st.yaml", "AVF-CL.dynamic_dmr_st.10", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.05/o2wlslm", "ppa.output.dynamic_dmr_st.yaml", "AVF-CL.dynamic_dmr_st.5", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.01/o2wlslm", "ppa.output.dynamic_dmr_st.yaml", "AVF-CL.dynamic_dmr_st.1", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.1/o2wlslm", "ppa.output.dynamic_tmr_st.yaml", "AVF-CL.dynamic_tmr_st.10", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.05/o2wlslm", "ppa.output.dynamic_tmr_st.yaml", "AVF-CL.dynamic_tmr_st.5", ignored_apps=ignored_apps)
	svm.LoadPower("svm.dynamic.0.01/o2wlslm", "ppa.output.dynamic_tmr_st.yaml", "AVF-CL.dynamic_tmr_st.1", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.yaml", "baseline", ignored_apps=ignored_apps)
	#svm.LoadPower("coretime/o2wlslm", "ppa.output.parity.yaml", "parity", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.parity_plus.yaml", "parity_plus", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.ecc.yaml", "ecc", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.dmr_st.yaml", "dmr_st", ignored_apps=ignored_apps)
	svm.LoadPower("coretime/o2wlslm", "ppa.output.tmr_st.yaml", "tmr_st", ignored_apps=ignored_apps)
	svm.LoadPower("svm/o2wlslm", "ppa.output.avfcl.yaml", "AVF-CL.base", ignored_apps=ignored_apps)

	svm.ProcessPower()

	svm.Plot(mode="bargraph", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=[
			svm.BarItem("AVF-CL.base", "AVF-CL(1)", color='red'),
			#svm.BarItem("parity", "parity(2)", color='blue'),
			svm.BarItem("parity_plus", "parity_plus(2)", color='darkblue'),
			svm.BarItem("ecc", "ecc(3)", color='green'),
			svm.BarItem("AVF-CL.dynamic.10", "dynamic.10(4)", color='yellow'),
			svm.BarItem("AVF-CL.dynamic.5", "dynamic.5(5)", color='orange'),
			svm.BarItem("AVF-CL.dynamic.1", "dynamic.1(6)", color='brown'),
			svm.BarItem("AVF-CL.dynamic_ecc.10", "dynamic_ecc.10(4)", color='yellow'),
			svm.BarItem("AVF-CL.dynamic_ecc.5", "dynamic_ecc.5(5)", color='orange'),
			svm.BarItem("AVF-CL.dynamic_ecc.1", "dynamic_ecc.1(6)", color='brown'),
			svm.BarItem("AVF-CL.dynamic_dmr_st.10", "dynamic_dmr_st.10(4)", color='yellow'),
			svm.BarItem("AVF-CL.dynamic_dmr_st.5", "dynamic_dmr_st.5(5)", color='orange'),
			svm.BarItem("AVF-CL.dynamic_dmr_st.1", "dynamic_dmr_st.1(6)", color='brown'),
			svm.BarItem("AVF-CL.dynamic_tmr_st.10", "dynamic_tmr_st.10(4)", color='yellow'),
			svm.BarItem("AVF-CL.dynamic_tmr_st.5", "dynamic_tmr_st.5(5)", color='orange'),
			svm.BarItem("AVF-CL.dynamic_tmr_st.1", "dynamic_tmr_st.1(6)", color='brown'),
			svm.BarItem("dmr_st", "dmr_st(7)", color='gray'),
			svm.BarItem("tmr_st", "tmr_st(8)", color='black'),
			],
		filename="norm_energy_dynamic_plus.pdf",
		size=[7, 4], legend_loc=None, legend_cols=6)

def analyze_vf_dynamic(log=logging.INFO):
	svm = Svm(log)
	ignored_apps = ["222.montecarlo"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", ts_info={"active_size":1024*1024, "rarate":1, "methods":["avf", "avfr_woprf_wom_avf_0.100000", "avfr_woprf_wom_avf_0.050000", "avfr_woprf_wom_avf_0.010000", ]}, ignored_apps=ignored_apps)
	# svm.Load("svm.dynamic.0.1/o2wlslm", ts_info={"active_size":1024*1024, "rarate":1, "methods":["avf"]}, ignored_apps=ignored_apps)
	# svm.Load("svm.dynamic.0.05/o2wlslm", ts_info={"active_size":1024*1024, "rarate":1, "methods":["avf"]}, ignored_apps=ignored_apps)
	# svm.Load("svm.dynamic.0.01/o2wlslm", ts_info={"active_size":1024*1024, "rarate":1, "methods":["avf"]}, ignored_apps=ignored_apps)

	svm.Process()

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_vfs",
		ylabel="AVF",
		items=[
			svm.BarItem("svm/o2wlslm", "baseline", "avf", "red"),
			# svm.BarItem("svm.dynamic.0.1/o2wlslm", "dynamic.10%", "avf", 'salmon'),
			# svm.BarItem("svm.dynamic.0.05/o2wlslm", "dynamic.5%","avf","orange"),
			# svm.BarItem("svm.dynamic.0.01/o2wlslm", "dynamic.1%)", "avf", 'green'),
			svm.BarItem("svm/o2wlslm", "AVF-R.dyn.10%", "avfr_woprf_wom_avf_0.100000", 'blue'),
			svm.BarItem("svm/o2wlslm", "AVF-R.dyn.5%","avfr_woprf_wom_avf_0.050000","darkblue"),
			svm.BarItem("svm/o2wlslm", "AVF-R.dyn.1%)", "avfr_woprf_wom_avf_0.010000", 'cyan'),
			],
		arithmean=True, structures=["sys"], size=[9,4],
		legend_loc="outside",
		filename="vf_svm_dynamic_base_$structure$.pdf")

def analyze_area_base(log=logging.INFO):
	svm = Svm(log)

	svm.LoadArea("coretime/o2wlslm", "ppa.output.yaml", "baseline")
	svm.LoadArea("coretime/o2wlslm", "ppa.output.parity.yaml", "parity")
	svm.LoadArea("coretime/o2wlslm", "ppa.output.parity_plus.yaml", "parity_plus")
	svm.LoadArea("coretime/o2wlslm", "ppa.output.ecc.yaml", "ecc")
	svm.LoadArea("svm/o2wlslm", "ppa.output.avfcl.yaml", "AVF-CL.base")

	grouping = [
		{"name": "AVF-CL.SRAMs", "color": "red", "suffix": ["_svf"]},
		{"name": "AVF-CL.logics", "color": "lightsalmon", "suffix": ["_svfmult", "_svfvlt", "_svfcounter", "spu"]},
		{"name": "M-TLB", "color": "green", "suffix": ["MTLB"]},
		{"name": "M-cache", "color": "limegreen", "suffix": ["L1MCache"]},
		{"name": "ECC.SRAMs", "color": "black", "suffix": ["_ecc"]},
		{"name": "ECC.logics", "color": "gray", "suffix": ["_ecc_checker", "_ecc_corrector"]},
	]
	grouping_detailed = [
		{"name": "AVF-CL.SRAMs.IBuff", "color": "maroon", "suffix": ["InstBuff_svf"]},
		{"name": "AVF-CL.SRAMs.RF", "color": "darkred", "suffix": ["AMT_svf"]},
		{"name": "AVF-CL.SRAMs.IQ", "color": "red", "suffix": ["PayloadRAM_svf"]},
		{"name": "AVF-CL.SRAMs.LQ", "color": "firebrick", "suffix": ["LDQ_CAM_svf"]},
		{"name": "AVF-CL.SRAMs.SQ", "color": "tomato", "suffix": ["STQ_DATA_RAM_svf"]},
		{"name": "AVF-CL.SRAMs.ROB", "color": "salmon", "suffix": ["ActiveList_dp_svf", "ActiveList_wb_svf"]},
		{"name": "AVF-CL.logics", "color": "lightsalmon", "suffix": ["_svfmult", "_svfvlt", "_svfcounter", "spu"]},
		{"name": "M-TLB", "color": "green", "suffix": ["MTLB"]},
		{"name": "M-cache", "color": "limegreen", "suffix": ["L1MCache"]},
		{"name": "ECC.SRAMs", "color": "black", "suffix": ["_ecc"]},
		{"name": "ECC.logics", "color": "gray", "suffix": ["_ecc_checker", "_ecc_corrector"]},
	]
	others = {"name": "I-cache overhead", "color": "cornflowerblue"}
	svm.ProcessArea(grouping, others)

	size = [3,4]
	yrange=[0,0.15]
	legend_loc = "best"
	svm.Plot(mode="bargraph_single_value", type="area_norm_ovd",
		ylabel="Normalized area overhead",
		items=[
			svm.BarItem("AVF-CL.base", "AVF-CL(1)", color='red'),
			svm.BarItem("parity", "parity(2)", color='blue'),
			svm.BarItem("parity_plus", "parity_plus(3)", color='darkblue'),
			svm.BarItem("ecc", "ecc(4)", color='green'),
			],
		filename="norm_area.pdf", size=size,
		legend_loc=legend_loc)

	# colors = [x["color"] for x in grouping]
	# colors.append(others["color"])
	# svm.Plot(mode="bargraph_single_stack", type="area_breakdown_norm",
	# 	ylabel="Normalized area overhead",
	# 	items=[
	# 		svm.BarItem("AVF-CL.base", "AVF-CL.base"),
	# 		svm.BarItem("AVF-CL.ic", "AVF-CL.ic"),
	# 		svm.BarItem("AVF-CL.sm", "AVF-CL.sm"),
	# 		svm.BarItem("AVF-CL.sm.ic", "AVF-CL.sm.ic"),
	# 		svm.BarItem("AVF-CL.nm", "AVF-CL.nm"),
	# 		svm.BarItem("ECC", "ECC"),
	# 		],
	# 	colors=colors,
	# 	filename="norm_area_breakdown.pdf", size=size, yrange=yrange,
	# 	legend_loc=legend_loc)

def analyze_rob_opcode(log=logging.INFO):
	svm = Svm(log)
	svm.RobOpcode("svm/o2wlslm")

# ---- OLD code ----------------------------------------------------------------

def analyze_perf_base_old(log=logging.INFO):
	svm = Svm(log)

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", "AVF-CL.base")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic")
	svm.Load("svm.small_mcache/o2wlslm", "AVF-CL.sm")
	svm.Load("svm.small_mcache.icache/o2wlslm", "AVF-CL.sm.ic")
	svm.Load("svm.no_mcache/o2wlslm", "AVF-CL.nm")

	svm.Process()

	svm.Plot(mode="bargraph", type="perf_norm",
		ylabel="Normalized execution time",
		items=[
			svm.BarItem("AVF-CL.base", color='red'),
			svm.BarItem("AVF-CL.ic", color='salmon'),
			svm.BarItem("AVF-CL.sm", color='green'),
			svm.BarItem("AVF-CL.sm.ic", color='limegreen'),
			svm.BarItem("AVF-CL.nm", color='cornflowerblue'),
			],
		filename="norm_exec_time_base.pdf",
		geomean=True, yrange=[0.9, 1.15], size=[7, 4])

def analyze_perf_nopinjection(log=logging.INFO):
	svm = Svm(log)

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", "AVF-CL.base")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic")
	svm.Load("svm.nop5/o2wlslm", "AVF-CL.nop5")
	svm.Load("svm.nop20/o2wlslm", "AVF-CL.nop20")
	svm.Load("svm.nop_inject1/o2wlslm", "AVF-CL.nop_inject1")
	svm.Load("svm.nop_inject2/o2wlslm", "AVF-CL.nop_inject2")
	svm.Load("svm.nop_inject_cache/o2wlslm", "AVF-CL.nop_inject_cache")
	svm.Load("svm.nop_inject_decode/o2wlslm", "AVF-CL.nop_inject_decode")
	svm.Load("svm.nop_inject_first_fetch/o2wlslm", "AVF-CL.nop_inject_first_fetch")
	svm.Load("svm.nop_scheme5.default/o2wlslm", "AVF-CL.nop_scheme5.default")
	svm.Load("svm.nop_scheme5.firstskip/o2wlslm", "AVF-CL.nop_scheme5.firstskip")
	# svm.Load("svm.small_mcache/o2wlslm", "AVF-CL.sm")
	# svm.Load("svm.small_mcache.icache/o2wlslm", "AVF-CL.sm.ic")
	# svm.Load("svm.no_mcache/o2wlslm", "AVF-CL.nm")

	svm.Process(vf=False)

	svm.Plot(mode="bargraph", type="perf_norm",
		ylabel="Normalized execution time",
		items=[
			svm.BarItem("AVF-CL.base", color='red'),
			svm.BarItem("AVF-CL.ic", color='salmon'),
			svm.BarItem("AVF-CL.nop5", color='green'),
			svm.BarItem("AVF-CL.nop20", color='limegreen'),
			svm.BarItem("AVF-CL.nop_inject1", color='cornflowerblue'),
			svm.BarItem("AVF-CL.nop_inject2", color='blue'),
			svm.BarItem("AVF-CL.nop_inject_cache", color='black'),
			svm.BarItem("AVF-CL.nop_inject_decode", color='gray'),
			svm.BarItem("AVF-CL.nop_inject_first_fetch", color='orange'),
			svm.BarItem("AVF-CL.nop_scheme5.default", color='purple'),
			svm.BarItem("AVF-CL.nop_scheme5.firstskip", color='darkred'),
			# svm.BarItem("AVF-CL.sm", color='green'),
			# svm.BarItem("AVF-CL.sm.ic", color='limegreen'),
			# svm.BarItem("AVF-CL.nm", color='cornflowerblue'),
			],
		filename="norm_exec_time_nopinject.pdf",
		geomean=True, yrange=[0.9, 1.15], size=[7, 4])

def analyze_perf_nopinjection_4w(log=logging.INFO):
	svm = Svm(log)

	svm.Load("coretime/o4wlslm", "baseline")
	svm.Load("svm.nop_inject1/o4wlslm", "AVF-CL.nop_inject1")
	svm.Load("svm.nop_inject_cache/o4wlslm", "AVF-CL.nop_inject_cache")
	svm.Load("svm.nop_inject_decode/o4wlslm", "AVF-CL.nop_inject_decode")
	svm.Load("svm.nop_inject_first_fetch/o4wlslm", "AVF-CL.nop_inject_first_fetch")

	svm.Process(vf=False)

	svm.Plot(mode="bargraph", type="perf_norm",
		ylabel="Normalized execution time",
		items=[
			svm.BarItem("AVF-CL.nop_inject1", color='cornflowerblue'),
			svm.BarItem("AVF-CL.nop_inject_cache", color='black'),
			svm.BarItem("AVF-CL.nop_inject_decode", color='gray'),
			svm.BarItem("AVF-CL.nop_inject_first_fetch", color='orange'),
			],
		filename="norm_exec_time_nopinject.pdf",
		geomean=True, yrange=[0.9, 1.15], size=[7, 4])

def analyze_perf_sampling(log=logging.INFO):
	svm = Svm(log)

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", "AVF-CL.base")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic")

	svm.Load("svm/o2wlslm", "AVF-CL.base.4M.1/1")
	svm.Load("svm/o2wlslm", "AVF-CL.base.1M.1/1")
	svm.Load("svm/o2wlslm", "AVF-CL.base.256K.1/1")
	svm.Load("svm/o2wlslm", "AVF-CL.base.64K.1/1")
	svm.Load("svm/o2wlslm", "AVF-CL.base.16K.1/1")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.4M.1/1")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.1M.1/1")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.256K.1/1")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.64K.1/1")
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.16K.1/1")

	svm.Load("svm.sampling_svf.4194304.2/o2wlslm", "AVF-CL.base.4M.1/2")
	svm.Load("svm.sampling_svf.4194304.4/o2wlslm", "AVF-CL.base.4M.1/4")
	svm.Load("svm.sampling_svf.4194304.8/o2wlslm", "AVF-CL.base.4M.1/8")
	svm.Load("svm.sampling_svf.1048576.2/o2wlslm", "AVF-CL.base.1M.1/2")
	svm.Load("svm.sampling_svf.1048576.4/o2wlslm", "AVF-CL.base.1M.1/4")
	svm.Load("svm.sampling_svf.1048576.8/o2wlslm", "AVF-CL.base.1M.1/8")
	svm.Load("svm.sampling_svf.1048576.16/o2wlslm", "AVF-CL.base.1M.1/16")
	svm.Load("svm.sampling_svf.1048576.32/o2wlslm", "AVF-CL.base.1M.1/32")
	svm.Load("svm.sampling_svf.262144.2/o2wlslm", "AVF-CL.base.256K.1/2")
	svm.Load("svm.sampling_svf.262144.4/o2wlslm", "AVF-CL.base.256K.1/4")
	svm.Load("svm.sampling_svf.262144.8/o2wlslm", "AVF-CL.base.256K.1/8")
	svm.Load("svm.sampling_svf.262144.16/o2wlslm", "AVF-CL.base.256K.1/16")
	svm.Load("svm.sampling_svf.262144.32/o2wlslm", "AVF-CL.base.256K.1/32")
	svm.Load("svm.sampling_svf.262144.64/o2wlslm", "AVF-CL.base.256K.1/64")
	svm.Load("svm.sampling_svf.65536.2/o2wlslm", "AVF-CL.base.64K.1/2")
	svm.Load("svm.sampling_svf.65536.4/o2wlslm", "AVF-CL.base.64K.1/4")
	svm.Load("svm.sampling_svf.65536.8/o2wlslm", "AVF-CL.base.64K.1/8")
	svm.Load("svm.sampling_svf.65536.16/o2wlslm", "AVF-CL.base.64K.1/16")
	svm.Load("svm.sampling_svf.65536.32/o2wlslm", "AVF-CL.base.64K.1/32")
	svm.Load("svm.sampling_svf.65536.64/o2wlslm", "AVF-CL.base.64K.1/64")
	svm.Load("svm.sampling_svf.16384.2/o2wlslm", "AVF-CL.base.16K.1/2")
	svm.Load("svm.sampling_svf.16384.4/o2wlslm", "AVF-CL.base.16K.1/4")
	svm.Load("svm.sampling_svf.16384.8/o2wlslm", "AVF-CL.base.16K.1/8")
	svm.Load("svm.sampling_svf.16384.16/o2wlslm", "AVF-CL.base.16K.1/16")
	svm.Load("svm.sampling_svf.16384.32/o2wlslm", "AVF-CL.base.16K.1/32")
	svm.Load("svm.sampling_svf.16384.64/o2wlslm", "AVF-CL.base.16K.1/64")

	svm.Load("svm.icache.sampling_svf.4194304.2/o2wlslm", "AVF-CL.ic.4M.1/2")
	svm.Load("svm.icache.sampling_svf.4194304.4/o2wlslm", "AVF-CL.ic.4M.1/4")
	svm.Load("svm.icache.sampling_svf.4194304.8/o2wlslm", "AVF-CL.ic.4M.1/8")
	svm.Load("svm.icache.sampling_svf.1048576.2/o2wlslm", "AVF-CL.ic.1M.1/2")
	svm.Load("svm.icache.sampling_svf.1048576.4/o2wlslm", "AVF-CL.ic.1M.1/4")
	svm.Load("svm.icache.sampling_svf.1048576.8/o2wlslm", "AVF-CL.ic.1M.1/8")
	svm.Load("svm.icache.sampling_svf.1048576.16/o2wlslm", "AVF-CL.ic.1M.1/16")
	svm.Load("svm.icache.sampling_svf.1048576.32/o2wlslm", "AVF-CL.ic.1M.1/32")
	svm.Load("svm.icache.sampling_svf.262144.2/o2wlslm", "AVF-CL.ic.256K.1/2")
	svm.Load("svm.icache.sampling_svf.262144.4/o2wlslm", "AVF-CL.ic.256K.1/4")
	svm.Load("svm.icache.sampling_svf.262144.8/o2wlslm", "AVF-CL.ic.256K.1/8")
	svm.Load("svm.icache.sampling_svf.262144.16/o2wlslm", "AVF-CL.ic.256K.1/16")
	svm.Load("svm.icache.sampling_svf.262144.32/o2wlslm", "AVF-CL.ic.256K.1/32")
	svm.Load("svm.icache.sampling_svf.262144.64/o2wlslm", "AVF-CL.ic.256K.1/64")
	svm.Load("svm.icache.sampling_svf.65536.2/o2wlslm", "AVF-CL.ic.64K.1/2")
	svm.Load("svm.icache.sampling_svf.65536.4/o2wlslm", "AVF-CL.ic.64K.1/4")
	svm.Load("svm.icache.sampling_svf.65536.8/o2wlslm", "AVF-CL.ic.64K.1/8")
	svm.Load("svm.icache.sampling_svf.65536.16/o2wlslm", "AVF-CL.ic.64K.1/16")
	svm.Load("svm.icache.sampling_svf.65536.32/o2wlslm", "AVF-CL.ic.64K.1/32")
	svm.Load("svm.icache.sampling_svf.65536.64/o2wlslm", "AVF-CL.ic.64K.1/64")
	svm.Load("svm.icache.sampling_svf.16384.2/o2wlslm", "AVF-CL.ic.16K.1/2")
	svm.Load("svm.icache.sampling_svf.16384.4/o2wlslm", "AVF-CL.ic.16K.1/4")
	svm.Load("svm.icache.sampling_svf.16384.8/o2wlslm", "AVF-CL.ic.16K.1/8")
	svm.Load("svm.icache.sampling_svf.16384.16/o2wlslm", "AVF-CL.ic.16K.1/16")
	svm.Load("svm.icache.sampling_svf.16384.32/o2wlslm", "AVF-CL.ic.16K.1/32")
	svm.Load("svm.icache.sampling_svf.16384.64/o2wlslm", "AVF-CL.ic.16K.1/64")

	svm.Process()

	items_base = \
		[{"linelabel": "4M", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "1M", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "256K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "64K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "16K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		]
	items_ic = \
		[{"linelabel": "4M", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "1M", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "256K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "64K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "16K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		]
	xaxis = ["1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64"]
	size = [2.4,4]
	styles = ['-o', '-*', '-+', '-x', '-D']

	apps = ["403.gcc", "456.hmmer", "464.h264ref", "483.xalancbmk", "geomean"]
	legend_shown = False
	for app in apps:
		svm.Plot(mode="linegraph_single_app",
			ylabel="Execution time overhead",
			xlabel="Duty cycle",
			items=items_base,
			filename="linegraph_perf_sampling_base_%s.pdf" % app.replace(".","_"),
			type="perf_norm_ovd",
			xaxis=xaxis, size=size, app=app, legend=not legend_shown,
			styles=styles)
		svm.Plot(mode="linegraph_single_app",
			ylabel="Execution time overhead",
			xlabel="Duty cycle",
			items=items_ic,
			filename="linegraph_perf_sampling_ic_%s.pdf" % app.replace(".","_"),
			type="perf_norm_ovd",
			xaxis=xaxis, size=size, app=app, legend=not legend_shown,
			styles=styles)
		legend_shown = True

	items_sampling_base = [
		svm.BarItem("AVF-CL.base", "AVF-CL.base"),
		svm.BarItem("AVF-CL.ic", "AVF-CL.ic"),
		svm.BarItem("AVF-CL.base.4M.1/2", "4M.1/2"),
		svm.BarItem("AVF-CL.base.4M.1/4", "4M.1/4"),
		svm.BarItem("AVF-CL.base.4M.1/8", "4M.1/8"),
		svm.BarItem("AVF-CL.base.1M.1/2", "1M.1/2"),
		svm.BarItem("AVF-CL.base.1M.1/4", "1M.1/4"),
		svm.BarItem("AVF-CL.base.1M.1/8", "1M.1/8"),
		svm.BarItem("AVF-CL.base.1M.1/16", "1M.1/16"),
		svm.BarItem("AVF-CL.base.1M.1/32", "1M.1/32"),
		svm.BarItem("AVF-CL.base.256K.1/2", "256K.1/2"),
		svm.BarItem("AVF-CL.base.256K.1/4", "256K.1/4"),
		svm.BarItem("AVF-CL.base.256K.1/8", "256K.1/8"),
		svm.BarItem("AVF-CL.base.256K.1/16", "256K.1/16"),
		svm.BarItem("AVF-CL.base.256K.1/32", "256K.1/32"),
		svm.BarItem("AVF-CL.base.256K.1/64", "256K.1/64"),
		svm.BarItem("AVF-CL.base.64K.1/2", "64K.1/2"),
		svm.BarItem("AVF-CL.base.64K.1/4", "64K.1/4"),
		svm.BarItem("AVF-CL.base.64K.1/8", "64K.1/8"),
		svm.BarItem("AVF-CL.base.64K.1/16", "64K.1/16"),
		svm.BarItem("AVF-CL.base.64K.1/32", "64K.1/32"),
		svm.BarItem("AVF-CL.base.64K.1/64", "64K.1/64"),
		svm.BarItem("AVF-CL.base.16K.1/2", "16K.1/2"),
		svm.BarItem("AVF-CL.base.16K.1/4", "16K.1/4"),
		svm.BarItem("AVF-CL.base.16K.1/8", "16K.1/8"),
		svm.BarItem("AVF-CL.base.16K.1/16", "16K.1/16"),
		svm.BarItem("AVF-CL.base.16K.1/32", "16K.1/32"),
		svm.BarItem("AVF-CL.base.16K.1/64", "16K.1/64"),
	]

	items_sampling_icache = [
		svm.BarItem("AVF-CL.base", "AVF-CL.base"),
		svm.BarItem("AVF-CL.ic", "AVF-CL.ic"),
		svm.BarItem("AVF-CL.ic.4M.1/2", "4M.1/2"),
		svm.BarItem("AVF-CL.ic.4M.1/4", "4M.1/4"),
		svm.BarItem("AVF-CL.ic.4M.1/8", "4M.1/8"),
		svm.BarItem("AVF-CL.ic.1M.1/2", "1M.1/2"),
		svm.BarItem("AVF-CL.ic.1M.1/4", "1M.1/4"),
		svm.BarItem("AVF-CL.ic.1M.1/8", "1M.1/8"),
		svm.BarItem("AVF-CL.ic.1M.1/16", "1M.1/16"),
		svm.BarItem("AVF-CL.ic.1M.1/32", "1M.1/32"),
		svm.BarItem("AVF-CL.ic.256K.1/2", "256K.1/2"),
		svm.BarItem("AVF-CL.ic.256K.1/4", "256K.1/4"),
		svm.BarItem("AVF-CL.ic.256K.1/8", "256K.1/8"),
		svm.BarItem("AVF-CL.ic.256K.1/16", "256K.1/16"),
		svm.BarItem("AVF-CL.ic.256K.1/32", "256K.1/32"),
		svm.BarItem("AVF-CL.ic.256K.1/64", "256K.1/64"),
		svm.BarItem("AVF-CL.ic.64K.1/2", "64K.1/2"),
		svm.BarItem("AVF-CL.ic.64K.1/4", "64K.1/4"),
		svm.BarItem("AVF-CL.ic.64K.1/8", "64K.1/8"),
		svm.BarItem("AVF-CL.ic.64K.1/16", "64K.1/16"),
		svm.BarItem("AVF-CL.ic.64K.1/32", "64K.1/32"),
		svm.BarItem("AVF-CL.ic.64K.1/64", "64K.1/64"),
		svm.BarItem("AVF-CL.ic.16K.1/2", "16K.1/2"),
		svm.BarItem("AVF-CL.ic.16K.1/4", "16K.1/4"),
		svm.BarItem("AVF-CL.ic.16K.1/8", "16K.1/8"),
		svm.BarItem("AVF-CL.ic.16K.1/16", "16K.1/16"),
		svm.BarItem("AVF-CL.ic.16K.1/32", "16K.1/32"),
		svm.BarItem("AVF-CL.ic.16K.1/64", "16K.1/64"),
	]

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_base,
		filename="norm_exec_time_sampling_403.pdf",
		app="403.gcc", yrange=[0.9, 1.15], size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_base,
		filename="norm_exec_time_sampling_456.pdf",
		app="456.hmmer", yrange=[0.9, 1.15], size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_base,
		filename="norm_exec_time_sampling_464.pdf",
		app="464.h264ref", yrange=[0.9, 1.15], size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_base,
		filename="norm_exec_time_sampling_483.pdf",
		app="483.xalancbmk", yrange=[0.9, 1.15], size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_icache,
		filename="norm_exec_time_sampling_icache_403.pdf",
		app="403.gcc", yrange=[0.9, 1.15], size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_icache,
		filename="norm_exec_time_sampling_icache_456.pdf",
		app="456.hmmer", yrange=[0.9, 1.15], size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_icache,
		filename="norm_exec_time_sampling_icache_464.pdf",
		app="464.h264ref", yrange=[0.9, 1.15], size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="perf_norm",
		ylabel="Normalized execution time",
		items=items_sampling_icache,
		filename="norm_exec_time_sampling_icache_483.pdf",
		app="483.xalancbmk", yrange=[0.9, 1.15], size=[12, 4])

def analyze_perf_adaptive(log=logging.INFO):
	svm = Svm(log)

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm.adaptive_svf.16384.64.0.05/o2wlslm")
	svm.Load("svm.adaptive_svf.16384.64.0.1/o2wlslm")
	svm.Load("svm.icache.adaptive_svf.16384.64.0.05/o2wlslm")
	svm.Load("svm.icache.adaptive_svf.16384.64.0.1/o2wlslm")

	svm.Process()

	svm.Plot(mode="bargraph", type="perf_norm",
		ylabel="Normalized execution time",
		items=[
			svm.BarItem("svm.adaptive_svf.16384.64.0.1/o2wlslm", "AVF-CL.S.base.10%", color='red'),
			svm.BarItem("svm.adaptive_svf.16384.64.0.05/o2wlslm", "AVF-CL.S.base.5%", color='salmon'),
			],
		filename="norm_exec_time_adaptive_base.pdf",
		geomean=True, yrange=[0.95, 1.05], size=[6, 4])

	svm.Plot(mode="bargraph", type="perf_norm",
		ylabel="Normalized execution time",
		items=[
			svm.BarItem("svm.icache.adaptive_svf.16384.64.0.1/o2wlslm", "AVF-CL.S.ic.10%", color='blue'),
			svm.BarItem("svm.icache.adaptive_svf.16384.64.0.05/o2wlslm", "AVF-CL.S.ic.5%", color='cornflowerblue'),
			],
		filename="norm_exec_time_adaptive_ic.pdf",
		geomean=True, yrange=[0.95, 1.05], size=[6, 4])

def analyze_vf_new(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "avfcl", "svf", "occ", "svf.prf"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur.256/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	# svm.Load("svm.icache/o2wlslm",
	# 	ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	# svm.Load("svm.small_mcache/o2wlslm",
	# 	ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	# svm.Load("svm.small_mcache.icache/o2wlslm",
	# 	ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	# svm.Load("svm.no_mcache/o2wlslm",
	# 	ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	# svm.Plot(mode="bargraph_vf",
	# 	ylabel="Vulnerability Factor",
	# 	items=[
	# 		svm.BarVfItem("svm/o2wlslm", "avf", "AVF(1)", 'black'),
	# 		svm.BarVfItem("svm/o2wlslm", "svf", "AVF-CL.base(2)", 'red'),
	# 		svm.BarVfItem("svm.sdur/o2wlslm", "avfcl", "AVF-CL(3)", 'salmon'),
	# 		# svm.BarVfItem("svm/o2wlslm", "avfcl", "AVFCL", 'salmon'),
	# 		# svm.BarVfItem("svm.icache/o2wlslm", "svf", "AVF-CL.ic(3)", 'salmon'),
	# 		# svm.BarVfItem("svm.small_mcache/o2wlslm", "svf.nlv", "AVF-CL.sm(4)", 'green'),
	# 		# svm.BarVfItem("svm.small_mcache.icache/o2wlslm", "svf.nlv", "AVF-CL.sm.ic(5)", 'limegreen'),
	# 		# svm.BarVfItem("svm.no_mcache/o2wlslm", "svf.nm", "AVF-CL.nm(6)", 'cornflowerblue'),
	# 		svm.BarVfItem("svm/o2wlslm", "occ", "Occupancy(7)", 'purple'),
	# 		],
	# 	filename="vf_svm_new.pdf", yrange=[0, 0.25],
	# 	legend_loc="outside",
	# 	arithmean=True, size=[9, 4])

	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	structure_aliases = ["iq", "rob", "lq", "sq", "rf", "ib", "combined"]
	aliases = {"avf": "AVF", "svf": "AVF-CL.base", "occ": "Occupancy",
		"avfe": "AVF-HP", "svf.nlv": "AVF-CL.nlv", "svf.prf": "AVF-CL.prf", "avfcl": "AVFCL"}
	colors = {"avf": "black", "svf": "red", "occ": "purple",
		"avfe": "cyan", "svf.nlv": "blue", "svf.prf": "cornflowerblue", "avfcl": "blue"}
	linestyles = {"avf": "-", "svf": "--", "occ": "-.",
		"avfe": "-", "svf.prf": ":", "avfcl": "-"}
	linewidths = {"avf": 5, "svf": 2, "occ": 2, "avfe": 2, "svf.prf": 2, "avfcl": 2}

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm.sdur/o2wlslm"},
			{"config": "svm.sdur.256/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		structure_aliases=structure_aliases, filename="ts_base",
		linestyles=linestyles, linewidths=linewidths)

def analyze_vf_new_rf_comp_svf(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "svf_nc", "svf_wc", "svf_rc", "svf.prf"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm.sdur/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	structure_aliases = ["iq", "rob", "lq", "sq", "rf", "ib", "combined"]
	aliases = {"avf": "AVF", "svf": "AVF-CL.base", "occ": "Occupancy",
		"avfe": "AVF-HP", "svf.nlv": "AVF-CL.nlv", "svf.prf": "AVF-CL.prf", "avfcl": "AVFCL", "svf_nc": "SVF-NC", "svf_wc": "SVF-WC", "svf_rc": "SVF-RC"}
	colors = {"avf": "black", "svf": "red", "occ": "purple", "svf_nc": "blue", "svf_wc": "green", "svf_rc": "yellow",
		"avfe": "cyan", "svf.nlv": "blue", "svf.prf": "cornflowerblue", "avfcl": "blue"}
	linestyles = {"avf": "-", "svf": "--", "occ": "-.",
		"avfe": "-", "svf.prf": ":", "avfcl": "-", "svf_nc": "-", "svf_wc": "-", "svf_rc": "-"}
	linewidths = {"avf": 5, "svf": 2, "occ": 2, "avfe": 2, "svf.prf": 2, "avfcl": 2, "svf_nc": 2, "svf_wc": 2, "svf_rc": 2}

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm.sdur/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		structure_aliases=structure_aliases, filename="ts_new_rf_comp_svf",
		linestyles=linestyles, linewidths=linewidths)

def analyze_vf_new_rf_comp_avfcl(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "svf_nc", "avfcl_wc", "avfcl_rc", "svf.prf"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm.sdur/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	structure_aliases = ["iq", "rob", "lq", "sq", "rf", "ib", "combined"]
	aliases = {"avf": "AVF", "svf": "AVF-CL.base", "occ": "Occupancy",
		"avfe": "AVF-HP", "svf.nlv": "AVF-CL.nlv", "svf.prf": "AVF-CL.prf", "avfcl": "AVFCL", "svf_nc": "SVF-NC", "avfcl_wc": "SVF-WC", "avfcl_rc": "SVF-RC"}
	colors = {"avf": "black", "svf": "red", "occ": "purple", "svf_nc": "blue", "avfcl_wc": "green", "avfcl_rc": "yellow",
		"avfe": "cyan", "svf.nlv": "blue", "svf.prf": "cornflowerblue", "avfcl": "blue"}
	linestyles = {"avf": "-", "svf": "--", "occ": "-.",
		"avfe": "-", "svf.prf": ":", "avfcl": "-", "svf_nc": "-", "avfcl_wc": "-", "avfcl_rc": "-"}
	linewidths = {"avf": 5, "svf": 2, "occ": 2, "avfe": 2, "svf.prf": 2, "avfcl": 2, "svf_nc": 2, "avfcl_wc": 2, "avfcl_rc": 2}

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm.sdur/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		structure_aliases=structure_aliases, filename="ts_new_rf_comp_svf",
		linestyles=linestyles, linewidths=linewidths)

def analyze_vf_new_mae(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "avfcl", "svf", "svf.prf", "svf_nc", "occ"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur.256/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur_sync.256/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur_sync.64/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur_sync.16/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur_sync.4/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.256/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.64/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.16/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.4/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.256.thd8/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.256.thd32/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.256.thd64/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.256.thd128/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	items=[
			svm.BarItem("svm/o2wlslm","AVF-CL.base(1)","svf","red"),
			svm.BarItem("svm.sdur/o2wlslm", "AVF-CL.prf(3)", "svf.prf", 'salmon'),
			# svm.BarItem("svm.sdur/o2wlslm", "AVF-CL.sdur(2)", "avfcl", 'green'),
			# svm.BarItem("svm.sdur.256/o2wlslm", "AVF-CL.sdur.256(4)", "avfcl", 'limegreen'),
			svm.BarItem("svm.sdur/o2wlslm", "AVF-CL.nc(5)", "svf_nc", 'purple'),
			svm.BarItem("svm.sdur_sync.256/o2wlslm", "AVF-CL.sync.256(7)", "avfcl", 'black'),
			# svm.BarItem("svm.sdur_sync.64/o2wlslm", "AVF-CL.sync.64(8)", "avfcl", 'gray'),
			# svm.BarItem("svm.sdur_sync.16/o2wlslm", "AVF-CL.sync.16(9)", "avfcl", 'orange'),
			svm.BarItem("svm.sdur_sync.4/o2wlslm", "AVF-CL.sync.4(10)", "avfcl", 'yellow'),
			svm.BarItem("svm.iqfs.256/o2wlslm", "AVF-CL.iqfs.256(11)", "avfcl", 'darkblue'),
			svm.BarItem("svm.iqfs.64/o2wlslm", "AVF-CL.iqfs.64(12)", "avfcl", 'blue'),
			svm.BarItem("svm.iqfs.16/o2wlslm", "AVF-CL.iqfs.16(13)", "avfcl", 'royalblue'),
			svm.BarItem("svm.iqfs.4/o2wlslm", "AVF-CL.iqfs.4(14)", "avfcl", 'lightblue'),
			svm.BarItem("svm.iqfs.256.thd8/o2wlslm", "AVF-CL.iqfs.256.thd8", "avfcl", 'darkgreen'),
			svm.BarItem("svm.iqfs.256/o2wlslm", "AVF-CL.iqfs.256.thd16", "avfcl", 'green'),
			svm.BarItem("svm.iqfs.256.thd32/o2wlslm", "AVF-CL.iqfs.256.thd32", "avfcl", 'forestgreen'),
			svm.BarItem("svm.iqfs.256.thd64/o2wlslm", "AVF-CL.iqfs.256.thd64", "avfcl", 'lightgreen'),
			svm.BarItem("svm.iqfs.256.thd128/o2wlslm", "AVF-CL.iqfs.256.thd128", "avfcl", 'greenyellow'),
			]

	for s in ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]:
		svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
			ylabel="Mean Absolute Error",
			items=items,
			arithmean=True, structures=[s],
			size=[7,4],
			filename="vf_svm_new_mae_$structure$.pdf")


def analyze_vf_qavf_mae(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "avfcl", "svf", "svf.prf", "svf_nc", "occ", "qavf", \
		"qavf_noself", "qavf_wprf", "qavf_woprf"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sdur/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.4/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.iqfs.256.thd128/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	items=[
			svm.BarItem("svm/o2wlslm","AVF-CL.base(1)","svf","red"),
			svm.BarItem("svm.sdur/o2wlslm", "AVF-CL.prf(2)", "svf.prf", 'salmon'),
			svm.BarItem("svm.sdur/o2wlslm", "AVF-CL.nc(3)", "svf_nc", 'purple'),
			svm.BarItem("svm.iqfs.4/o2wlslm", "AVF-CL.iqfs.4(4)", "avfcl", 'lightblue'),
			svm.BarItem("svm.iqfs.256.thd128/o2wlslm", "AVF-CL.iqfs.256.thd128(5)", "avfcl", 'greenyellow'),
			# svm.BarItem("svm/o2wlslm", "Q-AVF(5)", "qavf", 'orange'),
			# svm.BarItem("svm/o2wlslm", "Q-AVF-90\%(6)", "qavf_noself", 'black'),
			svm.BarItem("svm/o2wlslm", "Q-AVF-woPRF(6)", "qavf_woprf", 'orange'),
			svm.BarItem("svm/o2wlslm", "Q-AVF-wPRF(7)", "qavf_wprf", 'gray'),
			]

	for s in ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]:
		svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
			ylabel="Mean Absolute Error",
			items=items,
			arithmean=True, structures=[s],
			size=[7,4],
			filename="vf_svm_qavf_mae_$structure$.pdf")

def analyze_vf_qavf(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "avfcl", "svf.prf", "qavf_wprf", "qavf_woprf"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	structure_aliases = ["iq", "rob", "lq", "sq", "rf", "ib", "combined"]
	aliases = {"avf": "AVF", "svf": "AVF-CL.base", "occ": "Occupancy",
		"avfe": "AVF-HP", "svf.nlv": "AVF-CL.nlv", "svf.prf": "AVF-CL.prf", "avfcl": "AVFCL", "svf_nc": "SVF-NC", "qavf_wprf": "qavf_wprf", "qavf_woprf": "qavf_woprf"}
	colors = {"avf": "black", "svf": "red", "occ": "purple", "svf_nc": "blue", "qavf_wprf": "green", "qavf_woprf": "yellow",
		"avfe": "cyan", "svf.nlv": "blue", "svf.prf": "cornflowerblue", "avfcl": "blue"}
	linestyles = {"avf": "-", "svf": "--", "occ": "-.",
		"avfe": "-", "svf.prf": ":", "avfcl": "-", "svf_nc": "-", "qavf_wprf": "-", "qavf_woprf": "-"}
	linewidths = {"avf": 5, "svf": 2, "occ": 2, "avfe": 2, "svf.prf": 2, "avfcl": 2, "svf_nc": 2, "qavf_wprf": 2, "qavf_woprf": 2}

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		structure_aliases=structure_aliases, filename="ts_qavf",
		linestyles=linestyles, linewidths=linewidths)

def analyze_vf_base(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "occ"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.small_mcache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.small_mcache.icache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.no_mcache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	svm.Plot(mode="bargraph_vf",
		ylabel="Vulnerability Factor",
		items=[
			svm.BarVfItem("svm.no_mcache/o2wlslm", "avf", "AVF(1)", 'black'),
			svm.BarVfItem("svm/o2wlslm", "svf", "AVF-CL.base(2)", 'red'),
			svm.BarVfItem("svm.icache/o2wlslm", "svf", "AVF-CL.ic(3)", 'salmon'),
			svm.BarVfItem("svm.small_mcache/o2wlslm", "svf.nlv", "AVF-CL.sm(4)", 'green'),
			svm.BarVfItem("svm.small_mcache.icache/o2wlslm", "svf.nlv", "AVF-CL.sm.ic(5)", 'limegreen'),
			svm.BarVfItem("svm.no_mcache/o2wlslm", "svf.nm", "AVF-CL.nm(6)", 'cornflowerblue'),
			svm.BarVfItem("svm/o2wlslm", "occ", "Occupancy(7)", 'purple'),
			],
		filename="vf_svm_base.pdf", yrange=[0, 0.25],
		legend_loc="outside",
		arithmean=True, size=[9, 4])

	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	structure_aliases = ["iq", "rob", "lq", "sq", "rf", "ib", "combined"]
	aliases = {"avf": "AVF", "svf": "AVF-CL.base", "occ": "Occupancy",
		"avfe": "AVF-HP", "svf.nlv": "AVF-CL.nlv", "svf.nm": "AVF-CL.nm"}
	colors = {"avf": "black", "svf": "red", "occ": "purple",
		"avfe": "cyan", "svf.nlv": "blue", "svf.nm": "cornflowerblue"}
	linestyles = {"avf": "-", "svf": "--", "occ": "-.",
		"avfe": "-", "svf.nm": ":"}
	linewidths = {"avf": 5, "svf": 2, "occ": 2, "avfe": 2, "svf.nm": 2}

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		structure_aliases=structure_aliases, filename="ts_base",
		linestyles=linestyles, linewidths=linewidths)

def analyze_vf_base_mae(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "svf.nlv", "svf.nm", "occ"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.small_mcache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.small_mcache.icache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.no_mcache/o2wlslm",
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
		ylabel="Mean Absolute Error",
		items=[
			svm.BarItem("svm/o2wlslm","AVF-CL.base(1)","svf","red"),
			svm.BarItem("svm.icache/o2wlslm", "AVF-CL.ic(2)", "svf", 'salmon'),
			svm.BarItem("svm.small_mcache/o2wlslm", "AVF-CL.sm(3)", "svf.nlv", 'green'),
			svm.BarItem("svm.small_mcache.icache/o2wlslm", "AVF-CL.sm.ic(4)", "svf.nlv", 'limegreen'),
			svm.BarItem("svm.no_mcache/o2wlslm", "AVF-CL.nm(5)", "svf.nm", 'cornflowerblue'),
			svm.BarItem("svm/o2wlslm","Occupancy(6)","occ","purple"),
			],
		arithmean=True, structures=["sys"], size=[9,4], yrange=[0,0.04],
		legend_loc="outside",
		filename="vf_svm_base_mae_$structure$.pdf")

def analyze_vf_adaptive(log=logging.INFO):
	svm = Svm(log)

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm.no_mcache/o2wlslm")
	svm.Load("svm.adaptive_svf.16384.64.0.1/o2wlslm")
	svm.Load("svm.adaptive_svf.16384.64.0.05/o2wlslm")
	svm.Load("svm.icache.adaptive_svf.16384.64.0.1/o2wlslm")
	svm.Load("svm.icache.adaptive_svf.16384.64.0.05/o2wlslm")

	svm.Process()

	svm.Plot(mode="bargraph_vf",
		ylabel="AVF",
		items=[
			svm.BarVfItem("svm.no_mcache/o2wlslm", "avf", "noECC", 'black'),
			svm.BarVfItem("svm.adaptive_svf.16384.64.0.1/o2wlslm", "avf_ecc", "AVF-CL.S.base.10%", 'red'),
			svm.BarVfItem("svm.adaptive_svf.16384.64.0.05/o2wlslm", "avf_ecc", "AVF-CL.S.base.5%", 'salmon'),
			],
		filename="vf_svm_adaptive_base.pdf",
		arithmean=True, size=[6, 4])

	svm.Plot(mode="bargraph_vf",
		ylabel="AVF",
		items=[
			svm.BarVfItem("svm.no_mcache/o2wlslm", "avf", "noECC", 'black'),
			svm.BarVfItem("svm.icache.adaptive_svf.16384.64.0.1/o2wlslm", "avf_ecc", "AVF-CL.S.ic.10%", 'red'),
			svm.BarVfItem("svm.icache.adaptive_svf.16384.64.0.05/o2wlslm", "avf_ecc", "AVF-CL.S.ic.5%", 'salmon'),
			],
		filename="vf_svm_adaptive_ic.pdf",
		arithmean=True, size=[6, 4])

def analyze_vf_sampling(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "occ"]

	svm.Load("coretime/o2wlslm", "baseline")

	svm.Load("svm/o2wlslm", "AVF-CL.base.4M.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm/o2wlslm", "AVF-CL.base.1M.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm/o2wlslm", "AVF-CL.base.256K.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm/o2wlslm", "AVF-CL.base.64K.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm/o2wlslm", "AVF-CL.base.16K.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.4M.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.1M.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.256K.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.64K.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache/o2wlslm", "AVF-CL.ic.16K.1/1",
		ts_info={"active_size":1*1024*1024, "rarate":1, "methods":methods})

	svm.Load("svm.sampling_svf.4194304.2/o2wlslm", "AVF-CL.base.4M.1/2",
		ts_info={"active_size":4*1024*1024, "rarate":2, "methods":methods})
	svm.Load("svm.sampling_svf.4194304.4/o2wlslm", "AVF-CL.base.4M.1/4",
		ts_info={"active_size":4*1024*1024, "rarate":4, "methods":methods})
	svm.Load("svm.sampling_svf.4194304.8/o2wlslm", "AVF-CL.base.4M.1/8",
		ts_info={"active_size":4*1024*1024, "rarate":8, "methods":methods})
	svm.Load("svm.sampling_svf.1048576.2/o2wlslm", "AVF-CL.base.1M.1/2",
		ts_info={"active_size":1*1024*1024, "rarate":2, "methods":methods})
	svm.Load("svm.sampling_svf.1048576.4/o2wlslm", "AVF-CL.base.1M.1/4",
		ts_info={"active_size":1*1024*1024, "rarate":4, "methods":methods})
	svm.Load("svm.sampling_svf.1048576.8/o2wlslm", "AVF-CL.base.1M.1/8",
		ts_info={"active_size":1*1024*1024, "rarate":8, "methods":methods})
	svm.Load("svm.sampling_svf.1048576.16/o2wlslm", "AVF-CL.base.1M.1/16",
		ts_info={"active_size":1*1024*1024, "rarate":16, "methods":methods})
	svm.Load("svm.sampling_svf.1048576.32/o2wlslm", "AVF-CL.base.1M.1/32",
		ts_info={"active_size":1*1024*1024, "rarate":32, "methods":methods})
	svm.Load("svm.sampling_svf.262144.2/o2wlslm", "AVF-CL.base.256K.1/2",
		ts_info={"active_size":256*1024, "rarate":2, "methods":methods})
	svm.Load("svm.sampling_svf.262144.4/o2wlslm", "AVF-CL.base.256K.1/4",
		ts_info={"active_size":256*1024, "rarate":4, "methods":methods})
	svm.Load("svm.sampling_svf.262144.8/o2wlslm", "AVF-CL.base.256K.1/8",
		ts_info={"active_size":256*1024, "rarate":8, "methods":methods})
	svm.Load("svm.sampling_svf.262144.16/o2wlslm", "AVF-CL.base.256K.1/16",
		ts_info={"active_size":256*1024, "rarate":16, "methods":methods})
	svm.Load("svm.sampling_svf.262144.32/o2wlslm", "AVF-CL.base.256K.1/32",
		ts_info={"active_size":256*1024, "rarate":32, "methods":methods})
	svm.Load("svm.sampling_svf.262144.64/o2wlslm", "AVF-CL.base.256K.1/64",
		ts_info={"active_size":256*1024, "rarate":64, "methods":methods})
	svm.Load("svm.sampling_svf.65536.2/o2wlslm", "AVF-CL.base.64K.1/2",
		ts_info={"active_size":64*1024, "rarate":2, "methods":methods})
	svm.Load("svm.sampling_svf.65536.4/o2wlslm", "AVF-CL.base.64K.1/4",
		ts_info={"active_size":64*1024, "rarate":4, "methods":methods})
	svm.Load("svm.sampling_svf.65536.8/o2wlslm", "AVF-CL.base.64K.1/8",
		ts_info={"active_size":64*1024, "rarate":8, "methods":methods})
	svm.Load("svm.sampling_svf.65536.16/o2wlslm", "AVF-CL.base.64K.1/16",
		ts_info={"active_size":64*1024, "rarate":16, "methods":methods})
	svm.Load("svm.sampling_svf.65536.32/o2wlslm", "AVF-CL.base.64K.1/32",
		ts_info={"active_size":64*1024, "rarate":32, "methods":methods})
	svm.Load("svm.sampling_svf.65536.64/o2wlslm", "AVF-CL.base.64K.1/64",
		ts_info={"active_size":64*1024, "rarate":64, "methods":methods})
	svm.Load("svm.sampling_svf.16384.2/o2wlslm", "AVF-CL.base.16K.1/2",
		ts_info={"active_size":16*1024, "rarate":2, "methods":methods})
	svm.Load("svm.sampling_svf.16384.4/o2wlslm", "AVF-CL.base.16K.1/4",
		ts_info={"active_size":16*1024, "rarate":4, "methods":methods})
	svm.Load("svm.sampling_svf.16384.8/o2wlslm", "AVF-CL.base.16K.1/8",
		ts_info={"active_size":16*1024, "rarate":8, "methods":methods})
	svm.Load("svm.sampling_svf.16384.16/o2wlslm", "AVF-CL.base.16K.1/16",
		ts_info={"active_size":16*1024, "rarate":16, "methods":methods})
	svm.Load("svm.sampling_svf.16384.32/o2wlslm", "AVF-CL.base.16K.1/32",
		ts_info={"active_size":16*1024, "rarate":32, "methods":methods})
	svm.Load("svm.sampling_svf.16384.64/o2wlslm", "AVF-CL.base.16K.1/64",
		ts_info={"active_size":16*1024, "rarate":64, "methods":methods})
	svm.Load("svm.icache.sampling_svf.4194304.2/o2wlslm", "AVF-CL.ic.4M.1/2",
		ts_info={"active_size":4*1024*1024, "rarate":2, "methods":methods})
	svm.Load("svm.icache.sampling_svf.4194304.4/o2wlslm", "AVF-CL.ic.4M.1/4",
		ts_info={"active_size":4*1024*1024, "rarate":4, "methods":methods})
	svm.Load("svm.icache.sampling_svf.4194304.8/o2wlslm", "AVF-CL.ic.4M.1/8",
		ts_info={"active_size":4*1024*1024, "rarate":8, "methods":methods})
	svm.Load("svm.icache.sampling_svf.1048576.2/o2wlslm", "AVF-CL.ic.1M.1/2",
		ts_info={"active_size":1*1024*1024, "rarate":2, "methods":methods})
	svm.Load("svm.icache.sampling_svf.1048576.4/o2wlslm", "AVF-CL.ic.1M.1/4",
		ts_info={"active_size":1*1024*1024, "rarate":4, "methods":methods})
	svm.Load("svm.icache.sampling_svf.1048576.8/o2wlslm", "AVF-CL.ic.1M.1/8",
		ts_info={"active_size":1*1024*1024, "rarate":8, "methods":methods})
	svm.Load("svm.icache.sampling_svf.1048576.16/o2wlslm", "AVF-CL.ic.1M.1/16",
		ts_info={"active_size":1*1024*1024, "rarate":16, "methods":methods})
	svm.Load("svm.icache.sampling_svf.1048576.32/o2wlslm", "AVF-CL.ic.1M.1/32",
		ts_info={"active_size":1*1024*1024, "rarate":32, "methods":methods})
	svm.Load("svm.icache.sampling_svf.262144.2/o2wlslm", "AVF-CL.ic.256K.1/2",
		ts_info={"active_size":256*1024, "rarate":2, "methods":methods})
	svm.Load("svm.icache.sampling_svf.262144.4/o2wlslm", "AVF-CL.ic.256K.1/4",
		ts_info={"active_size":256*1024, "rarate":4, "methods":methods})
	svm.Load("svm.icache.sampling_svf.262144.8/o2wlslm", "AVF-CL.ic.256K.1/8",
		ts_info={"active_size":256*1024, "rarate":8, "methods":methods})
	svm.Load("svm.icache.sampling_svf.262144.16/o2wlslm", "AVF-CL.ic.256K.1/16",
		ts_info={"active_size":256*1024, "rarate":16, "methods":methods})
	svm.Load("svm.icache.sampling_svf.262144.32/o2wlslm", "AVF-CL.ic.256K.1/32",
		ts_info={"active_size":256*1024, "rarate":32, "methods":methods})
	svm.Load("svm.icache.sampling_svf.262144.64/o2wlslm", "AVF-CL.ic.256K.1/64",
		ts_info={"active_size":256*1024, "rarate":64, "methods":methods})
	svm.Load("svm.icache.sampling_svf.65536.2/o2wlslm", "AVF-CL.ic.64K.1/2",
		ts_info={"active_size":64*1024, "rarate":2, "methods":methods})
	svm.Load("svm.icache.sampling_svf.65536.4/o2wlslm", "AVF-CL.ic.64K.1/4",
		ts_info={"active_size":64*1024, "rarate":4, "methods":methods})
	svm.Load("svm.icache.sampling_svf.65536.8/o2wlslm", "AVF-CL.ic.64K.1/8",
		ts_info={"active_size":64*1024, "rarate":8, "methods":methods})
	svm.Load("svm.icache.sampling_svf.65536.16/o2wlslm", "AVF-CL.ic.64K.1/16",
		ts_info={"active_size":64*1024, "rarate":16, "methods":methods})
	svm.Load("svm.icache.sampling_svf.65536.32/o2wlslm", "AVF-CL.ic.64K.1/32",
		ts_info={"active_size":64*1024, "rarate":32, "methods":methods})
	svm.Load("svm.icache.sampling_svf.65536.64/o2wlslm", "AVF-CL.ic.64K.1/64",
		ts_info={"active_size":64*1024, "rarate":64, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.2/o2wlslm", "AVF-CL.ic.16K.1/2",
		ts_info={"active_size":16*1024, "rarate":2, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.4/o2wlslm", "AVF-CL.ic.16K.1/4",
		ts_info={"active_size":16*1024, "rarate":4, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.8/o2wlslm", "AVF-CL.ic.16K.1/8",
		ts_info={"active_size":16*1024, "rarate":8, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.16/o2wlslm", "AVF-CL.ic.16K.1/16",
		ts_info={"active_size":16*1024, "rarate":16, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.32/o2wlslm", "AVF-CL.ic.16K.1/32",
		ts_info={"active_size":16*1024, "rarate":32, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.64/o2wlslm", "AVF-CL.ic.16K.1/64",
		ts_info={"active_size":16*1024, "rarate":64, "methods":methods})

	svm.Process()

	items_base = \
		[{"linelabel": "4M", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "1M", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "256K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "64K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "16K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		]
	items_ic = \
		[{"linelabel": "4M", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "1M", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "256K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "64K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "16K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		]
	xaxis = ["1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64"]
	size = [2.4,4]
	styles = ['-o', '-*', '-+', '-x', '-D']

	apps = ["403.gcc", "456.hmmer", "464.h264ref", "483.xalancbmk"]
	legend_shown = False
	for app in apps:
		svm.Plot(mode="linegraph_single_app",
			ylabel="Mean absolute error",
			xlabel="Duty cycle",
			items=items_base,
			filename="linegraph_vf_sampling_base_%s.pdf" % app.replace(".","_"),
			type="vf_instant_aggr_errors", yrange=[0, 0.04],
			xaxis=xaxis, size=size, app=app, legend=not legend_shown,
			methods=["svf"], structures=["sys"],
			styles=styles)
		svm.Plot(mode="linegraph_single_app",
			ylabel="Mean absolute error",
			xlabel="Duty cycle",
			items=items_ic,
			filename="linegraph_vf_sampling_ic_%s.pdf" % app.replace(".","_"),
			type="vf_instant_aggr_errors", yrange=[0, 0.04],
			xaxis=xaxis, size=size, app=app, legend=not legend_shown,
			methods=["svf"], structures=["sys"],
			styles=styles)
		legend_shown = True

def analyze_vf_sampling_16k_64(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "occ"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", \
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.sampling_svf.16384.64/o2wlslm", \
		ts_info={"active_size":16*1024, "rarate":64, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.64/o2wlslm", \
		ts_info={"active_size":16*1024, "rarate":64, "methods":methods})

	svm.Process()

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
		ylabel="Mean Absolute Error",
		items=[
			svm.BarItem("svm/o2wlslm","AVF-CL.base","svf","red"),
			svm.BarItem("svm/o2wlslm","Occupancy","occ","purple"),
			svm.BarItem("svm.sampling_svf.16384.64/o2wlslm",
				"AVF-CL.S.base.16k.1/64","svf","green"),
			svm.BarItem("svm.icache.sampling_svf.16384.64/o2wlslm",
				"AVF-CL.S.ic.16k.1/64","svf","limegreen"),
			],
		arithmean=True,
		structures=["sys"],
		size=[5,5],
		filename="bargraph_sampling_16K_64_only_$structure$.pdf")

def analyze_vf_avfe(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf", "occ", "avfe"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", \
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})

	svm.Process()

	structures = ["iq", "rob", "lq", "sq"]
	aliases = {"avf": "AVF", "svf": "AVF-CL.base", "occ": "Occupancy",
		"avfe": "AVF-HP", "svf.nm": "AVF-CL.nm"}
	colors = {"avf": "black", "svf": "red", "occ": "purple",
		"avfe": "cyan", "svf.nm": "darkblue"}
	methods = ["svf", "svf.nm", "occ"]
	linestyles = {"avf": "-", "svf": "--", "occ": "-.",
		"avfe": "-", "svf.nm": ":"}
	linewidths = {"avf": 5, "svf": 2, "occ": 2, "avfe": 2, "svf.nm": 2}

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
		ylabel="Mean Absolute Error",
		items=[
			svm.BarItem("svm/o2wlslm","AVF-CL.base","svf","red"),
			svm.BarItem("svm/o2wlslm","AVF-HP","avfe","cyan"),
			],
		arithmean=True,
		structures=structures,
		size=[5,5],
		filename="bargraph_avfe_$structure$.pdf")

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures,
		linestyles=linestyles, linewidths=linewidths, filename="ts_avfe")

def analyze_vf_montecarlo(log=logging.INFO):
	svm = Svm(log)
	methods = ["avf", "svf.nm", "svf", "avfe"]

	svm.Load("coretime/o2wlslm", "baseline")
	svm.Load("svm/o2wlslm", \
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.montecarlo/o2wlslm", \
		ts_info={"active_size":1024*1024, "rarate":1, "methods":methods})
	svm.Load("svm.icache.sampling_svf.262144.4.montecarlo/o2wlslm", \
		ts_info={"active_size":256*1024, "rarate":4, "methods":methods})
	svm.Load("svm.icache.sampling_svf.65536.16.montecarlo/o2wlslm", \
		ts_info={"active_size":64*1024, "rarate":16, "methods":methods})
	svm.Load("svm.icache.sampling_svf.16384.64.montecarlo/o2wlslm", \
		ts_info={"active_size":16*1024, "rarate":64, "methods":methods})
	svm.Load("svm.sampling_svf.262144.4.montecarlo/o2wlslm", \
		ts_info={"active_size":256*1024, "rarate":4, "methods":methods})
	svm.Load("svm.sampling_svf.65536.16.montecarlo/o2wlslm", \
		ts_info={"active_size":64*1024, "rarate":16, "methods":methods})
	svm.Load("svm.sampling_svf.16384.64.montecarlo/o2wlslm", \
		ts_info={"active_size":16*1024, "rarate":64, "methods":methods})

	svm.Process()

	structures = ["iq", "rob", "lq", "sq", "rf", "ib"]
	structures_bargraph = ["iq", "rob", "lq", "sq", "rf", "ib", "sys"]
	aliases = {"avf": "AVF", "svf": "AVF-CL.S.ic", "occ": "Occupancy",
		"avfe": "AVF-HP", "svf.nm": "AVF-CL.S.ic'"}
	colors = {"avf": "black", "svf": "red", "occ": "purple",
		"avfe": "cyan", "svf.nm": "darkblue"}
	linestyles = {"avf": "-", "svf": "--", "occ": "-.",
		"avfe": "-", "svf.nm": ":"}
	linewidths = {"avf": 5, "svf": 2, "occ": 2, "avfe": 2, "svf.nm": 2}
	methods = ["svf", "avfe", "svf.nm", "occ"]

	svm.Plot(mode="bargraph_methods", type="vf_instant_aggr_errors",
		ylabel="Mean Absolute Error",
		items=[
			svm.BarItem("svm.montecarlo/o2wlslm","AVF-CL.base","svf","red"),
			svm.BarItem("svm.montecarlo/o2wlslm","AVF-HP","avfe","cyan"),
			svm.BarItem("svm.montecarlo/o2wlslm","AVF-CL.base.nm","svf.nm",
				"cornflowerblue"),
			svm.BarItem("svm.montecarlo/o2wlslm","Occupancy","occ","purple"),
			svm.BarItem("svm.icache.sampling_svf.16384.64.montecarlo/o2wlslm",
				"AVF-CL.S.ic","svf","salmon"),
			svm.BarItem("svm.icache.sampling_svf.16384.64.montecarlo/o2wlslm",
				"AVF-HP","avfe","aqua"),
			svm.BarItem("svm.icache.sampling_svf.16384.64.montecarlo/o2wlslm",
				"AVF-CL.S.ic'","svf.nm","lightskyblue"),
			svm.BarItem("svm.icache.sampling_svf.16384.64.montecarlo/o2wlslm",
				"Occupancy","occ","mediumorchid"),
			],
		structures=structures_bargraph,
		filename="bargraph_montecarlo_$structure$.pdf")

	# svm.Plot(mode="bargraph_single_config", type="vf_instant_aggr_errors",
	# 	ylabel="Mean Absolute Error",
	# 	items="svm.icache.sampling_svf.16384.64.montecarlo/o2wlslm",
	# 	structures=structures, aliases=aliases, colors=colors, methods=methods,
	# 	filename="bargraph_montecarlo_sampling_$structure$.pdf")

	svm.Plot(mode="timeseries", type="ts_vulnerability",
		ylabel="Vulnerability Factor", xlabel="Million Cycles",
		items=[
			{"config": "svm.montecarlo/o2wlslm"},
			{"config": "svm.icache.sampling_svf.262144.4.montecarlo/o2wlslm"},
			{"config": "svm.icache.sampling_svf.65536.16.montecarlo/o2wlslm"},
			{"config": "svm.icache.sampling_svf.16384.64.montecarlo/o2wlslm"},
			{"config": "svm.sampling_svf.262144.4.montecarlo/o2wlslm"},
			{"config": "svm.sampling_svf.65536.16.montecarlo/o2wlslm"},
			{"config": "svm.sampling_svf.16384.64.montecarlo/o2wlslm"},
			],
		aliases=aliases, colors=colors, structures=structures, filename="ts",
		linestyles=linestyles, linewidths=linewidths)

def analyze_power_base_old(log=logging.INFO):
	svm = Svm(log)

	svm.LoadPower("coretime/o2wlslm", "ppa.output.yaml", "baseline")
	svm.LoadPower("coretime/o2wlslm", "ppa.output.ecc.yaml", "ECC")
	svm.LoadPower("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base")
	svm.LoadPower("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic")
	svm.LoadPower("svm.small_mcache/o2wlslm", "ppa.output.svf.smallmd.yaml", "AVF-CL.sm")
	svm.LoadPower("svm.small_mcache.icache/o2wlslm", "ppa.output.svf.icache.smallmd.yaml", "AVF-CL.sm.ic")
	svm.LoadPower("svm.no_mcache/o2wlslm", "ppa.output.svf.nomd.yaml", "AVF-CL.nm")

	svm.ProcessPower()

	svm.Plot(mode="bargraph", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=[
			svm.BarItem("AVF-CL.base", "AVF-CL.base(1)", color='red'),
			svm.BarItem("AVF-CL.ic", "AVF-CL.ic(2)", color='salmon'),
			svm.BarItem("AVF-CL.sm", "AVF-CL.sm(3)", color='green'),
			svm.BarItem("AVF-CL.sm.ic", "AVF-CL.sm.ic(4)", color='limegreen'),
			svm.BarItem("AVF-CL.nm", "AVF-CL.nm(5)", color='cornflowerblue'),
			svm.BarItem("ECC", "ECC(6)", color='black'),
			],
		filename="norm_energy_base.pdf",
		size=[7, 4], legend_loc=None, legend_cols=6)

def analyze_power_sampling(log=logging.INFO):
	svm = Svm(log)

	svm.LoadPower("coretime/o2wlslm", "ppa.output.yaml", "baseline")
	svm.LoadPower("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base")
	svm.LoadPower("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic")
	svm.LoadPower("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base.4M.1/1")
	svm.LoadPower("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base.1M.1/1")
	svm.LoadPower("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base.256K.1/1")
	svm.LoadPower("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base.64K.1/1")
	svm.LoadPower("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base.16K.1/1")
	svm.LoadPower("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic.4M.1/1")
	svm.LoadPower("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic.1M.1/1")
	svm.LoadPower("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic.256K.1/1")
	svm.LoadPower("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic.64K.1/1")
	svm.LoadPower("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic.16K.1/1")
	svm.LoadPower("svm.sampling_svf.4194304.2/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.4M.1/2")
	svm.LoadPower("svm.sampling_svf.4194304.4/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.4M.1/4")
	svm.LoadPower("svm.sampling_svf.4194304.8/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.4M.1/8")
	svm.LoadPower("svm.sampling_svf.1048576.2/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.1M.1/2")
	svm.LoadPower("svm.sampling_svf.1048576.4/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.1M.1/4")
	svm.LoadPower("svm.sampling_svf.1048576.8/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.1M.1/8")
	svm.LoadPower("svm.sampling_svf.1048576.16/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.1M.1/16")
	svm.LoadPower("svm.sampling_svf.1048576.32/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.1M.1/32")
	svm.LoadPower("svm.sampling_svf.262144.2/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.256K.1/2")
	svm.LoadPower("svm.sampling_svf.262144.4/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.256K.1/4")
	svm.LoadPower("svm.sampling_svf.262144.8/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.256K.1/8")
	svm.LoadPower("svm.sampling_svf.262144.16/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.256K.1/16")
	svm.LoadPower("svm.sampling_svf.262144.32/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.256K.1/32")
	svm.LoadPower("svm.sampling_svf.262144.64/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.256K.1/64")
	svm.LoadPower("svm.sampling_svf.65536.2/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.64K.1/2")
	svm.LoadPower("svm.sampling_svf.65536.4/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.64K.1/4")
	svm.LoadPower("svm.sampling_svf.65536.8/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.64K.1/8")
	svm.LoadPower("svm.sampling_svf.65536.16/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.64K.1/16")
	svm.LoadPower("svm.sampling_svf.65536.32/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.64K.1/32")
	svm.LoadPower("svm.sampling_svf.65536.64/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.64K.1/64")
	svm.LoadPower("svm.sampling_svf.16384.2/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.16K.1/2")
	svm.LoadPower("svm.sampling_svf.16384.4/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.16K.1/4")
	svm.LoadPower("svm.sampling_svf.16384.8/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.16K.1/8")
	svm.LoadPower("svm.sampling_svf.16384.16/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.16K.1/16")
	svm.LoadPower("svm.sampling_svf.16384.32/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.16K.1/32")
	svm.LoadPower("svm.sampling_svf.16384.64/o2wlslm", "ppa.output.sampling.yaml", "AVF-CL.base.16K.1/64")
	svm.LoadPower("svm.icache.sampling_svf.4194304.2/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.4M.1/2")
	svm.LoadPower("svm.icache.sampling_svf.4194304.4/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.4M.1/4")
	svm.LoadPower("svm.icache.sampling_svf.4194304.8/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.4M.1/8")
	svm.LoadPower("svm.icache.sampling_svf.1048576.2/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.1M.1/2")
	svm.LoadPower("svm.icache.sampling_svf.1048576.4/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.1M.1/4")
	svm.LoadPower("svm.icache.sampling_svf.1048576.8/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.1M.1/8")
	svm.LoadPower("svm.icache.sampling_svf.1048576.16/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.1M.1/16")
	svm.LoadPower("svm.icache.sampling_svf.1048576.32/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.1M.1/32")
	svm.LoadPower("svm.icache.sampling_svf.262144.2/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.256K.1/2")
	svm.LoadPower("svm.icache.sampling_svf.262144.4/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.256K.1/4")
	svm.LoadPower("svm.icache.sampling_svf.262144.8/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.256K.1/8")
	svm.LoadPower("svm.icache.sampling_svf.262144.16/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.256K.1/16")
	svm.LoadPower("svm.icache.sampling_svf.262144.32/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.256K.1/32")
	svm.LoadPower("svm.icache.sampling_svf.262144.64/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.256K.1/64")
	svm.LoadPower("svm.icache.sampling_svf.65536.2/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.64K.1/2")
	svm.LoadPower("svm.icache.sampling_svf.65536.4/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.64K.1/4")
	svm.LoadPower("svm.icache.sampling_svf.65536.8/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.64K.1/8")
	svm.LoadPower("svm.icache.sampling_svf.65536.16/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.64K.1/16")
	svm.LoadPower("svm.icache.sampling_svf.65536.32/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.64K.1/32")
	svm.LoadPower("svm.icache.sampling_svf.65536.64/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.64K.1/64")
	svm.LoadPower("svm.icache.sampling_svf.16384.2/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.16K.1/2")
	svm.LoadPower("svm.icache.sampling_svf.16384.4/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.16K.1/4")
	svm.LoadPower("svm.icache.sampling_svf.16384.8/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.16K.1/8")
	svm.LoadPower("svm.icache.sampling_svf.16384.16/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.16K.1/16")
	svm.LoadPower("svm.icache.sampling_svf.16384.32/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.16K.1/32")
	svm.LoadPower("svm.icache.sampling_svf.16384.64/o2wlslm", "ppa.output.sampling.icache.yaml", "AVF-CL.ic.16K.1/64")

	svm.ProcessPower()

	items_base = \
		[{"linelabel": "4M", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "1M", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "256K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "64K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		 {"linelabel": "16K", "format": "AVF-CL.base.$linelabel$.$xaxis$"},
		]
	items_ic = \
		[{"linelabel": "4M", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "1M", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "256K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "64K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		 {"linelabel": "16K", "format": "AVF-CL.ic.$linelabel$.$xaxis$"},
		]
	xaxis = ["1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64"]
	size = [2.4,4]
	styles = ['-o', '-*', '-+', '-x', '-D']

	apps = ["403.gcc", "456.hmmer", "464.h264ref", "483.xalancbmk", "geomean"]
	legend_shown = False
	for app in apps:
		svm.Plot(mode="linegraph_single_app",
			ylabel="Energy consumption overhead",
			xlabel="Duty cycle",
			items=items_base,
			filename="linegraph_power_sampling_base_%s.pdf" % app.replace(".","_"),
			type="power_norm_ovd", yrange=[0, 0.12],
			xaxis=xaxis, size=size, app=app, legend=not legend_shown,
			styles=styles)
		svm.Plot(mode="linegraph_single_app",
			ylabel="Energy consumption overhead",
			xlabel="Duty cycle",
			items=items_ic,
			filename="linegraph_power_sampling_ic_%s.pdf" % app.replace(".","_"),
			type="power_norm_ovd", yrange=[0, 0.12],
			xaxis=xaxis, size=size, app=app, legend=not legend_shown,
			styles=styles)
		legend_shown = True

	items_sampling_base = [
		svm.BarItem("AVF-CL.base", "AVF-CL.base", color='red'),
		svm.BarItem("AVF-CL.ic", "AVF-CL.ic", color='salmon'),
		svm.BarItem("AVF-CL.base.4M.1/2", "4M.1/2"),
		svm.BarItem("AVF-CL.base.4M.1/4", "4M.1/4"),
		svm.BarItem("AVF-CL.base.4M.1/8", "4M.1/8"),
		svm.BarItem("AVF-CL.base.1M.1/2", "1M.1/2"),
		svm.BarItem("AVF-CL.base.1M.1/4", "1M.1/4"),
		svm.BarItem("AVF-CL.base.1M.1/8", "1M.1/8"),
		svm.BarItem("AVF-CL.base.1M.1/16", "1M.1/16"),
		svm.BarItem("AVF-CL.base.1M.1/32", "1M.1/32"),
		svm.BarItem("AVF-CL.base.256K.1/2", "256K.1/2"),
		svm.BarItem("AVF-CL.base.256K.1/4", "256K.1/4"),
		svm.BarItem("AVF-CL.base.256K.1/8", "256K.1/8"),
		svm.BarItem("AVF-CL.base.256K.1/16", "256K.1/16"),
		svm.BarItem("AVF-CL.base.256K.1/32", "256K.1/32"),
		svm.BarItem("AVF-CL.base.256K.1/64", "256K.1/64"),
		svm.BarItem("AVF-CL.base.64K.1/2", "64K.1/2"),
		svm.BarItem("AVF-CL.base.64K.1/4", "64K.1/4"),
		svm.BarItem("AVF-CL.base.64K.1/8", "64K.1/8"),
		svm.BarItem("AVF-CL.base.64K.1/16", "64K.1/16"),
		svm.BarItem("AVF-CL.base.64K.1/32", "64K.1/32"),
		svm.BarItem("AVF-CL.base.64K.1/64", "64K.1/64"),
		svm.BarItem("AVF-CL.base.16K.1/2", "16K.1/2"),
		svm.BarItem("AVF-CL.base.16K.1/4", "16K.1/4"),
		svm.BarItem("AVF-CL.base.16K.1/8", "16K.1/8"),
		svm.BarItem("AVF-CL.base.16K.1/16", "16K.1/16"),
		svm.BarItem("AVF-CL.base.16K.1/32", "16K.1/32"),
		svm.BarItem("AVF-CL.base.16K.1/64", "16K.1/64"),
	]

	items_sampling_icache = [
		svm.BarItem("AVF-CL.base", "AVF-CL.base", color='red'),
		svm.BarItem("AVF-CL.ic", "AVF-CL.ic", color='salmon'),
		svm.BarItem("AVF-CL.ic.4M.1/2", "4M.1/2"),
		svm.BarItem("AVF-CL.ic.4M.1/4", "4M.1/4"),
		svm.BarItem("AVF-CL.ic.4M.1/8", "4M.1/8"),
		svm.BarItem("AVF-CL.ic.1M.1/2", "1M.1/2"),
		svm.BarItem("AVF-CL.ic.1M.1/4", "1M.1/4"),
		svm.BarItem("AVF-CL.ic.1M.1/8", "1M.1/8"),
		svm.BarItem("AVF-CL.ic.1M.1/16", "1M.1/16"),
		svm.BarItem("AVF-CL.ic.1M.1/32", "1M.1/32"),
		svm.BarItem("AVF-CL.ic.256K.1/2", "256K.1/2"),
		svm.BarItem("AVF-CL.ic.256K.1/4", "256K.1/4"),
		svm.BarItem("AVF-CL.ic.256K.1/8", "256K.1/8"),
		svm.BarItem("AVF-CL.ic.256K.1/16", "256K.1/16"),
		svm.BarItem("AVF-CL.ic.256K.1/32", "256K.1/32"),
		svm.BarItem("AVF-CL.ic.256K.1/64", "256K.1/64"),
		svm.BarItem("AVF-CL.ic.64K.1/2", "64K.1/2"),
		svm.BarItem("AVF-CL.ic.64K.1/4", "64K.1/4"),
		svm.BarItem("AVF-CL.ic.64K.1/8", "64K.1/8"),
		svm.BarItem("AVF-CL.ic.64K.1/16", "64K.1/16"),
		svm.BarItem("AVF-CL.ic.64K.1/32", "64K.1/32"),
		svm.BarItem("AVF-CL.ic.64K.1/64", "64K.1/64"),
		svm.BarItem("AVF-CL.ic.16K.1/2", "16K.1/2"),
		svm.BarItem("AVF-CL.ic.16K.1/4", "16K.1/4"),
		svm.BarItem("AVF-CL.ic.16K.1/8", "16K.1/8"),
		svm.BarItem("AVF-CL.ic.16K.1/16", "16K.1/16"),
		svm.BarItem("AVF-CL.ic.16K.1/32", "16K.1/32"),
		svm.BarItem("AVF-CL.ic.16K.1/64", "16K.1/64"),
	]

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_base,
		filename="norm_energy_sampling_403.pdf",
		app="403.gcc", size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_base,
		filename="norm_energy_sampling_456.pdf",
		app="456.hmmer", size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_base,
		filename="norm_energy_sampling_464.pdf",
		app="464.h264ref", size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_base,
		filename="norm_energy_sampling_483.pdf",
		app="483.xalancbmk", size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_icache,
		filename="norm_energy_sampling_icache_403.pdf",
		app="403.gcc", size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_icache,
		filename="norm_energy_sampling_icache_456.pdf",
		app="456.hmmer", size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_icache,
		filename="norm_energy_sampling_icache_464.pdf",
		app="464.h264ref", size=[12, 4])

	svm.Plot(mode="bargraph_single_app", type="power_norm_ovd",
		ylabel="Normalized energy consumption",
		items=items_sampling_icache,
		filename="norm_energy_sampling_icache_483.pdf",
		app="483.xalancbmk", size=[12, 4])

def analyze_power_adaptive(log=logging.INFO):
	svm = Svm(log)

	svm.LoadPower("coretime/o2wlslm", "ppa.output.yaml", "baseline")
	svm.LoadPower("coretime/o2wlslm", "ppa.output.ecc.yaml", "ECC")
	svm.LoadPower("svm.adaptive_svf.16384.64.0.1/o2wlslm", "ppa.output.adaptive.yaml", "AVF-CL.base.10%")
	svm.LoadPower("svm.adaptive_svf.16384.64.0.05/o2wlslm", "ppa.output.adaptive.yaml", "AVF-CL.base.5%")
	svm.LoadPower("svm.icache.adaptive_svf.16384.64.0.1/o2wlslm", "ppa.output.adaptive.icache.yaml", "AVF-CL.ic.10%")
	svm.LoadPower("svm.icache.adaptive_svf.16384.64.0.05/o2wlslm", "ppa.output.adaptive.icache.yaml", "AVF-CL.ic.5%")

	svm.ProcessPower()

	svm.Plot(mode="bargraph", type="power_norm_ovd",
		ylabel="Normalized energy overhead",
		items=[
			svm.BarItem("ECC", "ECC", color='black'),
			svm.BarItem("AVF-CL.base.10%", "AVF-CL.S.base.10%", color='red'),
			svm.BarItem("AVF-CL.base.5%", "AVF-CL.S.base.5%", color='salmon'),
			],
		filename="norm_energy_adaptive_base.pdf",
		size=[6, 4])

	svm.Plot(mode="bargraph", type="power_norm_ovd",
		ylabel="Normalized energy overhead",
		items=[
			svm.BarItem("ECC", "ECC", color='black'),
			svm.BarItem("AVF-CL.ic.10%", "AVF-CL.S.ic.10%", color='blue'),
			svm.BarItem("AVF-CL.ic.5%", "AVF-CL.S.ic.5%", color='cornflowerblue'),
			],
		filename="norm_energy_adaptive_ic.pdf",
		size=[6, 4])

def analyze_area_base_old(log=logging.INFO):
	svm = Svm(log)

	svm.LoadArea("coretime/o2wlslm", "ppa.output.yaml", "baseline")
	svm.LoadArea("coretime/o2wlslm", "ppa.output.ecc.yaml", "ECC")
	svm.LoadArea("svm/o2wlslm", "ppa.output.svf.yaml", "AVF-CL.base")
	svm.LoadArea("svm.icache/o2wlslm", "ppa.output.svf.icache.yaml", "AVF-CL.ic")
	svm.LoadArea("svm.small_mcache/o2wlslm", "ppa.output.svf.smallmd.yaml", "AVF-CL.sm")
	svm.LoadArea("svm.small_mcache.icache/o2wlslm", "ppa.output.svf.icache.smallmd.yaml", "AVF-CL.sm.ic")
	svm.LoadArea("svm.no_mcache/o2wlslm", "ppa.output.svf.nomd.yaml", "AVF-CL.nm")

	grouping = [
		{"name": "AVF-CL.SRAMs", "color": "red", "suffix": ["_svf"]},
		{"name": "AVF-CL.logics", "color": "lightsalmon", "suffix": ["_svfmult", "_svfvlt", "_svfcounter", "spu"]},
		{"name": "M-TLB", "color": "green", "suffix": ["MTLB"]},
		{"name": "M-cache", "color": "limegreen", "suffix": ["L1MCache"]},
		{"name": "ECC.SRAMs", "color": "black", "suffix": ["_ecc"]},
		{"name": "ECC.logics", "color": "gray", "suffix": ["_ecc_checker", "_ecc_corrector"]},
	]
	grouping_detailed = [
		{"name": "AVF-CL.SRAMs.IBuff", "color": "maroon", "suffix": ["InstBuff_svf"]},
		{"name": "AVF-CL.SRAMs.RF", "color": "darkred", "suffix": ["AMT_svf"]},
		{"name": "AVF-CL.SRAMs.IQ", "color": "red", "suffix": ["PayloadRAM_svf"]},
		{"name": "AVF-CL.SRAMs.LQ", "color": "firebrick", "suffix": ["LDQ_CAM_svf"]},
		{"name": "AVF-CL.SRAMs.SQ", "color": "tomato", "suffix": ["STQ_DATA_RAM_svf"]},
		{"name": "AVF-CL.SRAMs.ROB", "color": "salmon", "suffix": ["ActiveList_dp_svf", "ActiveList_wb_svf"]},
		{"name": "AVF-CL.logics", "color": "lightsalmon", "suffix": ["_svfmult", "_svfvlt", "_svfcounter", "spu"]},
		{"name": "M-TLB", "color": "green", "suffix": ["MTLB"]},
		{"name": "M-cache", "color": "limegreen", "suffix": ["L1MCache"]},
		{"name": "ECC.SRAMs", "color": "black", "suffix": ["_ecc"]},
		{"name": "ECC.logics", "color": "gray", "suffix": ["_ecc_checker", "_ecc_corrector"]},
	]
	others = {"name": "I-cache overhead", "color": "cornflowerblue"}
	svm.ProcessArea(grouping, others)

	size = [3,4]
	yrange=[0,0.15]
	legend_loc = "best"
	svm.Plot(mode="bargraph_single_value", type="area_norm_ovd",
		ylabel="Normalized area overhead",
		items=[
			svm.BarItem("AVF-CL.base", "AVF-CL.base", color='red'),
			svm.BarItem("AVF-CL.ic", "AVF-CL.ic", color='salmon'),
			svm.BarItem("AVF-CL.sm", "AVF-CL.sm", color='green'),
			svm.BarItem("AVF-CL.sm.ic", "AVF-CL.sm.ic", color='limegreen'),
			svm.BarItem("AVF-CL.nm", "AVF-CL.nm", color='cornflowerblue'),
			svm.BarItem("ECC", "ECC", color='black'),
			],
		filename="norm_area.pdf", size=size, yrange=yrange,
		legend_loc=legend_loc)

	colors = [x["color"] for x in grouping]
	colors.append(others["color"])
	svm.Plot(mode="bargraph_single_stack", type="area_breakdown_norm",
		ylabel="Normalized area overhead",
		items=[
			svm.BarItem("AVF-CL.base", "AVF-CL.base"),
			svm.BarItem("AVF-CL.ic", "AVF-CL.ic"),
			svm.BarItem("AVF-CL.sm", "AVF-CL.sm"),
			svm.BarItem("AVF-CL.sm.ic", "AVF-CL.sm.ic"),
			svm.BarItem("AVF-CL.nm", "AVF-CL.nm"),
			svm.BarItem("ECC", "ECC"),
			],
		colors=colors,
		filename="norm_area_breakdown.pdf", size=size, yrange=yrange,
		legend_loc=legend_loc)

def build_analyze_list():
	analyze_list = []
	if sys.argv[1] == "all": # run all
		for fn in sorted(globals()):
			if fn.startswith("analyze_") and eval("callable("+fn+")"):
				analyze_list.append(fn)
	elif sys.argv[1].startswith("all."): # run.[str]
		suffix = sys.argv[1].replace("all.","")
		for fn in sorted(globals()):
			if fn.startswith("analyze_") and eval("callable("+fn+")") and \
				suffix in fn:
				analyze_list.append(fn)
	elif sys.argv[1] == "list": # just list down the supported runs
		for fn in sorted(globals()):
			if fn.startswith("analyze_") and eval("callable("+fn+")"):
				analyze_list.append(fn)
		print "[INFO] The following runs are supported:"
		for analyze in analyze_list:
			print "- %s" % (analyze.replace("analyze_",""))
		print "\n[INFO] Other supported runs:"
		print "- all"
		print "- list"
		exit(0) # exit application
	else: # specific runs
		for arg in sys.argv[1:]:
			fn = "analyze_" + arg
			if fn in globals() and eval("callable("+fn+")"):
				analyze_list.append(fn)
			else:
				print "Error! Undefined analysis run '%s'" % fn
				exit(1)

	## Show the list of analyses will be run
	print "[INFO] The following analyses will be run:"
	for analyze in analyze_list:
		print "- " + analyze + "()"
	print ""

	return analyze_list

def main():
	## making sure that argv[1] exists, else append "list"
	if len(sys.argv) == 1:
		sys.argv.append("list")

	## builing analyze_list
	analyze_list = build_analyze_list()

	## Run analysis one by one
	for analyze in analyze_list:
		print "[INFO] Running %s()" % analyze
		eval(analyze+"()")

if __name__ == '__main__':
	main()
