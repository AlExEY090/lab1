//
// Created by 1alex on 19.10.2025.
//

#ifndef LAB1_FIELD_MODULE_H
#define LAB1_FIELD_MODULE_H

struct Dino {
    int x;
    int y;
    char on;

};
struct Field {
    char **grid;
    int width;
    int height;
    struct PIT *pits;
    int max_pits;
    int act_pits;
    struct Tree *trees;
    int max_trees;
    int tree_count;
    struct Rock *rocks;
    int max_rocks;
    int rocks_count;

};
struct PIT {
    int x;
    int y;
    char on;
    int is_active;

};
struct Tree {
    int x;
    int y;
    char on;
    int is_active;

};
struct Rock {
    int x;
    int y;
    char on;
    int is_active;

};
void init_dino(struct Dino *, int x, int y,char on);
int init_field(struct Field *, int, int);
void clean_field(struct Field *);
void print_Field(struct Field*);
void dino_move(struct Field*,char* dir,struct Dino *);
void paint_square(struct Field*,struct Dino*,char* sym);
void dig(struct Field * field,struct Dino * dino,char* dir);
void mountain(struct Field * field,struct Dino * dino,char* dir);
char  check_square(struct Field * field,int x,int y);
struct PIT * find_pit(struct Field *field, int x, int y);
void add_pit(struct Field *field, int x, int y, char background);
void deactivate_pit(struct Field *field, int x, int y);
void dino_jump(struct Field *field, char* dir, struct Dino *dino, int n);
int write_output(const struct Field * field,const char * filename);
void render_field(struct Field * field, unsigned long interval);
struct Tree * find_tree(struct Field *field, int x, int y);
void add_tree(struct Field *field, int x, int y, char background);
void deactivate_tree(struct Field *field, int x, int y);
void grow(struct Field * field,struct Dino * dino,char* dir);
void cut(struct Field * field,struct Dino * dino,char* dir);
struct Rock * find_rock(struct Field *field, int x, int y);
void add_rock(struct Field *field, int x, int y, char background);
void deactivate_rock(struct Field *field, int x, int y);
void make(struct Field * field,struct Dino * dino,char* dir);
int push_rock(struct Field *field, int rock_x, int rock_y, int step_x, int step_y);
void dino_push_rock(struct Field *field, char* dir, struct Dino *dino);

#endif //LAB1_FIELD_MODULE_H