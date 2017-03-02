TBENCHSRC	= pmem_tb.sv
STDCELLSRC	= /afs/eos.ncsu.edu/lockers/research/ece/wdavis/tech/nangate/NangateOpenCellLibrary_PDKv1_2_v2008_10/verilog/NangateOpenCellLibrary_PDKv1_2_v2008_10_typical_conditional.v
SRCDIR		= ../src/rtl
SRCDIR_GATE	= ../src/gate
VERILOGSRC	= $(wildcard $(SRCDIR)/*.sv)
GATESRC     = $(wildcard $(SRCDIR_GATE)/*.v)
VERILOGTOP	= pmem_tb

# Mentor Graphics QuestaSim/ModelSim
sim:	$(VERILOGSRC) $(TBENCHSRC)
	vlib work
	vlog $(STDCELLSRC) $(VERILOGSRC)
	vlog -sv $(TBENCHSRC)
	vopt $(VERILOGTOP) -o $(VERILOGTOP)_opt
	time vsim $(VERILOGTOP)_opt -c -do "run -all"

sim_gate:	$(GATESRC) $(TBENCHSRC)
	vlib work
	vlog -sv $(TBENCHSRC)
	vlog $(STDCELLSRC) $(GATESRC)
	vopt $(VERILOGTOP) -o $(VERILOGTOP)_opt
	time vsim $(VERILOGTOP)_opt -c -do "run -all"

gui:	$(VERILOGSRC) $(TBENCHSRC)
	vlib work
	vlog $(VERILOGSRC)
	vlog -sv $(TBENCHSRC)
	vsim -novopt $(VERILOGTOP)

gui_gate:	$(GATESRC) $(TBENCHSRC)
	vlib work
	vlog $(GATESRC)
	vlog -sv $(TBENCHSRC)
	vsim -novopt $(VERILOGTOP)

clean:
	-rm -rf work
	-rm -f transcript
	-rm -f vsim.wlf
	-rm -f waves.vcd
