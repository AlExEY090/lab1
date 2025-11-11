//
// Created by 1alex on 09.10.2025.
//
#include "module1.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "field_module.h"

int space_counter(const char* line) {
    int spc=0;

    const char *line1 = line;

    while (*line1!='\0'){
        if (*line1==' ') {
            spc++;
        }
        line1++;
    }
    return spc;
}
char** line_splitter(const char* line) {
    int spc=space_counter(line);
    char **substrings = (char **) malloc((spc + 1) * sizeof(char *));

    for (int i = 0; i < spc + 1; i++) {

        substrings[i] = (char*) calloc(1024, sizeof(char));
    }

    int j = 0; // Индекс текущей подстроки в массиве substrings
    int k = 0; // Индекс текущего символа внутри подстроки

    while (*line != '\0') {
        if (*line == ' ') {
            substrings[j][k] = '\0'; // Завершаем текущую подстроку корректно
            j++;                     // Переходим к следующей подстроке
            k = 0;                   // Сбрасываем индекс символа
        } else {
            substrings[j][k] = *line; // Копируем символ
            k++;                      // Увеличиваем индекс для следующего символа
        }
        line++;
    }

    // ОЧЕНЬ ВАЖНО: Завершаем последнюю подстроку после выхода из цикла
    substrings[j][k] = '\0';




    return substrings;
}
int line_compare(const char* line1, const char* line2) {
    int k=0;
    while (*line2!='\0'){
        if (*line1==*line2) {
            line1++;
            line2++;
        }
        else {
            k=0;
            return k;
        }
    }
    k=1;
    return k;
}






