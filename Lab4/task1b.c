#include "util.h"

extern int system_call();

void count(int flagD, char* cDescI, char* cDescO) {
    int counter = 0;
    char c[1] = "";
    char prev[1] = " ";
    
    int descI = 0, descO = 1;
    if (strcmp(cDescI, "") != 0) {
        descI = system_call(5, cDescI, 0, 0777);
        if (flagD) {
            system_call(4, 2, "Debug: 5 ", 9);
            system_call(4, 2, itoa(descI), strlen(itoa(descI)));
            system_call(4, 2, "\n", 1);
        }
    }
    if (strcmp(cDescO, "") != 0) {
        descO = system_call(5, cDescO, 1, 0777);
        if (flagD) {
            system_call(4, 2, "Debug: 5 ", 9);
            system_call(4, 2, itoa(descO), strlen(itoa(descO)));
            system_call(4, 2, "\n", 1);
        }
    }
    
    if (flagD) {
        system_call(4, 2, "in: ", 4);
        system_call(4, 2, itoa(descI), strlen(itoa(descI)));
        system_call(4, 2, " out: ", 6);
        system_call(4, 2, itoa(descO), strlen(itoa(descO)));
        system_call(4, 2, "\n", 1);
    }
    
    int val;
    while ((val = system_call(3, descI, c, 1)) > 0) {
        if (flagD) {
            system_call(4, 2, "Debug: ", 7);
            system_call(4, 2, "3 ", 2);
            system_call(4, 2, itoa(val), 1);
            system_call(4, 2, "\n", 1);
        }
        if (strcmp(c, "\n") == 0) break;
        else {
            if (prev[0] == ' ' && strcmp(c, " ") != 0) counter++;
            prev[0] = c[0];
        }
    }
    char* sCounter = itoa(counter);
    val = system_call(4, descO, sCounter, strlen(sCounter));
    if (flagD) {
        system_call(4, 2, "\nDebug: 4 ", 10);
        system_call(4, 2, itoa(val), 1);
    }
    
}


int main(int argc, char** argv) {

    int flagD = 0;
    char* descI = "";
    char* descO = "";
    int i;
    for (i = 1; i < argc; i++) {
        if (strncmp (argv[i], "-D", 2) == 0) flagD = 1;
        else if (strncmp(argv[i], "-i", 2) == 0) { 
            descI = argv[i] + 2;
        }
        else if (strncmp(argv[i], "-o", 2) == 0) {
            descO = argv[i] + 2;
        }
    }
    
    
    count(flagD, descI, descO);
    
    return 0;
}
