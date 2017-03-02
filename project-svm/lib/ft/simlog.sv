/**********************************************************
 *
 * Simulation log functionality source code
 *
 * First author: Bagus Wibowo
 * created: 05/09/2014
 **********************************************************/

//`define SIMLOG_DEBUG
`define SIMLOG_LIMIT (10_000_000)

`ifndef BASE_OUTPUT_DIR
`define BASE_OUTPUT_DIR "."
`endif

import "DPI-C" function void simlog_init(string base_output_dir);
import "DPI-C" function void simlog_close();
import "DPI-C" function void simlog_loadmetadata(string path);
import "DPI-C" function void simlog_clocktick(longint clk, longint commit_cnt);
import "DPI-C" function void simlog_fetch(longint clk, longint seq_no,
        longint pc, int opcode, int is_syscall, int is_call, int is_ret,
        int m_cache_hit);
import "DPI-C" function void simlog_rename(longint clk, longint seq_no,
        int dest_valid, longint dest_log, longint dest_phy,
        int src1_valid, longint src1_log, longint src1_phy,
        int src2_valid, longint src2_log, longint src2_phy);
import "DPI-C" function void simlog_dispatch(longint clk, longint seq_no,
        int is_load, longint load_id, int is_store, longint store_id);
import "DPI-C" function void simlog_issue(longint clk, longint seq_no,
        int lane);
import "DPI-C" function void simlog_regread(longint clk, longint seq_no,
        longint src1, longint src2);
import "DPI-C" function void simlog_memexecute(longint clk, longint seq_no);
import "DPI-C" function void simlog_loadexecute(longint clk, longint load_id);
import "DPI-C" function void simlog_writeback(longint clk, longint seq_no);
import "DPI-C" function void simlog_retire(longint clk, longint seq_no,
        int is_control, int mispred);
import "DPI-C" function void simlog_storecommit(longint clk, longint store_id);
import "DPI-C" function void simlog_freedreg(longint clk, longint reg_id);
import "DPI-C" function void simlog_flush(longint clk);
import "DPI-C" function void simlog_exception(longint clk, longint seq_no,
        longint pc, longint v0);
import "DPI-C" function void simlog_analyze(longint clk, int exit_simulation);
import "DPI-C" function longint get_reg(int tid, int n);

`ifdef MCACHE
import "DPI-C" function int cache_l1m_peek(longint addr);
`define METADATA_OFFSET 64'h00004000_00000000
`define METADATA_WIDTH (32)
`endif

