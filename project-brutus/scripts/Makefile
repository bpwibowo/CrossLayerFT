include Make.defs

.PHONY: all
all:

.PHONY: onoff
onoff:
	@if [ "$(RARATE)" == "" ]; then echo "Error: A required RARATE variable is undefined."; exit 1; fi;
	@if [ "$(ACTIVE)" == "" ]; then echo "Error: A required ACTIVE variable is undefined."; exit 1; fi;
	~/parallel -vv --no-run-if-empty -j $(PARALLEL_JOBS) --bar ./run.py {1} --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags=\"--warm 1000000 --skip 1000000000 --sim 20000000\" -q :::: ./applist.txt

.PHONY: onoff_bsub
onoff_bsub:
	@if [ "$(RARATE)" == "" ]; then echo "Error: A required RARATE variable is undefined."; exit 1; fi;
	@if [ "$(ACTIVE)" == "" ]; then echo "Error: A required ACTIVE variable is undefined."; exit 1; fi;
	bsub -q tuck -W 10000 -e runjob.401.bzip2.svm_onoff.%J.err -o runjob.401.bzip2.svm_onoff.%J.out ./run.py 401.bzip2 --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.403.gcc.svm_onoff.%J.err -o runjob.403.gcc.svm_onoff.%J.out ./run.py 403.gcc --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.429.mcf.svm_onoff.%J.err -o runjob.429.mcf.svm_onoff.%J.out ./run.py 429.mcf --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.445.gobmk.svm_onoff.%J.err -o runjob.445.gobmk.svm_onoff.%J.out ./run.py 445.gobmk --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.456.hmmer.svm_onoff.%J.err -o runjob.456.hmmer.svm_onoff.%J.out ./run.py 456.hmmer --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.458.sjeng.svm_onoff.%J.err -o runjob.458.sjeng.svm_onoff.%J.out ./run.py 458.sjeng --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.462.libquantum.svm_onoff.%J.err -o runjob.462.libquantum.svm_onoff.%J.out ./run.py 462.libquantum --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.464.h264ref.svm_onoff.%J.err -o runjob.464.h264ref.svm_onoff.%J.out ./run.py 464.h264ref --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.471.omnetpp.svm_onoff.%J.err -o runjob.471.omnetpp.svm_onoff.%J.out ./run.py 471.omnetpp --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.473.astar.svm_onoff.%J.err -o runjob.473.astar.svm_onoff.%J.out ./run.py 473.astar --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q
	bsub -q tuck -W 10000 -e runjob.483.xalancbmk.svm_onoff.%J.err -o runjob.483.xalancbmk.svm_onoff.%J.out ./run.py 483.xalancbmk --cfg $(CORE_CFG) -q --svm_adaptive_svf --svm_svf_active_size $(ACTIVE) --svm_svf_rarate $(RARATE) --svm_no_logallinsts --toolflags="--warm 1000000 --skip 2000000000 --sim 20000000" -q

.PHONY: allonoff
allonoff:
	make $(OPT) RARATE=1 ACTIVE=4194304
	make $(OPT) RARATE=2 ACTIVE=4194304
	make $(OPT) RARATE=8 ACTIVE=4194304
	make $(OPT) RARATE=2 ACTIVE=1048576
	make $(OPT) RARATE=4 ACTIVE=1048576
	make $(OPT) RARATE=8 ACTIVE=1048576
	make $(OPT) RARATE=16 ACTIVE=1048576
	make $(OPT) RARATE=2 ACTIVE=262144
	make $(OPT) RARATE=4 ACTIVE=262144
	make $(OPT) RARATE=8 ACTIVE=262144
	make $(OPT) RARATE=16 ACTIVE=262144
	make $(OPT) RARATE=32 ACTIVE=262144
	make $(OPT) RARATE=64 ACTIVE=262144
	make $(OPT) RARATE=2 ACTIVE=65536
	make $(OPT) RARATE=4 ACTIVE=65536
	make $(OPT) RARATE=8 ACTIVE=65536
	make $(OPT) RARATE=16 ACTIVE=65536
	make $(OPT) RARATE=32 ACTIVE=65536
	make $(OPT) RARATE=64 ACTIVE=65536
	make $(OPT) RARATE=2 ACTIVE=16384
	make $(OPT) RARATE=4 ACTIVE=16384
	make $(OPT) RARATE=8 ACTIVE=16384
	make $(OPT) RARATE=16 ACTIVE=16384
	make $(OPT) RARATE=32 ACTIVE=16384
	make $(OPT) RARATE=64 ACTIVE=16384

