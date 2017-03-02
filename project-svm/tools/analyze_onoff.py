import os
import sys
import yaml
import benchmarks

import numpy as np
import matplotlib
matplotlib.use('PDF') # this need to be before import pyplot
from matplotlib import pyplot as plt

################################################################################

BASE_SAMPLES = 128 # number of smaller samples in 1M cycles period
STRUCTURES = ["iq", "rob", "lq", "sq", "rf", "ib", "sys",
	"fu_simp", "fu_comp", "fu_ctrl", "fu_mem"]

################################################################################

def LoadResultLog(log_path):
	# list of structures
	columns = STRUCTURES

	with open(log_path, "r") as handler:
		res = {}
		for linenum, line in enumerate(handler):
			if linenum == 0: continue # skip header line

			cells = line.strip().split("\t")
			for col, cell in enumerate(cells):
				if columns[col] not in res: res[columns[col]] = []
				res[columns[col]].append(float(cell))
		# return
		return res

def LoadResultLogSampling(log_path, recp_act_rate=1):
	# list of structures
	columns = STRUCTURES
	tmp = [0 for n in xrange(len(columns))]

	active_samples = BASE_SAMPLES/recp_act_rate
	assert(active_samples > 0)

	with open(log_path, "r") as handler:
		res = {}
		for linenum, line in enumerate(handler):
			if linenum == 0: continue # skip header line

			time_for_posted = (((linenum)%BASE_SAMPLES) == 0)

			cells = line.strip().split("\t")
			for col, cell in enumerate(cells):
				if ((linenum-1)%BASE_SAMPLES) < (active_samples):
					tmp[col] += float(cell)
				if time_for_posted:
					if columns[col] not in res: res[columns[col]] = []
					average_tmp = tmp[col]/active_samples # get average
					res[columns[col]].append(average_tmp)

			# if last tmp was posted, clear them all
			if time_for_posted:
				tmp = [0 for n in xrange(len(columns))]

		# return
		return res

def LoadSvmSummary(res_path):
	summary_file = res_path + "/svm_results.yaml"
	summary = yaml.load(open(summary_file, "r").read())
	return summary

def LoadResultsSingleSim(res_path, svfs_recp_act_rate=1):
	calcs = ["avf", "occ", "svf", "avfe"]

	# load logs
	logs = {}
	for calc in calcs:
		if svfs_recp_act_rate != 1 and calc == "svf":
			# in case of SVF sampling mode
			logs[calc] = LoadResultLogSampling(
				"%s/%s_group_8K.log" % (res_path, calc), svfs_recp_act_rate)
		else:
			logs[calc] = LoadResultLog("%s/%s_group_1M.log" % (res_path, calc))

	# load SVM summary
	summary = LoadSvmSummary(res_path)

	res = {}
	res["logs"] = logs
	res["summary"] = summary
	return res

def LoadResultsSingleConfig(path, svfs_recp_act_rate=1):
	res = {}

	# get all application directories
	apps = os.walk(path).next()[1]

	for app in sorted(apps):
		res[app] = {}
		# get all checkpoint directories in app result
		checkpoints = os.walk(path + "/" + app).next()[1]

		for chkpt in sorted(checkpoints):
			res_path = path + "/" + app + "/" + chkpt + "/results"
			res[app][chkpt] = LoadResultsSingleSim(res_path, svfs_recp_act_rate)

	return res

def LoadResults():
	res = {}

	# list of all configs
	configs = [
		# config path, reciprocal active rate
		["svm/o2wlslm", 1],
		["svm.adaptive_svf.2/o2wlslm", 2],
		["svm.adaptive_svf.4/o2wlslm", 4],
		["svm.adaptive_svf.8/o2wlslm", 8],
		["svm.adaptive_svf.128/o2wlslm", 128]
		]

	# load results for all configs
	for config in configs:
		res[config[0]] = LoadResultsSingleConfig(config[0], config[1])

	return res

################################################################################

