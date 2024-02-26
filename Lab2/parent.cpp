#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>


using namespace std;


void print_menu()
{
    cout << "|+: - 1st option |" << endl;
    cout << "|*: - 1st option |" << endl;
    cout << "|&: - 1st option |" << endl;
    cout << "|e: - exit       |" << endl;
}

void check_daughter_proc_status(const int & status)
{
    if (WIFEXITED(status)) {
        int exitStatus = WEXITSTATUS(status);
        std::cout << "Child proc done with code: " << exitStatus << std::endl;
        std::cout << " _____________________________"  << std::endl;
    } else if (WIFSIGNALED(status)) {
        int signalNumber = WTERMSIG(status);
        std::cout << "Child proc ends cause of signal: " << signalNumber << std::endl;
        std::cout << " _____________________________"  << std::endl;
    }
}

void task()
{
    setlocale(LC_COLLATE, "C");
    int child_counter = 0;
    int ch;


    while((ch = cin.get()) != (int)'e')
    {

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(ch)
        {
            case '+':
            {
                child_counter++;
                cout << "Opt +" << endl;
                pid_t pid = fork();
                int status;
                wait(&status);
                check_daughter_proc_status(status);
                if (pid == 0) {
                    char* argv[] = { getenv("CHILD_PATH"), const_cast<char*>(to_string(child_counter).c_str()), "+", nullptr };
                    int result = execve("child", argv, environ);
                    if (result == -1) {
                        std::cerr << "execve: " << strerror(errno) << std::endl;
                        exit(1);
                    }
                }

                break;
            }

            case '*':
            {
                child_counter++;
                cout << "Opt *" << endl;
                pid_t pid = fork();
                int status;
                wait(&status);
                check_daughter_proc_status(status);
                if (pid == 0) {
                    char* argv[] = { getenv("CHILD_PATH"), const_cast<char*>(to_string(child_counter).c_str()), "*", nullptr };
                    int result = execve("child", argv, environ);
                    if (result == -1) {
                        std::cerr << "execve: " << strerror(errno) << std::endl;
                        exit(1);
                    }
                }

                break;
            }

            case '&':
            {
                child_counter++;
                cout << "Opt &" << endl;
                pid_t pid = fork();
                int status;
                wait(&status);
                check_daughter_proc_status(status);
                if (pid == 0) {
                    char* argv[] = { getenv("CHILD_PATH"), const_cast<char*>(to_string(child_counter).c_str()), "&", nullptr };
                    int result = execve("child", argv, environ);
                    if (result == -1) {
                        std::cerr << "execve: " << strerror(errno) << std::endl;
                        exit(1);
                    }
                }

                break;
            }

            default:
                cout << "Wrong option! Try again!" << endl;
                break;
        }

    }
}

int main() {
    setlocale(LC_COLLATE, "C");
    int child_counter = 0;
    int ch;

    vector<string> env_vars;
    for(int i = 0; environ[i] != nullptr; ++i)
        env_vars.emplace_back(environ[i]);
    sort(env_vars.begin(), env_vars.end());
    for(const auto& i : env_vars)
        cout << i << endl;

    print_menu();

    task();



    return 0;
}
