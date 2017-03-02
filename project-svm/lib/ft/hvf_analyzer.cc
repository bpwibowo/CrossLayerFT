#include <iostream>
#include "hvf_analyzer.h"
#include <iomanip>
#include "vbit_table.h"
#include "svm.h"

#define INVALID_SEQ_NO ((uint64_t)-1)

// *****************************************************************************
// AVF Statistical Aproximation
// NCSU ECE Department
// Cross Layer Reliability Project
//
// Module : HVF Analyzer Fault Injection scheme
// This program is similar to hvf_analyzer except it is based on fault injection
// instead of monitoring for a fault.  Every 512 clock cycles a random number is
// generated for each tested structure.  If withing the structure there is valid
// data at the location within the structure indicated by the random number then
// the potential for a fault has occured and the HVF flags are set. Results are
// then tabulated ever 2^20 clock cycles by summing the total of the number of
// HVF flags times a PVS score then dividing by 2048, the number of 512 clock
// cycle windows within 2^20 clock cycles.
//
// Author Thomas P. Stanton
// Date: 17 July 2014
// Revision 1.2
// Rev Date 27 July 2014
//
// Revision History
// Inital REV --AVF Calculations using Fault Injection scheme for computations.
//              Copied form hvf_analyzer and cleaned up code.
// 1.1 Cleaned up code, added counters to evaluate the reorder buffer.
// 1.2 Cleaned up code, added error checking for Log retire and Reorder buffer
//******************************************************************************


HvfAnalyzer::HvfAnalyzer(SimLog *simlog, std::string base_output_path,
    PvfAnalyzer *pvf_analyzer) : FtAnalyzer(simlog, base_output_path),
    StoreQ(SVM->cfg().core_size_sq), Issue(SVM->cfg().core_size_iq_int),
    Load(SVM->cfg().core_size_lq), ReOrder(SVM->cfg().core_size_al) {

    // initialize everything
    //HVF Counters

    IqHVFcounter = 0;   //Issue Queue counter
    ROBHVFcounter = 0;  //Reorder Buffer counter
    StoreHVFcounter = 0;//Store queue counter
    LoadHVFcounter = 0; //Load queue counter
    ROBHitCount = 0; //Counter for testing
    ROBHit512 = 0;
    ROBSize = 0;
    LogDispatchCount = 0.00;
    // PVF Scores

    IqPvsScore = 1.0;   //PVF for Issue Queue
    ROBPvsScore = 1.0;  //PVF for ReorderBuffer
    StoreQPvsScore = 1.0; //PVF for Store queue
    LoadQPvsScore = 1.0; //PVF for Load queue

    // PVS x HVF for structures
    IqPvs_x_HvfL = 0.00; //Issue Queue running total
    ROBPvs_x_HVF = 0.00; //ROB running total
    LoadPvs_x_HVF = 0.00; //Load Queue running total
    StorePvs_x_HVF = 0.00;   //Store Queue running total

    //Derating values
    ElementClear = INVALID_SEQ_NO;  //Definition for an empty location

    //Issue Queue initialization
    EraseALL = true; //set the boolian EraseALL to true
    QueueClear(Issue, EraseALL, SVM->cfg().core_size_iq_int); //call the Queueclear function

    //Load Queue Initialization
    QueueClear(Load, EraseALL, SVM->cfg().core_size_lq);

    //ReOrder Buffer Initialization
    start = 0;
    stop = SVM->cfg().core_size_al;
    //Call buffer clear initialize the buffer
    BufferClear (ReOrder, start, stop);

    // set ROBHead = 0 and ROBTail = ROBHead
    ROBHead = 0;
    ROBTail  = ROBHead;

    // clear vbits of ROB, LQ, and SQ
    // at which during dispatch and execute/complete has different vbits
    ROBvbits = 0;
    LQvbits = 0;
    SQvbits = 0;

    // Build store queue structure, set all flags to false and
    // seq_no to FFFF FFFF FFFF FFFF.
    QueueClear(StoreQ,EraseALL,SVM->cfg().core_size_sq);

    pvf_analyzer_ = pvf_analyzer;

    //Open Log file and set headers for it
    time_t starttime;
    time (&starttime);//set timestamp
    outfile_HvfResults.open(FULL_PATH("results/HVF_Results.txt"),
                            std::ios::out | std::ios::trunc);
    outfile_HvfResults<< "This file is a result of using Vilas Sriharan " <<
        "and David R. Kaeli"<<std::endl;
    outfile_HvfResults<<"method for calculating AVF based on their paper " <<
        "'Using Hardware Vulnerability Factors"<<std::endl;
    outfile_HvfResults<<"to Enhance AVF Calculations' Dated June 2010 " <<
        "**Fault Injection**"<<std::endl <<std::endl;
    //print timestamp to file
    outfile_HvfResults<<ctime(&starttime)<<"\n"<<std::endl;
    outfile_HvfResults<<"Clock Count"<<"\t"<<"Issue Queue"<<"\t"<<"ROB"<<
        "\t"<<"Load Queue";
    outfile_HvfResults<<"\t" <<"Store Queue"<<std::endl <<std::endl;

}

