//
// Created by 1alex on 19.10.2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "field_module.h"
#include "module1.h"
#include <string.h>

char  check_square(struct Field * field,int x,int y) {
    return field->grid[x][y];
}

int init_field(struct Field *field, int w, int h) {
    field->width = w;
    field->height = h;
    field->max_pits=w*h;
    field->pits = malloc(field->max_pits * sizeof(struct PIT));
    field->act_pits=0;
    field->max_trees=w*h;
    field->trees = malloc(field->max_trees * sizeof(struct Tree));
    field->tree_count=0;
    field->max_rocks=w*h;
    field->rocks = malloc(field->max_rocks * sizeof(struct Rock));
    field->rocks_count=0;
    char **rows = calloc(w, sizeof(char*));
    if (rows == NULL) {
        return 1;
    }
    for (int i = 0; i < w; ++i) {
        rows[i] = calloc(h, sizeof(char));
        if (rows[i] == NULL) {
            return 1;
        }
    }

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            rows[i][j] = '_';
        }
    }
    field->grid = rows;
    return 0;
}
void init_dino(struct Dino * dino, int x, int y,char on) {
    dino->x = x;
    dino->y = y;
    dino->on=on;
}
void dino_move(struct Field *field, char* dir, struct Dino *dino) {
    int step_x = 0;
    int step_y = 0;


    if (line_compare(dir, "LEFT") == 1) { step_y = -1; }
    else if (line_compare(dir, "RIGHT") == 1) { step_y = 1; }
    else if (line_compare(dir, "UP") == 1) { step_x = -1; }
    else if (line_compare(dir, "DOWN") == 1) { step_x = 1; }
    else {
        printf("invalid argument of MOVE\n");
        abort();
    }


    int next_x = (dino->x + step_x + field->width) % field->width;
    int next_y = (dino->y + step_y + field->height) % field->height;
    char next_char = field->grid[next_x][next_y];




    if (next_char == '^' || next_char == '&' ||next_char == '@') {
        printf("Dino blocked by obstacle (%c) at (%d, %d).\n", next_char, next_x, next_y);
        return;
    }




    field->grid[dino->x][dino->y] = dino->on;


    init_dino(dino, next_x, next_y, field->grid[next_x][next_y]);


    field->grid[dino->x][dino->y] = '#';


    if (dino->on == '%') {
        printf("FATAL: Dino fell into a pit at (%d, %d)! Game Over.\n", dino->x, dino->y);
        abort();
    }
}
#if 0//перемещен в console_render.c
void print_Field(struct Field * field) {
    for (int i = 0; i < field->width; ++i) {
        for (int j = 0; j < field->height; ++j) {
            printf("%c", field->grid[i][j]);
        }
        printf("\n");
    }
}
#endif


void clean_field(struct Field * field) {

    int height= field->height;
    for (int i =0;i<height;i++) {
        free(field->grid[i]);
    }
    free(field->grid);
    free(field->pits);
    free(field->trees);
    free(field->rocks);
}
void paint_square(struct Field *field ,struct Dino *dino,  char* sym) {
    if (((int)(*sym)<=122)&&((int)(*sym)>=97)) {
        dino->on=*sym;
    }
    else {
        printf("invalid symbol for painting\n");
        abort();
    }

}
void dig(struct Field *field, struct Dino *dino,char* dir) {
    int target_x = dino->x;
    int target_y = dino->y;


    if (line_compare(dir, "LEFT") == 1) {
        target_y = (dino->y - 1 + field->height) % field->height;
    } else if (line_compare(dir, "RIGHT") == 1) {
        target_y = (dino->y + 1) % field->height;
    } else if (line_compare(dir, "UP") == 1) {
        target_x = (dino->x - 1 + field->width) % field->width;
    } else if (line_compare(dir, "DOWN") == 1) {
        target_x = (dino->x + 1) % field->width;
    } else {
        printf("invalid argument of DIG direction\n");
        abort();
    }


    if (field->grid[target_x][target_y] =='^') {
        printf("Cannot dig a pit into a mountain at (%d, %d)!\n", target_x, target_y);
        return;
    }
    if (target_x == dino->x && target_y == dino->y) {
        printf("Cannot dig pit under the dino!\n");
        return;
    }
    if (field->grid[target_x][target_y] == '%') {
        printf("A pit already exists at (%d, %d)!\n", target_x, target_y);
        return;
    }


    char background_before_pit = field->grid[target_x][target_y];


    add_pit(field, target_x, target_y, background_before_pit);


    field->grid[target_x][target_y] = '%';
}

