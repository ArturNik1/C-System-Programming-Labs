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
        printf("%d\n",c);
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
 
int main(int argc, char **argv){    
    int base_len = 5;
    char arr1[base_len];
    char* arr2 = map(arr1, base_len, my_get);
    char* arr3 = map(arr2, base_len, encrypt);
    char* arr4 = map(arr3, base_len, dprt);
    char* arr5 = map(arr4, base_len, decrypt);
    char* arr6 = map(arr5, base_len, cprt);
    free(arr2);
    free(arr3);
    free(arr4);
    free(arr5);
    free(arr6);
    
    return 0;
}
