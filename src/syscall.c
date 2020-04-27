#include "exc.h"
#include "io.h"
#include "pcb.h"
#include "sem.h"
#include "sched.h"
#include "sem.h"
#include "syscall.h"
#include "time.h"

/**
 * Terminates the current syscall handling by placing the return value given
 * by @c retval at an appropriate old area register. Code placed after this
 * call is ignored, for a jump to the previously running user-space code is
 * performed.
 * @param retval Return value the currently executing syscall should return.
 */
static void sys_return(unsigned retval);
/**
 * Like @c sys_return(), but performs no jumps and can therefore be followed by
 * any other instruction.
 * @param retval Return value the currently executing syscall should return.
 */
static void sys_return_stay(unsigned retval);

static void sys_cpu_time(unsigned int* user, unsigned int* kernel, unsigned int* wallclock);
static void sys_fork(unsigned arg1, unsigned arg2, unsigned arg3);
/**
 * Terminates the process identified by @c arg1 and all of its descendant
 * processes. Two cases might occur while invoking this syscall
 *
 * <ul>
 * 		<li>Either the previously running process is amongst the ones to be
 * 		killed</li>
 * 		<li>Or it is not</li>
 * </ul>
 *
 * In the first case, a context-switch is automatically operated at the end
 * of the syscall, with any other instruction following this syscall being
 * ignored. In the second, the execution resumes ordinarily.
 *
 * @param arg1 PCB pointer to the process to terminate. If @c NULL, the
 * previously running process is selected.
 * @param arg2 Unused.
 * @param arg3 Unused.
 * @return @c 0 in case of success, @c -1 otherwise (for example, in cases
 * where @c arg1 identifies an already freed or invalid PCB pointer).
 */
static void sys_kill(unsigned arg1, unsigned arg2, unsigned arg3);
/**
 * Implements the V() operation of classical semaphores. If no semaphore
 * could be identified starting from @c arg1, the kernel will enter @c PANIC
 * mode.
 * @param arg1 To be cast to <tt>(int *)</tt>, identifies the semaphore key.
 * @param arg2 Unused.
 * @param arg3 Unused.
 * @return The updated value of the identified semaphore.
 */
static void sys_verhogen(unsigned arg1, unsigned arg2, unsigned arg3);
/**
 * Implements the P() operation of classical semaphores. If no semaphore
 * could be identified starting from @c arg1, the kernel will enter @c PANIC
 * mode.
 * @param arg1 To be cast to <tt>(int *)</tt>, identifies the semaphore key.
 * @param arg2 Unused.
 * @param arg3 Unused.
 * @return The updated value of the identified semaphore.
 */
static void sys_passeren(unsigned arg1, unsigned arg2, unsigned arg3);
/**
 * This syscall issues an I/O operation by copying @c arg1 in the command
 * field of the device register pointed to by @c device. The caller is
 * blocked until command termination since operation on devices are synchronous.
 *
 * @param arg1: I/O operation to execute
 * @param arg2: pointer to the register of the desired device
 * @param arg3: used only if device is a terminal, to distinguish between
 * transmitting and receiving subdevice; trans = 0, recv = 1.
 * @return TODO Fill in.
 */
static void sys_iocmd(unsigned arg1, unsigned arg2, unsigned arg3);
/**
 * TODO Document.
 * @param type
 * @param old_area
 * @param new_area
 */
static void sys_spec_passup(unsigned type, unsigned old_area, unsigned new_area);
static void sys_get_pid(unsigned arg1, unsigned arg2, unsigned arg3);


/**
 * A syscall function type.
 */
typedef void (*syscall_t)(unsigned, unsigned, unsigned);

/**
 * A mapping from syscall ID to the corresponding syscall function pointer.
 */
static syscall_t sys_id_to_syscall[] = {
		sys_cpu_time,
		sys_fork,
		sys_kill,
		sys_verhogen,
		sys_passeren,
		sys_iocmd,
		sys_spec_passup,
		sys_get_pid,
};


void bka_sys_call(unsigned id, unsigned arg1, unsigned arg2, unsigned arg3) {
	/* Predefined syscalls. */
    if (BKA_SYS_CPU_TIME <= id && id <= BKA_SYS_GETPID) {
        sys_id_to_syscall[id](arg1, arg2, arg3);
	/* Custom syscalls. */
    } else if (BKA_SYS_GETPID < id) {
    	/* Old area and new area variables, respectively. */
    	state_t *oa_dest = bka_sched_curr->sysbk_area[0];
		state_t *na = bka_sched_curr->sysbk_area[1];

		if (oa_dest && na) {
			state_t *oa_src = (state_t *) SYSBK_OLDAREA;

			/* TODO Is this correct according to the specifications? What is the
			 * protocol for exiting out of a custom syscall?
			 */
			bka_memcpy(oa_dest, oa_src, sizeof(state_t));
			LDST(na);
        } else {
			bka_sched_kill(bka_sched_curr);
			bka_sched_resume();
        }
    } else {
        PANIC();
    }
}


