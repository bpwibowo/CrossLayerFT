#ifndef _SIMLOG_CONFIG_H_
#define _SIMLOG_CONFIG_H_

#define POINTS_IN_GRAPH 500

#define COPROC1
// When SIM_CONFIG_INC needs to be defined during compilation,
// use flag: -DSIM_CONFIG_INC='"<path>"'
#ifdef SIM_CONFIG_INC
#include SIM_CONFIG_INC
#endif

#define INSTRUCTION_TYPE_SIMPLE 0
#define INSTRUCTION_TYPE_COMPLEX 1
#define INSTRUCTION_TYPE_CONTROL 2
#define INSTRUCTION_TYPE_MEMORY 3
#define INSTRUCTION_TYPE_FP 4

#define SIZE_AMT SIZE_RMT

#endif /* _SIMLOG_CONFIG_H_ */
