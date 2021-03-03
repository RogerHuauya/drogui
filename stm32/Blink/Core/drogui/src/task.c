#include "task.h"
#include <stdbool.h>
int tasks_count = 0;

typedef struct _task{
    uint32_t period, last_time;
    int priority, ignored;
    void (*function) ();
} task;


task Tasks[100]; 


void executeTask(task* t){
    t->last_time = TIME;
    t->function();
}

bool isTaskReady(task* t){
    return (TIME - (t->last_time)) > (t->period);
}

void addTask(void (*f) (), uint32_t period, int priority){
    Tasks[tasks_count].period = period;
    Tasks[tasks_count].priority = priority;
    Tasks[tasks_count].ignored = 0;
    Tasks[tasks_count++].function = f;
}
void initRTOS(){
    while(1){
        
        int max_score = 0, id = -1;
        
        for(int i = 0; i < tasks_count; i++){
            int score = Tasks[i].priority + Tasks[i].ignored;
            if(isTaskReady(&Tasks[i]) && max_score < score)
                max_score = score, id = i;    
        }

        for(int i = 0; i < tasks_count; i++){
            if(isTaskReady(&Tasks[i]) && i != id)
                Tasks[i].ignored++;
        }

        if(id >= 0)
            executeTask(&Tasks[id]), Tasks[id].ignored = 0;
    }
}