void sys_return(unsigned retval) {
	sys_return_stay(retval);
	bka_na_exit(SYSBK_NEWAREA);
}

void sys_return_stay(unsigned retval) {
	state_t *old_area = (state_t *) SYSBK_OLDAREA;

#ifdef BKA_ARCH_UARM
	old_area->a1 = retval;
#elif defined(BKA_ARCH_UMPS)
	old_area->reg_v0 = retval;
#endif
}

/*
Return -1 if the time can't be returned, 0 otherwise.
*/
void sys_cpu_time(unsigned int* user, unsigned int* kernel, unsigned int* wallclock) {
	
	pcb_t *to_calculate = bka_sched_curr;

	/* If the process is in the free PCB list return -1.*/
	if (bka_pcb_stat(to_calculate)) 
		sys_return(-1);


    /*The difference between the current value of TODLO and the initial value(when the process was created)*/
	*wallclock = (BKA_TOD_LO) - (to_calculate->full_timer);
    *kernel = to_calculate->kernel_timer;
	*user = (*wallclock - *kernel);
	sys_return_stay(0);
	
}

void sys_fork(unsigned arg1, unsigned arg2, unsigned arg3) {
	/* TODO Implement. */
	sys_return(-1);
}

void sys_kill(unsigned arg1, unsigned arg2, unsigned arg3) {
	pcb_t *to_kill = ((pcb_t *) arg1) == NULL ? bka_sched_curr: (pcb_t *) arg1;

	switch (bka_sched_kill(to_kill)) {
		case BKA_E_INVARG:
			sys_return(-1);
		case BKA_SCHED_KR:
			sys_return_stay(0);
			bka_sched_resume();
		case 0:
		default:
			sys_return(0);
	}
}

void sys_verhogen(unsigned arg1, unsigned arg2, unsigned arg3) {
	semd_t *sem = bka_sem_get((int *) arg1);

	if (!sem) {
		/* TODO Print out an error message */
		PANIC();
	}

	sys_return((unsigned) bka_sem_v(sem, bka_sched_curr));
}

void sys_passeren(unsigned arg1, unsigned arg2, unsigned arg3) {
	int *semkey = (int *) arg1;
	semd_t *sem = bka_sem_get(semkey);

	if (!sem) {
		/* TODO Print out an error message */
		PANIC();
	}

	sys_return_stay((unsigned) bka_sem_p(sem, bka_sched_curr));
	bka_sched_resume();
}

void sys_iocmd(unsigned arg1, unsigned arg2, unsigned arg3) {
	unsigned *uns_dev = (unsigned *) arg2;
	dtpreg_t *dtp_dev = (dtpreg_t *) arg2;
	termreg_t *term_dev = (termreg_t *) arg2;
	unsigned *command, *status;
	semd_t *sem = bka_dev_sem_get(uns_dev, arg3);

	if (bka_dev_line(uns_dev) == IL_TERMINAL) {
		command = arg3 ? &term_dev->recv_command: &term_dev->transm_command;
		status = arg3 ? &term_dev->recv_status: &term_dev->transm_command;
	} else {
		command = &dtp_dev->command;
		status = &dtp_dev->status;
	}

	*command = arg1;

	if (!sem) {
		/* TODO Print out error message. */
		PANIC();
	}

	bka_sem_p(sem, bka_sched_curr);

	sys_return_stay(*status);
	bka_sched_resume();
}

void sys_spec_passup(unsigned type, unsigned old_area, unsigned new_area) {
    state_t* oa = (state_t*) old_area, *na = (state_t*) new_area;
    pcb_t *const curr = bka_sched_curr;
    state_t **old_areas[] = {
			curr->sysbk_area + 0, curr->tlb_area + 0, curr->trap_area + 0
    };
	state_t **new_areas[] = {
			curr->sysbk_area + 1, curr->tlb_area + 1, curr->trap_area + 1
	};

	/* Invalid arguments received. */
	if (type < 0 || 2 < type || !oa || !na) {
		sys_return_stay((unsigned) -1);
		bka_sched_resume();
	}

	/* Syscall already once invoked. */
	if (*old_areas[type] || *new_areas[type]) {
		bka_sched_kill(bka_sched_curr);
		bka_sched_resume();
	} else {
		*old_areas[type] = oa;
		*new_areas[type] = na;

		sys_return(0);
	}
}

void sys_get_pid(unsigned arg1, unsigned arg2, unsigned arg3) {
	/* TODO Implement. */
	sys_return(-1);
}
