################################################################################
#
# Fault tolerance analysis library Makefile
#
# First author: Bagus Wibowo
# created: 7/22/2014
################################################################################

PREFIX       = .
FABSCLR_DIR  = ../../../project-fabscalar/exp/fabscalar
BRUTUS_DIR   = $(FABSCLR_DIR)/brutus
MIPSARCH_DIR = $(BRUTUS_DIR)/mipsarch
UTIL_DIR     = $(BRUTUS_DIR)/util
DPISIM_DIR   = $(FABSCLR_DIR)/rtl/dpisim
BUILD_DIR    = $(PREFIX)/ft_build
INSTALL_DIR  = $(PREFIX)/lib
TARGET_LIB   = $(INSTALL_DIR)/libft.so
STATIC_LIB   = $(INSTALL_DIR)/libFT.a
INCLUDE_DPI  = yes

# path to fabscalar configuration file
FSCONFIG     =
CONFIG_H     = $(notdir $(FSCONFIG)).h
# path to tools folder
TOOLS_DIR    =

CXXFLAGS    += -O3 -m64 -Wall -fPIC -I. -I$(BRUTUS_DIR) -I$(MIPSARCH_DIR) \
	-I$(UTIL_DIR) -DFPU64 -D__STDC_FORMAT_MACROS $(FLAGS)

ifneq ($(FSCONFIG),)
CXXFLAGS    += -DSIM_CONFIG_INC='"$(CONFIG_H)"'
endif

FILES_CC = \
   svm.cc \
   simlog.cc \
   simlog_nonspec.cc \
   ft_analyzer.cc \
   ft_analyzer_list.cc \
   sim_logger.cc \
   sim_listener_list.cc \
   avf_analyzer.cc \
   pvf_analyzer.cc \
   hvf_analyzer.cc \
   occupancy_analyzer.cc \
   svf_analyzer.cc \
   spec_analyzer.cc \
   mreader.cc \
   simsyscall.cc \
   vbit_table.cc \
   vbit_analyzer.cc \
   func_stats.cc \
   pvs_profiler.cc

ifeq ($(INCLUDE_DPI),yes)
FILES_CC += simlog_dpi.cc
endif

FILES_OBJ = $(FILES_CC:%.cc=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(TARGET_LIB) $(STATIC_LIB)

$(BUILD_DIR)/%.o: %.cc $(CONFIG_H)
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(TARGET_LIB): $(FILES_OBJ)
	mkdir -p $(INSTALL_DIR)
	echo $(FSCONFIG)
	$(CXX) -shared $(CXXFLAGS) -o $@ $^

$(STATIC_LIB): $(FILES_OBJ)
	mkdir -p $(INSTALL_DIR)
	ar r $@ $^

$(CONFIG_H): $(FSCONFIG)
ifneq ($(FSCONFIG),)
	python $(TOOLS_DIR)/v2h.py < $< > $@
endif

clean:
	rm -f $(TARGET_LIB) $(FILES_OBJ) $(CONFIG_H)
