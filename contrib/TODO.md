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
1. [x] `void initPcbs(void)`: Oscar
1. [x] `void freePcb(pcb_t *p)`: Oscar
1. [x] `pcb_t *allocPcb(void)`: Oscar

#### Process queue functions
1. [x] `void mkEmptyProcQ(struct list_head *head)`: ???
1. [x] `int emptyProcQ(struct list_head *head)`: ???
1. [x] `void insertProcQ(struct list_head *head, pcb_t *p)`: Oscar
1. [x] `pcb_t *headProcQ(struct list_head *head)`: ???
1. [x] `pcb_t *removeProcQ(struct list_head *head)`: Oscar
1. [x] `pcb_t *outProcQ(struct list_head *head, pcb_t *p)`: ???

#### Process tree functions
1. [x] `int emptyChild(pcb_t *this)`: ???
1. [x] `void insertChild(pcb_t *prnt, pcb_t *p)`: Oscar
1. [x] `pcb_t *removeChild(pcb_t *p)`: ???
1. [x] `pcb_t *outChild(pcb_t *p)`: Oscar

### `asl.h`
1. [ ] `semd_t* getSemd(int *key)`: ???
1. [ ] `void initASL()`: ???
1. [ ] `int insertBlocked(int *key, pcb_t* p)`: ???
1. [ ] `pcb_t* removeBlocked(int *key)`: ???
1. [ ] `pcb_t* outBlocked(pcb_t *p)`: ???
1. [ ] `pcb_t* headBlocked(int *key)`: ???
1. [ ] `void outChildBlocked(pcb_t *p)`: ???

### Extra
1. `list_del(l)` of `list.h` does **not** reset the list fields of `l` upon
 removal, shall we perform an edit?
