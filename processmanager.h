// processmanager.h
#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unistd.h>

// C 언어로 작성된 kernel_create_process 함수 선언을 위한 extern "C" 블록
extern "C" {
    bool kernel_create_process(const char *process_name);
    void kernel_list_processes();
    bool kernel_kill_process(const char *process_name);
}

struct Process {
    int pid;
    std::string name;
    bool running;
};

class ProcessManager {
public:
    ProcessManager() : next_pid(1) {}

    int createProcess(const std::string& name);
    bool killProcess(int pid);
    void listProcesses() const;

private:
    int next_pid;
    std::unordered_map<int, Process> processes;
};

#endif // PROCESSMANAGER_H