.PHONY: checkpoint
checkpoint:
	mkdir -p ../exp/intermediate/$(APP)/checkpoints.$(SIM_SIZE)
	$(CHECKPOINT_BIN) --checkpoint_exact --checkpoint_exact_begin $(SKIPS) --job ../exp/jobs/$(APP).ref.job --checkpoint_filename ../exp/intermediate/$(APP)/checkpoints.$(SIM_SIZE)/checkpoint --checkpoint_exact_weights $(WEIGHTS) --checkpoint_exact_length $(CHKPT_SIZE)

.PHONY: allcheckpoints
all_checkpoints:
	make checkpoint APP=222.montecarlo SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="140000000 540000000 590000000 690000000 940000000 990000000 1140000000 1190000000    " WEIGHTS="0.25 0.357143 0.0714286 0.142857 0.25 0.285714 0.107143 0.428571    "
	make checkpoint APP=401.bzip2 SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="90000000 91190000000 109790000000 181140000000 194190000000 252090000000 280290000000 290390000000 383690000000 408740000000 556190000000 " WEIGHTS="0.171337 0.0757158 0.124343 0.118651 0.0717427 0.0910659 0.0939979 0.152984 0.414626 0.513926 0.171611 "
	make checkpoint APP=403.gcc SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="6190000000 27440000000 48740000000 48890000000 52990000000 55440000000 67590000000 69240000000 104390000000   " WEIGHTS="0.701493 0.335198 0.0713287 0.0718284 0.0839161 0.364569 0.145522 0.144988 0.0811567   "
	make checkpoint APP=429.mcf SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="130690000000 139940000000 213340000000 225040000000 225240000000 226490000000 236190000000 245490000000 255490000000 258090000000 286340000000 327790000000" WEIGHTS="0.143789 0.262657 0.248043 0.00352296 0.00808977 0.0542797 0.119259 0.00495825 0.42667 0.417928 0.0597599 0.251044"
	make checkpoint APP=445.gobmk SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="9490000000 37890000000 41990000000 53640000000 87490000000 157890000000 224890000000 251390000000 268090000000 336590000000 359240000000 " WEIGHTS="0.215132 0.013899 0.159778 0.252719 0.243171 0.195311 0.2219 0.193981 0.17984 0.176698 0.147571 "
	make checkpoint APP=456.hmmer SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="1606490000000 1735290000000 1785540000000 2033690000000 2038190000000 2098290000000 2873190000000 3077340000000 3773590000000 3799090000000  " WEIGHTS="0.332491 0.244201 0.0995592 0.238199 0.235721 0.0909135 0.330277 0.0962438 0.233611 0.0987831  "
	make checkpoint APP=458.sjeng SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="99290000000 552740000000 554190000000 555690000000 873490000000 1368790000000 1883590000000 1980990000000 2888390000000 2998890000000  " WEIGHTS="0.0893814 0.219402 0.280487 0.195924 0.0002609 0.0852697 0.358053 0.357571 0.333384 0.0802664  "
	make checkpoint APP=462.libquantum SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="57990000000 183290000000 281140000000 394690000000 549990000000 574990000000 859190000000 1099390000000 2006990000000 2287190000000 2407540000000 " WEIGHTS="0.0960045 0.146966 0.240507 0.200653 0.0462598 0.233806 0.0459604 0.258748 0.289429 0.365482 0.0761855 "
	make checkpoint APP=464.h264ref SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="7390000000 26090000000 94090000000 187590000000 303990000000 361490000000 418090000000 459290000000 545290000000 639840000000 654490000000 " WEIGHTS="0.096997 0.0400871 0.0495458 0.0914414 0.428121 0.354253 0.349399 0.443994 0.0174912 0.110502 0.0181682 "
	make checkpoint APP=471.omnetpp SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="22190000000 24190000000 31340000000 153040000000 606490000000 2676390000000      " WEIGHTS="0.00821815 0.00825551 0.00474412 0.987 0.987 0.00478147      "
	make checkpoint APP=473.astar SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="890000000 990000000 40590000000 43990000000 128690000000 149990000000 248390000000 410990000000 411590000000 411640000000 492540000000 499590000000" WEIGHTS="0.0138203 0.0143139 0.147581 0.13998 0.585587 0.456367 0.129023 0.00848963 0.0475814 0.0479763 0.204738 0.204541"
	make checkpoint APP=483.xalancbmk SIM_SIZE=50M_100M CHKPT_SIZE=120000000 SKIPS="205090000000 209390000000 601390000000 924990000000 998190000000 1027740000000 1203440000000 1317190000000 1381440000000   " WEIGHTS="0.275283 0.275148 0.405533 0.0695765 0.24212 0.118245 0.302214 0.077064 0.234817   "