HvfAnalyzer::~HvfAnalyzer() {
    if(outfile_HvfResults.is_open()) outfile_HvfResults.close();
}

void HvfAnalyzer::LogClockTick(LogClockTickArgs args) {
    //Initially and every 512 cycles
    if (IsPhaseCrossing(args.clk, (0x1<<9)) ||
        (args.clk == 1 && !SVM->cfg().enable_warm_up)){
        SetMonitor();
    }

    // Every 2^20 cycles

    if (IsPhaseCrossing(args.clk, (0x1<<20))) {

        //update AVF calculation
        //devide the sum of structure calulations of AVF by number of samples
        //(2048)

        AVFIQ = IqPvs_x_HvfL /2048.00;
        AVFROB = ROBPvs_x_HVF/2048.00;
        AVFLoadQueue = LoadPvs_x_HVF/2048.00;
        AVFStoreQueue = StorePvs_x_HVF/2048.00;

        //write results to file

        if(outfile_HvfResults.is_open()){
            outfile_HvfResults <<std::setw(9)<< args.clk;
            outfile_HvfResults << "\t"<<AVFIQ<<"\t"<<AVFROB;
            outfile_HvfResults <<"\t"<<AVFLoadQueue<<"\t"<<AVFStoreQueue<<
                std::endl;
            outfile_HvfResults<<std::setw(9)<<args.clk<<"\t"<<IqPvs_x_HvfL;
            outfile_HvfResults<<"\t"<<ROBPvs_x_HVF<<"\t"<<LoadPvs_x_HVF;
            outfile_HvfResults<<"\t"<<StorePvs_x_HVF<<std::endl;
            }
        else
        std::cout <<"Results file not open"<< std::endl;

        //reset temp value Structure AVF counter to 0
        IqPvs_x_HvfL = 0.00;
        ROBPvs_x_HVF = 0.00;
        LoadPvs_x_HVF = 0.00;
        StorePvs_x_HVF = 0.00;

    }
}

