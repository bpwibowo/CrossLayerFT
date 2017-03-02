###################################################
#
# run_ptpx.tcl
#
# 4/26/2011 W. Rhett Davis (rhett_davis@ncsu.edu)
# updated 4/5/2012, 3/28/2014, 1/26/2015
#
#####################################################

set begintime [clock seconds]


# setup name of the clock in your design.
set clkname clk

# set variable "modname" to the name of topmost module in design
#set modname SRAM_top
set modname AvfCl_top

# set STRIP_PATH to the path of the instance to be analyzed in the VCD file
#set STRIP_PATH "pmem_tb/u_pmem"
set STRIP_PATH "pmem_tb/u_avfcl"

# set variable "GATE_DIR" to the output directory w.r.t synthesis directory
set GATE_DIR    ../src/gate/

# set variable "PR_DIR" to the HDL & SPEF directory w.r.t synthesis directory
set PR_DIR    ../../pr/

# set variable "type" to routed, trialrouted, or unrouted
set type unrouted

# set PERIOD to the period for power analysis, or -1 for the entire simulation
set PERIOD -1

# set MAX_SIM_TIME to the maximum VCD time, if PERIOD is not -1
set MAX_SIM_TIME 95295

array set START_TIME {0 1000 1 11000 2 12000 3 22000 4 23000 5 1000 6 33000}
array set END_TIME {0 11000  1 12000 2 22000 3 23000 4 33000 5 33000 6 34000}

# array set START_TIME {0 153000 1 51000 2 51500}
# array set END_TIME {0 153485 1 51500 2 52000}

# set variable "corner" to one of the following:
#   typical     (typical transistors, 1.1  V,  25 degC)
#   worst_low   (   slow transistors, 0.95 V, -40 degC)
#   low_temp    (   fast transistors, 1.25 V, -40 degC)
#   fast        (   fast transistors, 1.25 V,   0 degC)
#   slow        (   slow transistors, 0.95 V, 125 degC)
set corner fast

#set the number of digits to be used for delay results
set report_default_significant_digits 4

set CLK_PER 40
set DFF_CKQ 0.638
set MAX_INS_DELAY 1.0
set IP_DELAY [expr $MAX_INS_DELAY + $DFF_CKQ]
set DR_CELL_NAME DFFR_X1
set DR_CELL_PIN  Q

# Define a helpful function for printing out time strings
proc timef {sec} {
  set hrs [expr $sec/3600]
  set sec [expr $sec-($hrs*3600)]
  set min [expr $sec/60]
  set sec [expr $sec-($min*60)]
  return "${hrs}h ${min}m ${sec}s"
}

set link_library "NangateOpenCellLibrary_${corner}_conditional_ccs.db *"

if { $type eq "unrouted" } then {
  read_verilog "${GATE_DIR}${modname}_final.v"
} else {
  read_verilog "${PR_DIR}${modname}_${type}.v"
}
current_design $modname

link_design

create_clock -name $clkname -period $CLK_PER $clkname
set_input_delay $IP_DELAY -clock $clkname [remove_from_collection [all_inputs] $clkname]
set_output_delay -clock $clkname 0 [all_outputs]
set_driving_cell -lib_cell "$DR_CELL_NAME" -pin "$DR_CELL_PIN" [remove_from_collection [all_inputs] $clkname]


set power_enable_analysis TRUE
set power_analysis_mode averaged

if { $type ne "unrouted" } then {
  read_parasitics -format spef "${PR_DIR}${modname}_${type}.spef"
}

check_timing
update_timing
report_timing

#####################################################################
#       read switching activity file
#####################################################################
#read_saif "../sim/waves.saif" -strip_path $STRIP_PATH

#if { $PERIOD == -1 } then {
#  read_vcd "../sim/waves.vcd" -strip_path $STRIP_PATH
#}  else {
#    read_vcd "../sim/waves.vcd" -strip_path $STRIP_PATH -time " 0 $PERIOD"
#}

set start $START_TIME(0)
set end $END_TIME(0)

read_vcd "../sim/waves.vcd" -strip_path $STRIP_PATH -time " $start $end"

report_switching_activity -list_not_annotated

#####################################################################
#       check/update/report power
#####################################################################
check_power
update_power
report_power -hierarchy > power_ptpx_${corner}_${start}_${end}.rpt


if { $PERIOD != -1 } then {
    set i $PERIOD
    set j [ expr $i + $PERIOD ]
    while { $j < $MAX_SIM_TIME } {
	read_vcd "../sim/waves.vcd" -strip_path $STRIP_PATH -time "$i $j"
        update_power
        report_power -hierarchy >> power_ptpx_${corner}.rpt
        set i $j
        set j [ expr $i + $PERIOD ]
    }
}

foreach idx [array names START_TIME] {
  if {$idx == 0} {continue}
   set start $START_TIME($idx)
   set end $END_TIME($idx)
  read_vcd "../sim/waves.vcd" -strip_path $STRIP_PATH -time "$start $end"
        update_power
        report_power -hierarchy > power_ptpx_${corner}_${start}_${end}.rpt
} 

exit
