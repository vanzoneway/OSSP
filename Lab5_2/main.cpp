#include <iostream>
#include <vector>
#include "message.h"



int curr_storage = 0;
int STORAGE_MAX = 10;
int STORAGE_MIN = 0;
int to_consume;
MessageQueue* messageQueue;
pthread_mutex_t mutex;
pthread_cond_t cond;
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

        pthread_mutex_lock(&mutex);

        if(curr_storage >= STORAGE_MAX) {
            pthread_cond_signal(&cond);
        }else {
            curr_storage++;
            Message message = generate_message();
            add_message(messageQueue, message);

            printf("Producer: Generated message:\n ");
            print_message(message);

        }
        pthread_mutex_unlock(&mutex);


        sleep(5);

    }
}

void* consumer(void* arg) {

    srand(time(nullptr));

    while(true) {

        if(CLOSE_THREADS) { pthread_exit(nullptr); }

        pthread_mutex_lock(&mutex);

        pthread_cond_wait(&cond, &mutex);

        to_consume = curr_storage - STORAGE_MIN;

        for(int i = 0; i < to_consume; i++) {
            Message message = get_message(messageQueue);

            printf("Consumer: Got a message:\n");
            print_message(message);
        }

        curr_storage -= to_consume;
        pthread_mutex_unlock(&mutex);

    }

}


int main() {
    std::string input;
    std::vector <pthread_t> threads;
    auto *thread = new pthread_t;

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
                STORAGE_MAX++;
                break;
            }

            case '-':
            {

                if (STORAGE_MAX > 0) {
                    STORAGE_MAX--;
                    std::cout << "The size of queue of messages: " << STORAGE_MAX<< std::endl;
                } else if (STORAGE_MAX == 0) {
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

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
