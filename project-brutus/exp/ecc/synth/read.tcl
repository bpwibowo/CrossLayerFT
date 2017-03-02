#------------------------------------------------------------
#
# Basic Synthesis Script (TCL format)
#                                  
# Revision History                
#   1/15/03  : Author Shane T. Gehring - from class example
#   2/09/07  : Author Zhengtao Yu      - from class example
#   12/14/07 : Author Ravi Jenkal      - updated to 180 nm & tcl
#
#------------------------------------------------------------

#---------------------------------------------------------
# Read in Verilog file and map (synthesize) onto a generic
# library.
# MAKE SURE THAT YOU CORRECT ALL WARNINGS THAT APPEAR
# during the execution of the read command are fixed 
# or understood to have no impact.
# ALSO CHECK your latch/flip-flop list for unintended 
# latches                                            
#---------------------------------------------------------

read_verilog $RTL_DIR/mem_with_ecc_3.sv -rtl
read_verilog $RTL_DIR/mem_with_ecc_4.sv -rtl
read_verilog $RTL_DIR/mem_with_ecc_5.sv -rtl
read_verilog $RTL_DIR/mem_with_ecc_6.sv -rtl
read_verilog $RTL_DIR/mem_with_ecc_7.sv -rtl
read_verilog $RTL_DIR/mem_with_ecc_8.sv -rtl
read_verilog $RTL_DIR/mem_with_ecc_9.sv -rtl
read_verilog $RTL_DIR/mem_with_ecc_10.sv -rtl
read_verilog $RTL_DIR/parity.sv -rtl

