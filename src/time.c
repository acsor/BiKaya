#include "arch.h"
#include "time.h"

static time_t tick_start;


void bk_time_tick() {
	tick_start = bk_time_now();
}

time_t bk_time_tock() {
	return bk_time_now() - tick_start;
}