include Make.run
SEND_EMAIL_NOTIFICATION = echo "Simulations completed --- RUN = $@ --- APP = $(APP)" | mail -s "[Notification] Simulations completed on `uname -n`" `whoami`@ncsu.edu

.PHONY: runall
runall:
	make $(RUN) APP=401.bzip2
	make $(RUN) APP=429.mcf
	make $(RUN) APP=462.libquantum
	make $(RUN) APP=471.omnetpp
	make $(RUN) APP=473.astar
	make $(RUN) APP=483.xalancbmk
	make $(RUN) APP=403.gcc
	make $(RUN) APP=445.gobmk
	make $(RUN) APP=456.hmmer
	make $(RUN) APP=458.sjeng
	make $(RUN) APP=464.h264ref

.PHONY: runpartial
runpartial:
	make $(RUN) APP=403.gcc
	make $(RUN) APP=456.hmmer
	make $(RUN) APP=464.h264ref
	make $(RUN) APP=483.xalancbmk

.PHONY: runmontecarlo
runmontecarlo:
	make $(RUN) APP=222.montecarlo

.PHONY: coretime.run
coretime.run:
	make $(MAKE_FLAGS) RUN_FLAGS=-q TOOL=coretime $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

# base run with multiple modes
.PHONY: svm.base.run
svm.base.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: svm.base.icache.run
svm.base.icache.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_icache" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: svm.base.smallmd.run
svm.base.smallmd.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_small_mcache" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: svm.base.icache.smallmd.run
svm.base.icache.smallmd.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_icache --svm_small_mcache" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: svm.base.nomd.run
svm.base.nomd.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_no_mcache" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

# SVF sampling modes
.PHONY: svm.sampling.run
svm.sampling.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q $(SAMPLING_FLAGS)" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: svm.sampling.icache.run
svm.sampling.icache.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_icache $(SAMPLING_FLAGS)" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

# Dynamic protection with 10% threshold
.PHONY: svm.sampling.16k.64.10.run
svm.sampling.16k.64.10.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q $(ADAPTIVE_10_FLAGS)" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: svm.sampling.icache.16k.64.10.run
svm.sampling.icache.16k.64.10.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_icache $(ADAPTIVE_10_FLAGS)" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

# Dynamic protection with 5% threshold
.PHONY: svm.sampling.16k.64.5.run
svm.sampling.16k.64.5.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q $(ADAPTIVE_5_FLAGS)" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: svm.sampling.icache.16k.64.5.run
svm.sampling.icache.16k.64.5.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_icache $(ADAPTIVE_5_FLAGS)" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

# Dynamic protection based on AVF-CL
.PHONY: svm.dynamic.run
svm.dynamic.run:
	make $(MAKE_FLAGS) RUN_FLAGS="-q --svm_no_mcache $(DYNAMIC_PROTECTION_FLAGS)" TOOL_FLAGS="$(SVM_ONLY_FLAGS)" TOOL=svm $(APP).run
	$(SEND_EMAIL_NOTIFICATION)

