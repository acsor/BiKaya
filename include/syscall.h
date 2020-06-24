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

#define BKA_SYS_FIRST	BKA_SYS_CPU_TIME
#define BKA_SYS_LAST	BKA_SYS_GETPID

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
void bka_syscall(unsigned id, unsigned arg1, unsigned arg2, unsigned arg3);
/**
 * @return @c 1 if the syscall identified by @c id is registered, @c 0
 * otherwise.
 */
#define bka_syscall_avail(id)	(BKA_SYS_FIRST <= (id) && (id) <= BKA_SYS_LAST)
void bka_syscall_retval(pcb_t *p, unsigned retval);


#endif
