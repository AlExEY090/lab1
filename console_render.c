#include "field_module.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void print_Field(struct Field * field) {
    for (int i = 0; i < field->width; ++i) {
        for (int j = 0; j < field->height; ++j) {
            printf("%c", field->grid[i][j]);
        }
        printf("\n");
    }
}
void render_field(struct Field * field, unsigned long interval) {
    print_Field(field);
    usleep(interval*1000000);
    system("cls");

}