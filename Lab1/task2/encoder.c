#include <stdio.h>
#include <string.h>

void oneA() {
    int cur;
    FILE* output = stdout;
    while((cur = fgetc(stdin)) != EOF) {
        (cur > 64 && cur < 91) ? printf(".") : fputc(cur, output);
    }   
}

void oneB(FILE* input, FILE* output) {
    int cur;
    int dot = 46;
    int counter = 0;
    
    while((cur = fgetc(input)) != EOF) {
        if (cur > 64 && cur < 91) {
            fprintf(output, "%d %d\n", cur, dot);
            printf(".");
            counter++;
        }
        else {
            if (cur != 10) {
                fprintf(output, "%d %d\n", cur, cur); 
                fputc(cur, stdout);
            }
            else {
                fprintf(output, "the number of letters: %d\n", counter);
                counter = 0;
                printf("\n");
            }
        }
    }
}

void oneCplus(char c, FILE* fileName, FILE* output) {
    int val;
    if (c >= '0' && c <= '9') {
        val = c - '0';
    }
    else {
        val = c - 'A' + 10;
    }
    
    int cur;
    int counter = 0;
    int firstChar;
    while ((cur = fgetc(fileName)) != EOF) {
        if (cur == 10) {
            int i;
            for (i = 0; i < val; i++) {
                fputc(firstChar, output);
            }
            fprintf(output, "\n");
            counter = 0;    
        }
        else {
            if (counter == 0) {
                firstChar = cur;
            }
            counter++;
            fputc(cur, output);
        }
    }
}

void oneCminus(char c, FILE* fileName, FILE* output) {
    int val;
    if (c >= '0' && c <= '9') {
        val = c - '0';
    }
    else {
        val = c - 'A' + 10;
    }
    
    int tempVal = val;
    int cur;
    while ((cur = fgetc(fileName)) != EOF) {
        if (cur == 10) {
            if (tempVal > 0) fprintf(output, "-NONE-");
            tempVal = val;
            fprintf(output, "\n");
        }
        else {
            if (tempVal > 0) {
                tempVal--;
            }
            else {
                fputc(cur, output);
            }
        }
    }
}

void readFile(char* s) {
    FILE* file = fopen(s, "r");
    int cur;
    while ((cur = fgetc(file)) != EOF) {
        fputc(cur, stdout);
    }
}

int main(int argc, char **argv) {
    
    if (argc > 1) {
        int i;
        char* s = "";
        char* o = "";
        FILE* fileName;
        FILE* outputName;
        for (i = 1; i < argc; i++) {
            if (strncmp(argv[i],"-i", 2) == 0) {
                s = argv[i]+2;
            }
            else if (strncmp(argv[i], "-o", 2) == 0) {
                o = argv[i]+2;
            }
        }
        if (strcmp(o, "") != 0) outputName = fopen(o, "w");
        else outputName = stdout;
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-D") == 0) {
                if (strcmp(o, "") != 0) {
                    outputName = fopen(o, "a");
                }
                if (strcmp(s, "") != 0) {
                    fileName = fopen(s, "r");
                    oneB(fileName, outputName);
                }
                else {
                    oneB(stdin, outputName);
                }
            }
            else if (strncmp(argv[i], "+e", 2) == 0) {
                if (strcmp(o, "") != 0) {
                    outputName = fopen(o, "a");
                }
                if (strcmp(s, "") != 0) {
                    fileName = fopen(s, "r");
                    oneCplus(argv[i][2], fileName, outputName);
                }
                else {
                    oneCplus(argv[i][2], stdin, outputName);   
                }
            }
            else if (strncmp(argv[i], "-e", 2) == 0) {
                if (strcmp(o, "") != 0) {
                    outputName = fopen(o, "a");
                }
                if (strcmp(s, "") != 0) {
                    fileName = fopen(s, "r");
                    oneCminus(argv[i][2], fileName, outputName);
                }
                else {
                    oneCminus(argv[i][2], stdin, outputName);   
                }
            }
        }
    }
    else {
        oneA();
    }


    return 0;
}