def ComputeAvgErrorSingleSim(simres):
	data_avf = simres["avf"]
	calcs_error = {}
	# iterate through each calcs
	for calc in simres:
		if calc == "avf": continue # skip "avf vs avf error"
		calcs_error[calc] = {}
		# iterate through structures
		for structure in simres[calc]:
			data_calc = simres[calc]
			assert(len(data_calc[structure]) == len(data_avf[structure]))
			abs_error = 0;
			for n in xrange(len(data_calc[structure])):
				if n == 0: continue # skip the first row (warm-up row)
				abs_error += abs(data_calc[structure][n]-data_avf[structure][n])
			abs_error /= len(data_calc[structure])-1;
			calcs_error[calc][structure] = abs_error
	return calcs_error

def ComputeAvgError(res):
	calcs_error = {}
	# iterate through available configs
	for config in res:
		calcs_error[config] = {}
		# iterate through applications
		for app in res[config]:
			calcs_error[config][app] = {}
			# iterate through checkpoints
			for chkpt in res[config][app]:
				data = ComputeAvgErrorSingleSim(res[config][app][chkpt]["logs"])
				calcs_error[config][app][chkpt] = data
	return calcs_error

def AggregateAvgErrorSingleApp(calc_err, app, cfg_res):
	aggr = {}
	numerator = {}
	denominator = {}

	for chkpt in calc_err[app]:
		weight = benchmarks.weights[app][chkpt]
		for calc in calc_err[app][chkpt]:
			if calc not in aggr:
				aggr[calc] = {}
				numerator[calc] = {}
				denominator[calc] = {}
			for structure in calc_err[app][chkpt][calc]:
				if structure not in aggr[calc]:
					aggr[calc][structure] = 0
					numerator[calc][structure] = 0
					denominator[calc][structure] = 0
				vf_err = calc_err[app][chkpt][calc][structure]
				cyc = cfg_res[app][chkpt]["summary"]["clocks_simulated"]

				numerator[calc][structure] += vf_err * cyc * weight
				denominator[calc][structure] += cyc * weight

	for calc in aggr:
		for structure in aggr[calc]:
			aggr[calc][structure] = \
				numerator[calc][structure]/denominator[calc][structure]
	return aggr

def AggregateAvgError(calcs_error_raw, res):
	aggr_err = {}
	# iterate through available configs
	for config in calcs_error_raw:
		aggr_err[config] = {}
		# iterate through applications
		for app in calcs_error_raw[config]:
			aggr = AggregateAvgErrorSingleApp(
				calcs_error_raw[config], app, res[config])
			aggr_err[config][app] = aggr
	return aggr_err

def ProcessResults(res):
	calcs_error_raw = ComputeAvgError(res)
	calcs_error_aggr = AggregateAvgError(calcs_error_raw, res)

	ret = {}
	ret["error_aggr"] = calcs_error_aggr
	ret["time_series"] = res

	return ret

################################################################################

def GeneratePlotAbsErrorOneStructure(res, structure):
	data = []
	x_val = []

	raw = res["error_aggr"]["svm/o2wlslm"]
	colors = ['r','b','c','y','g','k']
	calcs = ['avfe','svf'] #,'occ']
	calcs_excluded = []

	for app in sorted(raw):
		x_val.append(app)
		if len(data) == 0:
			for n in xrange(len(calcs)):
				if structure in raw[app][calcs[n]]: # ensure calc has data
					data.append([])
				else: # else, exclude calc in the plot
					calcs_excluded.append(calcs[n])
			for calc in calcs_excluded:
				# make sure the calc to be excluded is in the list
				assert(calc in calcs)
				n = calcs.index(calc)
				# remove both color and calc item from the list
				del colors[n]
				calcs.remove(calc)

		for n, calc in enumerate(calcs):
			data[n].append(raw[app][calc][structure])

	raw = res["error_aggr"]["svm.adaptive_svf.2/o2wlslm"]
	subdata = []
	for app in sorted(raw):
		subdata.append(raw[app]['svf'][structure])
	calcs.append('svfs.1/2')
	data.append(subdata)

	raw = res["error_aggr"]["svm.adaptive_svf.4/o2wlslm"]
	subdata = []
	for app in sorted(raw):
		subdata.append(raw[app]['svf'][structure])
	calcs.append('svfs.1/4')
	data.append(subdata)

	raw = res["error_aggr"]["svm.adaptive_svf.8/o2wlslm"]
	subdata = []
	for app in sorted(raw):
		subdata.append(raw[app]['svf'][structure])
	calcs.append('svfs.1/8')
	data.append(subdata)

	raw = res["error_aggr"]["svm.adaptive_svf.128/o2wlslm"]
	subdata = []
	for app in sorted(raw):
		subdata.append(raw[app]['svf'][structure])
	calcs.append('svfs.1/128')
	data.append(subdata)

	# compute mean
	x_val.append("mean")
	for n in xrange(len(data)):
		data[n].append(sum(data[n])/len(data[n]))

	# generate plot
	fig = plt.figure()
	plt.ylabel('Average Absolute Error')
	plt.grid(True, axis='y')

	width = 0.8/len(calcs)
	ind = np.arange(len(x_val))
	plt.xticks(ind + 0.4, x_val)
	for n, calc in enumerate(calcs):
		plt.bar(ind+n*width, data[n], width, color=colors[n], label=calcs[n])

	plt.legend(loc='best')
	fig.autofmt_xdate()

	plt.savefig("bargraph_%s.pdf" % structure)
	plt.close() # need this to close plot explicitly

