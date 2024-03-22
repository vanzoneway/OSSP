#include <iostream>
#include <fcntl.h>
#include <vector>
#include "message.h"



sem_t* full;
sem_t* empty;
MessageQueue* messageQueue;
pthread_mutex_t mutex;
bool CLOSE_THREADS = false;


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

void* producer(void* arg) {

    srand(time(nullptr));

    while(true) {

        if(CLOSE_THREADS) { pthread_exit(nullptr); }


        sem_wait(empty);
        pthread_mutex_lock(&mutex);

        Message message = generate_message();
        add_message(messageQueue, message);
        printf("Producer: Generated message:\n ");
        print_message(message);

        pthread_mutex_unlock(&mutex);
        sem_post(full);


        sleep(5);

    }
 }

void* consumer(void* arg) {

    srand(time(nullptr));

    while(true) {

        if(CLOSE_THREADS) { pthread_exit(nullptr); }


        sem_wait(full);
        pthread_mutex_lock(&mutex);

        Message message = get_message(messageQueue);

        printf("Consumer: Got a message:\n");
        print_message(message);

        pthread_mutex_unlock(&mutex);
        sem_post(empty);


    }

}


int main() {
    std::string input;
    std::vector <pthread_t> threads;
    auto *thread = new pthread_t;

    sem_unlink("/empty");
    sem_unlink("/full");
    empty = sem_open("/empty", O_CREAT, 0777, 5);
    full = sem_open("/full", O_CREAT, 0777, 0);


    messageQueue = new MessageQueue ();
    initialize_queue(messageQueue);


    print_menu();
    std::cout << "Enter your choice: \n";
    do {
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch((char) input[0]) {
            case 'p':
            {
                if (pthread_create(thread, nullptr, producer, nullptr) != 0)
                {
                    std::cout << "Failed to create thread" << std::endl;
                    return 1;
                }

                threads.push_back(*thread);

                break;
            }

            case 'c':
            {

                if (pthread_create(thread, nullptr, consumer, nullptr) != 0)
                {
                    std::cout << "Failed to create thread" << std::endl;
                    return 1;
                }

                threads.push_back(*thread);

                break;
            }

            case 'l':
            {
                print_message_queue(messageQueue);
                break;
            }

            case '+':
            {
                int value;
                sem_post(empty);
                sem_getvalue(empty, &value);
                std::cout << "The size of queue of messages: " << value << std::endl;
                break;
            }

            case '-':
            {
                int value;
                if (sem_trywait(empty) == 0) {
                    sem_getvalue(empty, &value);
                    std::cout << "The size of queue of messages: " << value << std::endl;
                } else if (errno == EAGAIN) {
                    std::cout << "The size of queue of messages already 0 | Producers can generate messages, you should put + |" << std::endl;
                }

                break;
            }


            case 'q':
            {
                pthread_mutex_lock(&mutex);
                CLOSE_THREADS = true;
                for(pthread_t i : threads) {
                    pthread_cancel(i);
                    pthread_join(i, nullptr);
                }
                pthread_mutex_unlock(&mutex);

                return 0;

            }

            default:
                std::cout << "Invalid input" << std::endl;
                break;


        }

    }while(input[0] != 'q');

    sem_unlink("empty");
    sem_unlink("full");

    sem_close(empty);
    sem_close(full);

    pthread_mutex_destroy(&mutex);

    return 0;
}
