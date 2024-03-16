#include "other_funcs.h"


#define MAX_COUNTER_ALARMS 10

using namespace std;

// FOR CHILD PROC NEEDED
struct Data {
    int a;
    int b;
};

struct Results {
    int zz;
    int zo;
    int oz;
    int oo;
};

static Data child_data = { 0, 0 };
static int ALARMS_COUNTER = 0;
static Results child_results = {0, 0, 0, 0};
bool ABILITY_TO_SHOW_STAT = true;

void put_data_int_results() {

    if (child_data.a == 0 && child_data.b == 0) {
        child_results.zz++;
    } else if (child_data.a == 0 && child_data.b == 1) {
        child_results.oz++;
    } else if (child_data.a == 1 && child_data.b == 0) {
        child_results.zo++;
    } else {
        child_results.zz++;
    }
}

void alarm_handler(int signum) {
    put_data_int_results();
    if(ABILITY_TO_SHOW_STAT || ALARMS_COUNTER == MAX_COUNTER_ALARMS) {

        if(ALARMS_COUNTER == MAX_COUNTER_ALARMS) {
            cout << "PID: " << getpid()
                 << ", PPID: " << getppid()
                 << ", Vector: "
                 << child_results.oo << ", "
                 << child_results.oz << ", "
                 << child_results.zo << ", "
                 << child_results.zz
                 << endl;
            ALARMS_COUNTER = 0;
        }
        ALARMS_COUNTER++;
    }

    alarm(1);
}

void stat_handler(int signum) {
    if(signum == SIGUSR1) {
        ABILITY_TO_SHOW_STAT = false;
        ALARMS_COUNTER = MAX_COUNTER_ALARMS;
    }
    if(signum == SIGUSR2) {
        ABILITY_TO_SHOW_STAT = true;

    }
}

//ENDS FOR CHILD NEEDED

//FOR PARENT NEEDED
vector<int> parent_global_vector;
map<int,int> child_global_pid_map;
void parent_alarm_handler (int signum) {
    if(parent_global_vector.empty()) {
        for (auto & it : child_global_pid_map) {
            kill(it.second, SIGUSR2);

            cout << " Allows to show stat in child proc with PID " << it.second << " [C_" << it.first << "]"<< endl;

        }
    } else {
        for(int i : parent_global_vector) {
            kill(child_global_pid_map[i], SIGUSR2);
            cout << " Allows to show stat in child proc with PID " << child_global_pid_map[i] << " [C_" << i << "]" << endl;
        }
    }
    child_global_pid_map.clear();
}

int main() {
    string input;
    map<int, int> child_pid_map;
    int number_of_last_child_proc = 1;

    print_menu();
    cout << "Enter your choice: \n";
    do {

        cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch ((char) input[0]) {
            case '+': {
                pid_t child_pid = fork();

                if (child_pid == 0) {

                    signal(SIGALRM, alarm_handler);
                    signal(SIGUSR1, stat_handler);
                    signal(SIGUSR2, stat_handler);

                    alarm(1);
                    while (true) {
                        child_data = {0, 0};
                        child_data = {1, 1};
                    }

                } else if (child_pid > 0) {
                    child_pid_map.insert(make_pair(number_of_last_child_proc++, child_pid));
                } else {
                    cout << "Unable to create a daughter process" << endl;
                }

                break;
            }
            case '-':
            {
                kill_last_created_child_proc(number_of_last_child_proc, child_pid_map);
                break;
            }
            case 'l':
            {
                print_map(child_pid_map);
                break;
            }
            case 'k':
            {
                while(number_of_last_child_proc > 1) {
                    kill_last_created_child_proc(number_of_last_child_proc, child_pid_map);
                }
                break;
            }
            case 's':
            {
                std::vector<int> p = parse_string_in_switch_case(input);
                if(p.empty()) {
                    for (auto & it : child_pid_map) {
                        kill(it.second, SIGUSR1);

                        cout << " Blocks to show stat in child proc with PID " << it.second << " [C_" << it.second << "]" << endl;

                    }
                } else {
                    for(int i : p) {
                        kill(child_pid_map[i], SIGUSR1);
                        cout << " Blocks to show stat in child proc with PID " << child_pid_map[i] << " [C_" << i << "]" << endl;
                    }
                }

                break;
            }

            case 'g':
            {
                std::vector<int> p = parse_string_in_switch_case(input);
                if(p.empty()) {
                    for (auto & it : child_pid_map) {
                        kill(it.second, SIGUSR2);

                        cout << " Allows to show stat in child proc with PID " << it.second << " [C_" << it.first << "]"<< endl;

                    }
                } else {
                    for(int i : p) {
                        kill(child_pid_map[i], SIGUSR2);
                        cout << " Allows to show stat in child proc with PID " << child_pid_map[i] << " [C_" << i << "]" << endl;
                    }
                }

                break;
            }

            case 'p':
            {
                parent_global_vector = parse_string_in_switch_case(input);
                child_global_pid_map = child_pid_map;
                if(parent_global_vector.empty()) {
                    for (auto & it : child_pid_map) {
                        kill(it.second, SIGUSR1);

                        cout << " Blocks to show stat in child proc with PID " << it.second << " [C_" << it.first << "]" << endl;

                    }
                } else {
                    for(int i : parent_global_vector) {
                        kill(child_pid_map[i], SIGUSR1);
                        cout << " Blocks to show stat in child proc with PID " << child_pid_map[i] << " [C_" << i << "]" << endl;
                    }
                }

                signal(SIGALRM, parent_alarm_handler);
                alarm(30);



                break;

            }

            case 'q':
                return 0;

            default:
                cout << "Invalid input" << endl;
                break;
        }

    } while(input[0] != 'q');


    return 0;
}

