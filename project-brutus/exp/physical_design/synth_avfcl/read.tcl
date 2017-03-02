##################################################
# Revision History: 03/02/2016, by Bagus Wibowo
##################################################

set INCLUDES "\
	$RTL_DIR/configs/config-1.v \
	$RTL_DIR/include/structs.svh \
	$RTL_DIR/ISA/mips64r2_encodings.svh \
	$RTL_DIR/ISA/mips64r2_ISA.v \
	"

set FETCH "\
	$RTL_DIR/fetch/BranchPrediction_2-bit.sv \
	$RTL_DIR/fetch/BTB.sv \
	$RTL_DIR/fetch/CtrlQueue.sv \
	$RTL_DIR/fetch/Fetch1Fetch2.sv \
	$RTL_DIR/fetch/Fetch2Decode.sv \
	$RTL_DIR/fetch/FetchStage1.sv \
	$RTL_DIR/fetch/FetchStage2.sv \
	$RTL_DIR/fetch/L1ICache.sv \
	$RTL_DIR/fetch/RAS.sv \
	$RTL_DIR/fetch/SelectInst.sv \
	"

set DECODE "\
	$RTL_DIR/decode/Decode_MIPS.sv \
	$RTL_DIR/decode/Decode.sv \
	$RTL_DIR/decode/InstructionBuffer.sv \
	$RTL_DIR/decode/PreDecode_MIPS.sv \
	"

set RENAME "\
	$RTL_DIR/rename/RenameDispatch.sv \
	$RTL_DIR/rename/RenameMapTable.sv \
	$RTL_DIR/rename/Rename.sv \
	$RTL_DIR/rename/Rename_TOP.sv \
	$RTL_DIR/rename/SpecFreeList.sv \
	"

set DISPATCH "\
	$RTL_DIR/dispatch/Dispatch.sv \
	$RTL_DIR/dispatch/ExePipeScheduler.sv \
	$RTL_DIR/dispatch/ldViolationPred.sv \
	"

set ISSUEQ "\
	$RTL_DIR/issue/Encoder.sv \
	$RTL_DIR/issue/IssueQFreeList_FP.sv \
	$RTL_DIR/issue/IssueQRegRead.sv \
	$RTL_DIR/issue/IssueQueue_INT.sv \
	$RTL_DIR/issue/RSR_FP.sv \
	$RTL_DIR/issue/SelectBlock.sv \
	$RTL_DIR/issue/Select.sv \
	$RTL_DIR/issue/FreeIssueq.sv \
	$RTL_DIR/issue/IssueQFreeList_INT.sv \
	$RTL_DIR/issue/IssueQueue_FP.sv \
	$RTL_DIR/issue/PriorityEncoder.sv \
	$RTL_DIR/issue/RSR_INT.sv \
	$RTL_DIR/issue/SelectFromBlock.sv \
	"

set REGREAD "\
	$RTL_DIR/regRead/ByPass1D_FP.sv \
	$RTL_DIR/regRead/ByPass1D.sv \
	$RTL_DIR/regRead/PhyRegFile_FP.sv \
	$RTL_DIR/regRead/PhyRegFile_INT.sv \
	$RTL_DIR/regRead/RegRead1D_FP.sv \
	$RTL_DIR/regRead/RegRead1D_LSU.sv \
	$RTL_DIR/regRead/RegRead1D.sv \
	$RTL_DIR/regRead/RegReadExecute_FP.sv \
	$RTL_DIR/regRead/RegReadExecute_LSU.sv \
	$RTL_DIR/regRead/RegReadExecute.sv \
	"

set EXECUTE "\
	$RTL_DIR/execute/AgenLsu.sv \
	$RTL_DIR/execute/Ctrl_ALU.sv \
	$RTL_DIR/execute/Execute_FP.sv \
	$RTL_DIR/execute/ExecutionPipe_Branch_1D.sv \
	$RTL_DIR/execute/ExecutionPipe_SC_1D.sv \
	$RTL_DIR/execute/FP_ALU.sv \
	$RTL_DIR/execute/fu4.sv \
	$RTL_DIR/execute/AGEN.sv \
	$RTL_DIR/execute/Demux.sv \
	$RTL_DIR/execute/Execute_loadstore.sv \
	$RTL_DIR/execute/ExecutionPipe_FP_1D.sv \
	$RTL_DIR/execute/ForwardCheck_FP.sv \
	$RTL_DIR/execute/fu2.sv \
	$RTL_DIR/execute/Mux.sv \
	$RTL_DIR/execute/Complex_ALU.sv \
	$RTL_DIR/execute/Execute_branch.sv \
	$RTL_DIR/execute/Execute_SC.sv \
	$RTL_DIR/execute/ExecutionPipe_LoadStore_1D.sv \
	$RTL_DIR/execute/ForwardCheck.sv \
	$RTL_DIR/execute/fu3.sv \
	$RTL_DIR/execute/Simple_ALU.sv \
	"