def GenerateAbsErrorPlots(res):
	structures_err = STRUCTURES
	for s in structures_err:
		GeneratePlotAbsErrorOneStructure(res, s)

def GenerateTimeSeriesPlotsSingleAppOld(res, suffix=''):
	structures = STRUCTURES
	suffix = suffix.replace("/",".")

	if not os.path.isdir("ts"): os.makedirs("ts")

	for chkpt in res:
		data = res[chkpt]["logs"]
		x = range(len(data["avf"][structures[0]]))
		for s in structures:
			for n, calc in enumerate(data):
				 # exclude calc if structure s data is not available
				if s not in data[calc]: continue
				if calc == "occ": continue # skip occ plot
				plt.plot(x[1:], data[calc][s][1:], linewidth=2, label=calc)
			plt.legend(loc='best')
			plt.savefig("ts/%s_%s.pdf" % (s, suffix))
			plt.close() # need this to close plot explicitly

def GenerateTimeSeriesPlotsSingleApp(res, app, cfg):
	structures = STRUCTURES
	cfg = cfg.replace("/","_")

	if not os.path.isdir("ts"): os.makedirs("ts")

	for chkpt in res:
		data = res[chkpt]["logs"]
		x = range(len(data["avf"][structures[0]])-1)
		fig, subs = plt.subplots(nrows=len(structures), sharex=True)
		fig.set_size_inches(7, 4*len(structures))
		fig.set_tight_layout(True)
		for s, st in enumerate(structures):
			subs[s].set_title(st)
			subs[s].set_ylim(bottom=0,auto=True)
			for n, calc in enumerate(data):
				 # exclude calc if structure s data is not available
				if st not in data[calc]: continue
				lw = 2 if calc is not "avf" else 4
				subs[s].plot(x,data[calc][st][1:],linewidth=lw,label=calc)

			if s == 0: subs[s].legend(loc='best')
		plt.savefig("ts/ts.%s.%s.%s.pdf" % (app, chkpt, cfg))
		plt.close() # need this to close plot explicitly

def GenerateTimeSeriesPlots(res):
	ts = res["time_series"]
	for cfg in ts:
		for app in ts[cfg]:
			GenerateTimeSeriesPlotsSingleApp(ts[cfg][app], app, cfg)

def GeneratePlots(res):
	GenerateAbsErrorPlots(res)
	GenerateTimeSeriesPlots(res)

################################################################################

def Info(str):
	FORMAT_BLUE_BOLD = "\033[1;34m"
	FORMAT_CLEAR = "\033[0m"
	print FORMAT_BLUE_BOLD + "INFO: " + str + FORMAT_CLEAR

def main():
	# load data from simulation results
	Info("loading results data...")
	raw_results = LoadResults()

	# process raw simulation results data
	Info("processing results data...")
	results = ProcessResults(raw_results)

	# generate plots
	Info("generating plots...")
	GeneratePlots(results)

if __name__ == "__main__":
    main()
