BENCHES = 401.bzip2 403.gcc 429.mcf 445.gobmk 456.hmmer 458.sjeng 462.libquantum 464.h264ref 471.omnetpp 473.astar 483.xalancbmk 222.montecarlo

BBV_50M = $(BENCHES:%=%.50M.bb.gz)
BBV_100M = $(BENCHES:%=%.100M.bb.gz)
SIMPOINTS_50M = $(BENCHES:%=%.50M.simpoints)
SIMPOINTS_100M = $(BENCHES:%=%.100M.simpoints)

BBV_COMBINER_PY = ../../../project-svm/tools/bbv_combiner.py
SIMPOINT_BIN = ../../../project-fabscalar/tools/SimPoint.3.2/bin/simpoint

.PHONY: all
all: 401.bzip2.50M.bb.gz 401.bzip2.100M.bb.gz 429.mcf.50M.bb.gz 429.mcf.100M.bb.gz 445.gobmk.50M.bb.gz 445.gobmk.100M.bb.gz 456.hmmer.50M.bb.gz 456.hmmer.100M.bb.gz 458.sjeng.50M.bb.gz 458.sjeng.100M.bb.gz 462.libquantum.50M.bb.gz 462.libquantum.100M.bb.gz 464.h264ref.50M.bb.gz 464.h264ref.100M.bb.gz 471.omnetpp.50M.bb.gz 471.omnetpp.100M.bb.gz 483.xalancbmk.50M.bb.gz 483.xalancbmk.100M.bb.gz 403.gcc.50M.bb.gz 403.gcc.100M.bb.gz 473.astar.50M.bb.gz 473.astar.100M.bb.gz 222.montecarlo.50M.bb.gz 222.montecarlo.100M.bb.gz

%.50M.bb.gz: %.10M.bb.gz
	python $(BBV_COMBINER_PY) $< $@ 5

%.100M.bb.gz: %.10M.bb.gz
	python $(BBV_COMBINER_PY) $< $@ 10

MAX_K = 6
COVERAGE = 0.9

%.simpoints: %.bb.gz
	$(SIMPOINT_BIN) -inputVectorsGzipped -loadFVFile $< -maxK $(MAX_K) -saveSimpoints $@ -saveSimpointWeights $@.weights -coveragePct $(COVERAGE)

.PHONY: combine
combine: $(BBV_50M) $(BBV_100M)

.PHONY: simpoints
simpoints: $(SIMPOINTS_50M) $(SIMPOINTS_100M)