set WRITEBK "\
	$RTL_DIR/writeback/WriteBack_Branchpipe_1D.sv \
	$RTL_DIR/writeback/WriteBack_FloatingPointpipe_1D.sv \
	$RTL_DIR/writeback/WriteBack_LoadStorepipe_1D.sv \
	$RTL_DIR/writeback/WriteBack_SC_1D.sv \
	"

set LSQ "\
	$RTL_DIR/memory/CommitLoad.sv \
	$RTL_DIR/memory/CommitStore.sv \
	$RTL_DIR/memory/DispatchedLoad.sv \
	$RTL_DIR/memory/DispatchedStore.sv \
	$RTL_DIR/memory/L1DataCache.sv \
	$RTL_DIR/memory/LDX_path.sv \
	$RTL_DIR/memory/LoadStoreUnit.sv \
	$RTL_DIR/memory/LSUControl.sv \
	$RTL_DIR/memory/LSUDatapath.sv \
	$RTL_DIR/memory/STX_path.sv \
	"

set RETIRE "\
	$RTL_DIR/retire/ActiveList.sv \
	$RTL_DIR/retire/ArchMapTable.sv \
	"

set PMEM "\
	$RTL_DIR/pmems/ALCTRL_RAM.sv \
	$RTL_DIR/pmems/ALREADY_RAM.sv \
	$RTL_DIR/pmems/IBUFF_RAM.sv \
	$RTL_DIR/pmems/SRAM_1R1W_BP.sv \
	$RTL_DIR/pmems/SRAM_4R4W_AMT.sv \
	$RTL_DIR/pmems/SRAM_PRF_FP.sv \
	$RTL_DIR/pmems/SRAM_RMT.sv \
	$RTL_DIR/pmems/ALDATA_RAM.sv \
	$RTL_DIR/pmems/ALVIO_RAM.sv \
	$RTL_DIR/pmems/IQFREELIST_RAM.sv \
	$RTL_DIR/pmems/SRAM_1R1W.sv \
	$RTL_DIR/pmems/SRAM_CTI_COMMIT.sv \
	$RTL_DIR/pmems/SRAM_PRF_INT.sv \
	$RTL_DIR/pmems/SRAM.sv \
	$RTL_DIR/pmems/ALNPC_RAM.sv \
	$RTL_DIR/pmems/FREELIST_RAM.sv \
	$RTL_DIR/pmems/IQPAYLOAD_RAM.sv \
	$RTL_DIR/pmems/SRAM_4R1W.sv \
	$RTL_DIR/pmems/SRAM_CTI_COUNTER.sv \
	$RTL_DIR/pmems/SRAM_PRF.sv \
	$RTL_DIR/pmems/WAKEUP_CAM.sv \
	"

set FAB "\
	$RTL_DIR/fabscalar/FABSCALAR.sv \
	"

set PMEM_TOP "\
	$RTL_DIR/pmems/SRAM.sv \
	$RTL_DIR/pmems/SRAM_top.sv \
	"

set AVFCL_TOP "\
	$RTL_DIR/avfcl/avfcl_top.sv \
	$RTL_DIR/avfcl/mdecode.sv \
	$RTL_DIR/avfcl/metadata_decode.sv \
	$RTL_DIR/avfcl/nop_lane.sv \
	$RTL_DIR/avfcl/vop_decoder.sv \
	$RTL_DIR/avfcl/accumulator.sv \
	$RTL_DIR/avfcl/counter.sv \
	$RTL_DIR/avfcl/ib_dur.sv \
	$RTL_DIR/avfcl/iq_dur.sv \
	$RTL_DIR/avfcl/lq_dur.sv \
	$RTL_DIR/avfcl/rob_dur.sv \
	$RTL_DIR/avfcl/sq_dur.sv \
	$RTL_DIR/avfcl/vlt.sv \
	$RTL_DIR/avfcl/parity.sv \
	"

read_sverilog "$AVFCL_TOP"
