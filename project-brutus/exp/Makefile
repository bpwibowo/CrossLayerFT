MIPS64_BIN_PATH = bench/bin
FT_MIPS_SCRIPT_PATH = ../../project-fabscalar/scripts/ft_mips
BINARIES = $(wildcard $(MIPS64_BIN_PATH)/*.mips64)
DISASSEMBLIES = $(addsuffix .mips64.dis, $(basename $(BINARIES)))
METADATAS = $(addsuffix .mips64.metadata, $(basename $(BINARIES)))

CHECK_XTOOLS = @if [ "$(XTOOLSPREFIX)" == "" ]; then echo "Error: A required XTOOLSPREFIX variable is undefined."; exit 1; fi;

.PHONY: all
all: metadata

%.mips64.dis: %.mips64
	$(CHECK_XTOOLS)
	$(XTOOLSPREFIX)/bin/mips-linux-gnu-objdump -d $< > $@

%.mips64.metadata: %.mips64.dis
	python $(FT_MIPS_SCRIPT_PATH)/parse_disassembly.py -f $< -o $@ -u >& /dev/null

CHECK_INTERMEDIATE_PATH = @if [ "$(INTERMEDIATE_PATH)" == "" ]; then echo "Error: A required INTERMEDIATE_PATH variable is undefined."; exit 1; fi;

.PHONY: load_intermediate
load_intermediate:
	$(CHECK_INTERMEDIATE_PATH)
	@for d in $(INTERMEDIATE_PATH)/*/ ; do \
		mkdir -p ./intermediate/$$(basename "$$d"); \
		echo Processing $$d/intermediate.tar.xz ...; \
		tar xfJ $$d/intermediate.tar.xz -C ./intermediate/$$(basename "$$d"); \
	done

.PHONY: disassembly metadata
disassembly: $(DISASSEMBLIES)
metadata:  $(METADATAS)

clean:
	rm -rf $(DISASSEMBLIES)
	rm -rf $(METADATAS)
