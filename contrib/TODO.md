# TODO
This file is for contributors only and features the work that has to be carried
out in order to complete the project.

## Phase 1
The following is a list of primitive functions that have to be implemented to
complete the phase1 stage. Each entry has two fields: a "done/not done" marker,
and a list of author names. An example is

```
[x] void print(): <user name> [, <user name> [ ... ]]
```

### `pcb.h`
#### Basic PCB functions
1. [x] `void bka_pcbs_init(void)`: Oscar
1. [x] `void bka_pcb_free(pcb_t *p)`: Oscar
1. [x] `pcb_t *bka_pcb_alloc(void)`: Oscar

#### Process queue functions
1. [x] `void bka_pcb_queue_init(struct list_head *head)`: Oscar
1. [x] `int bka_pcb_queue_isempty(struct list_head *head)`: Oscar
1. [x] `void bka_pcb_queue_ins(struct list_head *head, pcb_t *p)`: Oscar
1. [x] `pcb_t *bka_pcb_queue_head(struct list_head *head)`: Oscar
1. [x] `pcb_t *bka_pcb_queue_pop(struct list_head *head)`: Oscar
1. [x] `pcb_t *bka_pcb_queue_rm(struct list_head *head, pcb_t *p)`: Oscar

#### Process tree functions
1. [x] `int bka_pcb_tree_isempty(pcb_t *this)`: Oscar
1. [x] `void bka_pcb_tree_push(pcb_t *prnt, pcb_t *p)`: Oscar
1. [x] `pcb_t *bka_pcb_tree_pop(pcb_t *p)`: Oscar
1. [x] `pcb_t *bka_pcb_tree_parentrm(pcb_t *p)`: Oscar

### `asl.h`
1. [x] `void initASL()`: Daniele, Oscar
1. [x] `void bka_sem_free()`: Oscar
1. [x] `semd_t* getSemd(int *key)`: Daniele, Oscar
1. [x] `int insertBlocked(int *key, pcb_t* p)`: Oscar
1. [x] `pcb_t* removeBlocked(int *key)`: Daniele, Oscar
1. [x] `pcb_t* outBlocked(pcb_t *p)`: Oscar
1. [x] `pcb_t* headBlocked(int *key)`: Daniele, Oscar
1. [x] `void outChildBlocked(pcb_t *p)`: Oscar

### Extra
1. `list_del(l)` of `list.h` does **not** reset the list fields of `l` upon
 removal, shall we perform an edit?


## Phase 1.5
### `callback.h`
1. [x] `void bka_na_init(state_t *s, nac_t c)`: Andrea, Daniele, Nicolò

### `pcb.h`
1. [x] `void bka_pcb_init(pcb_t *p, pfun_t f)`: Andrea, Daniele, Nicolò
1. [x] `int bka_pcb_to_pid(pcb_t const * const p)`: Oscar
1. [x] `pcb_t * bka_pid_to_pcb(unsigned pid)`: Oscar

### `test2.c`
1. [x] `pcb_t* pcb_test_factory(unsigned test_no)`: Oscar
