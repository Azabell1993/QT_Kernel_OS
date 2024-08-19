// processmanager.h
#include "processmanager.h"

#include "kernel_lib.h"

int ProcessManager::createProcess(const std::string& name) {
    Process process;
    process.pid = next_pid++;
    process.name = name;
    process.running = true;
    processes[process.pid] = process;

    std::cout << "Created process: " << name << " (PID: " << process.pid << ")\n";
    return process.pid;
}

bool ProcessManager::killProcess(int pid) {
    auto it = processes.find(pid);
    if (it != processes.end() && it->second.running) {
        it->second.running = false;
        std::cout << "Killed process with PID: " << pid << "\n";
        return true;
    }
    std::cout << "No running process found with PID: " << pid << "\n";
    return false;
}

void ProcessManager::listProcesses() const {
    std::cout << "PID\tName\t\tStatus\n";
    std::cout << "---------------------------------\n";
    for (const auto& [pid, process] : processes) {
        std::cout << pid << "\t" << process.name << "\t\t"
                  << (process.running ? "Running" : "Stopped") << "\n";
    }
}
