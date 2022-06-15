#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;

typedef struct link {
    link *nextVirus;
    virus *vir;
} link;

typedef struct fun_desc {
    char *name;
    char (*fun)(char);
} fun_desc;

link* list;

void PrintHex(unsigned char* buffer, size_t length, FILE* output) {
    int i;
    for (i= 0; i < length; i++) {
        fprintf(output, "%02X ", buffer[i]);
    }
    fprintf(output, "\n\n");
}

void readVirus(virus* vir, FILE* input) {
    if (!vir->sig) free(vir->sig);
    fread(&vir->SigSize, sizeof(short), 1, input);
    vir->sig = calloc(vir->SigSize, sizeof(unsigned char));
    fread(vir->sig, vir->SigSize, 1, input);
    fread(&vir->virusName, 16, 1, input);

}

void printVirus(virus* vir, FILE* output) {
    fprintf(output, "Virus name: %s \n", vir->virusName);
    fprintf(output, "Virus size: %d \n", vir->SigSize);
    fprintf(output, "Virus Signature: \n");
    PrintHex(vir->sig, vir->SigSize, output);
}

void list_print(link *virus_list, FILE* output) {
    /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
    if (virus_list->vir->SigSize != 0) printVirus(virus_list->vir, output);
    if (virus_list->nextVirus != NULL) list_print(virus_list->nextVirus, output);
}

link* list_append(link* virus_list, link* to_add) {
    /* Add the given link to the list
       (either at the end or the beginning, depending on what your TA tells you),
       and return a pointer to the list (i.e., the first link in the list).
       If the list is null - return the given entry. */
    to_add->nextVirus = virus_list;
    return to_add;
}

void list_free(link *virus_list) {
    /* Free the memory allocated by the list. */
    if (virus_list->nextVirus) {
        free(virus_list->vir->sig);
        free(virus_list->vir);
        free(virus_list);
    }
    else {
        list_free(virus_list->nextVirus);
    }
}

void load_signatures() {
    printf("Please Enter File Name.\n");
    char name[128];
    fgets(name, 128, stdin);
    name[strcspn(name, "\n")] = 0;
    FILE* file = fopen(name, "r");
    if (file != NULL) {
        while(1) {
            virus* curVirus = malloc(sizeof(virus));
            readVirus(curVirus, file);
            link* curLink = malloc(sizeof(link));
            curLink->vir = curVirus;
            list = list_append(list, curLink);
            if (feof(file)) break;
        }
    }
    fclose(file);
}

void print_signatures() {
    if (list != NULL) list_print(list, stdout);
}

void detect_virus() {
    printf("Please Enter File Name.\n");
    char name[128];
    fgets(name, 128, stdin);
    name[strcspn(name, "\n")] = 0;
    FILE* file = fopen(name, "r");
    char* buffer = malloc(10000);

    fseek(file, 0, SEEK_END);
    unsigned int endIndex = ftell(file);
    if (endIndex > 10000) endIndex = 10000;
    fseek(file, 0, SEEK_SET);
    fread(buffer, endIndex, 1, file);

    unsigned int size;
    link* cur = list->nextVirus;
    while(cur != NULL) {
        size = cur->vir->SigSize;
        int i;
        for ( i = 0; i < endIndex - size + 1; i++) {
            if (memcmp(&buffer[i], cur->vir->sig, size) == 0) {
                printf("Starting location: %d\n", i);
                printf("Virus name: %s\n", cur->vir->virusName);
                printf("Virus size signature: %d\n\n", size);
            }
        }
        cur = cur->nextVirus;
    }
    free(buffer);
    fclose(file);
}
void kill_virus(char* fileName,int signatureOffset ,int signatureSize){
    FILE* file=fopen(fileName,"r+");
    fseek(file, signatureOffset, SEEK_SET);
    char buff[signatureSize];
    int i;
    for(i=0; i<signatureSize;i++){
        buff[i] = ';';
    }
    fwrite(buff, signatureSize, 1, file);
    fclose(file);
}

void fix(){
    printf("enter file name: ");
    char* buffer=malloc(128);
    fgets(buffer,128,stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    int offset;
    printf("enter starting byte: ");
    scanf("%d",&offset);
    getchar();
    int size;
    printf("enter size: ");
    scanf("%d",&size);
    getchar();
    kill_virus(buffer, offset,size);
    free(buffer);
}

void quit() {
    exit(0);
}

void menu() {

    fun_desc menu[] = { {"Load signatures", load_signatures}, {"Print signatures", print_signatures}, {"Detect viruses", detect_virus}, {"Fix file", fix}, {"Quit", quit}, {NULL, NULL} };


    while(1){
        printf("please enter a number\n");
        int i;
        for (i = 1; i < 6; i++) {
            printf("%d) %s\n", i, menu[i-1].name);
        }
        char c = fgetc(stdin);
        char s = fgetc(stdin);
        if (c >= '1' && c <= '5') {
            printf("Option: %c\n", c);
            void (*f) () = menu[(c - '0') - 1].fun;
            f();
        }
        else {
            printf("Not within bounds\n");
            quit();
        }
    }


}


int main(int argc, char **argv)
{
    menu();
    return 0;
}
