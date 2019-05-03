#ifndef LABOS_PARAM_H
#define LABOS_PARAM_H

#if defined(__linux__) // any linux distribution
#define PLATFORM "linux"
#include <sys/sysinfo.h>
#elif defined(_WIN32) // any windows system
#define PLATFORM "windows"
    #include <Windows.h>
#else
    #define PLATFORM "Is not linux or windows"
#endif

#define ever ;;
char* filename = "data.bin";
char* semaphore_name = "semaphore";
#define false 0
#define true 1
#define bool _Bool

#endif //LABOS_PARAM_H
