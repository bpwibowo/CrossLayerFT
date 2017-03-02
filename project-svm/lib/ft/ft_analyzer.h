/**********************************************************
 *
 * Fault-Tolerance base analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#ifndef _FT_ANALYZER_H_
#define _FT_ANALYZER_H_

#include <string>
#include "simlog.h"

#define FULL_PATH(path) ((base_output_path_ + "/" + path).c_str())

struct VulnerabilityFactor {
    unsigned short iq;
    unsigned short rob;
    unsigned short lq;
    unsigned short sq;
    unsigned short rf;
    unsigned short ibuff;
    unsigned short fu_simple;
    unsigned short fu_complex;
    unsigned short fu_control;
    unsigned short fu_memory;
};

template <typename T> struct TotalVulnerableBits {
    T iq;
    T rob;
    T lq;
    T sq;
    T rf;
    T ibuff;
    T sys;
    T fu_simple;
    T fu_complex;
    T fu_control;
    T fu_memory;

    void clear() {
        iq = rob = lq = sq = rf = ibuff = sys = 0;
        fu_simple = fu_complex = fu_control = fu_memory = 0;
    }
    void add(struct VulnerabilityFactor vf) {
        iq+=vf.iq; rob+=vf.rob; lq+=vf.lq; sq+=vf.sq; rf+=vf.rf;
        ibuff+=vf.ibuff;
        sys+=vf.iq+vf.rob+vf.lq+vf.sq+vf.rf+vf.ibuff;
        fu_simple+=vf.fu_simple;
        fu_complex+=vf.fu_complex;
        fu_control+=vf.fu_control;
        fu_memory+=vf.fu_memory;
    }
    void add(struct TotalVulnerableBits<T> vbit) {
        iq+=vbit.iq; rob+=vbit.rob; lq+=vbit.lq; sq+=vbit.sq; rf+=vbit.rf;
        ibuff+=vbit.ibuff;
        sys+=vbit.iq+vbit.rob+vbit.lq+vbit.sq+vbit.rf+vbit.ibuff;
        fu_simple+=vbit.fu_simple;
        fu_complex+=vbit.fu_complex;
        fu_control+=vbit.fu_control;
        fu_memory+=vbit.fu_memory;
    }
    void sub(struct TotalVulnerableBits<T> vbit) {
        struct TotalVulnerableBits<T> amt;
        amt.iq=(vbit.iq<iq?vbit.iq:iq);amt.rob=(vbit.rob<rob?vbit.rob:rob);
        amt.lq=(vbit.lq<lq?vbit.lq:lq);amt.sq=(vbit.sq<sq?vbit.sq:sq);
        amt.rf=(vbit.rf<rf?vbit.rf:rf);
        amt.ibuff=(vbit.ibuff<ibuff?vbit.ibuff:ibuff);
        amt.fu_simple=(vbit.fu_simple<fu_simple?vbit.fu_simple:fu_simple);
        amt.fu_complex=(vbit.fu_complex<fu_complex?vbit.fu_complex:fu_complex);
        amt.fu_control=(vbit.fu_control<fu_control?vbit.fu_control:fu_control);
        amt.fu_memory=(vbit.fu_memory<fu_memory?vbit.fu_memory:fu_memory);
        amt.sys=amt.iq+amt.rob+amt.lq+amt.sq+amt.rf+amt.ibuff;
        iq-=amt.iq; rob-=amt.rob; lq-=amt.lq; sq-=amt.sq; rf-=amt.rf;
        ibuff-=amt.ibuff; sys-=amt.sys;
        fu_simple-=amt.fu_simple;
        fu_complex-=amt.fu_complex;
        fu_control-=amt.fu_control;
        fu_memory-=amt.fu_memory;
    }
    struct TotalVulnerableBits<double> div(double denom) {
        struct TotalVulnerableBits<double> res;
        res.iq=iq/denom;res.rob=rob/denom;res.lq=lq/denom;
        res.sq=sq/denom;res.rf=rf/denom;res.ibuff=ibuff/denom;res.sys=sys/denom;
        res.fu_simple=fu_simple/denom;
        res.fu_complex=fu_complex/denom;
        res.fu_control=fu_control/denom;
        res.fu_memory=fu_memory/denom;
        return res;
    }
    struct TotalVulnerableBits<T> diff(
        struct TotalVulnerableBits<T> val) {
        struct TotalVulnerableBits<T> res;
        res.iq=iq-val.iq;res.rob=rob-val.rob;res.lq=lq-val.lq;
        res.sq=sq-val.sq;res.rf=rf-val.rf;res.ibuff=ibuff-val.ibuff;
        res.sys=sys-val.sys;
        res.fu_simple=fu_simple-val.fu_simple;
        res.fu_complex=fu_complex-val.fu_complex;
        res.fu_control=fu_control-val.fu_control;
        res.fu_memory=fu_memory-val.fu_memory;
        return res;
    }
    void abs() {
        iq=iq<0?-iq:iq; rob=rob<0?-rob:rob; lq=lq<0?-lq:lq;
        sq=sq<0?-sq:sq; rf=rf<0?-rf:rf; ibuff=ibuff<0?-ibuff:ibuff;
        sys=sys<0?-sys:sys;
        fu_simple=fu_simple<0?-fu_simple:fu_simple;
        fu_complex=fu_complex<0?-fu_complex:fu_complex;
        fu_control=fu_control<0?-fu_control:fu_control;
        fu_memory=fu_memory<0?-fu_memory:fu_memory;
    }
};

template <typename T> struct InstructionCount {
    T iq;
    T rob;
    T lq;
    T sq;
    void clear() { iq = rob = lq = sq = 0; }
    void add(struct InstructionCount<T> op) {
        iq += op.iq; rob += op.rob; lq += op.lq; sq += op.sq;
    }
};

class FtAnalyzer {
  public:
    FtAnalyzer(SimLog *simlog, std::string base_output_path);
    virtual ~FtAnalyzer();

    void SetSummaryFileObject(std::ofstream *outfile_summary);
    virtual bool Analyze(uint64_t max_clock) = 0;

  protected:
    SimLog *simlog_;
    std::string base_output_path_;
    std::ofstream *p_outfile_summary_;
    bool zero_vf_on_protection_;

    void AnalyzeGrouped(uint64_t simulation_size, const char *in_file,
                        const char *out_file);
    void AnalyzeGrouped(uint64_t simulation_size, uint64_t group_size,
                        const char *in_file, const char *out_file);
    void AnalyzeGrouped(uint64_t simulation_size, const char *in_file,
                        const char *out_file,
                        struct TotalVulnerableBits<uint64_t> *out_total);
    void AnalyzeGrouped(uint64_t simulation_size, uint64_t group_size,
                        const char *in_file, const char *out_file,
                        struct TotalVulnerableBits<uint64_t> *out_total);
    void AnalyzeGroupError(const char *in_file1, const char *in_file2,
                           const char *out_file);
    void DumpSummaryTotalVBit(std::string id,
                              TotalVulnerableBits<uint64_t> tvb);
    void DumpSummaryTotalVBit(std::string id, TotalVulnerableBits<int64_t> tvb);
    void DumpSummaryTotalVBit(std::string id, TotalVulnerableBits<double> tvb);
    struct InstructionCount<double> GetRatio(
        struct InstructionCount<int64_t> numerator,
        struct InstructionCount<int64_t> denominator);
    bool IsPhaseCrossing(uint64_t cycle, uint64_t phase_period);

  private:
    struct TotalVulnerableBits<double> GetLineValueFromFile(
        std::ifstream *input_file);
};

#endif /* _FT_ANALYZER_H_ */

 /* end of file */