void HvfAnalyzer::LogDispatch(LogDispatchArgs args) {

    //this function updates the Issue Queue and ROB by storing the seq_no into
    // the first available location of IqElement. Second if the index of the
    // IqElement equals the Monitor element the IQHvfFlaq is set true.

    if (SVM->cfg().nop_issued || // if NOP is always issued to FU
        simlog_->logdb_[args.seq_no].opcode != OPCODE_NOP // non-NOP inst
        ) {

        for (uint32_t i=0; i < SVM->cfg().core_size_iq_int; i++){

            if (Issue[i].seq_no == ElementClear){
                Issue[i].seq_no = args.seq_no;
                Issue[i].valid = true;

                if(i == IQMonitorElement){
                    Issue[i].monitored = true;
                    IQMonitorElement = SVM->cfg().core_size_iq_int +2;
                }
                break;
            }

            if (i==SVM->cfg().core_size_iq_int) {
                std::cout << "---- WARNING: no empty space in issue queue" <<
                    std::endl;
            }
        }
    }
    // put entery in reorder buffer

    ReOrder[ROBTail].seq_no = args.seq_no; //store the sequence number
    ReOrder[ROBTail].IQFlag = false; //set all flags initially to false
    ReOrder[ROBTail].LoadFlag = false;
    ReOrder[ROBTail].ROBFlag = false;
    ROBTail++; //increment the ROB Tail.
    // check to see if at the end of the ReOrder
    // buffer

    if(ROBTail >= SVM->cfg().core_size_al){
        ROBTail = 0;                //reset the tail to the beginning
    }
    ROBSize ++; //increment Size count
    LogDispatchCount = LogDispatchCount + 1.00;

    if(ROBSize > SVM->cfg().core_size_al){
        std::cout<<"---- WARNING: Reorder Buffer Overrun Fault Injection-----"<<
            ROBSize<< " at :"<<args.clk<<std::endl;
    }

}

void HvfAnalyzer::LogDispatchLoad(LogDispatchLoadArgs args){
    //load the Load queue at location specified by load_id
    Load[args.load_id].seq_no = args.seq_no;

    if(args.seq_no != INVALID_SEQ_NO){
        Load[args.load_id].valid = true;
    }
}

void HvfAnalyzer::LogDispatchStore(LogDispatchStoreArgs args){
    // upon a dispatch store instruction store seq_no into store queue at
    // location indicated by the store_id and set the data valid flag for that
    // location

    StoreQ[args.store_id].seq_no = args.seq_no;

    if(args.seq_no != INVALID_SEQ_NO){
        StoreQ[args.store_id].valid = true;
        // test to see if monitored flag is set to true and set to false if it
        // is meaning that the data has been overwritten and is now correct.

        if (StoreQ[args.store_id].monitored == true){
            StoreQ[args.store_id].monitored = false;
        }

        else{

            if(args.store_id == StoreQMonitor){
                StoreQ[args.store_id].monitored = true;
                StoreQMonitor = SVM->cfg().core_size_sq +2;
            }
        }
    }

    else{
        StoreQ[args.store_id].valid = false;

    }
}

