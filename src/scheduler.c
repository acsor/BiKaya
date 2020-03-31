#include "scheduler.h"
#include "sys.h"


void bka_interval_timer_set(unsigned time, unsigned unit) {
	unsigned *interval_timer = (unsigned*) BUS_REG_TIMER;
	unsigned const time_scale = *((unsigned*) BUS_REG_TIME_SCALE);

	*interval_timer = 1000000 * time_scale * time * unit;
}
