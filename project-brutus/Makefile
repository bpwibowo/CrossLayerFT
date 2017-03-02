BUILD_DIR = ./build
BIN_DIR = $(BUILD_DIR)/bin
LIB_DIR = $(BUILD_DIR)/lib

.PHONY: brutus
brutus: build prepare_script_dir

.PHONY: svm
svm: build_svm prepare_script_dir

.PHONY: setup
setup:
	git clone https://`whoami`@github.ncsu.edu/bpwibowo/FabScalarFT-mips64 exp/fabscalar -b brutus
	rsync -avz login.hpc.ncsu.edu:/tuck_data/bpwibowo/gudang/bench exp/.
	mkdir -p exp/intermediate
	rsync -avz login.hpc.ncsu.edu:/tuck_data/bpwibowo/gudang/checkpoints.50M_100M.tgz exp/intermediate/.
	rsync -avz login.hpc.ncsu.edu:/tuck_data/bpwibowo/gudang/profiles exp/
	tar xvfz exp/intermediate/checkpoints.50M_100M.tgz -C exp/intermediate/
	make -C exp/bench
	make clean svm

# build brutus
.PHONY: build
build:
	mkdir -p $(BIN_DIR) $(LIB_DIR)
	make -C exp/fabscalar/brutus BINDIR=`pwd`/$(BIN_DIR) LIBDIR=`pwd`/$(LIB_DIR)

# build brutus with SVM support
.PHONY: build_svm
build_svm:
	mkdir -p $(BIN_DIR) $(LIB_DIR)
	make -C ../project-svm/lib/ft PREFIX=`pwd`/$(BUILD_DIR) INCLUDE_DPI=no FABSCLR_DIR=`pwd`/exp/fabscalar
	make -C exp/fabscalar/brutus BINDIR=`pwd`/$(BIN_DIR) LIBDIR=`pwd`/$(LIB_DIR) \
		CFLAGS="-I`pwd`/../project-svm -DENABLE_SVM" \
		LDFLAGS="-lFT"

.PHONY: prepare_script_dir
prepare_script_dir:
	./scripts/run.py --tool prep all

.PHONY: clean
clean:
	make -C exp/fabscalar/brutus clean
	rm -rf build
	rm -rf exp/jobs
