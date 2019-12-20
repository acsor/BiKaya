#include "system.h"
#include "termprint.h"


int main(void) {
    term_puts("hello, world\n");

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();

    return 0;
}
