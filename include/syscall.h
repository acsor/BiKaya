#ifndef BKA_SYSCALL
#define BKA_SYSCALL

#define BKA_SYS_KILL	3


/**
 * Forwards the invocation of a syscall, precisely the one identified by @c id.
 * @param id ID of the syscall to execute. If not recognized, the processor
 * enters panic mode.
 * @param arg1 First syscall argument
 * @param arg2 Second syscall argument
 * @param arg3 Third syscall argument
 */
void bka_sys_call(unsigned id, unsigned arg1, unsigned arg2, unsigned arg3);

/**
 * Similar to @c bka_sys_kill, but selects the currently running process. No
 * action will be performed if there are no current processes.
 * @see bka_sys_kill
 */
void bka_sys_kill_running();
/**
 * Terminates the process identified by @c pid and all of its descendant
 * children.
 * @param pid ID of the process to terminate.
 * @see bka_pcb_to_pid, bka_pid_to_pcb
 */
void bka_sys_kill(unsigned pid);


#endif
