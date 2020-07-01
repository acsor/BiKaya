#ifndef BK_SYSCALL
#define BK_SYSCALL

#define BK_SYS_CPU_TIME		0
#define BK_SYS_FORK			1
#define BK_SYS_KILL			2
#define BK_SYS_VERHOGEN		3
#define BK_SYS_PASSEREN		4
#define BK_SYS_IOCMD		5
#define BK_SYS_SPEC_PASSUP	6
#define BK_SYS_GETPID		7

#define BK_SYS_FIRST	BK_SYS_CPU_TIME
#define BK_SYS_LAST		BK_SYS_GETPID

#define BK_SP_SYSBK		0
#define BK_SP_TLB		1
#define BK_SP_TRAP		2


/**
 * Forwards the invocation of a syscall, precisely the one identified by @c id.
 * @param id ID of the syscall to execute. If not recognized, the processor
 * enters panic mode.
 * @param arg1 First syscall argument.
 * @param arg2 Second syscall argument.
 * @param arg3 Third syscall argument.
 */
void bk_syscall(unsigned id, unsigned arg1, unsigned arg2, unsigned arg3);
/**
 * @return @c 1 if the syscall identified by @c id is registered, @c 0
 * otherwise.
 */
#define bk_syscall_avail(id)	(BK_SYS_FIRST <= (id) && (id) <= BK_SYS_LAST)
void bk_syscall_retval(pcb_t *p, unsigned retval);


#endif