.PHONY: runall.ppa
runall.ppa:
	## specify DATA ##
	# ppa default
	make runall RUN=ppa.run DATA=$(DATA) CFG=coretime
	# ppa ecc
	make runall RUN=ppa.ecc.run DATA=$(DATA) CFG=coretime
	# ppa svf
	make runall RUN=ppa.svf.run DATA=$(DATA) CFG=svm
	make runall RUN=ppa.svf.run DATA=$(DATA) CFG=svm.icache PPA_FLAGS="--ppa_use_icache"
	make runall RUN=ppa.svf.run DATA=$(DATA) CFG=svm.small_mcache PPA_FLAGS="--ppa_use_smallmd"
	make runall RUN=ppa.svf.run DATA=$(DATA) CFG=svm.small_mcache.icache PPA_FLAGS="--ppa_use_smallmd --ppa_use_icache"
	make runall RUN=ppa.svf.run DATA=$(DATA) CFG=svm.no_mcache PPA_FLAGS="--ppa_use_nomd"
	# ppa sampling
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '1048576/16'" CFG=svm.sampling_svf.1048576.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '1048576/2'" CFG=svm.sampling_svf.1048576.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '1048576/32'" CFG=svm.sampling_svf.1048576.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '1048576/4'" CFG=svm.sampling_svf.1048576.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '1048576/8'" CFG=svm.sampling_svf.1048576.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/16'" CFG=svm.sampling_svf.16384.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/2'" CFG=svm.sampling_svf.16384.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/32'" CFG=svm.sampling_svf.16384.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/4'" CFG=svm.sampling_svf.16384.4
	make runall RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/64'" CFG=svm.sampling_svf.16384.64
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/8'" CFG=svm.sampling_svf.16384.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '262144/16'" CFG=svm.sampling_svf.262144.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '262144/2'" CFG=svm.sampling_svf.262144.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '262144/32'" CFG=svm.sampling_svf.262144.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '262144/4'" CFG=svm.sampling_svf.262144.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '262144/64'" CFG=svm.sampling_svf.262144.64
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '262144/8'" CFG=svm.sampling_svf.262144.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '4194304/2'" CFG=svm.sampling_svf.4194304.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '4194304/4'" CFG=svm.sampling_svf.4194304.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '4194304/8'" CFG=svm.sampling_svf.4194304.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '65536/16'" CFG=svm.sampling_svf.65536.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '65536/2'" CFG=svm.sampling_svf.65536.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '65536/32'" CFG=svm.sampling_svf.65536.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '65536/4'" CFG=svm.sampling_svf.65536.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '65536/64'" CFG=svm.sampling_svf.65536.64
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '65536/8'" CFG=svm.sampling_svf.65536.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '1048576/16'" CFG=svm.icache.sampling_svf.1048576.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '1048576/2'" CFG=svm.icache.sampling_svf.1048576.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '1048576/32'" CFG=svm.icache.sampling_svf.1048576.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '1048576/4'" CFG=svm.icache.sampling_svf.1048576.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '1048576/8'" CFG=svm.icache.sampling_svf.1048576.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/16'" CFG=svm.icache.sampling_svf.16384.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/2'" CFG=svm.icache.sampling_svf.16384.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/32'" CFG=svm.icache.sampling_svf.16384.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/4'" CFG=svm.icache.sampling_svf.16384.4
	make runall RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/64'" CFG=svm.icache.sampling_svf.16384.64
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/8'" CFG=svm.icache.sampling_svf.16384.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '262144/16'" CFG=svm.icache.sampling_svf.262144.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '262144/2'" CFG=svm.icache.sampling_svf.262144.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '262144/32'" CFG=svm.icache.sampling_svf.262144.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '262144/4'" CFG=svm.icache.sampling_svf.262144.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '262144/64'" CFG=svm.icache.sampling_svf.262144.64
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '262144/8'" CFG=svm.icache.sampling_svf.262144.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '4194304/2'" CFG=svm.icache.sampling_svf.4194304.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '4194304/4'" CFG=svm.icache.sampling_svf.4194304.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '4194304/8'" CFG=svm.icache.sampling_svf.4194304.8
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '65536/16'" CFG=svm.icache.sampling_svf.65536.16
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '65536/2'" CFG=svm.icache.sampling_svf.65536.2
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '65536/32'" CFG=svm.icache.sampling_svf.65536.32
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '65536/4'" CFG=svm.icache.sampling_svf.65536.4
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '65536/64'" CFG=svm.icache.sampling_svf.65536.64
	make runpartial RUN=ppa.sampling.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '65536/8'" CFG=svm.icache.sampling_svf.65536.8
	# ppa adaptive
	make runall RUN=ppa.adaptive.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/64'" CFG=svm.adaptive_svf.16384.64.0.05
	make runall RUN=ppa.adaptive.run DATA=$(DATA) PPA_FLAGS="--ppa_sampling_cfg '16384/64'" CFG=svm.adaptive_svf.16384.64.0.1
	make runall RUN=ppa.adaptive.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/64'" CFG=svm.icache.adaptive_svf.16384.64.0.05
	make runall RUN=ppa.adaptive.run DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/64'" CFG=svm.icache.adaptive_svf.16384.64.0.1
	# ppa montecarlo
	make ppa.svf.run APP=222.montecarlo DATA=$(DATA) CFG=svm.montecarlo
	make ppa.svf.run APP=222.montecarlo DATA=$(DATA) CFG=svm.montecarlo.nochange
	make ppa.sampling.run APP=222.montecarlo DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/64'" CFG=svm.icache.sampling_svf.16384.64.montecarlo
	make ppa.sampling.run APP=222.montecarlo DATA=$(DATA) PPA_FLAGS="--ppa_use_icache --ppa_sampling_cfg '16384/64'" CFG=svm.icache.sampling_svf.16384.64.montecarlo.nochange

