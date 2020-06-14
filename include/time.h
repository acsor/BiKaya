#ifndef BIKAYA_TIME_H
#define BIKAYA_TIME_H


typedef unsigned time_t;


/**
 * @return The current time stamp.
 */
#define bka_time_now()	BKA_TOD_LO

/**
 * Starts measurements for future calls to @c bka_time_tock(). If this
 * function is invoked multiple times before a call to @c bka_time_tock() is
 * made, only the last one will have an effect (and hence override all the
 * preceding ones).
 * @see @c bka_time_tock().
 */
void bka_time_tick();
/**
 * @return The time delta value since the last call to @c bka_time_tick().
 * Undefined behavior might occur if @c bka_time_tick() has never been invoked.
 * @see @c bka_time_tick().
 */
time_t bka_time_tock();


#endif
