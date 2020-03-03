#ifndef TASK_H_
#define TASK_H_
#include "common.h"
#include <util/delay.h>
#include <inttypes.h>

#define REDTASK 0
#define YELLOWTASK 1
#define BUTTONATASK 2
#define BUTTONCTASK 3
#define PRINT 4

typedef  struct {
    uint8_t ready;
    uint32_t nextRelease;
    uint32_t period;
    void (*tick_fn) (void);
} task;


extern task _redTask;
extern task _yellowTask;
extern task _buttonATask;
extern task _buttonCTask;
extern task _print;

#endif