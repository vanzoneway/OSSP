#include "message.h"
#include "shared_semaphores.h"


MessageQueue* messageQueue;
SharedSemaphores* shared_semaphores;


void print_menu() {
    printf("╔════════════════════════╗\n");
    printf("║        Options         ║\n");
    printf("╠════════════════════════╣\n");
    printf("║  +  - Create Producer  ║\n");
    printf("║  -  - Create Consumer  ║\n");
    printf("║  l  - View Statistics  ║\n");
    printf("║  q  - Quit             ║\n");
    printf("╚════════════════════════╝\n");
}


void producer(pid_t child_pid) {
    srand(time(NULL));

    if (child_pid == -1) {
        perror("Ошибка fork()");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {

        while (true) {
            sem_wait(&shared_semaphores->empty);
            sem_wait(&shared_semaphores->mutex);
            Message  message = generate_message();
            add_message(messageQueue, message);
            printf("Producer: Generated message:\n ");
            print_message(message);

            sem_post(&shared_semaphores->mutex);
            sem_post(&shared_semaphores->full);
            sleep(5);
        }
    }
}

void consumer(pid_t child_pid) {


    if (child_pid == -1) {
        perror("Ошибка fork()");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        while (true) {
            sem_wait(&shared_semaphores->full);
            sem_wait(&shared_semaphores->mutex);

            Message message = get_message(messageQueue);

            printf("Consumer: Got a message:\n");
            print_message(message);

            sem_post(&shared_semaphores->mutex);
            sem_post(&shared_semaphores->empty);


        }
    }
}

int initialize_shmem(int proj_id) {
    key_t queue_key = ftok(".", proj_id);
    if (queue_key == -1) {
        perror("ftok");
        exit(1);
    }
    int shmid = shmget(queue_key, sizeof(SharedSemaphores), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget ");
        exit(1);
    }
    return shmid;
}


//ftok() - generates a key for our IPC context
//shmget() - returns an id of our shared memory and as parameters takes key from ftok(), size of shared memory, flags
//btw id say shmget() as a malloc, but returns a memory for all processes
//we have to detach shmemory from our process with shmdt

int main() {

    std::string input;

    int queue_shmid = initialize_shmem(123);

    messageQueue = (MessageQueue*)shmat(queue_shmid, NULL, 0);
    if (messageQueue == (void*)-1) {
        perror("shmat (messageQueue)");
        exit(1);
    }

    int semaphores_shmid = initialize_shmem(124);

    shared_semaphores = (SharedSemaphores*)shmat(semaphores_shmid, NULL, 0);
    if (shared_semaphores == (void*)-1) {
        perror("shmat (semaphores)");
        exit(1);
    }

    initialize_queue(messageQueue);
    initialize_semaphores(shared_semaphores);

    print_menu();
    std::cout << "Enter your choice: \n";
    do {
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch((char) input[0]) {
            case '+':
            {

                pid_t child_pid = fork();
                if (child_pid == 0) {
                    messageQueue = (MessageQueue*)shmat(queue_shmid, NULL, 0);
                    if (messageQueue == (void*)-1) {
                        perror("shmat (messageQueue)");
                        exit(1);
                    }

                    shared_semaphores = (SharedSemaphores*)shmat(semaphores_shmid, NULL, 0);
                    if (shared_semaphores == (void*)-1) {
                        perror("shmat (semaphores)");
                        exit(1);
                    }
                    producer(child_pid);

                    detach_shared_memory(messageQueue);

                    detach_shared_memory(shared_semaphores);
                } else {
                    messageQueue = (MessageQueue*)shmat(queue_shmid, NULL, 0);
                    if (messageQueue == (void*)-1) {
                        perror("shmat (messageQueue)");
                        exit(1);
                    }
                    shared_semaphores = (SharedSemaphores*)shmat(semaphores_shmid, NULL, 0);
                    if (shared_semaphores == (void*)-1) {
                        perror("shmat (semaphores)");
                        exit(1);
                    }
                }


                break;
            }

            case '-':
            {
                pid_t child_pid = fork();
                if(child_pid == 0) {
                    messageQueue = (MessageQueue*)shmat(queue_shmid, NULL, 0);
                    if (messageQueue == (void*)-1) {
                        perror("shmat (messageQueue)");
                        exit(1);
                    }

                    shared_semaphores = (SharedSemaphores*)shmat(semaphores_shmid, NULL, 0);
                    if (shared_semaphores == (void*)-1) {
                        perror("shmat (semaphores)");
                        exit(1);
                    }
                    consumer(child_pid);

                    detach_shared_memory(messageQueue);

                    detach_shared_memory(shared_semaphores);
                } else {
                    messageQueue = (MessageQueue*)shmat(queue_shmid, NULL, 0);
                    if (messageQueue == (void*)-1) {
                        perror("shmat (messageQueue)");
                        exit(1);
                    }
                    shared_semaphores = (SharedSemaphores*)shmat(semaphores_shmid, NULL, 0);
                    if (shared_semaphores == (void*)-1) {
                        perror("shmat (semaphores)");
                        exit(1);
                    }
                }


                break;
            }

            case 'l':
            {
                sem_wait(&shared_semaphores->mutex);
                print_message_queue(messageQueue);
                sem_post(&shared_semaphores->mutex);

                break;
            }


            case 'q':
            {
                return 0;

            }

            default:
                std::cout << "Invalid input" << std::endl;
                break;


        }

    }while(input[0] != 'q');


    detach_shared_memory(messageQueue);

    detach_shared_memory(shared_semaphores);

    if (shmctl(queue_shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        return 1;
    }

    if (shmctl(semaphores_shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        return 1;
    }
    printf("Shared memory cleared successfully.\n");


    return 0;
}