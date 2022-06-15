#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  int i;
  for (i = 0; i < array_length; i++) {
    mapped_array[i] = f(array[i]);
  }
  return mapped_array;
}

/* Gets a char c and returns its encrypted form by adding 2 to its value. 
          If c is not between 0x41 and 0x7a it is returned unchanged */
char encrypt(char c){
    if (c >= 0x41 && c <= 0x7a) {
        return c + 2;
    }
    return c;
}

/* Gets a char c and returns its decrypted form by reducing 2 to its value. 
            If c is not between 0x41 and 0x7a it is returned unchanged */
char decrypt(char c) {
    if (c >= 0x41 && c <= 0x7a) {
        return c - 2;
    }
    return c;
}

/* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
char dprt(char c) {
    printf ("%d\n", c);
    return c;
}

/* If c is a number between 0x41 and 0x7a, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('*') character. After printing, cprt returns 
                    the value of c unchanged. */
char cprt(char c) {
    if (c >= 0x41 && c <= 0x7a) 
        printf("%c\n",c);
    else
        printf("*\n");
    return c;
}

/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c) {
    return fgetc(stdin);
}

char quit(char c) {
    if (c == 'q') exit(0);
    return c;
}

typedef struct fun_desc {
  char *name;
  char (*fun)(char);
}fun_desc;

void menu() {
    char* carray = (char*)(malloc(5 * sizeof(char)));
    int i;
    for (i = 0; i < 5; i++) {
        carray[i] = '\0';
    }
    
    fun_desc menu[] = { {"Censor", censor}, {"Encrypt", encrypt}, {"Decrypt", decrypt}, {"Print dec", dprt}, {"Print string", cprt}, {"Get string", my_get}, {"Quit", quit}, {NULL, NULL} };
    
    
    while(1){
        printf("please enter a number\n");
        for (i = 0; i < 7; i++) {
            printf("%d) %s\n", i, menu[i].name);
        }
        char c = fgetc(stdin);
        char s = fgetc(stdin);
        if (c >= '0' && c <= '6') {
            printf("Option: %c\n", c);
            printf("Within bounds\n");
            if (c == '6') exit(0);
        }
        else {
            printf("Not within bounds\n");
            quit('q');
        }
        char* carray2 = map(carray,5,menu[c-'0'].fun);
        strcpy(carray, carray2);
        free(carray2);
        printf("Done\n");
    }
    
    
}

int main(int argc, char **argv){    

    menu();
    
    return 0;
}
