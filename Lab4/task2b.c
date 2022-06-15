#include "util.h"

extern int system_call();

typedef struct ent{
	int inode;
	int offset;
	short len;
	char buf[1];
} ent;

#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WH 14

char* getType(int type){
    if(type == DT_UNKNOWN)
      return "unknown type";
    else if(type == DT_FIFO)
      return "FIFO type";
    else if(type == DT_CHR)
      return "char dev type";
    else if(type == DT_DIR)
      return "directory type";
    else if(type == DT_BLK)
      return "block dev type";
    else if(type == DT_REG)
      return "regular type";
    else if(type == DT_LNK)
      return "symlink type";
    else if(type == DT_SOCK)
      return "socket type";
    else
      return "type is not known";
}

void flame(int flagD, char* prefix) {
    int flagP = 0;
    if (strcmp(prefix, "") != 0) flagP = 1;
    
    system_call(4, 1, "Flame 2 strikes!\n", 17);
    
    char entity[8192];
    ent* d;
    int currentDesc = system_call(5, ".", 0, 0777);
    int nread = system_call(141, currentDesc, entity, 8192);
    
    if (flagD) {
        system_call(4, 2, "Debug: 5 ", 9);
        system_call(4, 2, itoa(currentDesc), strlen(itoa(currentDesc)));
        system_call(4, 2, "\n", 1);
        system_call(4, 2, "Debug: 141 ", 11);
        system_call(4, 2, itoa(nread), strlen(itoa(nread)));
        system_call(4, 2, "\n", 1);
    }
    
    int i;
    int val;
    
    char* d_type = "";
    
    for (i = 0; i < nread;) {
        d = (ent*) (entity + i);
        
        int enter = 0;
        
        if (flagP) {
            if (strncmp(d->buf, prefix, strlen(prefix)) == 0) {
                val = system_call(4, 1, strcat(d->buf, "\n"), 1 + strlen(d->buf));
                enter = 1;
            }
        }
        else {
            val = system_call(4, 1, strcat(d->buf, "\n"), 1 + strlen(d->buf));
            enter = 1;
        }
        
        d_type = entity + i + d->len - 1;
        char* strType = getType(positive_atoi(d_type));
        
        if (flagP && enter) {
            system_call(4, 2, "Type: ", 6);
            system_call(4, 2, strType, strlen(strType));
            system_call(4, 2, "\n", 1);
        }
        
        if (flagD && enter) {
            system_call(4, 2, "Debug: 4 ", 9);
            system_call(4, 2, itoa(val), strlen(itoa(val)));
            system_call(4, 2, "\n", 1);
            system_call(4, 2, "Length: ", 8);
            system_call(4, 2, itoa(d->len), strlen(itoa(d->len)));
            system_call(4, 2, " Type: ", 7);
            system_call(4, 2, strType, strlen(strType));
            system_call(4, 2, "\n", 1);
        }
        
        i += d->len;
    }
    
}

int main(int argc, char** argv) {
    char* prefix = "";
    int i;
    int flagD = 0;
    for (i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-D", 2) == 0) flagD = 1;
        else if (strncmp (argv[i], "-p", 2) == 0) prefix = argv[i] + 2;
    }
    
    flame(flagD, prefix);
    
    return 0;
}
