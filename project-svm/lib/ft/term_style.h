#ifndef _TERM_STYLE_H_
#define _TERM_STYLE_H_

#ifdef DISABLE_TERM_STYLE

#define TERM_BLACK ""
#define TERM_RED ""
#define TERM_GREEN ""
#define TERM_YELLOW ""
#define TERM_BLUE ""
#define TERM_MAGENTA ""
#define TERM_CYAN ""
#define TERM_WHITE ""
#define TERM_RESET ""

#else

#define TERM_BLACK "\x1b[30m"
#define TERM_RED "\x1b[31m"
#define TERM_GREEN "\x1b[32m"
#define TERM_YELLOW "\x1b[33m"
#define TERM_BLUE "\x1b[34m"
#define TERM_MAGENTA "\x1b[35m"
#define TERM_CYAN "\x1b[36m"
#define TERM_WHITE "\x1b[37m"
#define TERM_RESET "\x1b[0m"

#endif

#endif
