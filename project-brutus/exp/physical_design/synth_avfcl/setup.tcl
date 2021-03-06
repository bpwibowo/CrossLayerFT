##################################################
# Revision History: 03/02/2016, by Bagus Wibowo
##################################################

# setup name of the clock in your design.
set clkname clk

# set variable "modname" to the name of topmost module in design
set modname AvfCl_top

# set variable "RTL_DIR" to the HDL directory w.r.t synthesis directory
set RTL_DIR    ../src/rtl

# set variable "GATE_DIR" to the output directory w.r.t synthesis directory
set GATE_DIR    ../src/gate

# set variable "type" to a name that distinguishes this synthesis run
set type fabscalar

#set the number of digits to be used for delay results
set report_default_significant_digits 4

set CLK_PER 40
