
#ifndef LAB3_OTHER_FUNCS_H
#define LAB3_OTHER_FUNCS_H

#include <iostream>
#include <map>
#include <iomanip>
#include <csignal>
#include <limits>
#include <sys/wait.h>
#include <vector>


void print_menu();
void print_map(const std::map<int, int>& my_map);
void kill_last_created_child_proc(int& number_of_last_child_proc, std::map<int, int>& child_pid_map);
std::vector<int> parse_string_in_switch_case(const std::string & str);

#endif //LAB3_OTHER_FUNCS_H