module Simlog (input clk);
    parameter METADATA_PATH = "";

    integer fd_log;

    initial begin
        `ifdef SIMLOG_DEBUG
        fd_log = $fopen({`BASE_OUTPUT_DIR,"/","results/simlog_v.out"},"w");
        `endif
        simlog_init(`BASE_OUTPUT_DIR);

        // load metadata file if provided
        if (METADATA_PATH != "") begin
            simlog_loadmetadata($sformatf(METADATA_PATH));
        end
    end /* initial */

    renPkt ibPacket [0:(`MAX_FISSION*`FETCH_WIDTH)-1];
    renPkt renPacket [0:`DISPATCH_WIDTH-1];
    disPkt disPacket [0:`DISPATCH_WIDTH-1];
    iqPkt iqPacket [0:`DISPATCH_WIDTH-1];
    alPkt alPacket [0:`DISPATCH_WIDTH-1];
    lsqPkt lsqPacket [0:`DISPATCH_WIDTH-1];
    payloadPkt rrPacket_int [0:`ISSUE_WIDTH-1];
    payloadPkt rrPacket_int_l1 [0:`ISSUE_WIDTH-1];
    memPkt memPacket_int_agen;
    memPkt memPacket_int_i;
    ctrlPkt ctrlPacket_int [0:`ISSUE_WIDTH-1];
    commitPkt amtPacket_int [0:`COMMIT_WIDTH-1];
    phys_reg freedPhyReg_int [0:`COMMIT_WIDTH-1];
    reg [`COMMIT_WIDTH-1:0] commitStore;
    reg [`COMMIT_WIDTH-1:0] commitLoad;
    reg [`COMMIT_WIDTH-1:0] commitCti;

    always_comb
    begin
        int i;
        for (i = 0; i < (`MAX_FISSION*`FETCH_WIDTH); i++)
        begin
            ibPacket[i] = simulate.fabScalar.ibPacket[i];
        end
        for (i = 0; i < `DISPATCH_WIDTH; i++)
        begin
            renPacket[i] = simulate.fabScalar.renPacket[i];
            disPacket[i] = simulate.fabScalar.disPacket[i];
            iqPacket[i]  = simulate.fabScalar.iqPacket[i];
            alPacket[i]  = simulate.fabScalar.alPacket[i];
            lsqPacket[i]  = simulate.fabScalar.lsqPacket[i];
        end
        for (i = 0; i < `ISSUE_WIDTH; i++)
        begin
            rrPacket_int[i]  = simulate.fabScalar.rrPacket_int[i];
            rrPacket_int_l1[i]  = simulate.fabScalar.rrPacket_int_l1[i];
        end
        memPacket_int_agen = simulate.fabScalar.lsu.memPacket_int_i;
        memPacket_int_i =
            simulate.fabScalar.lsu.datapath.ldx_path.memPacket_int_i;
        for (i = 0; i < `ISSUE_WIDTH; i++)
        begin
            ctrlPacket_int[i]  = simulate.fabScalar.ctrlPacket_int[i];
        end
        for (i = 0; i < `COMMIT_WIDTH; i++)
        begin
            amtPacket_int[i]  = simulate.fabScalar.amtPacket_int[i];
            commitStore[i]  = simulate.fabScalar.commitStore[i];
            commitLoad[i]  = simulate.fabScalar.commitLoad[i];
            commitCti[i]  = simulate.fabScalar.commitCti[i];
            freedPhyReg_int[i] = simulate.fabScalar.freedPhyReg_int[i];
        end
    end

    always @(posedge clk) begin
        int i;
        int m_cache_hit;

        `ifdef SIMLOG_DEBUG
        $fwrite(fd_log, "-----------------------------\n");
        $fwrite(fd_log, "cycle %1x\n", simulate.CYCLE_COUNT);
        `endif

        // clock tick
        simlog_clocktick(simulate.CYCLE_COUNT, simulate.COMMIT_COUNT);

        for (i = 0; i < (`MAX_FISSION*`FETCH_WIDTH); i++)
        begin
            if (ibPacket[i].valid && simulate.fabScalar.instBuf.decodeReady_i &&
                ~simulate.fabScalar.instBuf.stallFetch) begin

                // decide metadata cache access status
                `ifdef MCACHE
                // if metadata in the cache, assume it was hit
                m_cache_hit = cache_l1m_peek(
                    (ibPacket[i].pc/(32/`METADATA_WIDTH)) + `METADATA_OFFSET);
                `else
                 // assume always hit if m-cache is not implemented
                m_cache_hit = 1;
                `endif

                `ifdef SIMLOG_DEBUG
                $fwrite(fd_log, "Decode %1x %1x %1x\n", ibPacket[i].pc,
                    ibPacket[i].seqNo, m_cache_hit);
                `endif
                simlog_fetch(simulate.CYCLE_COUNT, ibPacket[i].seqNo,
                    ibPacket[i].pc, ibPacket[i].opcode,
                    ibPacket[i].inst == {26'h0,`SYSCALL_M}, // 6-bit SYSCALL_M
                    ibPacket[i].fu == `INSTRUCTION_TYPE2 &&
                        (ibPacket[i].opcode == JAL ||
                         ibPacket[i].opcode == JALR ||
                         ibPacket[i].opcode == JALR_HB ||
                         ibPacket[i].opcode == BLTZAL ||
                         ibPacket[i].opcode == BGEZAL ||
                         ibPacket[i].opcode == BLTZALL ||
                         ibPacket[i].opcode == BGEZALL),
                    ibPacket[i].fu == `INSTRUCTION_TYPE2 &&
                        ibPacket[i].ctrlType == `RETURN,
                    m_cache_hit);
            end
        end
        for (i = 0; i < `DISPATCH_WIDTH; i++)
        begin
            // for rename stage, make sure that there exists instruction in
            // the instruction buffer
            if (simulate.fabScalar.instBuf.instCount >= i &&
                simulate.fabScalar.instBufferReady &&
                !simulate.fabScalar.stallfrontEnd) begin

                `ifdef SIMLOG_DEBUG
                $fwrite(fd_log, "RN %1x: D %1x %1x > %1x,",
                        disPacket[i].seqNo, disPacket[i].phyDestValid,
                        disPacket[i].logDest, disPacket[i].phyDest);
                $fwrite(fd_log, " S1 %1x %1x > %1x, S2 %1x %1x > %1x\n",
                        disPacket[i].phySrc1Valid,
                        renPacket[i].logSrc1, disPacket[i].phySrc1,
                        disPacket[i].phySrc2Valid,
                        renPacket[i].logSrc2, disPacket[i].phySrc2);
                `endif
                simlog_rename(simulate.CYCLE_COUNT, disPacket[i].seqNo,
                              disPacket[i].phyDestValid,
                              disPacket[i].logDest, disPacket[i].phyDest,
                              disPacket[i].phySrc1Valid,
                              renPacket[i].logSrc1, disPacket[i].phySrc1,
                              disPacket[i].phySrc2Valid,
                              renPacket[i].logSrc2, disPacket[i].phySrc2);
            end
            if (simulate.fabScalar.backEndReady &&
                ~simulate.fabScalar.recoverFlag &&
                ~simulate.fabScalar.exceptionFlag &&
                ~simulate.fabScalar.loadViolation) begin

                `ifdef SIMLOG_DEBUG
                $fwrite(fd_log, "IQ %1x\n", iqPacket[i].seqNo);
                $fwrite(fd_log, "AL %1x\n", alPacket[i].seqNo);
                `endif
                simlog_dispatch(simulate.CYCLE_COUNT, iqPacket[i].seqNo,
                                lsqPacket[i].isLoad,
                                simulate.fabScalar.lsqID[i],
                                lsqPacket[i].isStore,
                                simulate.fabScalar.lsqID[i]);
                if (lsqPacket[i].isLoad) begin
                    `ifdef SIMLOG_DEBUG
                    $fwrite(fd_log, "LQ %1x\n", lsqPacket[i].seqNo);
                    `endif
                end
                if (lsqPacket[i].isStore) begin
                    `ifdef SIMLOG_DEBUG
                    $fwrite(fd_log, "SQ %1x %1x\n", lsqPacket[i].seqNo,
                            simulate.fabScalar.lsqID[i]);
                    `endif
                end
            end
        end
        for (i = 0; i < `ISSUE_WIDTH; i++)
        begin
            if (rrPacket_int[i].valid) begin
                `ifdef SIMLOG_DEBUG
                $fwrite(fd_log, "Issue %1x %1x %1x %1x %1x\n",
                        rrPacket_int[i].seqNo, rrPacket_int[i].valid,
                        rrPacket_int[i].phySrc1, rrPacket_int[i].phySrc2,
                        rrPacket_int[i].phyDest);
                `endif
                simlog_issue(simulate.CYCLE_COUNT, rrPacket_int[i].seqNo, i);
            end
            if (rrPacket_int_l1[i].valid) begin
                `ifdef SIMLOG_DEBUG
                $fwrite(fd_log, "RegRead %1x %1x %1x\n",
                        rrPacket_int_l1[i].seqNo, rrPacket_int_l1[i].phySrc1,
                        rrPacket_int_l1[i].phySrc2);
                `endif
                simlog_regread(simulate.CYCLE_COUNT, rrPacket_int_l1[i].seqNo,
                               rrPacket_int_l1[i].phySrc1,
                               rrPacket_int_l1[i].phySrc2);
            end
        end
        if (memPacket_int_agen.valid) begin
            `ifdef SIMLOG_DEBUG
            $fwrite(fd_log, "MX %1x %1x %1x\n", memPacket_int_agen.seqNo);
            `endif
            simlog_memexecute(simulate.CYCLE_COUNT, memPacket_int_agen.seqNo);
        end
        if (simulate.fabScalar.lsu.datapath.ldx_path.loadDataValid_o) begin
            `ifdef SIMLOG_DEBUG
            $fwrite(fd_log, "LDI %1x %1x %1x\n", memPacket_int_i.seqNo,
                    memPacket_int_i.lsqID, memPacket_int_i.phyDest);
            `endif
            simlog_loadexecute(simulate.CYCLE_COUNT, memPacket_int_i.lsqID);
        end
        for (i = 0; i < `ISSUE_WIDTH; i++)
        begin
            if (ctrlPacket_int[i].valid) begin
                `ifdef SIMLOG_DEBUG
                $fwrite(fd_log, "WB %1x %1x\n", ctrlPacket_int[i].seqNo,
                        ctrlPacket_int[i].valid);
                `endif
                simlog_writeback(simulate.CYCLE_COUNT, ctrlPacket_int[i].seqNo);
            end
        end
        for (i = 0; i < simulate.fabScalar.activeList.totalCommit; i++)
        begin
            if (freedPhyReg_int[i].valid & !simulate.fabScalar.recoverFlag)
            begin
                `ifdef SIMLOG_DEBUG
                $fwrite(fd_log, "Freeing : %1x\n", freedPhyReg_int[i].reg_id);
                `endif
                simlog_freedreg(simulate.CYCLE_COUNT,
                                freedPhyReg_int[i].reg_id);
            end
            `ifdef SIMLOG_DEBUG
            $fwrite(fd_log, "Commit %1x %1x %1x %1x %1x\n",
                    amtPacket_int[i].seqNo, amtPacket_int[i].valid,
                    commitLoad[i], commitStore[i], commitCti[i]);
            `endif
            simlog_retire(simulate.CYCLE_COUNT, amtPacket_int[i].seqNo,
                          commitCti[i],
                          simulate.fabScalar.activeList.mispredFlag[i]);
        end
        if (simulate.fabScalar.lsu.commitSt) begin
            `ifdef SIMLOG_DEBUG
            $fwrite(fd_log, "Store commit %1x\n",
                    simulate.fabScalar.lsu.stqHead);
            `endif
            simlog_storecommit(simulate.CYCLE_COUNT,
                               simulate.fabScalar.lsu.stqHead);
        end

        if (simulate.fabScalar.recoverFlag | simulate.fabScalar.loadViolation)
        begin
            `ifdef SIMLOG_DEBUG
            $fwrite(fd_log, "recoverFlag = 1\n");
            `endif
            simlog_flush(simulate.CYCLE_COUNT);
        end
        if (simulate.fabScalar.exceptionFlag) begin
            `ifdef SIMLOG_DEBUG
            $fwrite(fd_log, "exceptionFlag = 1 -- %1x PC: %1x, v0: %1x\n",
                    simulate.fabScalar.activeList.exceptionSeqNo,
                    simulate.fabScalar.exceptionPC-4, get_reg(0,2));
            `endif
            simlog_exception(simulate.CYCLE_COUNT,
                             simulate.fabScalar.activeList.exceptionSeqNo,
                             simulate.fabScalar.exceptionPC-4, get_reg(0,2));
        end
        if (simulate.fabScalar.stallfrontEnd) begin
            `ifdef SIMLOG_DEBUG
            $fwrite(fd_log, "stallfrontEnd = 1\n");
            `endif
        end

        if (simulate.onemorecycle || (simulate.COMMIT_COUNT >= `SIMLOG_LIMIT))
        begin
            simlog_analyze(simulate.CYCLE_COUNT,
                           (simulate.COMMIT_COUNT >= `SIMLOG_LIMIT));
            $fclose(fd_log);
            simlog_close();
        end

    end /* always @(negedge clk) */

endmodule

/* end of file */