void dino_jump(struct Field *field, char* dir, struct Dino *dino, int n) {
    if (n < 1) {
        printf("Jump distance n must be at least 1\n");
        return;
    }

    int step_x = 0;
    int step_y = 0;


    if (line_compare(dir, "LEFT") == 1) {
        step_y = -1;
    } else if (line_compare(dir, "RIGHT") == 1) {
        step_y = 1;
    } else if (line_compare(dir, "UP") == 1) {
        step_x = -1;
    } else if (line_compare(dir, "DOWN") == 1) {
        step_x = 1;
    } else {
        printf("invalid argument of JUMP direction\n");
        abort();
    }


    for (int i = 0; i < n; i++) {

        int next_x = (dino->x + step_x + field->width) % field->width;
        int next_y = (dino->y + step_y + field->height) % field->height;


        if (field->grid[next_x][next_y] == '^') {
            printf("Warning: Dino stopped before mountain at (%d, %d)!\n", next_x, next_y);

            break;
        }
        if (field->grid[next_x][next_y] == '&') {
            printf("Warning: Dino stopped before tree at (%d, %d)!\n", next_x, next_y);

            break;
        }

        field->grid[dino->x][dino->y] = dino->on;


        init_dino(dino, next_x, next_y, field->grid[next_x][next_y]);


        field->grid[dino->x][dino->y] = '#';
    }


    if (dino->on == '%') {
        printf("FATAL: Dino fell into a hole at (%d, %d)! Game Over.\n", dino->x, dino->y);
        abort();
    }
}





struct PIT* find_pit(struct Field *field, int x, int y) {
    for (int i = 0; i < field->max_pits; i++) {
        if (field->pits[i].is_active && field->pits[i].x == x && field->pits[i].y == y) {
            return &field->pits[i];
        }
    }
    return NULL; // Яма не найдена
}
void mountain(struct Field *field, struct Dino *dino,char* dir) {
    int target_x = dino->x;
    int target_y = dino->y;

    // Логика определения target_x/y по направлению: LEFT, RIGHT, UP, DOWN
    if (line_compare(dir, "LEFT") == 1) {
        target_y = (dino->y - 1 + field->height) % field->height;
    } else if (line_compare(dir, "RIGHT") == 1) {
        target_y = (dino->y + 1) % field->height;
    } else if (line_compare(dir, "UP") == 1) {
        target_x = (dino->x - 1 + field->width) % field->width;
    } else if (line_compare(dir, "DOWN") == 1) {
        target_x = (dino->x + 1) % field->width;
    } else {
        printf("invalid argument of MOUNTAIN direction\n");
        abort();
    }

    // --- ГЛАВНАЯ ПРОВЕРКА И ЛОГИКА ВОССТАНОВЛЕНИЯ ФОНА ЯМЫ ---
    struct PIT * existing_pit = find_pit(field, target_x, target_y); // Поиск ямы

    if (existing_pit != NULL) {
        // Мы нашли активную яму в целевой клетке!

        // 1. Восстанавливаем оригинальный фон из структуры Pit
        field->grid[target_x][target_y] = existing_pit->on;

        // 2. Деактивируем яму в массиве ям
        deactivate_pit(field, target_x, target_y);

        printf("Mountain filled the pit at (%d, %d)! Original background restored.\n", target_x, target_y);

    } else {
        // Дополнительная проверка: не пытаемся ли мы создать гору поверх динозавра
        if (target_x == dino->x && target_y == dino->y) {
            printf("Cannot place mountain on the dino's location!\n");
            return;
        }
        field->grid[target_x][target_y] ='^';
    }

    // Устанавливаем символ горы в вычисленной соседней клетке (либо на пустом месте, либо поверх восстановленного фона ямы)

}
// Функция добавления новой ямы
void add_pit(struct Field *field, int x, int y, char background) {
    if (field->act_pits >= field->max_pits) {
        printf("Error: Max holes limit reached!\n");
        return;
    }
    field->pits[field->act_pits].x = x;
    field->pits[field->act_pits].y = y;
    field->pits[field->act_pits].on = background;
    field->pits[field->act_pits].is_active = 1;
    field->act_pits++;
}

// Функция "закапывания" (удаления) ямы
void deactivate_pit(struct Field *field, int x, int y) {
    struct PIT* hole = find_pit(field, x, y);
    if (hole) {
        hole->is_active = 0;

    }
}
int write_output(const struct Field *field, const char *filename) {
    FILE *file;
    int i, j;


    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }


    for (i = 0; i < field->width; i++) {
        for (j = 0; j < field->height; j++) {
            fprintf(file, "%c", field->grid[i][j]);
        }
        fprintf(file, "\n"); // Переход на новую строку
    }


    fclose(file);

    return 0;
}