void HvfAnalyzer::LogRetire(LogRetireArgs args){
    //This function retires an instruction from the ROB
    //check to see if any HVF flags are set and increment appropriate
    // HVF flag counter
    //First check if sequence number equals the sequence at the head of ROB

    while(ReOrder[ROBHead].seq_no != args.seq_no){
        // Check if ROB is empty

        if (ROBHead == ROBTail){
            return;
        }
        //issue warning
        std::cout<<"---WARNING Sequence number stored at Head not equal to " <<
            "sequence number sent"<<std::endl;
        std::cout<<"Reorder Buffer head = "<<ReOrder[ROBHead].seq_no<<
            " Sequence sent = " <<args.seq_no<<std::endl;

        //clear the entry at the head
        start = ROBHead; //set start location
        stop = ROBHead + 1; // set stop location
        //clear the location indicated by ROBHead
        BufferClear (ReOrder, start, stop);
        ROBHead ++; // Increment the head location
        ROBSize --; //Decrement Reorder Buffer size.

        if(ROBHead >= SVM->cfg().core_size_al){ //Check if at end of buffer
            ROBHead = 0;  //reset to begining if at end
        }
    //reserved for summing flags in the future.

    }//close while

    if(args.seq_no != INVALID_SEQ_NO){

        if (ReOrder[ROBHead].seq_no == args.seq_no){

            //Check the Issue Queue HVF Flag in the ROB

            if (ReOrder[ROBHead].IQFlag == true){

                IqPvs_x_HvfL = (IqPvsScore)+IqPvs_x_HvfL;

                // derating = vulnerable bit count / total bit count in an entry
                double vbit_count = double(
                    GetVulnerableBitCount(STRUCTURE_IQ,
                                          simlog_->logdb_[args.seq_no].opcode,
                                          true));
                double entry_bit_count = double(
                    GetEntryBitsCount(STRUCTURE_IQ));

                    if(entry_bit_count == 0)//check to see if zero is returned
                    {
                        std::cout <<" WARNING vbit table returned a zero " <<
                            "value for entry bit count derating set to 1" <<
                            std::endl;
                        derating = 1.00;
                    }
                    else
                        derating = vbit_count/entry_bit_count;
                pvf_analyzer_->HvfSingleEvent(args.seq_no, ST_IQ, derating);

               /* IqHVFcounter ++;
                std::cout<<"Issue Queue Hit at sample number:  "<<
                    clk/512<<std::endl;

                if(IqHVFcounter > FULL){
                    IqPvs_x_HvfL = (IqPvsScore*8.0)+IqPvs_x_HvfL;
                    std::cout <<"IQ HVF counter full, data shifted:  "<<
                        IqPvs_x_HvfL<< std::endl;
                    IqHVFcounter = 0;
                }*/
            }
            //Check Load Queue HVF flag in the ROB

            if(ReOrder[ROBHead].LoadFlag == true){
                LoadPvs_x_HVF = (LoadQPvsScore) + LoadPvs_x_HVF;

                // derating = vulnerable bit count / total bit count in an entry
                double vbit_count = double(
                    GetVulnerableBitCount(STRUCTURE_LQ,
                                          simlog_->logdb_[args.seq_no].opcode,
                                          true));
                double entry_bit_count = double(
                    GetEntryBitsCount(STRUCTURE_LQ));

                if(entry_bit_count == 0)//check to see if zero is returned
                {
                    std::cout <<" WARNING vbit table returned a zero value " <<
                        "for entry bit count derating set to 1" <<std::endl;
                    derating = 1.00;
                }
                else
                    derating = vbit_count/entry_bit_count;

                pvf_analyzer_->HvfSingleEvent(args.seq_no, ST_LQ, derating);

                // Increment Load queue HVF counter
               /* LoadHVFcounter ++;
                std::cout<<"Load Queue hit at sample number  :"<<(clk+1)/512<<
                    "\t"<<clk<<std::endl;
                if(LoadHVFcounter > FULL){
                    LoadPvs_x_HVF = (LoadQPvsScore*8.0)+LoadPvs_x_HVF;
                    LoadHVFcounter = 0;
                }*/
            }
        }
        // clear the load queue

        for(uint64_t i = 0; i < SVM->cfg().core_size_lq; i++){

            if(ReOrder[ROBHead].seq_no == Load[i].seq_no){
                EraseALL = false;
                QueueClear(Load, EraseALL, i);
            }
        }

        //Check seq no versus store que and set commit flag if needed

        for (uint64_t i = 0; i < SVM->cfg().core_size_sq; i++){

            if(ReOrder[ROBHead].seq_no == StoreQ[i].seq_no){
                StoreQ[i].StoreCommit = true;
            }
        }
        //check ROB monitor point

        if (ReOrder[ROBHead].ROBFlag == true){
            ROBPvs_x_HVF = (ROBPvsScore)+ROBPvs_x_HVF;

            // derating = vulnerable bit count / total bit count in an entry
            double vbit_count = ROBvbits;
            double entry_bit_count = double(GetEntryBitsCount(STRUCTURE_ROB));

            if(entry_bit_count == 0)//check to see if zero is returned
            {
                std::cout <<" WARNING vbit table returned a zero value for " <<
                    "entry bit count derating set to 1" <<std::endl;
                derating = 1.00;
            }
            else
                derating = vbit_count/entry_bit_count;

            pvf_analyzer_->HvfSingleEvent(args.seq_no, ST_ROB, derating);

            /*ROBHVFcounter++;
            std::cout<<"ReOrder Buffer HIT at sample number :"<<clk/512<<
                std::endl;
            // Check condition of the ROB HVF counter

            if(ROBHVFcounter>=FULL){
                ROBPvs_x_HVF = (ROBPvsScore*8)+ROBPvs_x_HVF;
                //std::cout<<"ROB HVF Counter Full, data shifted:  "<<
                    ROBPvs_x_HVF<<"\t"<<clk/512<<std::endl;
                ROBHVFcounter = 0;
            } */
        }
        //retire the instruction
        start = ROBHead; //set start location
        stop = ROBHead + 1; // set stop location
        //clear the location indicated by ROBHead
        BufferClear (ReOrder, start, stop);
        ROBHead ++; // Increment the head location
        ROBSize --; //Decrement Reorder Buffer size.

        if(ROBHead >= SVM->cfg().core_size_al){ //Check if at end of buffer
            ROBHead = 0;  //reset to begining if at end
        }
    }

    else {
        std::cout<<"------Warning attempting to Retire Instruction While " <<
            "Reorder Buffer Empty-----"<<std::endl;
    }

}

