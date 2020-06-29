#ifndef BIKAYA_TIME_H
#define BIKAYA_TIME_H


typedef unsigned time_t;


/**
 * @return The current time stamp.
 */
#define bk_time_now()	BK_TOD_LO

/**
 * Starts measurements for future calls to @c bk_time_tock(). If this
 * function is invoked multiple times before a call to @c bk_time_tock() is
 * made, only the last one will have an effect (and hence override all the
 * preceding ones).
 * @see @c bk_time_tock().
 */
void bk_time_tick();
/**
 * @return The time delta value since the last call to @c bk_time_tick().
 * Undefined behavior might occur if @c bk_time_tick() has never been invoked.
 * @see @c bk_time_tick().
 */
time_t bk_time_tock();


#endif
