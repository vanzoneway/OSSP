//
// Created by ivan on 18.3.24.
//

#include "shared_semaphores.h"

void detach_shared_memory(void* ptr) {
    if (shmdt(ptr) == -1) {
        perror("shmdt");
        exit(1);
    }
}

void initialize_semaphores(SharedSemaphores* shared_semaphores_ref) {

    if (sem_init(&shared_semaphores_ref->mutex, 1, 1) == -1) {
        perror("sem_init(mutex)");
        exit(1);
    }
    if (sem_init(&shared_semaphores_ref->full, 1, 0) == -1) {
        perror("sem_init(full)");
        exit(1);
    }
    if (sem_init(&shared_semaphores_ref->empty, 1, BUFFER_SIZE - 1) == -1) {
        perror("sem_init(empty)");
        exit(1);
    }
}