void HvfAnalyzer::LogIssue(LogIssueArgs args) {

    //THis function issues the instruction from the Issue queue
    //and checks to see if it is a monitored element to set the ROB IQ flag
    uint64_t i;
    for (i=0; i<SVM->cfg().core_size_iq_int; i++){ //Scan IssueQueue

        if (Issue[i].seq_no == args.seq_no){ //find the seq_no

            if (Issue[i].monitored == true){ // check HVF flag

                if (ROBHead < ROBTail){ // check if head is less than tail

                    for(uint32_t n = ROBHead; n <= ROBTail; n++){ //Scan ROB

                        // find matching seq_no in ROB
                        if (Issue[i].seq_no == ReOrder[n].seq_no){
                            ReOrder[n].IQFlag = true; //set flag in ROB
                        }
                    }

                }

                else{
                    // scan from the ROBHead to the size of the ROB -1
                    //to find match

                    for (uint32_t n = ROBHead; n < (SVM->cfg().core_size_al); n++){

                        if (Issue[i].seq_no == ReOrder[n].seq_no){
                            ReOrder[n].IQFlag = true; //set flag in ROB
                        }
                    }
                    //scan from 0 to ROBTail to find match
                    for (uint32_t n = 0; n < ROBTail; n++){

                        if (Issue[i].seq_no == ReOrder[n].seq_no){
                            ReOrder[n].IQFlag = true; //set flag in ROB
                        }
                    }
                }
            }
            // clear the issue queue element
            EraseALL = false;
            QueueClear(Issue, EraseALL, i); // Clear IQ at the Indicated
                                            // location

            if (Issue[i].seq_no != ElementClear) {
                std::cout << "---- WARNING: element is not cleared" <<
                    std::endl;
            }
            break;
        }
    }

    if (i == SVM->cfg().core_size_iq_int) {
        std::cout << "---- WARNING: seq_no " << args.seq_no <<
            " is not found in IQ" << std::endl;
    }
}

void HvfAnalyzer::LogFlush(LogFlushArgs args) {
    // Clear the Load and Issue Queues
    //std::cout<<" Log Flush"<<std::endl;
    EraseALL = true; //set EraseALL flag to true to erase entire queue
    QueueClear(Issue,EraseALL,SVM->cfg().core_size_iq_int);
    QueueClear(Load,EraseALL,SVM->cfg().core_size_lq);

     //clear the reorder buffer
    start = 0; // Start location
    stop = SVM->cfg().core_size_al; //Stop Location
    BufferClear (ReOrder, start, stop); //call to clear function
    //set tail = to head;
    ROBTail = ROBHead; // empty ROB
    ROBSize = 0; //Reorder Buffer emptied
    //record kepping upkeep.

    // clear the uncommitted entries in the store queue
    EraseALL = false; //set EraseALL flag to false to erase a single element

    for(uint64_t i = 0; i < SVM->cfg().core_size_sq; i++){

        if(StoreQ[i].StoreCommit != true){
            // clear alll entries that are not committed
            QueueClear(StoreQ, EraseALL, i);
        }
    }
}

