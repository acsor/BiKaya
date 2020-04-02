#ifndef BKA_SCHEDULER_H
#define BKA_SCHEDULER_H


/**
 * Scheduling Time Units (STU) constants.
 */
#define BKA_STU_MICRO	1
#define BKA_STU_MILLI	1000
#define BKA_STU_SEC		1000000

/**
 * Sets the system-wide interval timer.
 * @param time Time (in the given unit) to elapse before the interval timer
 * interrupt.
 * @param unit Time unit, to be chosen among @c BKA_STU_MICRO, @c
 * BKA_STU_MILLI or @c BKA_STU_SEC.
 */
void bka_interval_timer_set(unsigned time, unsigned unit);


#endif
