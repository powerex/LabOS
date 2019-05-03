#include "../param.h"

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <float.h>
#include <sys/time.h>

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
    diff = y_ms - x_ms;
    return diff/1000;
}

// Отримання параметрів та виконання розрахунку
void action(void)
{
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
        if (success)
            printf("(a+b)/(c+d)=(%f+%f)/(%f+%f)=%f\n", a, b, c, d, (a + b) / (c + d));
        else
            printf("Can not read a, b, c from file.\n");
        fflush(stdout);
    }
}

int main(void)
{
    double min_time = DBL_MAX;
// Створення семафору
    sem_t *sem = sem_open(semaphore_name, O_CREAT);
    if (sem == NULL)
    {
        printf("Can not create semaphore.\n");
        return -1;
    }
// Нескінченний цикл
    for(ever)
    {
// Отримати дозвіл на запис у файл
        if (sem_wait(sem) == -1)
        {
            printf("Can not wait semaphore.\n");
            break;
        }
// Виконати дію над файлом
        struct timeval before , after;
        gettimeofday(&before , NULL);
        action();
        gettimeofday(&after , NULL);
// Дозволити іншим процесам працювати з файлом
        if (sem_post(sem) == -1)
        {
            printf("Can not post semaphore.\n");
            break;
        }
        double time_spent = time_diff(before, after);
        if (time_spent < min_time)
            min_time = time_spent;
        printf("Current time elpased is %f seconds\nMinimum time elapsed is %f seconds\n", time_spent, min_time);
    }
// Закриття семафору
    sem_close(sem);
    return 0;
}