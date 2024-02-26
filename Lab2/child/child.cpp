#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/prctl.h>
#include <string>
#include <vector>


void print_proc_info(const char* proc_name)
{
    std::cout << "Programm: " << proc_name << std::endl;
    pid_t childPid = getpid();
    pid_t childPpid = getppid();
    std::cout << "PID: " << childPid << " PPID: " << childPpid << std::endl;
    std::cout << " _____________________________"  << std::endl;
}

void print_env_first(const char filename[])
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        std::cerr << "Cannot open file\n" << std::endl;
        exit(0);
    }
    std::string line;

    std::cout << "Operation: [+]" << std::endl;
    while (std::getline(file, line)) {
        char* value = getenv(line.c_str());
        if (value != nullptr) {
            std::cout << line << "=" << value << std::endl;
        } else {
            std::cout << line << "= <Variable not found>" << std::endl;
        }
    }
    std::cout << " _____________________________"  << std::endl;
    std::cout << '\n';

    file.close();
}

void print_env_second(const char filename[], char* envp[])
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        std::cerr << "Cannot open file\n" << std::endl;
        exit(0);
    }

    std::cout << "Operation: [*]" << std::endl;

    std::string line;
    std::vector<std::string> env_vars;
    for(int i = 0; envp[i] != nullptr; ++i)
        env_vars.emplace_back(envp[i]);

    bool var;
    while (std::getline(file, line)){
        for(const auto& i : env_vars){
            if (i.find(line) != std::string::npos) {
                std::cout << i << std::endl;
                var = true;
                break;
            } else {
                var = false;
                continue;
            }
        }
        if(!var)
            std::cout << line << "= <Variable not found>" << std::endl;

    }


    file.close();

}


void print_env_third(const char filename[])
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        std::cerr << "Cannot open file\n" << std::endl;
        exit(0);
    }


    std::cout << "Operation: [&]" << std::endl;

    std::string line;
    std::vector<std::string> env_vars;
    for(int i = 0; environ[i] != nullptr; ++i)
        env_vars.emplace_back(environ[i]);

    bool var;
    while (std::getline(file, line)){
        for(const auto& i : env_vars){
            if (i.find(line) != std::string::npos) {
                std::cout << i << std::endl;
                var = true;
                break;
            } else {
                var = false;
                continue;
            }
        }
        if(!var)
            std::cout << line << "= <Variable not found>" << std::endl;

    }

    file.close();
}

int main(int argc, char* argv[], char* envp[]) {

    environ = envp;

    char new_proc_name[100];
    char q;

    sprintf(new_proc_name,"child_%s",argv[1]);
    argv[0] = new_proc_name;
    prctl(PR_SET_NAME, new_proc_name, 0, 0, 0);

    print_proc_info(new_proc_name);

    if(std::string(argv[2]) == "+"){
        print_env_first("../child/ENV_VARIABLES");
    }else if(std::string(argv[2]) == "*"){
        print_env_second("../child/ENV_VARIABLES", envp);
    }else if(std::string(argv[2]) == "&"){
        print_env_third("../child/ENV_VARIABLES");
    }

    std::cout << "To exit input: [q]" << std::endl;
    q = (char)std::cin.get();
    while(q != 'q'){
        std::cout << "Wrong input: should be [q]" << std::endl;
        q = (char)std::cin.get();
    }


    return 0;
}
