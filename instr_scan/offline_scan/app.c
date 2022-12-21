#include<stdio.h>
int main (){
    // int a = 1;
    unsigned long long target = 0;
	asm volatile("mrs x0, NZCV\n\t"
        "mrs x1, CurrentEL\n\t"
		"mov %[result], x0 \n\t"
		: [result]"=r"(target)
		::	
		);
	printf("NZCV: %llx\n", target);
    return 0;
    // printf("hello world\n");
}