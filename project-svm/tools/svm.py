import logging
import os
import pprint
import subprocess
import sys
import yaml
import pprint

import resultdb
import benchmarks
import log
import calc

import numpy as np
import matplotlib
matplotlib.use('PDF') # this need to be before import pyplot
from matplotlib import pyplot as plt

class Svm:
	#### properties ############################################################
	res = None # SVM results database handler
	plots_dir = "plots"
	fdump = None
	structures = ["iq", "rob", "lq", "sq", "rf", "ib", "sys",
		"fu_simp", "fu_comp", "fu_ctrl", "fu_mem"]
	sample_size = 8*1024

	#### public methods ########################################################
	def __init__(self, logging_level=logging.INFO):
		self.Clear()

		# initialize logging module
		log.Init("SVM", logging_level)

	def Clear(self):
		self.res = resultdb.ResultDb()

	def Load(self, config, alias=None, ts_info=None, ignored_apps=[]):
		if alias == None: alias = config
		logging.info("Loading config '%s' as '%s'..." % (config, alias))

		# make sure config directory exists
		if not os.path.isdir(config):
			logging.error("Couldn't find '%s' directory." % config)
			exit(1)

		# get all application directories
		apps = os.walk(config).next()[1]

		# repeat for each apps
		res = {}
		for app in sorted(apps):
			if app in ignored_apps: continue
			res[app] = {}

			# get all checkpoint directories in app result
			chkpts = os.walk(config+"/"+app).next()[1]

			for chkpt in sorted(chkpts):
				res_path = config+"/"+app+"/"+chkpt+"/results"
				res[app][chkpt] = self.LoadResultsSingleSim(res_path, ts_info)

		self.res.raw[alias] = res

	def LoadPower(self, config, ppa_out, alias, ignored_apps=[]):
		logging.info("Loading config '%s-%s' as %s" % (config, ppa_out, alias))

		# make sure config directory exists
		if not os.path.isdir(config):
			logging.error("Couldn't find '%s' directory." % config)
			exit(1)

		# get all application directories
		apps = os.walk(config).next()[1]

		# repeat for each apps
		res = {}
		for app in sorted(apps):
			if app in ignored_apps: continue
			res[app] = {}

			# get all checkpoint directories in app result
			chkpts = os.walk(config+"/"+app).next()[1]

			for chkpt in sorted(chkpts):
				res_path = config+"/"+app+"/"+chkpt+"/results"
				res[app][chkpt] = self.LoadPowerResultsSingleSim(res_path, ppa_out)

		self.res.raw[alias] = res

	def LoadArea(self, config, ppa_out, alias):
		logging.info("Loading config '%s-%s' as %s" % (config, ppa_out, alias))

		# make sure config directory exists
		if not os.path.isdir(config):
			logging.error("Couldn't find '%s' directory." % config)
			exit(1)

		# get all application directories
		apps = os.walk(config).next()[1]

		# repeat for each apps
		res = {}
		for app in sorted(apps):
			res[app] = {}

			# get all checkpoint directories in app result
			chkpts = os.walk(config+"/"+app).next()[1]

			for chkpt in sorted(chkpts):
				res_path = config+"/"+app+"/"+chkpt+"/results"
				res[app][chkpt] = self.LoadAreaResultsSingleSim(res_path, ppa_out)

		self.res.raw[alias] = res

	def Process(self, perf=True, vf=True):
		logging.info("Processing results...")

		# aggregate CPI of each applications
		self.res.processed["cpi"] = {}
		for config in self.res.raw:
			logging.info("  Computing CPI of '%s'" % config)
			cpis = {}
			for app in self.res.raw[config]:
				app_cpi = 0.0
				tweight = 0.0
				for chkpt in self.res.raw[config][app]:
					stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
					cycles = float(stats['cycles'])
					insts_retired = float(stats['process_0']['insts_retired'])
					weight = benchmarks.weights[app][chkpt]

					cpi = cycles/insts_retired
					wcpi = cpi * weight
					tweight += weight

					logging.debug(app + "-" + chkpt + "-CPI = %f" % cpi)
					logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
					logging.debug(app + "-" + chkpt + "-wCPI = %f" % wcpi)

					app_cpi += wcpi

				if tweight < 0.99:
					logging.warning("Config %s app %s has total weight %f" % \
						(config, app, tweight))

				app_cpi /= tweight

				logging.debug(app + "-CPI = %f" % app_cpi)
				cpis[app] = app_cpi

			self.res.processed["cpi"][config] = cpis
			logging.debug(cpis)

		# aggregate VBits/inst of each applications
		self.res.processed["avf"] = {}
		for config in self.res.raw:
			if "baseline" in config: continue # skip coretime
			if not vf: continue
			logging.info("  Computing avf_VBPI of '%s'" % config)
			avfs = {}
			for app in self.res.raw[config]:
				avf_app_vbpi = 0.0
				tweight = 0.0
				for chkpt in self.res.raw[config][app]:
					stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
					insts_retired = float(stats['process_0']['insts_retired'])
					svm_results = self.res.raw[config][app][chkpt]["svm_results"]
					tvb = float(svm_results["avf_total_vbits"]["sys"])
					weight = benchmarks.weights[app][chkpt]

					if "sys_bits" not in self.res.processed:
						self.res.processed["sys_bits"] = {}
					if config not in self.res.processed["sys_bits"]:
						self.res.processed["sys_bits"][config] = \
							svm_results["structures_total_size"]

					avf_vbpi = tvb/insts_retired
					avf_wvbpi = avf_vbpi * weight
					tweight += weight

					logging.debug(app + "-" + chkpt + "-avf_vbPI = %f" % avf_vbpi)
					logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
					logging.debug(app + "-" + chkpt + "-avf_wvbPI = %f" % avf_wvbpi)

					avf_app_vbpi += avf_wvbpi

				if tweight < 0.99:
					logging.warning("Config %s app %s has total weight %f" % \
						(config, app, tweight))

				avf_app_vbpi /= tweight

				logging.debug(app + "-avf_vbPI = %f" % avf_app_vbpi)

				app_cpi = self.res.processed["cpi"][config][app]
				app_vbpc = avf_app_vbpi/app_cpi
				app_avf = app_vbpc / self.res.processed["sys_bits"][config]

				avfs[app] = app_avf

			self.res.processed["avf"][config] = avfs
			logging.debug(avfs)
		# self.res.processed["svf"] = {}
		# for config in self.res.raw:
		# 	if "baseline" in config: continue # skip coretime
		# 	if not vf: continue
		# 	logging.info("  Computing svf_VBPI of '%s'" % config)
		# 	svfs = {}
		# 	for app in self.res.raw[config]:
		# 		svf_app_vbpi = 0.0
		# 		tweight = 0.0
		# 		for chkpt in self.res.raw[config][app]:
		# 			stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
		# 			insts_retired = float(stats['process_0']['insts_retired'])
		# 			svm_results = self.res.raw[config][app][chkpt]["svm_results"]
		# 			tvb = float(svm_results["svf_total_vbits"]["sys"])
		# 			weight = benchmarks.weights[app][chkpt]

		# 			svf_vbpi = tvb/insts_retired
		# 			svf_wvbpi = svf_vbpi * weight
		# 			tweight += weight

		# 			logging.debug(app + "-" + chkpt + "-svf_vbPI = %f" % svf_vbpi)
		# 			logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
		# 			logging.debug(app + "-" + chkpt + "-svf_wvbPI = %f" % svf_wvbpi)

		# 			svf_app_vbpi += svf_wvbpi

		# 		if tweight < 0.99:
		# 			logging.warning("Config %s app %s has total weight %f" % \
		# 				(config, app, tweight))

		# 		svf_app_vbpi /= tweight

		# 		logging.debug(app + "-svf_vbPI = %f" % svf_app_vbpi)

		# 		app_cpi = self.res.processed["cpi"][config][app]
		# 		app_vbpc = svf_app_vbpi/app_cpi
		# 		app_svf = app_vbpc / self.res.processed["sys_bits"][config]

		# 		svfs[app] = app_svf

		# 	self.res.processed["svf"][config] = svfs
		# 	logging.debug(svfs)
		# self.res.processed["svf.nlv"] = {}
		# for config in self.res.raw:
		# 	if "baseline" in config: continue # skip coretime
		# 	if not vf: continue
		# 	logging.info("  Computing svf_VBPI of '%s'" % config)
		# 	svfs = {}
		# 	for app in self.res.raw[config]:
		# 		svf_app_vbpi = 0.0
		# 		tweight = 0.0
		# 		for chkpt in self.res.raw[config][app]:
		# 			stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
		# 			insts_retired = float(stats['process_0']['insts_retired'])
		# 			svm_results = self.res.raw[config][app][chkpt]["svm_results"]
		# 			tvb = float(svm_results["svf_total_vbits_nlv"]["sys"])
		# 			weight = benchmarks.weights[app][chkpt]

		# 			svf_vbpi = tvb/insts_retired
		# 			svf_wvbpi = svf_vbpi * weight
		# 			tweight += weight

		# 			logging.debug(app + "-" + chkpt + "-svf_vbPI = %f" % svf_vbpi)
		# 			logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
		# 			logging.debug(app + "-" + chkpt + "-svf_wvbPI = %f" % svf_wvbpi)

		# 			svf_app_vbpi += svf_wvbpi

		# 		if tweight < 0.99:
		# 			logging.warning("Config %s app %s has total weight %f" % \
		# 				(config, app, tweight))

		# 		svf_app_vbpi /= tweight

		# 		logging.debug(app + "-svf_vbPI = %f" % svf_app_vbpi)

		# 		app_cpi = self.res.processed["cpi"][config][app]
		# 		app_vbpc = svf_app_vbpi/app_cpi
		# 		app_svf = app_vbpc / self.res.processed["sys_bits"][config]

		# 		svfs[app] = app_svf

		# 	self.res.processed["svf.nlv"][config] = svfs
		# 	logging.debug(svfs)
		# self.res.processed["svf.nm"] = {}
		# for config in self.res.raw:
		# 	if "baseline" in config: continue # skip coretime
		# 	if not vf: continue
		# 	logging.info("  Computing svf_VBPI of '%s'" % config)
		# 	svfs = {}
		# 	for app in self.res.raw[config]:
		# 		svf_app_vbpi = 0.0
		# 		tweight = 0.0
		# 		for chkpt in self.res.raw[config][app]:
		# 			stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
		# 			insts_retired = float(stats['process_0']['insts_retired'])
		# 			svm_results = self.res.raw[config][app][chkpt]["svm_results"]
		# 			tvb = float(svm_results["svf_total_vbits_nm"]["sys"])
		# 			weight = benchmarks.weights[app][chkpt]

		# 			svf_vbpi = tvb/insts_retired
		# 			svf_wvbpi = svf_vbpi * weight
		# 			tweight += weight

		# 			logging.debug(app + "-" + chkpt + "-svf_vbPI = %f" % svf_vbpi)
		# 			logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
		# 			logging.debug(app + "-" + chkpt + "-svf_wvbPI = %f" % svf_wvbpi)

		# 			svf_app_vbpi += svf_wvbpi

		# 		if tweight < 0.99:
		# 			logging.warning("Config %s app %s has total weight %f" % \
		# 				(config, app, tweight))

		# 		svf_app_vbpi /= tweight

		# 		logging.debug(app + "-svf_vbPI = %f" % svf_app_vbpi)

		# 		app_cpi = self.res.processed["cpi"][config][app]
		# 		app_vbpc = svf_app_vbpi/app_cpi
		# 		app_svf = app_vbpc / self.res.processed["sys_bits"][config]

		# 		svfs[app] = app_svf

		# 	self.res.processed["svf.nm"][config] = svfs
		# 	logging.debug(svfs)
		# self.res.processed["occ"] = {}
		# for config in self.res.raw:
		# 	if "baseline" in config: continue # skip coretime
		# 	if not vf: continue
		# 	logging.info("  Computing occ_VBPI of '%s'" % config)
		# 	occs = {}
		# 	for app in self.res.raw[config]:
		# 		occ_app_vbpi = 0.0
		# 		tweight = 0.0
		# 		for chkpt in self.res.raw[config][app]:
		# 			stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
		# 			insts_retired = float(stats['process_0']['insts_retired'])
		# 			svm_results = self.res.raw[config][app][chkpt]["svm_results"]
		# 			tvb = float(svm_results["occupancy_total_vbits"]["sys"])
		# 			weight = benchmarks.weights[app][chkpt]

		# 			occ_vbpi = tvb/insts_retired
		# 			occ_wvbpi = occ_vbpi * weight
		# 			tweight += weight

		# 			logging.debug(app + "-" + chkpt + "-occ_vbPI = %f" % occ_vbpi)
		# 			logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
		# 			logging.debug(app + "-" + chkpt + "-occ_wvbPI = %f" % occ_wvbpi)

		# 			occ_app_vbpi += occ_wvbpi

		# 		if tweight < 0.99:
		# 			logging.warning("Config %s app %s has total weight %f" % \
		# 				(config, app, tweight))

		# 		occ_app_vbpi /= tweight

		# 		logging.debug(app + "-occ_vbPI = %f" % occ_app_vbpi)

		# 		app_cpi = self.res.processed["cpi"][config][app]
		# 		app_vbpc = occ_app_vbpi/app_cpi
		# 		app_occ = app_vbpc / self.res.processed["sys_bits"][config]

		# 		occs[app] = app_occ

		# 	self.res.processed["occ"][config] = occs
		# 	logging.debug(occs)

		# # aggregate AVF with considering the ECC activity
		# self.res.processed["avf_ecc"] = {}
		# for config in self.res.raw:
		# 	if "baseline" in config: continue # skip coretime
		# 	if not vf: continue
		# 	logging.info("  Computing avf with ECC of '%s'" % config)
		# 	avfs = {}
		# 	for app in self.res.raw[config]:
		# 		avf_app_vfpi = 0.0
		# 		tweight = 0.0
		# 		for chkpt in self.res.raw[config][app]:
		# 			stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
		# 			cycles = float(stats['cycles'])
		# 			insts_retired = float(stats['process_0']['insts_retired'])
		# 			weight = benchmarks.weights[app][chkpt]

		# 			ts_avf = self.res.raw[config][app][chkpt]["ts_avf"]
		# 			ts_ecc_activity = self.res.raw[config][app][chkpt]["ts_ecc_activity"]

		# 			avf_app = 0.0
		# 			avf_sys_idx = 6
		# 			assert(len(ts_ecc_activity)==len(ts_avf[avf_sys_idx]))
		# 			for n in xrange(len(ts_ecc_activity)):
		# 				avf_app += ts_avf[avf_sys_idx][n] * (0 if ts_ecc_activity[n] else 1)
		# 			avf_app /= len(ts_ecc_activity)

		# 			avf_vfpi = avf_app * cycles / insts_retired
		# 			avf_wvfpi = avf_vfpi * weight
		# 			tweight += weight

		# 			logging.debug(app + "-" + chkpt + "-avf_vfPI = %f" % avf_vfpi)
		# 			logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
		# 			logging.debug(app + "-" + chkpt + "-avf_wvfPI = %f" % avf_wvfpi)

		# 			avf_app_vfpi += avf_wvfpi

		# 		avf_app_vfpi /= tweight

		# 		logging.debug(app + "-avf_vfPI = %f" % avf_app_vfpi)

		# 		app_cpi = self.res.processed["cpi"][config][app]
		# 		avfs[app] = avf_app_vfpi/app_cpi

		# 	self.res.processed["avf_ecc"][config] = avfs
		# 	logging.debug(avfs)

		# compute performance comparison
		baseline = "baseline"
		if "baseline" not in self.res.processed["cpi"]:
			baseline = "coretime/o2wlslm"

		perf_norm = {}
		perf_norm_ovd = {}
		all_cpis = self.res.processed["cpi"]

		for config in all_cpis:
			if config == baseline: continue # skip baseline
			if not perf: continue
			logging.info("  Computing normalized perf of '%s'" % config)

			perf_norm[config] = {}
			perf_norm_ovd[config] = {}
			norms = []

			# update values
			for app in sorted(all_cpis[config]):
				if app not in all_cpis[baseline]:
					logging.warning(
						"    No baseline information for config '%s' app '%s'" % \
						(config, app))
					continue
				norm = all_cpis[config][app]/all_cpis[baseline][app]
				perf_norm[config][app] = norm
				perf_norm_ovd[config][app] = norm - 1
				norms.append(norm)

			if len(norms) > 0:
				geo = calc.geomean(norms)
				perf_norm[config]["geomean"] = geo
				perf_norm_ovd[config]["geomean"] = geo - 1

		self.res.processed["perf_norm"] = perf_norm
		self.res.processed["perf_norm_ovd"] = perf_norm_ovd

		# process average instantaneous error
		self.res.processed["ts_vulnerability"] = {}
		avg_errors = {}
		## compute average errors
		for config in self.res.raw:
			self.res.processed["ts_vulnerability"][config] = {}
			avg_errors[config] = {}
			for app in self.res.raw[config]:
				self.res.processed["ts_vulnerability"][config][app] = {}
				avg_errors[config][app] = {}
				for chkpt in self.res.raw[config][app]:
					if "ts_vulnerability" in self.res.raw[config][app][chkpt]:
						logs = self.res.raw[config][app][chkpt]["ts_vulnerability"]
						self.res.processed["ts_vulnerability"][config][app][chkpt] = logs

						print "%s %s" % (app, chkpt)
						data = self.ComputeAvgErrorSingleSim(logs)
						avg_errors[config][app][chkpt] = data

		self.res.processed["vf_instant_avg_errors"] = avg_errors

		## aggregate average errors
		aggr_errors = {}
		# iterate through available configs
		for config in avg_errors:
			aggr_errors[config] = {}
			# iterate through applications
			for app in avg_errors[config]:
				aggr = self.AggregateAvgErrorSingleApp(
					avg_errors[config], app, self.res.raw[config])
				aggr_errors[config][app] = aggr
		self.res.processed["vf_instant_aggr_errors"] = aggr_errors

		# process average vf based on timeseries data
		self.res.processed["ts_vulnerability"] = {}
		avg_vfs = {}
		## compute average errors
		for config in self.res.raw:
			self.res.processed["ts_vulnerability"][config] = {}
			avg_vfs[config] = {}
			for app in self.res.raw[config]:
				self.res.processed["ts_vulnerability"][config][app] = {}
				avg_vfs[config][app] = {}
				for chkpt in self.res.raw[config][app]:
					if "ts_vulnerability" in self.res.raw[config][app][chkpt]:
						logs = self.res.raw[config][app][chkpt]["ts_vulnerability"]
						self.res.processed["ts_vulnerability"][config][app][chkpt] = logs

						print "%s %s" % (app, chkpt)
						data = self.ComputeAvgVfSingleSim(logs)
						avg_vfs[config][app][chkpt] = data

		self.res.processed["vf_instant_avg_vfs"] = avg_vfs

		## aggregate average vf based on timeseries data
		aggr_vfs = {}
		# iterate through available configs
		for config in avg_vfs:
			aggr_vfs[config] = {}
			# iterate through applications
			for app in avg_vfs[config]:
				aggr = self.AggregateAvgVfSingleApp(
					avg_vfs[config], app, self.res.raw[config])
				aggr_vfs[config][app] = aggr
		self.res.processed["vf_instant_aggr_vfs"] = aggr_vfs

		# process average absolute error ratio
		self.res.processed["ts_vulnerability"] = {}
		avg_errors_ratio = {}
		## compute average errors
		for config in self.res.raw:
			self.res.processed["ts_vulnerability"][config] = {}
			avg_errors_ratio[config] = {}
			for app in self.res.raw[config]:
				self.res.processed["ts_vulnerability"][config][app] = {}
				avg_errors_ratio[config][app] = {}
				for chkpt in self.res.raw[config][app]:
					if "ts_vulnerability" in self.res.raw[config][app][chkpt]:
						logs = self.res.raw[config][app][chkpt]["ts_vulnerability"]
						self.res.processed["ts_vulnerability"][config][app][chkpt] = logs

						print "%s %s" % (app, chkpt)
						data = self.ComputeAvgErrorRatioSingleSim(logs)
						avg_errors_ratio[config][app][chkpt] = data

		self.res.processed["vf_instant_avg_errors_ratio"] = avg_errors_ratio

		## aggregate average errors ratio
		aggr_errors_ratio = {}
		# iterate through available configs
		for config in avg_errors_ratio:
			aggr_errors_ratio[config] = {}
			# iterate through applications
			for app in avg_errors_ratio[config]:
				aggr = self.AggregateAvgErrorSingleApp(
					avg_errors_ratio[config], app, self.res.raw[config])
				aggr_errors_ratio[config][app] = aggr
		self.res.processed["vf_instant_aggr_errors_ratio"] = aggr_errors_ratio

	def ProcessPower(self):
		logging.info("Processing power results...")

		# aggregate energy consumption of each applications
		self.res.processed["power"] = {}
		for config in self.res.raw:
			logging.info("  Computing power of '%s'" % config)
			powers = {}
			for app in self.res.raw[config]:
				app_epi = 0.0
				tweight = 0.0
				for chkpt in self.res.raw[config][app]:
					stats = self.res.raw[config][app][chkpt]["stats"]["stats"]
					insts_retired = float(stats['process_0']['insts_retired'])
					ppa_out = self.res.raw[config][app][chkpt]["power"]
					energy = float(ppa_out['processor']['core_0']['total_energy'].replace("nJ",""))
					weight = benchmarks.weights[app][chkpt]

					epi = energy/insts_retired
					wepi = epi * weight
					tweight += weight

					logging.debug(app + "-" + chkpt + "-epi = %f" % epi)
					logging.debug(app + "-" + chkpt + "-weight = %f" % weight)
					logging.debug(app + "-" + chkpt + "-wepi = %f" % wepi)

					app_epi += wepi

				if tweight < 0.99:
					logging.warning("Config %s app %s has total weight %f" % \
						(config, app, tweight))
				app_epi /= tweight

				logging.debug(app + "-epi = %f" % app_epi)
				powers[app] = app_epi

			self.res.processed["power"][config] = powers
			logging.debug(powers)

		# compute power comparison
		baseline = "baseline"

		power_norm = {}
		power_norm_ovd = {}
		all_powers = self.res.processed["power"]

		for config in all_powers:
			if config == baseline: continue # skip baseline
			logging.info("  Computing normalized power of '%s'" % config)

			power_norm[config] = {}
			power_norm_ovd[config] = {}
			norms = []

			# update values
			for app in sorted(all_powers[config]):
				norm = all_powers[config][app]/all_powers[baseline][app]
				power_norm[config][app] = norm
				power_norm_ovd[config][app] = norm - 1
				norms.append(norm)

			geo = calc.geomean(norms)
			power_norm[config]["geomean"] = geo
			power_norm_ovd[config]["geomean"] = geo - 1

		self.res.processed["power_norm"] = power_norm
		self.res.processed["power_norm_ovd"] = power_norm_ovd

	def ProcessArea(self, grouping, others):
		logging.info("Processing area results...")

		# find area of each config
		self.res.processed["area"] = {}
		for config in self.res.raw:
			logging.info("  Computing area of '%s'" % config)

			area = None

			for app in self.res.raw[config]:
				for chkpt in self.res.raw[config][app]:
					ppa_out = self.res.raw[config][app][chkpt]["area"]
					area = float(ppa_out['processor']['core_0']['area'].replace("mm^2",""))

				if area is not None:
					break

			self.res.processed["area"][config] = area
			logging.debug(area)

		# compute the breakdown of area
		self.res.processed["area_breakdown"] = {}
		for config in self.res.raw:
			logging.info("  Computing breakdown area of '%s'" % config)

			area_breakdown = {}
			for group in grouping:
				area_breakdown[group["name"]] = 0.0

			found = False
			for app in self.res.raw[config]:
				for chkpt in self.res.raw[config][app]:
					ppa_out = self.res.raw[config][app][chkpt]["area"]
					found = True

					worklist = [[ppa_out['processor']['core_0'], "core_0"]]
					while (len(worklist)):
						work = worklist.pop()

						db = work[0]
						name = work[1]

						for idx in db:
							if isinstance(db[idx], dict):
								worklist.append([db[idx],idx])
							elif idx == "area":
								unit_area = float(db[idx].replace("mm^2",""))
								for group in grouping:
									gid = group["name"]
									for suffix in group["suffix"]:
										if name.endswith(suffix):
											area_breakdown[gid] += unit_area
					break

				if area is not None:
					break

			self.res.processed["area_breakdown"][config] = area_breakdown
			logging.debug(area_breakdown)

		# compute area comparison
		baseline = "baseline"

		area_norm = {}
		area_norm_ovd = {}
		all_areas = self.res.processed["area"]

		for config in all_areas:
			if config == baseline: continue # skip baseline
			logging.info("  Computing normalized area of '%s'" % config)

			area_norm[config] = all_areas[config]/all_areas[baseline]
			area_norm_ovd[config] = area_norm[config] - 1

		self.res.processed["area_norm"] = area_norm
		self.res.processed["area_norm_ovd"] = area_norm_ovd

		# compute breakdown area comparison
		baseline = "baseline"

		breakdown_area_norm = {}
		all_areas = self.res.processed["area"]
		all_breakdown_areas = self.res.processed["area_breakdown"]

		for config in all_areas:
			if config == baseline: continue # skip baseline
			logging.info("  Computing normalized breakdown area of '%s'" % config)

			breakdown_area_norm[config] = {}
			groups_area = 0.0
			for group in all_breakdown_areas[config]:
				breakdown_area_norm[config][group] = all_breakdown_areas[config][group]/all_areas[baseline]
				groups_area += breakdown_area_norm[config][group]
			breakdown_area_norm[config][others["name"]] = self.res.processed["area_norm_ovd"][config] - groups_area
			assert(breakdown_area_norm[config][others["name"]] >= -0.001)
			if breakdown_area_norm[config][others["name"]] < 0.001:
				breakdown_area_norm[config][others["name"]] = 0.0

		self.res.processed["area_breakdown_norm"] = breakdown_area_norm

	def Plot(self, mode, ylabel, items, filename, xlabel=None, type=None,
		app=None, geomean=False, arithmean=False, yrange=None, size=None,
		colors=None, aliases=None, structures=None, methods=None, xaxis=None,
		legend=True, styles=None, legend_loc=None, legend_cols="default",
		structure_aliases=None, linestyles=None, linewidths=None):
		if mode == "bargraph":
			pdata = self.res.processed[type]

			pkg = {}
			pkg["ylabel"] = ylabel
			pkg["data"] = []
			pkg["colors"] = [i["color"] for i in items]
			pkg["barlabels"] = [i["alias"] for i in items]
			pkg["xaxis"] = [bar for bar in sorted(pdata[items[0]["config"]])]

			for item in items:
				subdata = []
				for x in pkg["xaxis"]:
					subdata.append(pdata[item["config"]][x])
				if geomean: subdata.append(calc.geomean(subdata))
				if arithmean: subdata.append(calc.mean(subdata))
				pkg["data"].append(subdata)

			if geomean: pkg["xaxis"].append("geomean")
			if arithmean: pkg["xaxis"].append("average")

			self.PlotBarGraph(pkg, filename, yrange, size,
				legend_loc=legend_loc, legend_cols=legend_cols)

		elif mode == "bargraph_single_app":
			pdata = self.res.processed[type]

			pkg = {}
			pkg["ylabel"] = ylabel
			pkg["data"] = []
			pkg["colors"] = [items[0]["color"]]
			pkg["barlabels"] = [app]
			pkg["xaxis"] = [item["alias"] for item in items]

			subdata = []
			for item in items:
				subdata.append(pdata[item["config"]][app])

			pkg["data"].append(subdata)

			self.PlotBarGraph(pkg, filename, yrange, size, legend=False)

		elif mode == "bargraph_single_value":
			pdata = self.res.processed[type]

			pkg = {}
			pkg["ylabel"] = ylabel
			pkg["data"] = []
			pkg["colors"] = [items[0]["color"]]
			pkg["barlabels"] = [app]
			pkg["xaxis"] = [item["alias"] for item in items]

			subdata = []
			for item in items:
				subdata.append(pdata[item["config"]])

			pkg["data"].append(subdata)

			self.PlotBarGraph(pkg, filename, yrange, size, legend=False)

		elif mode == "bargraph_single_stack":
			pdata = self.res.processed[type]

			pkg = {}
			pkg["ylabel"] = ylabel
			pkg["data"] = []
			pkg["colors"] = colors
			pkg["barlabels"] = [bar for bar in sorted(pdata[items[0]["config"]])]
			pkg["xaxis"] = [i["alias"] for i in items]

			for bar in pkg["barlabels"]:
				subdata = []
				for x in pkg["xaxis"]:
					subdata.append(pdata[x][bar])
				if geomean: subdata.append(calc.geomean(subdata))
				if arithmean: subdata.append(calc.mean(subdata))
				pkg["data"].append(subdata)

			if geomean: pkg["xaxis"].append("geomean")
			if arithmean: pkg["xaxis"].append("average")

			self.PlotBarGraph(pkg, filename, yrange, size, stacked=True, \
				legend_cols=2)

		elif mode == "bargraph_methods":
			pdata = self.res.processed[type]
			legend_shown = False

			for structure in structures:
				pkg = {}
				pkg["data"] = []
				pkg["ylabel"] = ylabel if not legend_shown else None
				pkg["barlabels"] = []
				pkg["colors"] = []
				pkg["xaxis"] = []

				for item in items:
					config = item["config"]
					method = item["method"]
					color = item["color"]
					alias = item["alias"]

					subdata = []
					for app in sorted(pdata[config]):
						if app not in pkg["xaxis"]: pkg["xaxis"].append(app)
						print pdata[config][app]
						print method
						print pdata[config][app][method]
						print structure
						if method in pdata[config][app] and \
							structure in pdata[config][app][method]:
							subdata.append(pdata[config][app][method][structure])
							if alias not in pkg["barlabels"]:
								pkg["barlabels"].append(alias)
								pkg["colors"].append(color)
					if len(subdata) > 0:
						if geomean: subdata.append(calc.geomean(subdata))
						if arithmean: subdata.append(calc.mean(subdata))
						pkg["data"].append(subdata)

				if geomean: pkg["xaxis"].append("geomean")
				if arithmean: pkg["xaxis"].append("average")

				self.PlotBarGraph(pkg,
					filename.replace("$structure$",structure), size=size,
					legend=not legend_shown, legend_loc=legend_loc,
					yrange=yrange)
				if not legend_shown: legend_shown=True

		elif mode == "bargraph_vf":
			pdata = self.res.processed

			pkg = {}
			pkg["ylabel"] = ylabel
			pkg["data"] = []
			pkg["colors"] = [i["color"] for i in items]
			pkg["barlabels"] = [i["alias"] for i in items]
			pkg["xaxis"] = [bar for bar in sorted(pdata[items[0]["calc"]][items[0]["config"]])]

			for item in items:
				subdata = []
				for x in pkg["xaxis"]:
					subdata.append(pdata[item["calc"]][item["config"]][x])
				if geomean: subdata.append(calc.geomean(subdata))
				if arithmean: subdata.append(calc.mean(subdata))
				pkg["data"].append(subdata)

			if geomean: pkg["xaxis"].append("geomean")
			if arithmean: pkg["xaxis"].append("average")

			self.PlotBarGraph(pkg, filename, yrange, size, legend_loc=legend_loc)

		elif mode == "linegraph_single_app":
			pdata = self.res.processed[type]

			pkg = {}
			pkg["ylabel"] = ylabel
			pkg["xlabel"] = xlabel
			pkg["data"] = []
			pkg["linelabels"] = []

			for item in items:
				subdata = []
				pkg["linelabels"].append(item["linelabel"])
				for x in xaxis:
					config = item["format"].replace("$linelabel$",item["linelabel"]).replace("$xaxis$",x)
					if config in pdata and app in pdata[config]:
						if structures != None and methods != None:
							if structures[0] in pdata[config][app][methods[0]]:
								subdata.append(pdata[config][app][methods[0]][structures[0]])
						else:
							subdata.append(pdata[config][app])
				assert(len(subdata) > 0)
				pkg["data"].append(subdata)

			pkg["xaxis"] = xaxis

			self.PlotLineGraph(pkg, filename, yrange, size, legend=legend, \
				styles=styles)

		elif mode == "timeseries":
			pdata = self.res.processed[type]
			basename = filename

			for item in items:
				for app in pdata[item["config"]]:
					cfg = item["config"].replace("/", "_")

					for chkpt in pdata[item["config"]][app]:
						pkg = {}
						pkg["data"] = pdata[item["config"]][app][chkpt]
						pkg["ylabel"] = ylabel
						pkg["xlabel"] = xlabel
						pkg["aliases"] = aliases
						pkg["colors"] = colors
						pkg["linestyles"] = linestyles
						pkg["linewidths"] = linewidths

						filename = ("%s.%s.%s.%s" % (basename,app,chkpt,cfg))
						filename = filename.replace(".","_")
						filename += ".pdf"

						self.PlotTimeSeries(pkg, filename, structures,
							structure_aliases)

		else:
			logging.error("Unsupported plot mode '%s'" % mode)

	def RobOpcode(self, config):
		# make sure config directory exists
		if not os.path.isdir(config):
			logging.error("Couldn't find '%s' directory." % config)
			exit(1)

		# get all application directories
		apps = os.walk(config).next()[1]

		# find vbit_summary.yaml file from one of the results
		vbit_summary = None
		for app in sorted(apps):
			# get all checkpoint directories in app result
			chkpts = os.walk(config+"/"+app).next()[1]

			for chkpt in sorted(chkpts):
				res_path = config+"/"+app+"/"+chkpt+"/results"
				# load svm_results file
				f = res_path+"/vbit_summary.yaml"
				if os.path.isfile(f):
					vbit_summary = yaml.load(open(f, "r").read())
					break
			if vbit_summary:
				break

		# now, analyze ROB, IQ, and IB vbits
		# LQ and SQ 'opcode' can be inferred from the available signal
		IQ = vbit_summary["IQ"]
		ROB = vbit_summary["ROB"]
		IB = vbit_summary["InstBuff"]

		op2vbits = {}
		opcnt = 0
		for opname in IQ:
			opcnt += 1
			vbits = [IQ[opname]['imp'], IQ[opname]['unimp']]
			vbits += [ROB[opname]['imp'], ROB[opname]['unimp']]
			vbits += [ROB[opname]['imp_dispatch'], ROB[opname]['unimp_dispatch']]
			vbits += [IB[opname]['imp'], IB[opname]['unimp']]

			found = False
			for op in op2vbits:
				if vbits == op2vbits[op]["vbits"]:
					found = True
					op2vbits[op]["original_opname"] += [opname]
					op2vbits[op]["original_opcode"] += [IQ[opname]['opcode']]
					break

			if not found:
				idx = len(op2vbits)
				op2vbit = {}
				op2vbit["vbits"] = vbits
				op2vbit["original_opname"] = [opname]
				op2vbit["original_opcode"] = [IQ[opname]['opcode']]
				op2vbits[idx] = op2vbit

		pprint.pprint(op2vbits)
		print "INFO: %d instruction opcodes analyzed." % opcnt

	def BarItem(self, config, alias=None, method=None, color='k'):
		bar = {
			"config": config,
			"alias": alias if alias != None else config,
			"method": method,
			"color": color,
			}
		return bar

	def BarVfItem(self, config, calc, alias, color='k'):
		bar = {
			"config": config,
			"calc": calc,
			"alias": alias,
			"color": color,
			}
		return bar

	#### private methods #######################################################

	def LoadResultsSingleSim(self, path, ts_info=None):
		res = {}
		path_svfonly = path.split("/")
		path_svfonly[0] += ".svfonly"
		path_svfonly = "/".join(path_svfonly)

		# load svm_results file
		f = path+"/svm_results.yaml"
		if os.path.isfile(f):
			res["svm_results"] = yaml.load(open(f, "r").read())

			f = path_svfonly+"/svm_results.yaml"
			if os.path.isfile(f):
				svfonly = yaml.load(open(f, "r").read())
				res["svm_results"].update(svfonly)

		# load stats file
		f = path+"/stats.yaml"
		if os.path.isfile(f):
			res["stats"] = yaml.load(open(f, "r").read())
		else:
			logging.error("File %s not found." % f)
			exit(1)

		# load ecc protection activity file if available
		f = path+"/ecc_8K.log"
		if os.path.isfile(f):
			res["ts_ecc_activity"] = []
			header_skipped = False
			for line in open(f, "r"):
				if not header_skipped: header_skipped = True; continue
				res["ts_ecc_activity"].append(int(line) != 0)

		# load AVFs per samples
		f = path+"/avf_group_8K.log"
		if os.path.isfile(f):
			res["ts_avf"] = []
			header_skipped = False
			for line in open(f, "r"):
				if not header_skipped:
					for n, val in enumerate(line.split("\t")):
						res["ts_avf"].append([])
					header_skipped = True
					continue

				for n, val in enumerate(line.split("\t")):
					res["ts_avf"][n].append(float(val))

		# load time series data if ts_info is provided
		if ts_info != None:
			active_size = ts_info["active_size"]
			rarate = ts_info["rarate"]
			methods = ts_info["methods"]
			window_size = active_size*rarate

			logs = {}
			for method in methods:
				logs_method = None
				if rarate != 1:
					if method in ["avf", "occ"]:
						logs_method = self.LoadResultLogSampling( \
							"%s/%s_group_8K.log" % (path, method), \
							window_size, 1)
					elif method in ["svf", "avfcl"]:
						f = "%s/%s_group_8K.log" % (path, method)
						f_so = "%s/%s_group_8K.log" % (path_svfonly, method)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLogSampling( \
								f_so, active_size,rarate)
						else:
							logs_method = self.LoadResultLogSampling( \
								f, active_size,rarate)
					elif method in ["svf.nlv"]:
						f = "%s/svf_group_8K.nlv.log" % (path)
						f_so = "%s/svf_group_8K.nlv.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLogSampling( \
								f_so, active_size,rarate)
						else:
							logs_method = self.LoadResultLogSampling( \
								f, active_size,rarate)
					elif method in ["svf.nm"]:
						f = "%s/svf_group_8K.nm.log" % (path)
						f_so = "%s/svf_group_8K.nm.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLogSampling( \
								f_so, active_size,rarate)
						else:
							logs_method = self.LoadResultLogSampling( \
								f, active_size,rarate)
					elif method in ["svf.prf"]:
						f = "%s/svf_group_8K.prf.log" % (path)
						f_so = "%s/svf_group_8K.prf.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLogSampling( \
								f_so, active_size,rarate)
						else:
							logs_method = self.LoadResultLogSampling( \
								f, active_size,rarate)
					elif method in ["svf.prf_nop2"]:
						f = "%s/svf_group_8K.prf_nop2.log" % (path)
						f_so = "%s/svf_group_8K.prf_nop2.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLogSampling( \
								f_so, active_size,rarate)
						else:
							logs_method = self.LoadResultLogSampling( \
								f, active_size,rarate)
					else:
						logs_method = self.LoadResultLog( \
							"%s/%s_group_1M.log" % (path, method))
				else:
					if method in ["svf.nlv"]:
						f = "%s/svf_group_1M.nlv.log" % (path)
						f_so = "%s/svf_group_1M.nlv.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLog(f_so)
						else:
							logs_method = self.LoadResultLog(f)
					elif method in ["svf.nm"]:
						f = "%s/svf_group_1M.nm.log" % (path)
						f_so = "%s/svf_group_1M.nm.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLog(f_so)
						else:
							logs_method = self.LoadResultLog(f)
					elif method in ["svf.prf"]:
						f = "%s/svf_group_1M.prf.log" % (path)
						f_so = "%s/svf_group_1M.prf.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLog(f_so)
						else:
							logs_method = self.LoadResultLog(f)
					elif method in ["svf.prf_nop2"]:
						f = "%s/svf_group_1M.prf_nop2.log" % (path)
						f_so = "%s/svf_group_1M.prf_nop2.log" % (path_svfonly)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLog(f_so)
						else:
							logs_method = self.LoadResultLog(f)
					else:
						f = "%s/%s_group_1M.log" % (path, method)
						f_so = "%s/%s_group_1M.log" % (path_svfonly, method)
						if os.path.isfile(f_so):
							logs_method = self.LoadResultLog(f_so)
						else:
							logs_method = self.LoadResultLog(f)
				if logs_method:
					logs[method] = logs_method

			res["ts_vulnerability"] = logs

		return res

	def LoadResultLog(self, log_path):
		# list of structures
		columns = self.structures

		if not os.path.isfile(log_path):
			return None

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

	def LoadResultLogSampling(self, log_path, active, recp_act_rate=1):
		# list of structures
		columns = self.structures
		tmp = [0 for n in xrange(len(columns))]

		active_samples = active / self.sample_size
		assert(active_samples > 0)

		if not os.path.isfile(log_path):
			return None

		with open(log_path, "r") as handler:
			res = {}
			for linenum, line in enumerate(handler):
				if linenum == 0: continue # skip header line

				time_for_posted = \
					(((linenum)%(active_samples*recp_act_rate)) == 0)

				cells = line.strip().split("\t")
				for col, cell in enumerate(cells):
					if ((linenum-1)%(active_samples*recp_act_rate)) < \
						(active_samples):
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

	def LoadPowerResultsSingleSim(self, path, ppa_out):
		res = {}

		# load stats file
		f = path+"/stats.yaml"
		if os.path.isfile(f):
			res["stats"] = yaml.load(open(f, "r").read())
		else:
			logging.error("File %s not found." % f)
			exit(1)

		# load ppa output file
		f = path+"/"+ppa_out
		if os.path.isfile(f):
			res["power"] = yaml.load(open(f, "r").read())
		else:
			logging.error("File %s not found." % f)
			exit(1)

		return res

	def LoadAreaResultsSingleSim(self, path, ppa_out):
		res = {}

		# load ppa output file
		f = path+"/"+ppa_out
		if os.path.isfile(f):
			res["area"] = yaml.load(open(f, "r").read())
		else:
			logging.error("File %s not found." % f)
			exit(1)

		return res

	def ComputeAvgErrorSingleSim(self, simres):
		data_avf = simres["avf"]
		avg_error = {}
		# iterate through each calcs
		for method in simres:
			if method == "avf": continue # skip "avf vs avf error"
			avg_error[method] = {}
			# iterate through structures
			for structure in simres[method]:
				data_method = simres[method]
				if(len(data_method[structure]) != len(data_avf[structure])):
					logging.warning("Data method len != Data AVF len (%d vs %d)"
						% (len(data_method[structure]), len(data_avf[structure])))
					if len(data_method[structure]) > len(data_avf[structure]):
						data_method[structure] = data_method[structure][0:len(data_avf[structure])]
					else:
						while len(data_method[structure]) < len(data_avf[structure]):
							data_method[structure].append(data_method[structure][-1])

				abs_error = 0;
				for n in xrange(len(data_method[structure])):
					abs_error += abs(\
						data_method[structure][n]-data_avf[structure][n])

				abs_error /= len(data_method[structure]);
				avg_error[method][structure] = abs_error
		return avg_error

	def ComputeAvgErrorRatioSingleSim(self, simres):
		data_avf = simres["avf"]
		avg_error_ratio = {}
		# iterate through each calcs
		for method in simres:
			if method == "avf": continue # skip "avf vs avf error"
			avg_error_ratio[method] = {}
			# iterate through structures
			for structure in simres[method]:
				data_method = simres[method]
				if(len(data_method[structure]) != len(data_avf[structure])):
					logging.warning("Data method len != Data AVF len (%d vs %d)"
						% (len(data_method[structure]), len(data_avf[structure])))
					if len(data_method[structure]) > len(data_avf[structure]):
						data_method[structure] = data_method[structure][0:len(data_avf[structure])]
					else:
						while len(data_method[structure]) < len(data_avf[structure]):
							data_method[structure].append(data_method[structure][-1])

				abs_error_ratio = 0;
				for n in xrange(len(data_method[structure])):
					if data_avf[structure][n] == 0: continue
					abs_error_ratio += abs(\
						data_method[structure][n]-data_avf[structure][n])/data_avf[structure][n]

				abs_error_ratio /= len(data_method[structure]);
				avg_error_ratio[method][structure] = abs_error_ratio
		return avg_error_ratio

	def AggregateAvgErrorSingleApp(self, method_err, app, cfg_res):
		aggr = {}
		numerator = {}
		denominator = {}

		for chkpt in method_err[app]:
			weight = benchmarks.weights[app][chkpt]
			for method in method_err[app][chkpt]:
				if method not in aggr:
					aggr[method] = {}
					numerator[method] = {}
					denominator[method] = {}
				for structure in method_err[app][chkpt][method]:
					if structure not in aggr[method]:
						aggr[method][structure] = 0
						numerator[method][structure] = 0
						denominator[method][structure] = 0
					vf_err = method_err[app][chkpt][method][structure]
					cyc = cfg_res[app][chkpt]["svm_results"]["clocks_simulated"]

					numerator[method][structure] += vf_err * cyc * weight
					denominator[method][structure] += cyc * weight

		for method in aggr:
			for structure in aggr[method]:
				aggr[method][structure] = \
					numerator[method][structure]/denominator[method][structure]
		return aggr

	def ComputeAvgVfSingleSim(self, simres):
		avg_vf = {}
		# iterate through each calcs
		for method in simres:
			avg_vf[method] = {}
			# iterate through structures
			for structure in simres[method]:
				data_method = simres[method]
				vf = 0;
				for n in xrange(len(data_method[structure])):
					vf += data_method[structure][n]

				vf /= len(data_method[structure]);
				avg_vf[method][structure] = vf
		return avg_vf

	def AggregateAvgVfSingleApp(self, method_vf, app, cfg_res):
		aggr = {}
		numerator = {}
		denominator = {}

		for chkpt in method_vf[app]:
			weight = benchmarks.weights[app][chkpt]
			for method in method_vf[app][chkpt]:
				if method not in aggr:
					aggr[method] = {}
					numerator[method] = {}
					denominator[method] = {}
				for structure in method_vf[app][chkpt][method]:
					if structure not in aggr[method]:
						aggr[method][structure] = 0
						numerator[method][structure] = 0
						denominator[method][structure] = 0
					vf = method_vf[app][chkpt][method][structure]
					cyc = cfg_res[app][chkpt]["svm_results"]["clocks_simulated"]

					numerator[method][structure] += vf * cyc * weight
					denominator[method][structure] += cyc * weight

		for method in aggr:
			for structure in aggr[method]:
				aggr[method][structure] = \
					numerator[method][structure]/denominator[method][structure]
		return aggr

	def PlotBarGraph(self, pkg, filename, yrange=None, size=None, legend=True, \
		legend_loc=None, legend_cols="default", stacked=False, title=None):
		logging.info("Plotting bargraph '%s'" % filename)

		fig = plt.figure()
		if size: fig.set_size_inches(size[0], size[1])

		ax = plt.subplot(111)
		if pkg["ylabel"]: ax.set_ylabel(pkg["ylabel"])
		ax.grid(True, axis='y')
		if yrange: ax.set_ylim(bottom=yrange[0], top=yrange[1])

		if title: ax.set_title(title)

		width = 0.8/len(pkg["barlabels"])
		ind = np.arange(len(pkg["xaxis"]))
		ax.set_xticks(ind + 0.4)
		ax.set_xticklabels(pkg["xaxis"])

		if stacked:
			bottom = [0 for k in pkg["data"][0]]
			for n, calc in enumerate(pkg["barlabels"]):
				ax.bar(ind, pkg["data"][n], width*len(pkg["barlabels"]), color=pkg["colors"][n],
					label=pkg["barlabels"][n], edgecolor="none", bottom=bottom)
				for l, k in enumerate(pkg["data"][n]):
					bottom[l] += k
		else:
			for n, calc in enumerate(pkg["barlabels"]):
				print pkg["data"][n]
				print pkg["barlabels"][n]
				print len(pkg["data"][n])
				ax.bar(ind+n*width, pkg["data"][n], width, color=pkg["colors"][n],
					label=pkg["barlabels"][n], edgecolor="none")

		if legend:
			if legend_loc == None: # imply a default location
				box = ax.get_position()
				ax.set_position([box.x0, box.y0, box.width, box.height * 0.6])
				ncols = (len(pkg["barlabels"])+1)/2 if legend_cols == "default" \
					else int(legend_cols)
				ax.legend(loc='upper center',
					bbox_to_anchor=(0.5, 1.15),
					fancybox=False,
					shadow=False,
					ncol=ncols,
					prop={'size':9})
			if legend_loc == "outside":
				box = ax.get_position()
				ax.set_position([box.x0, box.y0, box.width, box.height * 0.6])
				ncols = (len(pkg["barlabels"])+1)/2 if legend_cols == "default" \
					else int(legend_cols)
				ax.legend(loc='upper center',
					bbox_to_anchor=(0.5, 1.25),
					fancybox=False,
					shadow=False,
					ncol=ncols,
					prop={'size':9})
			else:
				ax.legend(loc=legend_loc, prop={'size':9})
		fig.autofmt_xdate()

		path = self.MakePath(filename)
		plt.savefig(path)
		plt.close() # need this to close plot explicitly

		## crop the generated pdf using pdfcrop
		subprocess.call(["pdfcrop",path,path])

		## dump the data into a text file
		fdump = open(self.MakePath(filename+".dump.txt"), "w")
		if pkg["ylabel"]: fdump.write(pkg["ylabel"])
		fdump.write("\n")
		fdump.write("\t")
		for barlabel in pkg["barlabels"]: fdump.write("%s\t" % barlabel)
		fdump.write("\n")
		for x, xlabel in enumerate(pkg["xaxis"]):
			fdump.write("%s\t" % xlabel)
			for b, barlabel in enumerate(pkg["barlabels"]):
				fdump.write(str(pkg["data"][b][x]))
				fdump.write("\t")
			fdump.write("\n")
		fdump.write('\n\n')

	def PlotLineGraph(self, pkg, filename, yrange=None, size=None, legend=True, \
		styles=None):
		logging.info("Plotting linegraph '%s'" % filename)

		fig = plt.figure()
		if size: fig.set_size_inches(size[0], size[1])

		ax = plt.subplot(111)
		if pkg["ylabel"]: ax.set_ylabel(pkg["ylabel"])
		if pkg["xlabel"]: ax.set_xlabel(pkg["xlabel"])
		ind = np.arange(len(pkg["xaxis"]))
		ax.set_xticks(ind + 0.4)
		ax.set_xticklabels(pkg["xaxis"])
		if yrange: ax.set_ylim(bottom=yrange[0], top=yrange[1])

		ax.grid(True, axis='y')
		if styles == None:
			styles = ['' for n in xrange(len(pkg["data"]))]

		for n, calc in enumerate(pkg["data"]):
			ax.plot(ind[:len(pkg["data"][n])] + 0.4, pkg["data"][n], styles[n],\
				linewidth=4, label=pkg["linelabels"][n], )

		if legend: ax.legend(loc="best", prop={'size':9})
		fig.autofmt_xdate()

		path = self.MakePath(filename)
		plt.savefig(path)
		plt.close() # need this to close plot explicitly

		## crop the generated pdf using pdfcrop
		subprocess.call(["pdfcrop",path,path])

		## dump the data into a text file
		fdump = open(self.MakePath(filename+".dump.txt"), "w")
		if pkg["ylabel"]: fdump.write(pkg["ylabel"])
		fdump.write("\n")
		fdump.write("\t")
		for xlabel in pkg["xaxis"]: fdump.write("%s\t" % xlabel)
		fdump.write("\n")
		for l, linelabel in enumerate(pkg["linelabels"]):
			fdump.write("%s\t" % linelabel)
			for val in pkg["data"][l]:
				fdump.write(str(val))
				fdump.write("\t")
			fdump.write("\n")
		fdump.write('\n\n')

	def PlotTimeSeries(self, pkg, filename, structures, structure_aliases):
		ts_dir = "plots/ts"
		if not os.path.isdir(ts_dir): os.makedirs(ts_dir)

		data = pkg["data"]
		aliases = pkg["aliases"]
		colors = pkg["colors"]
		linestyles = pkg["linestyles"]
		linewidths = pkg["linewidths"]

		x = range(len(data["avf"][structures[0]]))
		fig, subs = plt.subplots(ncols=len(structures), sharey=True)
		fig.set_size_inches(2.5*len(structures), 4)
		fig.set_tight_layout(True)

		for s, st in enumerate(structures):
			if structure_aliases:
				subs[s].set_title(structure_aliases[s])
			else:
				subs[s].set_title(st)
			# add AVF line
			method = "avf"
			assert(method in data)
			subs[s].plot(x,data[method][st],linewidth=linewidths[method],
				label=aliases[method],color=colors[method],
				linestyle=linestyles[method])
			# add other lines
			for n, method in enumerate(data):
				 # exclude method if structure s data is not available
				if st not in data[method]: continue
				if method is "avf": continue
				subs[s].plot(x,data[method][st],linewidth=linewidths[method],
					label=aliases[method],color=colors[method],
					linestyle=linestyles[method])

			if s == 0:
				subs[s].set_ylabel(pkg["ylabel"])
			if st == "sq":
				subs[s].legend(loc='best', prop={'size':9})
			subs[s].set_xlabel(pkg["xlabel"])
		path = ts_dir + "/" + filename
		plt.savefig(path)
		plt.close() # need this to close plot explicitly

		subprocess.call(["pdfcrop",path,path])

		fdump = open(path+".dump.txt", "w")
		if pkg["ylabel"]: fdump.write(pkg["ylabel"])
		fdump.write("\n")

		for s, st in enumerate(structures):
			fdump.write(st)
			fdump.write("\n")
			data_len = -1

			for n, method in enumerate(data):
				if st not in data[method]: continue
				fdump.write(aliases[method])
				fdump.write('\t')

				if data_len == -1:
					data_len = len(data[method][st])
			fdump.write('\n')

			for i in xrange(data_len):
				for n, method in enumerate(data):
					if st not in data[method]: continue
					fdump.write(str(data[method][st][i]))
					fdump.write('\t')
				fdump.write('\n')

			fdump.write('\n\n')

	def MakePath(self, filename):
		if not os.path.isdir(self.plots_dir): os.makedirs(self.plots_dir)
		return self.plots_dir+"/"+filename
