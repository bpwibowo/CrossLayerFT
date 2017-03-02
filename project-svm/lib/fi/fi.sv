/**********************************************************
 *
 * Fault injection model source code
 *
 * First author: Bagus Wibowo
 * created: 8/1/2014
 **********************************************************/

`ifndef BASE_OUTPUT_DIR
`define BASE_OUTPUT_DIR "."
`endif

module FaultInjection (input clk);

parameter RUN_ID = 0; // different run_id will use different random seed

import "DPI-C" function void fi_dpi_init(string base_output_dir, int run_id);
import "DPI-C" function int fi_dpi_clock(int cycle);
import "DPI-C" function int fi_dpi_get_injection_id();
import "DPI-C" function int fi_dpi_get_injection_entry();
import "DPI-C" function int fi_dpi_get_injection_offset();
import "DPI-C" function int fi_dpi_get_registered_bit_cnt();
import "DPI-C" function int fi_dpi_check_inst(longint PC, int tid, int logDest,
    longint destValue, int do_not_check, int isLoad, int isStore);
import "DPI-C" function int fi_dpi_check_store(longint addr, longint data,
    int size);
import "DPI-C" function void fi_dpi_handle_recovery();
import "DPI-C" function void fi_dpi_handle_braindead();
import "DPI-C" function void fi_dpi_handle_unexpected_syscall();

// allow recovery due to propagated fault
integer recovery_flag;
longint pc_to_continue;
integer fault_propagated_cnt;

initial begin
    fi_dpi_init(`BASE_OUTPUT_DIR, RUN_ID);
    register_signals();
    $display("Registered bit count: %1d bits", fi_dpi_get_registered_bit_cnt());

    recovery_flag = 0;
    pc_to_continue = 0;
end

always @(posedge clk) begin
    integer id, entry, offset;

    if (fi_dpi_clock(simulate.CYCLE_COUNT) != 0) begin
        id = fi_dpi_get_injection_id();
        entry = fi_dpi_get_injection_entry();
        offset = fi_dpi_get_injection_offset();

        inject_fault(id, entry, offset);
    end
end

always @(posedge clk)
begin: HANDLE_FAULT_RECOVERY
    int i;

    if (simulate.fabScalar.exceptionFlag & ~(|simulate.fabScalar.stqCount) &
      (recovery_flag>0)) begin
        $display("Handling fault recovery with next PC set to %1x",
            simulate.fabScalar.exceptionPC);

        // clear recovery_flag
        recovery_flag = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // Copy the new architectural register state and architectural memory state from the
        // functional simulator to the verilog simulator.
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        // Re-initialize integer register file.
`ifdef COPROC1
        for (i = 0; i < `SIZE_RMT-3; i = i + 1)
`else
        for (i = 0; i < `SIZE_RMT-2; i = i + 1)
`endif
        begin
            simulate.LOGICAL_REG[i] = get_reg(0,i);
        end

        simulate.LOGICAL_REG[`REG_LO] = get_lo(0);
        simulate.LOGICAL_REG[`REG_HI] = get_hi(0);
`ifdef COPROC1
        simulate.LOGICAL_REG[`REG_FCCR_INT]     = get_fccr(0);

        // Re-Initialize floating point register file.
        for (i = 0; i < `SIZE_FP_RMT-2; i = i + 1)
        begin
            simulate.LOGICAL_REG_FP[i]            = get_fp_reg(0,i);
        end

        simulate.LOGICAL_REG_FP[`REG_FP_TEMP_0]       = 0;
        simulate.LOGICAL_REG_FP[`REG_FCCR_FP][31:0]   = get_fccr(0);
        simulate.LOGICAL_REG_FP[`REG_FCCR_FP][63:32]  = 0;
`endif
        simulate.copyRF;
`ifdef COPROC1
        simulate.copyRF_FP;
`endif

        simulate.copy_mem;

        fi_dpi_handle_recovery();
    end
end

task handle_braindead;
    if (recovery_flag > 0) begin
        $display("Error: Previous failure was not handled yet.");
        simulate.onemorecycle = 1;
    end
    else begin
        recovery_flag = 1;
        pc_to_continue = get_arch_PC(0);
        fi_dpi_handle_braindead();
    end
endtask

task handle_unexpected_syscall;
    recovery_flag = 1;
    pc_to_continue = get_arch_PC(0);
    fi_dpi_handle_unexpected_syscall();
endtask;

task check_inst_begin;
    fault_propagated_cnt = 0;
endtask

task check_inst;
    input reg [`SIZE_PC-1:0]      PC;
    input integer                 zero;
    input reg [`SIZE_RMT_LOG-1:0] logDest;
    input reg [`SIZE_DATA-1:0]    result;
    input reg                     do_not_check;
    input reg                     isLoad;
    input reg                     isStore;
    begin
        fault_propagated_cnt += fi_dpi_check_inst(PC,zero,logDest,result,
            do_not_check, isLoad, isStore);
    end
endtask

task check_inst_end;
    // check store if the store is issued to memory
    if (simulate.fabScalar.lsu.commitSt) begin
        longint head;
        longint stCommitAddr;

        head = simulate.fabScalar.lsu.stqHead;
        stCommitAddr = {simulate.fabScalar.lsu.datapath.ldx_path.stqAddr1[head],
            simulate.fabScalar.lsu.datapath.ldx_path.stqAddr2[head]};

        fault_propagated_cnt += fi_dpi_check_store(stCommitAddr,
            simulate.fabScalar.lsu.datapath.ldx_path.stCommitData,
            simulate.fabScalar.lsu.datapath.ldx_path.stCommitSize);
    end

    // if fault propagated, make correction on RTL based on funcsim
    if (fault_propagated_cnt) begin
        $display("%1d fault(s) propagated!", fault_propagated_cnt);
        // force processor flush
        recovery_flag = 1;
    end

    // regardless fault propagated or not, update the pc_to_continue
    pc_to_continue = get_arch_PC(0);
endtask

endmodule

/* end of file */
