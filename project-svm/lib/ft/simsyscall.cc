/**********************************************************
 *
 * Simulation log module code: syscall submodule
 *
 * First author: Bagus Wibowo
 * created: 05/14/2014
 **********************************************************/

#include <iostream>
#include "simlog.h"
#include <mips64.h>
#include <syscall.h>

#define USE_V0 (1 << R_V0)
#define USE_A0 (1 << R_A0)
#define USE_A1 (1 << R_A1)
#define USE_A2 (1 << R_A2)
#define USE_A3 (1 << R_A3)
#define USE_SP (1 << R_SP)
#define DEF_V0 (1 << R_V0)
#define DEF_A3 (1 << R_A3)

struct UsesDefs SimLog::GetSyscallUsesDefs(int type) {
    struct UsesDefs ud;
    uint64_t use_flags=0, def_flags=0;

    // every syscall will define R_V0 and R_A3
    def_flags = DEF_V0 | DEF_A3;
    // the registers used by syscall might be different
    switch (type) {
        case SYSCALL_read:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 0
        case SYSCALL_write:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 1
        case SYSCALL_open:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 2
        case SYSCALL_close:
            use_flags = USE_V0 | USE_A0; break; // 3
        case SYSCALL_stat:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 4
        case SYSCALL_fstat:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 5
        case SYSCALL_lstat:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 6
        case SYSCALL_lseek:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 8
        case SYSCALL_mmap:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A3 | USE_SP; break; // 9
        case SYSCALL_mprotect:
            use_flags = USE_V0; break; // 10
        case SYSCALL_munmap:
            use_flags = USE_V0; break; // 11
        case SYSCALL_brk:
            use_flags = USE_V0 | USE_A0 | USE_SP; break; // 12
        case SYSCALL_rt_sigaction:
            use_flags = USE_V0; break; // 13
        case SYSCALL_rt_sigprocmask:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 14
        case SYSCALL_ioctl:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 15
        case SYSCALL_writev:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 19
        case SYSCALL_access:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 20
        case SYSCALL_mremap:
            use_flags = USE_V0; break; // 24
        case SYSCALL_dup:
            use_flags = USE_V0 | USE_A0; break; // 31
        case SYSCALL_nanosleep:
            use_flags = USE_V0; break; // 34
        case SYSCALL_getpid:
            use_flags = USE_V0; break; // 38
        case SYSCALL_exit:
            use_flags = USE_V0 | USE_A0; break; // 58
        case SYSCALL_kill:
            use_flags = USE_V0 | USE_A0; break; // 60
        case SYSCALL_uname:
            use_flags = USE_V0 | USE_A0; break; // 61
        case SYSCALL_fcntl:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 70
        case SYSCALL_fsync:
            use_flags = USE_V0 | USE_A0; break; // 72
        case SYSCALL_ftruncate:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 75
        case SYSCALL_getcwd:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 77
        case SYSCALL_chdir:
            use_flags = USE_V0; break; // 78
        case SYSCALL_rename:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 80
        case SYSCALL_mkdir:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 81
        case SYSCALL_unlink:
            use_flags = USE_V0 | USE_A0; break; // 85
        case SYSCALL_readlink:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 87
        case SYSCALL_gettimeofday:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 94
        case SYSCALL_getrlimit:
            use_flags = USE_V0 | USE_A0; break; // 95
        case SYSCALL_times:
            use_flags = USE_V0; break; // 98
        case SYSCALL_getuid:
            use_flags = USE_V0; break; // 100
        case SYSCALL_getgid:
            use_flags = USE_V0; break; // 102
        case SYSCALL_geteuid:
            use_flags = USE_V0; break; // 105
        case SYSCALL_getegid:
            use_flags = USE_V0; break; // 106
        case SYSCALL_setrlimit:
            use_flags = USE_V0; break; // 155
        case SYSCALL_gettid:
            use_flags = USE_V0; break; // 178
        case SYSCALL_futex:
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2; break; // 194
        case SYSCALL_cacheflush:
            use_flags = USE_V0; break; // 197
        case SYSCALL_exit_group:
            use_flags = USE_V0 | USE_A0; break; // 205
        case SYSCALL_tgkill:
            use_flags = USE_V0 | USE_A0 | USE_A1; break; // 225
        case SYSCALL_set_thread_area:
            use_flags = USE_V0; break; // 242
        default:
            //std::cout << "[simlog] WARNING: default SYSCALL uses-defs is used
            // for type " << type << std::endl;
            use_flags = USE_V0 | USE_A0 | USE_A1 | USE_A2;
            break;
    }

    // add uses-defs into the uses-defs vectors
    if (use_flags & USE_V0) ud.uses.push_back(R_V0);
    if (use_flags & USE_A0) ud.uses.push_back(R_A0);
    if (use_flags & USE_A1) ud.uses.push_back(R_A1);
    if (use_flags & USE_A2) ud.uses.push_back(R_A2);
    if (use_flags & USE_A3) ud.uses.push_back(R_A3);
    if (use_flags & USE_SP) ud.uses.push_back(R_SP);
    if (def_flags & DEF_V0) ud.defs.push_back(R_V0);
    if (def_flags & DEF_A3) ud.defs.push_back(R_A3);

    return ud;
}
