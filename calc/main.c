#include "../param.h"
#include <stdio.h>
#include <float.h>

#if defined(__linux__)
    #include <stdlib.h>
    #include <semaphore.h>
    #include <fcntl.h>
    #include <sys/time.h>

    double time_diff(struct timeval x , struct timeval y)
    {
        double x_ms , y_ms , diff;
        x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
        y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
        diff = y_ms - x_ms;
        return diff/1000;
    }
#endif

#if defined(_WIN32)
    #include <windows.h>
    #include <afxres.h>
    #include<time.h>
#include <math.h>
#endif

// Отримання параметрів та виконання розрахунку
double action(void)
{
    double res;
// Відкриття файлу
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
// Завантаження значень параметрів розрахунку з файлу
        float a, b, c, d;
        int success = fread(&a, sizeof(a), 1, file) == 1 &&
                      fread(&b, sizeof(b), 1, file) == 1 &&
                      fread(&c, sizeof(c), 1, file) == 1 &&
                      fread(&d, sizeof(d), 1, file) == 1;
        fclose(file);
        remove(filename);
// Виконання розрахунку
        if (success) {
            res = logf(sin(a-b+c*d));
            printf("(a+b)/(c+d)=(%f+%f)/(%f+%f)=%f\n", a, b, c, d, res);
        }
        else
            printf("Can not read a, b, c from file.\n");
        fflush(stdout);
    }
    return res;
}

int main(void)
{
    double min_time = DBL_MAX;
// Створення семафору
#if defined(__linux__)
    sem_t *sem = sem_open(semaphore_name, O_CREAT);
#endif
#if defined(_WIN32)
    HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, semaphore_name);
#endif
    if (sem == NULL)
    {
        printf("Can not create semaphore.\n");
        return -1;
    }
// Нескінченний цикл
    for(ever)
    {
// Отримати дозвіл на запис у файл
#if defined(__linux__)
        if (sem_wait(sem) == -1)
#endif
#if defined(_WIN32)
        if (WaitForSingleObject(sem, INFINITE) != WAIT_OBJECT_0)
#endif
        {
            printf("Can not wait semaphore.\n");
            break;
        }
// Виконати дію над файлом
        double x = action();
// Дозволити іншим процесам працювати з файлом
#if defined(__linux__)
        if (sem_post(sem) == -1)
#endif
#if defined(_WIN32)
        if (ReleaseSemaphore(sem, 1, NULL) == 0)
#endif
        {
            printf("Can not post semaphore.\n");
            break;
        }
        printf("Reduced data = %f", x/2);
    }
// Закриття семафору
#if defined(__linux__)
    sem_close(sem);
#endif
#if defined(_WIN32)
    CloseHandle(sem);
#endif
    return 0;
}