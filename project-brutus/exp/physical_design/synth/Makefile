MODPREFIX = pmems/
MODNAME = SRAM_top
RTL_DIR = ../cores/superset
GATE_DIR = ../gate
PR_DIR = ../../pr


$(GATE_DIR)/$(MODNAME)_final.v: $(RTL_DIR)/$(MODPREFIX)$(MODNAME).sv
	tcsh -c "add synopsys2015 ; \
	dc_shell-t -64bit -f run_synth.tcl |& tee run_synth.log"

.PHONY: pt ptsi ptpx

pt:
	tcsh -c "add synopsys2015 ; \
	pt_shell -f run_pt.tcl |& tee run_pt.log"

ptsi:
	tcsh -c "add synopsys2015 ; \
	pt_shell -f run_ptsi.tcl |& tee run_ptsi.log"

ptpx:
	tcsh -c "add synopsys2015 ; \
	pt_shell -f run_ptpx.tcl |& tee run_ptpx.log"


clean:
	-rm default.svf
	-rm fixed_names_init
	-rm $(MODNAME)_init.v
	-rm $(GATE_DIR)/$(MODNAME)_final.v
	-rm *.log
	-rm *.sdf
	-rm *.rpt
	-rm *.mr
	-rm *.pvl
	-rm *.syn

