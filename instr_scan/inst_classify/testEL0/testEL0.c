#include<stdio.h>

int main(){
    unsigned long long target = 0;
	asm volatile("mrs x0, PMCCNTR_EL0\n\t"
		"mov %[result], x0 \n\t"
		: [result]"=r"(target)
		::	
		);
	printf("PMCCNTR_EL0: %llx\n", target);
	

	return 0;
}