int HvfAnalyzer::RandomNumber (int NumberOfStructures){

    //this module generates a random number to indicate the structure to be
    // monitored, it receives the size of the structure from the calling
    // function and returns a interger representing the structure to be
    // monitored.

    int MonitorStructure;
    //srand(time(NULL)); //seed random number generator with system time
    MonitorStructure = rand() % NumberOfStructures;//generate the structure
                                                   //number
    return MonitorStructure;
}

void HvfAnalyzer::LogLoadExecute(LogLoadExecuteArgs args){
    //This module check
    //check monitor point in load queue

    // Note: We assume LQ data are read during after execute stage
    //       at which the load request is sent to D-Cache
    if (Load[args.load_id].monitored == true){
        //set fail flag in ROB for load queue

        for(uint32_t i = 0; i < SVM->cfg().core_size_al ; i++){
            //find entry in ROB
            if (ReOrder[i].seq_no == Load[args.load_id].seq_no){
                ReOrder[i].LoadFlag = true; //set Flag in ROB
                //reset Monitor point
                LoadQMonitor = SVM->cfg().core_size_lq + 2;
            }
        }
    }
}

void HvfAnalyzer::LogStoreCommit(LogStoreCommitArgs args){
    //Check the monitor flag

    if(StoreQ[args.store_id].monitored==true){
        // count the event
        StorePvs_x_HVF = StoreQPvsScore + StorePvs_x_HVF;

        // derating = vulnerable bit count / total bit count in an entry
        double vbit_count = SQvbits;
        double entry_bit_count = double(GetEntryBitsCount(STRUCTURE_SQ));

        if(entry_bit_count == 0)//check to see if zero is returned
        {
            std::cout <<" WARNING vbit table returned a zero value for " <<
                "entry bit count derating set to 1" <<std::endl;
            derating = 1.00;
        }
        else
            derating = vbit_count/entry_bit_count;

        pvf_analyzer_->HvfSingleEvent(StoreQ[args.store_id].seq_no, ST_SQ,
                                      derating);

        // increment HVF counter if flag is set
        /*StoreHVFcounter++;
        std::cout<<"Store Queue Hit at sample number :"<<clk/512<<"\t"<<clk<<
            std::endl;

        if(StoreHVFcounter>FULL){
            // Sum PVS x HVF and reset HVF counter
            StorePvs_x_HVF= (StoreQPvsScore*8)+StorePvs_x_HVF;
            StoreHVFcounter = 0;
        }*/
    }
    //clear the entry and reset all the flags
    EraseALL = false;
    QueueClear(StoreQ, EraseALL, args.store_id);
}

void HvfAnalyzer::WarmUpEnds(WarmUpEndsArgs args) {
    SetMonitor();
}

void HvfAnalyzer::QueueClear(std::vector<struct Queue> &pqueue, bool all,
    uint64_t item){
    // this functions clears the structure queue either completely or a single
    //  location

    if (all != true){
        // only clear the selectected queue at location indicated by item
        pqueue[item].seq_no = ElementClear;
        pqueue[item].valid = false;
        pqueue[item].monitored = false;
        pqueue[item].StoreCommit = false;
    }

    else {
        //Clear all if all is true then item has the size of the queue
        // and clear all of the queue

        for(uint64_t k = 0; k < item; k++){
            pqueue[k].seq_no = ElementClear;
            pqueue[k].valid = false;
            pqueue[k].monitored = false;
            pqueue[k].StoreCommit = false;
        }
    }
}

void HvfAnalyzer::BufferClear(std::vector<struct Buffer> &pbuffer, int start,
    int stop){
    // This Function clears the structure buffer from a start location to a
    // stop location.  It returns nothing.
    //std::cout<<"Start: "<<start<<" Stop: "<<stop<<std::endl;

    for(int i = start; i < stop; i++){
        pbuffer[i].seq_no = ElementClear;
        pbuffer[i].IQFlag = false;
        pbuffer[i].LoadFlag = false;
        pbuffer[i].ROBFlag = false;
    }
//std::cout<<"i = :"<<stop<<std::endl;
}

