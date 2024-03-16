
#include "other_funcs.h"


void kill_last_created_child_proc(int& number_of_last_child_proc, std::map<int, int>& child_pid_map) {

    int status;
    int child_pid;
    child_pid = child_pid_map[number_of_last_child_proc - 1];
    kill(child_pid, SIGTERM);
    waitpid(child_pid, &status, 0);
    child_pid_map.erase(--number_of_last_child_proc);
    if (WIFSIGNALED(status)) {
        std::cout << " Last child proc with PID " << child_pid << " ends with signal: : " << WTERMSIG(status) << std::endl;
    }
}

void print_map(const std::map<int, int>& my_map) {

    size_t key_width = 0;
    size_t value_width = 0;

    for (const auto& pair : my_map) {
        key_width = std::max(key_width, std::to_string(pair.first).length());
        value_width = std::max(value_width, std::to_string(pair.second).length());
    }

    std::cout << std::left << std::setw(key_width) << "Key"
              << " | "
              << std::left << std::setw(value_width) << "Value"
              << std::endl;

    std::cout << std::setfill('-') << std::setw(key_width + 3) << ""
              << "+"
              << std::setw(value_width + 3) << ""
              << std::setfill(' ')
              << std::endl;

    for (const auto& pair : my_map) {
        std::cout << std::left << std::setw(key_width) << pair.first
                  << " | "
                  << std::left << std::setw(value_width) << pair.second
                  << std::endl;
    }
}

void print_menu() {
    std::map<std::string, std::string> commands = {
            {"+", "Create a child process"},
            {"-", "Delete the last created child process"},
            {"l", "List parent and child processes"},
            {"k", "Delete all child processes"},
            {"s", "Disable statistics output for all child processes"},
            {"g", "Enable statistics output for all child processes"},
            {"s<num>", "Disable statistics output for specific child process"},
            {"g<num>", "Enable statistics output for specific child process"},
            {"p<num>", "Disable statistics output for all child processes and request statistics output from specific child process"},
            {"q", "Quit"}
    };

    std::cout << "Available commands:" << std::endl;
    for (auto& command : commands) {
        std::cout << std::left << std::setw(20) << command.first << command.second << std::endl;
    }


}
std::vector<int> parse_string_in_switch_case(const std::string & str) {
    std::vector<int> result_vector;
    std::string::size_type start = str.find('<');
    std::string::size_type end = str.find('>');
    if(start != std::string::npos && end != std::string::npos && end > start + 1) {
        std::string content = str.substr(start + 1, end - start - 1);
        std::istringstream  iss(content);
        std::string number;
        while(std::getline(iss, number, ',')) {
            int value = std::stoi(number);
            result_vector.push_back(value);
        }
    } else {
        std::cout << "There is no args in < >, action will be spread on all child procs" << std::endl;
    }
    return result_vector;

}
