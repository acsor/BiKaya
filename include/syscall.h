#ifndef BKA_SYSCALL
#define BKA_SYSCALL

#define BKA_SYS_CPU_TIME	0
#define BKA_SYS_FORK		1
#define BKA_SYS_KILL		2
#define BKA_SYS_VERHOGEN	3
#define BKA_SYS_PASSEREN	4
#define BKA_SYS_IOCMD		5
#define BKA_SYS_SPEC_PASSUP	6
#define BKA_SYS_GETPID		7

#define BKA_SP_SYSBK	0
#define BKA_SP_TLB		1
#define BKA_SP_TRAP		2


/**
 * Forwards the invocation of a syscall, precisely the one identified by @c id.
 * @param id ID of the syscall to execute. If not recognized, the processor
 * enters panic mode.
 * @param arg1 First syscall argument.
 * @param arg2 Second syscall argument.
 * @param arg3 Third syscall argument.
 */
void bka_sys_call(unsigned id, unsigned arg1, unsigned arg2, unsigned arg3);


#endif