void HvfAnalyzer::SetMonitor() {
    // Clear vbits
    ROBvbits = 0;
    LQvbits = 0;
    SQvbits = 0;

    //Get a new Element to monitor
    IQMonitorElement = RandomNumber(SVM->cfg().core_size_iq_int);

    if(Issue[IQMonitorElement].seq_no != ElementClear){
        Issue[IQMonitorElement].monitored = true;
        IQMonitorElement = SVM->cfg().core_size_iq_int + 2;
    }

    else{
        IQMonitorElement = SVM->cfg().core_size_iq_int + 2;
    }

    ROBMonitor = RandomNumber(SVM->cfg().core_size_al);

    if(ReOrder[ROBMonitor].seq_no != ElementClear){
        ReOrder[ROBMonitor].ROBFlag = true;
        bool dispatch_mode = // writeback_clk == INVALID --> not complete yet
            (simlog_->logdb_[ReOrder[ROBMonitor].seq_no].writeback_clk == INVALID_CLK);
        ROBvbits = double(
            GetVulnerableBitCount(STRUCTURE_ROB,
                                  simlog_->logdb_[ReOrder[ROBMonitor].seq_no].opcode,
                                  true, dispatch_mode));
        ROBMonitor = SVM->cfg().core_size_al + 2;
    }

    else
        ROBMonitor = SVM->cfg().core_size_al + 2;

    LoadQMonitor = uint64_t(RandomNumber(SVM->cfg().core_size_lq));
    // Check the load queue

    if (Load[LoadQMonitor].seq_no != ElementClear){
        Load[LoadQMonitor].monitored = true;
        bool dispatch_mode = // memexecute_clk == INVALID --> not executed yet
            (simlog_->logdb_[Load[LoadQMonitor].seq_no].memexecute_clk == INVALID_CLK);
        LQvbits = double(
            GetVulnerableBitCount(STRUCTURE_LQ,
                                  simlog_->logdb_[Load[LoadQMonitor].seq_no].opcode,
                                  true, dispatch_mode));
        LoadQMonitor = SVM->cfg().core_size_lq +2;
    }

    else
        LoadQMonitor = SVM->cfg().core_size_lq +2;
    StoreQMonitor = uint64_t(RandomNumber(SVM->cfg().core_size_sq));

    // check to see if there is data in the store que at monitor location
    // if so set the monitored falge to true and set the StoreQMonitor to
    // outside the range of the Store queue.

    if(StoreQ[StoreQMonitor].valid == true){
        StoreQ[StoreQMonitor].monitored = true;
        bool dispatch_mode = // memexecute_clk == INVALID --> not executed yet
            (simlog_->logdb_[StoreQ[StoreQMonitor].seq_no].memexecute_clk == INVALID_CLK);
        SQvbits = double(
            GetVulnerableBitCount(STRUCTURE_SQ,
                                  simlog_->logdb_[StoreQ[StoreQMonitor].seq_no].opcode,
                                  true, dispatch_mode));
        StoreQMonitor = SVM->cfg().core_size_sq + 2;
    }

    else{
        StoreQMonitor = SVM->cfg().core_size_sq + 2;
    }
}

bool HvfAnalyzer::Analyze(uint64_t max_clock) {
     //Print how many cycles run
    outfile_HvfResults<< "\n Number of Clock Cycles:\t";
    outfile_HvfResults<< max_clock<<std::endl;
   // outfile_HvfResults<<"\n Total ROB hits = "<<ROBHitCount<<std::endl;
    outfile_HvfResults<<"\n Average ROB usage per sample period: "<<
        (LogDispatchCount/max_clock)*512<<std::endl;
    outfile_HvfResults<<"\n Monitor Mode Simulation concluded at: \t";
    time_t endtime;
    time (&endtime);
    outfile_HvfResults<<ctime(&endtime)<<std::endl;
    if(outfile_HvfResults.is_open()) outfile_HvfResults.close();
    return true;
}

/* end of file */
