//
// Created by ivan on 18.3.24.
//

#ifndef LAB4_SHARED_SEMAPHORES_H
#define LAB4_SHARED_SEMAPHORES_H

#include <semaphore.h>
#include "message.h"

struct SharedSemaphores {
    sem_t mutex;
    sem_t full;
    sem_t empty;
};

void detach_shared_memory(void* ptr);
void initialize_semaphores(SharedSemaphores* shared_semaphores_ref);

#endif //LAB4_SHARED_SEMAPHORES_H
