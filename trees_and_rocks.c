#include <stdio.h>
#include <stdlib.h>
#include "field_module.h"
#include "module1.h"
// Функция поиска дерева по координатам
struct Tree* find_tree(struct Field *field, int x, int y) {
    for (int i = 0; i < field->max_trees; i++) {
        if (field->trees[i].is_active && field->trees[i].x == x && field->trees[i].y == y) {
            return &field->trees[i];
        }
    }
    return NULL; // Дерево не найдено
}

// Функция добавления нового дерева
void add_tree(struct Field *field, int x, int y, char background) {
    if (field->tree_count >= field->max_trees) {
        printf("Error: Max trees limit reached!\n");
        return;
    }
    field->trees[field->tree_count].x = x;
    field->trees[field->tree_count].y = y;
    field->trees[field->tree_count].on = background;
    field->trees[field->tree_count].is_active = 1;
    field->tree_count++;
}


void deactivate_tree(struct Field *field, int x, int y) {
    struct Tree* tree = find_tree(field, x, y);
    if (tree) {
        tree->is_active = 0;

    }
}

void grow(struct Field *field, struct Dino *dino, char* dir) {
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
        printf("invalid argument of GROW direction\n");
        abort();
    }

    // Проверки: нельзя сажать дерево в гору, яму или под динозавром
    if (field->grid[target_x][target_y] == '^') {
        printf("Cannot plant a tree into a mountain at (%d, %d)!\n", target_x, target_y);
        return;
    }
    if (field->grid[target_x][target_y] == '%') {
        printf("Cannot plant a tree into a pit at (%d, %d)!\n", target_x, target_y);
        return;
    }
    if (target_x == dino->x && target_y == dino->y) {
        printf("Cannot plant tree under the dino!\n");
        return;
    }
    if (field->grid[target_x][target_y] == '&') {
        printf("A tree already exists at (%d, %d)!\n", target_x, target_y);
        return;
    }

    // Получаем текущий фон перед созданием дерева
    char background_before_tree = field->grid[target_x][target_y];

    // Добавляем дерево в наш массив структур
    add_tree(field, target_x, target_y, background_before_tree);

    // Обновляем отображение на поле
    field->grid[target_x][target_y] = '&';
}

void cut(struct Field *field, struct Dino *dino, char* dir) {
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
        printf("invalid argument of CUT direction\n");
        abort();
    }

    // --- ЛОГИКА УДАЛЕНИЯ ДЕРЕВА И ВОССТАНОВЛЕНИЯ ФОНА ---
    struct Tree* existing_tree = find_tree(field, target_x, target_y);

    if (existing_tree != NULL) {


        // 1. Восстанавливаем оригинальный фон из структуры Tree
        field->grid[target_x][target_y] = existing_tree->on;

        // 2. Деактивируем дерево в массиве деревьев
        deactivate_tree(field, target_x, target_y);

        printf("Tree cut down at (%d, %d)! Original background restored.\n", target_x, target_y);

    } else {
        printf("There is no tree to cut at (%d, %d).\n", target_x, target_y);
    }
}
struct Rock * find_rock(struct Field *field, int x, int y) {
    for (int i = 0; i < field->max_rocks; i++) {
        if (field->rocks[i].is_active && field->rocks[i].x == x && field->rocks[i].y == y) {
            return &field->rocks[i];
        }
    }
    return NULL;
}


void add_rock(struct Field *field, int x, int y, char background) {
    if (field->rocks_count>= field->max_rocks) {
        printf("Error: Max rocks limit reached!\n");
        return;
    }
    field->rocks[field->rocks_count].x = x;
    field->rocks[field->rocks_count].y = y;
    field->rocks[field->rocks_count].on = background;
    field->rocks[field->rocks_count].is_active = 1;
    field->rocks_count++;
}


void deactivate_rock(struct Field *field, int x, int y) {
    struct Rock* rock = find_rock(field, x, y);
    if (rock) {
        rock->is_active = 0;

    }
}


