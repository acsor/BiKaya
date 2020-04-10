#ifndef BKA_CALLBACK_H
#define BKA_CALLBACK_H

#include "sys.h"


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
 * 		<li>The SP (Stack Pointer) points to @c BKA_RAMTOP.<li>
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
void bka_na_init(state_t *s, sec_t c);
/**
 * Startup function to be invoked before managing one system-level exception
 * (e.g. program traps, syscalls, breakpoints, interrupts etc.). Similarly to
 * its counterpart, @c bka_na_exit(), this function performs initialization
 * steps needed to ensure the correct entering in of new areas/exception
 * managers.
 * @param new_area New area which the processor is entering in.
 * @see bka_na_exit
 */
void bka_na_enter(unsigned new_area);
/**
 * Like @c bka_na_enter(), but to be (preferably) invoked as the last
 * instruction of a system-level exception handler.
 * @param new_area New area which the processor is exiting out from.
 * @see bka_na_enter
 */
void bka_na_exit(unsigned new_area);


#endif
