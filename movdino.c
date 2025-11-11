#include <stdio.h>
#include <stdlib.h>
#include "field_module.h"
#include "module1.h"
#include <string.h>
//обработка комментариев
int remove_comments_from_file(const char *filename) {
    FILE *fin, *fout;

    char line[1024];
    char *comment_start;

    const char *temp_filename = "output.tmp";


    fin = fopen(filename, "r");
    if (fin == NULL) {
        perror("error during opening input file");
        return 1;
    }


    fout = fopen(temp_filename, "w");
    if (fout == NULL) {
        perror("error during opening output file");
        fclose(fin);
        return 1;
    }


    while (fgets(line, sizeof(line), fin) != NULL) {

        comment_start = strstr(line, "//");


        if (comment_start != NULL) {
            *comment_start = '\0';
        }

        fputs(line, fout);
    }


    fclose(fin);
    fclose(fout);


    if (remove(filename) != 0) {
        perror("error during deleting original file");
        return 1;
    }



    if (rename(temp_filename, filename) != 0) {
        perror("error during file renaming\n");
        return 1;
    }

    return 0;
}


int main(int argc,char *argv[]) {
    if (argc<2) {
        printf("not enough arguments to proceed");
        return 1;
    }
    unsigned long interval=1;
    int display=1;
    int save=1;
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "interval") == 0) {
            if (i + 1 < argc) {

                interval = strtol(argv[i+1], NULL, 10);

                i++;
            } else {

                fprintf(stderr, "no value after interval.\n");

            }
        }
        else if (strcmp(argv[i], "no-display") == 0) {
            display = 0;
        }
        else if (strcmp(argv[i], "no-save") == 0) {
            save = 0;
        }
    }

    if (remove_comments_from_file(argv[1]) == 0) {
        printf("comments deleted successful\n");
    } else {
        printf("error during deleting comments\n");
    }
    FILE *inputFile= fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("error during opening input file\n");
        return 1;
    }
    if (save==1) {//проверяем только если есть необходимость сохранять
        FILE *outputFile = fopen(argv[2], "w");
        if (outputFile == NULL) {
            perror("error during opening output file\n");
            fclose(inputFile);
            return 1;
        }
    }

    struct Field field;
    struct Dino dino;
    int start=0;
    char line[1024];
    fgets(line, sizeof(line), inputFile);
    char* line1= &line[0];
    char **lines = line_splitter(line1);
    if (line_compare(lines[0],"SIZE")==1){
        init_field(&field,atoi(lines[1]),atoi(lines[2]));
    }
    else {
        printf("NO SIZE TO START\n");
        return 0;
    }
    while (fgets(line, sizeof(line), inputFile)) {
        //printf("%s",line);
        strcat(line,"");
        char* linep= &line[0];
        char **lines1 = line_splitter(linep);

        if (strcmp(lines1[0],"START")==0){
            if ((atoi(lines1[1]) >= field.width) || (atoi(lines1[1]) <= 0) || (atoi(lines1[2]) >= field.height) || atoi(lines1[2]) <= 0) {
                printf("coordinates dont match field size\n");
                return 0;
            }
            if (start==1) {
                printf("using START command more than 1 time if forbidden\n");
                abort();
            }
            init_dino(&dino,atoi(lines1[1]),atoi(lines1[2]),'_');
            field.grid[dino.x][dino.y]='#';
            start=1;

        }
        else if(strcmp(lines1[0],"MOVE")==0) {
            char tmp[1024];
            strcpy(tmp,lines1[1]);
            char* dir=&tmp[0];
            //printf("%s",tmp);
            dino_move(&field,dir,&dino);

        }
        else if (strcmp(lines1[0],"PAINT")==0) {
            paint_square(&field,&dino,lines1[1]);
        }
        else if (strcmp(lines1[0],"MOUND")==0) {
            mountain(&field,&dino,lines1[1]);
        }
        else if (strcmp(lines1[0],"DIG")==0) {
            dig(&field,&dino,lines1[1]);
        }
        else if(strcmp(lines1[0],"JUMP")==0) {
            dino_jump(&field,lines1[1],&dino,atoi(lines1[2]));
        }
        else if(strcmp(lines1[0],"SIZE")==0) {
            printf("using SIZE command more than 1 time if forbidden\n");
            abort();
        }
        else if (strcmp(lines1[0],"GROW")==0) {
            grow(&field,&dino,lines1[1]);
        }
        else if (strcmp(lines1[0],"CUT")==0) {
            cut(&field,&dino,lines1[1]);
        }
        else if (strcmp(lines1[0],"MAKE")==0) {
            make(&field,&dino,lines1[1]);
        }
        else if (strcmp(lines1[0],"PUSH")==0) {
            dino_push_rock(&field,lines1[1],&dino);
        }
        if (display==1) {
            render_field(&field,interval);
        }

    }
    if (save==1) {
        write_output(&field,argv[2]);
    }
    clean_field(&field);
    fclose(inputFile);

    return 0;
}