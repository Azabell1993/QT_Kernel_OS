#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

    bool kernel_create_process(const char *process_name);

#ifdef __cplusplus
}
#endif

#endif
