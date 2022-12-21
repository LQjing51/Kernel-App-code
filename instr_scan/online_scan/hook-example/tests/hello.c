#include <stdio.h>
#include <sys/mman.h>
#define LEN (8*1024)
int main() {
    printf("hello world!\n");
    
    //char* addr;
    //addr = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //if (addr == MAP_FAILED) printf("mmap error\n");

    //if (mprotect(addr, LEN, PROT_READ|PROT_WRITE) == -1)
        //printf("mprotect1 error\n");
    
    //if (mprotect(addr, LEN, PROT_EXEC) == -1)
      //  printf("mprotect2 error\n");

    //if (mprotect(addr, LEN, PROT_WRITE|PROT_EXEC) == -1)
      //  printf("mprotect3 error\n");
    
    return 0;
}