void make(struct Field *field, struct Dino *dino, char* dir) {
    int target_x = dino->x;
    int target_y = dino->y;

    // 1. Определение координат цели с учетом тороидальности
    if (line_compare(dir, "LEFT") == 1) {
        target_y = (dino->y - 1 + field->height) % field->height;
    } else if (line_compare(dir, "RIGHT") == 1) {
        target_y = (dino->y + 1) % field->height;
    } else if (line_compare(dir, "UP") == 1) {
        target_x = (dino->x - 1 + field->width) % field->width;
    } else if (line_compare(dir, "DOWN") == 1) {
        target_x = (dino->x + 1) % field->width;
    } else {
        printf("invalid argument of MAKE direction\n");
        abort();
    }

    // 2. Проверки: нельзя размещать камень в препятствие
    char target_char = field->grid[target_x][target_y];

    if (target_char == '^' || target_char == '&' ||
        target_char == '%' || target_char == '@' ||
        (target_x == dino->x && target_y == dino->y))
    {
        printf("Cannot place rock at (%d, %d). Cell is occupied by an obstacle!\n", target_x, target_y);
        return;
    }


    add_rock(field, target_x, target_y,target_char);

    // 4. Обновляем отображение на поле
    field->grid[target_x][target_y] = '@';
    printf("Rock placed at (%d, %d).\n", target_x, target_y);
}

int push_rock(struct Field *field, int rock_x, int rock_y, int step_x, int step_y) {

    // Находим структуру камня, чтобы получить его оригинальный фон
    struct Rock* rock_to_move = find_rock(field, rock_x, rock_y);
    if (rock_to_move == NULL) {
        // Этого не должно произойти, если логика вызова корректна
        printf("Error: Rock structure not found at (%d, %d)!\n", rock_x, rock_y);
        return 1;
    }

    int target_x = (rock_x + step_x + field->width) % field->width;
    int target_y = (rock_y + step_y + field->height) % field->height;
    char target_char = field->grid[target_x][target_y];

    // Проверки препятствий (горы, деревья, другие камни, динозавр)
    if (target_char == '^' || target_char == '&' || target_char == '@' ) {
        printf("Rock is blocked by an obstacle at (%d, %d).\n", target_x, target_y);
        return 1; // Нельзя толкнуть
    }

    // Логика засыпания ямы
    if (target_char == '%') {
        struct PIT * pit = find_pit(field, target_x, target_y);
        if (pit != NULL) {
            // Восстанавливаем фон ямы (она исчезает)
            field->grid[target_x][target_y] = pit->on;
            deactivate_pit(field, target_x, target_y);

            // Удаляем камень из его текущей позиции, восстанавливая ЕГО фон
            field->grid[rock_x][rock_y] = rock_to_move->on;
            rock_to_move->is_active = 0; // Деактивируем камень

            printf("Rock fell into a pit and filled it at (%d, %d)!\n", target_x, target_y);
            return 0;
        }
    }

    // --- СТАНДАРТНОЕ ПЕРЕМЕЩЕНИЕ НА ПУСТУЮ КЛЕТКУ ---

    // 1. Очищаем старую позицию камня, восстанавливая его оригинальный фон
    field->grid[rock_x][rock_y] = rock_to_move->on;

    // 2. Фон новой клетки станет новым original_background для камня
    char background_at_target = field->grid[target_x][target_y];

    // 3. Перемещаем камень в новую позицию на поле
    field->grid[target_x][target_y] = '@';

    // 4. Обновляем координаты и фон в структуре Rock
    rock_to_move->x = target_x;
    rock_to_move->y = target_y;
    rock_to_move->on = background_at_target; // Обновляем фон!

    printf("Rock moved to (%d, %d).\n", target_x, target_y);
    return 0; // Камень успешно перемещен
}

void dino_push_rock(struct Field *field, char* dir, struct Dino *dino) {
    int step_x = 0;
    int step_y = 0;

    // 1. Определяем направление, куда динозавр пытается толкнуть (от себя)
    if (line_compare(dir, "LEFT") == 1) { step_y = -1; }
    else if (line_compare(dir, "RIGHT") == 1) { step_y = 1; }
    else if (line_compare(dir, "UP") == 1) { step_x = -1; }
    else if (line_compare(dir, "DOWN") == 1) { step_x = 1; }
    else {
        printf("invalid argument of PUSH direction\n");
        abort();
    }

    // 2. Определяем координаты камня (он должен быть ровно в 1 клетке от динозавра в этом направлении)
    int rock_x = (dino->x + step_x + field->width) % field->width;
    int rock_y = (dino->y + step_y + field->height) % field->height;

    // 3. Проверяем, действительно ли в этой клетке камень
    if (field->grid[rock_x][rock_y] != '@') {
        printf("There is no rock in that direction (%s) to push!\n", dir);
        return;
    }

    // 4. Направление движения камня СОВПАДАЕТ с step_x/step_y.
    int push_step_x = step_x;
    int push_step_y = step_y;

    // 5. Вызываем функцию push_rock для перемещения камня.
    // Функция push_rock обрабатывает всю логику перемещения камня, засыпание ям и блокировку.
    int success = push_rock(field, rock_x, rock_y, push_step_x, push_step_y);
    if (success==0) {
        printf("pushed rock succesfully\n");
    }

}



