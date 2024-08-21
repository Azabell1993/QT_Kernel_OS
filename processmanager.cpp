// processmanager.h
/*
 * ProcessManager
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#include "processmanager.h"
#include "kernel_lib.h"

/*
 * @brief 프로세스 생성 함수
 * @param name 생성할 프로세스의 이름
 * @return 생성된 프로세스의 PID (프로세스 ID)
 * @details 새로운 프로세스를 생성하고, 프로세스 목록에 추가합니다.
 */
int ProcessManager::createProcess(const std::string& name) {
    Process process;
    process.pid = next_pid++;
    process.name = name;
    process.running = true;
    processes[process.pid] = process;

    std::cout << "Created process: " << name << " (PID: " << process.pid << ")\n";
    return process.pid;
}

/*
 * @brief 프로세스 종료 함수
 * @param pid 종료할 프로세스의 PID
 * @return 성공적으로 종료된 경우 true, 그렇지 않으면 false
 * @details 주어진 PID를 가진 프로세스를 종료합니다. 프로세스가 실행 중이지 않은 경우 종료에 실패합니다.
 */
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

/*
 * @brief 프로세스 목록 출력 함수
 * @details 현재 관리 중인 모든 프로세스의 목록을 출력합니다. 각 프로세스의 PID, 이름, 상태(실행 중/중지됨)를 포함합니다.
 */
void ProcessManager::listProcesses() const {
    std::cout << "PID\tName\t\tStatus\n";
    std::cout << "---------------------------------\n";
    for (const auto& [pid, process] : processes) {
        std::cout << pid << "\t" << process.name << "\t\t"
                  << (process.running ? "Running" : "Stopped") << "\n";
    }
}
