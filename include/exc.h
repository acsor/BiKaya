#ifndef BK_EXC_H
#define BK_EXC_H

#include "arch.h"


#ifdef BK_ARCH_UARM
/**
 * @param p	A pointer to @c state_t.
 * @return The cause register of the state pointed to by @c p.
 */
#define	BK_STATE_CAUSE(p)	(p)->CP15_Cause
#elif defined(BK_ARCH_UMPS)
#define	BK_STATE_CAUSE(p)	(p)->cause
#endif


/**
 * Function type for a System Exception Callback (SEC).
 */
typedef void (*sec_t)(void);

/**
 * Initializes the new area (NA) pointed to by @c s in such a way that
 *
 * <ul>
 * 		<li>The new area program counter (PC) points to the code executed by
 * 		@c c</li>
 * 		<li>The SP (Stack Pointer) points to @c BK_RAMTOP.<li>
 * 		<li>The status register is initialized so that
 * 			<ul>
 * 				<li>Interrupts are masked/disabled</li>
 * 				<li>Virtual memory is disabled</li>
 * 				<li>Kernel mode is enabled</li>
 * 			</ul>
 * 		</li>
 * </ul>
 * @param s New area to initialize.
 * @param c Callback code to be executed upon reaching this new area.
 */
void bk_na_init(state_t *s, sec_t c);
/**
 * Startup function to be invoked before managing one kernel-level exception
 * (e.g. program traps, syscalls, breakpoints, interrupts etc.). Similarly to
 * its counterpart, @c bk_na_exit(), this function performs initialization
 * steps needed to ensure the correct entering in of new areas/exception
 * managers.
 * @param new_area New area which the processor is entering in.
 * @see bk_na_exit
 */
void bk_na_enter(unsigned new_area);
/**
 * Like @c bk_na_enter(), but to be (preferably) invoked as the last
 * instruction of a kernel-level exception handler.
 * @param new_area New area which the processor is exiting out from.
 * @see bk_na_enter
 */
void bk_na_exit(unsigned new_area);

/**
 * Sets up internal state for higher-level exception handling, whose
 * new area callback was registered through the Spec Passup (hence the
 * @c sp prefix) system call.
 * @param type The type of exception to be handled. Can only take values from
 * @c BK_SP_SYSBK, @c BK_SP_TLB or @c BK_SP_TRAP.
 */
void bk_sp_enter(unsigned type);
/**
 * The complementary operation of @c bk_sp_enter(). To be invoked after an
 * exception of type @c type has been handled.
 * @param type The type of exception to be handled. Can only take values from
 * @c BK_SP_SYSBK, @c BK_SP_TLB or @c BK_SP_TRAP.
 */
void bk_sp_exit(unsigned type);

/**
 * Callback function to be invoked just after entering kernel-level code.
 * @see bk_kernel_on_exit()
 */
void bk_kernel_on_enter();
/**
 * Callback function to be invoked just prior to exiting kernel-level code.
 * Note that this function has no power to perform any context switch, but
 * only executes stateful operations on process states (hence the @c _on
 * prefix).
 * @see bk_kernel_on_enter()
 */
void bk_kernel_on_exit();


#endif
