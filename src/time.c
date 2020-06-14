#include "arch.h"
#include "time.h"

static time_t tick_start;


void bka_time_tick() {
	tick_start = bka_time_now();
}

time_t bka_time_tock() {
	return bka_time_now() - tick_start;
}
