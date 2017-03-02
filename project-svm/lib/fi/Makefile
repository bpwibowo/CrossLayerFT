################################################################################
#
# Fault injection model Makefile
#
# First author: Bagus Wibowo
# created: 8/2/2014
################################################################################

PREFIX       = .
BRUTUS_DIR   = ../../exp/fabscalar/brutus
MIPSARCH_DIR = $(BRUTUS_DIR)/mipsarch
UTIL_DIR     = $(BRUTUS_DIR)/util
DPISIM_DIR   = ../../exp/fabscalar/rtl/dpisim
BUILD_DIR    = $(PREFIX)/fi_build
INSTALL_DIR  = $(PREFIX)/lib
TARGET_LIB   = $(INSTALL_DIR)/libfi.so

CXXFLAGS    += -O3 -m64 -Wall -fPIC -I. -I$(BRUTUS_DIR) -I$(MIPSARCH_DIR) \
	-I$(UTIL_DIR) -DFPU64 -D__STDC_FORMAT_MACROS $(FLAGS)

FILES_CC = fi_dpi.cc
FILES_OBJ = $(FILES_CC:%.cc=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(TARGET_LIB) fi_tasks.sv

$(BUILD_DIR)/%.o: %.cc
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(TARGET_LIB): $(FILES_OBJ)
	mkdir -p $(INSTALL_DIR)
	$(CXX) -shared $(CXXFLAGS) -o $@ $^

fi_tasks.sv: signal_list.txt figen.py
	python figen.py

clean:
	rm -f $(TARGET_LIB) $(FILES_OBJ) fi_tasks.sv *.pyc
