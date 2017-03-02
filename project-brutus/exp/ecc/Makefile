.PHONY: sim
sim:
	vlib work
	make compile
	vsim -c -novopt -do "run -all" ecc_test

.PHONY: gui
gui:
	vlib work
	make compile
	vsim -novopt -do "do wave.do; run -all" ecc_test

.PHONY: compile
compile:
	vlog *.v

.PHONY: clean
clean:
	rm -rf work/ transcript vsim.wlf

