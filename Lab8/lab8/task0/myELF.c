#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
 
int debug = 0;
int currentFD = -1; 
struct stat fd_stat;
void* map_start;
Elf32_Ehdr *header;

int loadFile() {
    char fileName[128];
    int fd;
    fscanf(stdin, "%s", fileName);
    fd = open(fileName, O_RDWR);
    fstat(fd, &fd_stat);
    map_start = mmap(0, fd_stat.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    currentFD = fd;
    return currentFD;
}

void toggleDebug() {
    if (debug == 0) {
        debug = 1;
        printf("Debug flag is now on\n");
    }
    else {
        debug = 0;
        printf("Debug flag is now off\n");
    }
}

void examineELF() {
    printf("Enter file name: ");
    loadFile();
    header = (Elf32_Ehdr*) map_start;
    if (strncmp((char*)header->e_ident, (char*)ELFMAG, 4) == 0) {
        printf("Magic: %X %X %X\n", header->e_ident[EI_MAG0], header->e_ident[EI_MAG1], header->e_ident[EI_MAG2]);
        char c = header->e_ident[5];
        if(c == ELFDATANONE) printf("Data: invalid data\n");
        else if (c == ELFDATA2LSB) printf("Data: little endian\n");
        else if (c == ELFDATA2MSB) printf("Data: big endian\n");
        else printf("Data: no data\n");
        printf("Entry point: %x\n", header->e_entry);
        printf("Section header offset: %d\n", header->e_shoff);
        printf("Sections: %d\n", header->e_shnum);
        printf("Size of section headers: %d\n", header->e_shentsize);
        printf("Program header offset: %d\n", header->e_phoff);
        printf("Programs: %d\n", header->e_phnum);
        printf("Size of program headers: %d\n", header->e_phentsize);
    }
    else {
        printf("This is not ELF file\n");
        close(currentFD);
        currentFD = -1;
    }
}

void printSections() {
    printf("Not implemented\n");
}

void printSymbols() {
    printf("Not implemented\n");
}

void quit() {
    exit(0);
}

typedef struct fun_desc {
  char *name;
  void (*fun)();
}fun_desc;

void menu() {
    fun_desc menu[] = { {"Toggle Debug Mode", toggleDebug}, {"Examine ELF File", examineELF}, {"Print Section Names", printSections}, {"Print Symbols", printSymbols}, {"Quit", quit}, {NULL, NULL} };
    
    
    while(1){
        printf("Choose action:\n");
        int i;
        for (i = 0; i < 5; i++) {
            printf("%d) %s\n", i, menu[i].name);
        }
        char c;
        scanf("%s", &c);
        if (c >= '0' && c <= '4') {
            menu[c-'0'].fun();
        }
        else {
            printf("Not within bounds\n");
            quit();
        }
        printf("\n");
    }
    
    
}

int main(int argc, char **argv){    

    menu();
    
    return 0;
}