.PHONY: ppa.run
ppa.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.avfcl.run
ppa.avfcl.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode avfcl $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.parity.run
ppa.parity.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode parity $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.parity_plus.run
ppa.parity_plus.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode parity_plus $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.ecc.run
ppa.ecc.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode ecc $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.svf.run
ppa.svf.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode svf $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.sampling.run
ppa.sampling.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode sampling $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.adaptive.run
ppa.adaptive.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode adaptive $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.dynamic.run
ppa.dynamic.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dynamic $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: ppa.dynamicecc.run
ppa.dynamicecc.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dynamic_ecc $(PPA_FLAGS)" TOOL=ppa $(APP).run

# modular redundancy at structure and core level
.PHONY: ppa.mr.run
ppa.mr.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dmr_st $(PPA_FLAGS)" TOOL=ppa $(APP).run
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode tmr_st $(PPA_FLAGS)" TOOL=ppa $(APP).run
	# make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dmr_core $(PPA_FLAGS)" TOOL=ppa $(APP).run
	# make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode tmr_core $(PPA_FLAGS)" TOOL=ppa $(APP).run

# dynamic modular redundancy at structure and core level
.PHONY: ppa.mr.run
ppa.dynamicmr.run:
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dynamic_dmr_st $(PPA_FLAGS)" TOOL=ppa $(APP).run
	make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dynamic_tmr_st $(PPA_FLAGS)" TOOL=ppa $(APP).run
	# make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dynamic_dmr_core $(PPA_FLAGS)" TOOL=ppa $(APP).run
	# make $(MAKE_FLAGS) RUN_FLAGS="--ppa_data $(DATA) --ppa_config $(CFG) --ppa_mode dynamic_tmr_core $(PPA_FLAGS)" TOOL=ppa $(APP).run

.PHONY: runall.ppa.static
runall.ppa.static:
	make runall RUN=ppa.run DATA=$(DATA) CFG=coretime
	make runall RUN=ppa.avfcl.run DATA=$(DATA) CFG=svm
	make runall RUN=ppa.parity.run DATA=$(DATA) CFG=coretime
	make runall RUN=ppa.parity_plus.run DATA=$(DATA) CFG=coretime
	make runall RUN=ppa.ecc.run DATA=$(DATA) CFG=coretime
	make runall RUN=ppa.mr.run DATA=$(DATA) CFG=coretime

.PHONY: runall.ppa.dynamic
runall.ppa.dynamic:
	make runall RUN=ppa.dynamic.run DATA=$(DATA) CFG=svm.dynamic.0.1
	make runall RUN=ppa.dynamic.run DATA=$(DATA) CFG=svm.dynamic.0.05
	make runall RUN=ppa.dynamic.run DATA=$(DATA) CFG=svm.dynamic.0.01
	make runall RUN=ppa.dynamicecc.run DATA=$(DATA) CFG=svm.dynamic.0.1
	make runall RUN=ppa.dynamicecc.run DATA=$(DATA) CFG=svm.dynamic.0.05
	make runall RUN=ppa.dynamicecc.run DATA=$(DATA) CFG=svm.dynamic.0.01
	make runall RUN=ppa.dynamicmr.run DATA=$(DATA) CFG=svm.dynamic.0.1
	make runall RUN=ppa.dynamicmr.run DATA=$(DATA) CFG=svm.dynamic.0.05
	make runall RUN=ppa.dynamicmr.run DATA=$(DATA) CFG=svm.dynamic.0.01

