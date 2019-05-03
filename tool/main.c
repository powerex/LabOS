#include "../param.h"

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

// Надання параметрів для виконання розрахунку
void action(void)
{
// Створення файл можливе тільки якщо файлу немає
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        fclose(file);
        return;
    }
// Отримання значень параметрів від користувача
    float a, b, c, d;
    printf("a="); scanf("%f", &a);
    printf("b="); scanf("%f", &b);
    printf("c="); scanf("%f", &c);
    printf("d="); scanf("%f", &d);
// Зберігання параметрів розрахунку у файл
    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Can not open file for write.\n");
        return;
    }
    int success = fwrite(&a, sizeof(a), 1, file) == 1 &&
                  fwrite(&b, sizeof(b), 1, file) == 1 &&
                  fwrite(&c, sizeof(c), 1, file) == 1 &&
                  fwrite(&d, sizeof(d), 1, file) == 1;
    fclose(file);
    if (!success)
    {
        printf("Can not write a, b, c to file.\n");
        remove(filename);
    }
}
int main(void)
{
// Створення семафору
    sem_t *sem = sem_open(semaphore_name, O_CREAT);
    if (sem == NULL)
    {
        printf("Can not create semaphore.\n");
        return -1;
    }
// Підготовка до початку роботи
    if (sem_post(sem) == -1)
    {
        printf("Can not post semaphore.\n");
        sem_close(sem);
        return -1;
    }
    remove(filename);
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
        action();
// Дозволити іншим процесам працювати з файлом
        if (sem_post(sem) == -1)
        {
            printf("Can not post semaphore.\n");
            break;
        }
    }
// Закриття семафору
    sem_close(sem);
    return 0;
}