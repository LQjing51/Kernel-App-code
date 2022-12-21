#include "hook.h"
#include "common.h"

#define INST_LENTH_STR 32
#define INST_LENTH_INT 4
#define FIX_LENGTH 27
#define MAP_SIZE 1000
unsigned int Map[MAP_SIZE];
char* itoa(unsigned int inst) {
    char* str_inst = kmalloc(INST_LENTH_STR, GFP_KERNEL);
    int i;
    for (i = 0; i < INST_LENTH_STR; i++) {
        str_inst[INST_LENTH_STR-1-i] = (inst & 1) + '0';
        inst >>= 1;
    }
    return str_inst;
}

char* itoa_ignore_low(unsigned int inst) {
    char* str_inst = kmalloc(INST_LENTH_STR - 5, GFP_KERNEL);
    int i;
    int ignore_low_inst;
    
    ignore_low_inst = inst >>= 5;
    for (i = 0; i < INST_LENTH_STR - 5; i++) {
        str_inst[INST_LENTH_STR-6-i] = (ignore_low_inst & 1) + '0';
        ignore_low_inst >>= 1;
    }
    return str_inst;
}

int range_atoi(int begin, int end, char* str) {
    int value = 0;
    int i;
    for (i = begin; i <= end; i++) {
        value += str[i]-'0';
        if (i != end) value <<= 1;
    }
    return value;
}
const int LOW10 = (1 << 10) - 1;
const int LOW7 = (1 << 7) - 1;
const int LOW2 = 3;

int hash(int code) {
    int high = (code >> 12) & LOW10;
    int low = (code >> 5) & LOW7;
    return -(high ^~ low);
}


/*void init_map(void) {
    struct file * filep = filp_open("/home/lqj/instr_scan/scan_elf/instcode.txt", O_RDONLY, 0);
    loff_t pos = 0;
    int total = 672;
    int size = 0;

    char buf[672];//21 inst
    char *buf_ptr = buf;
    memset(buf, 0, sizeof(buf));
    kernel_read(filep, buf, sizeof(buf), &pos);
    char code[INST_LENTH_STR] = {0};

    while(size < total) {
        memcpy(code, buf_ptr, 32);
    	buf_ptr += 32;
	    size += 32;
	    int value = range_atoi(0, 31, code);
        int index = hash(value);
        Map[index] = value;
    }
    // INFO("**********finish map init**********\n");

}*/

void insert_map(char* code) {
    int value = range_atoi(0, 31, code);
    int index = hash(value);
    Map[index] = value;
}

void init_map_nofile(void) {
    char* code = "11010101000010110111111000100000"; insert_map(code);
    code = "11010101000010110111101000100000"; insert_map(code);
    code = "11010101000010110111110100100000"; insert_map(code);
    code = "11010101000010110111110000100000"; insert_map(code);
    code = "11010101000010110111101100100000"; insert_map(code);
    code = "11010101000010110111010000100000"; insert_map(code);
    code = "11010101000010110111010100100000"; insert_map(code);
    code = "11010101001110110100010000000000"; insert_map(code);
    code = "11010101001110110100010000100000"; insert_map(code);
    code = "11010101001110110000000011100000"; insert_map(code);
    code = "11010101001110111101000001000000"; insert_map(code);
    code = "11010101001110111101000001100000"; insert_map(code);
    code = "11010101001110110100001010000000"; insert_map(code);
    code = "11010101001110110100001000000000"; insert_map(code);
    code = "11010101001110110100001011000000"; insert_map(code);
    code = "11010101000110110100010000000000"; insert_map(code);
    code = "11010101000110110100010000100000"; insert_map(code);
    code = "11010101000110111101000001000000"; insert_map(code);
    code = "11010101000110110100001010000000"; insert_map(code);
    code = "11010101000110110100001000000000"; insert_map(code);
    code = "11010101000110110100001011000000"; insert_map(code);
}

bool hash_varify(int cur_inst, int cmp_inst, int cmp_length) {
    if ((cur_inst) >> (INST_LENTH_STR-cmp_length) ==
                (cmp_inst) >> (INST_LENTH_STR-cmp_length))
                return true;

    return false;
}

bool is_privilege(int inst){
    //1101010100x01
    if (((inst >> 22) & LOW10) == 852 // 1101010100
            && ((inst >> 19) & LOW2) == 1) //01
            return true;
    //1101010100x1
    if (((inst >> 22) & LOW10) == 852 //1101010100
            && ((inst >> 20) & 1)) //1
            return true;
    return false;
}

void scan_page(struct page * page){
    
    phys_addr_t paddr;
    unsigned long vaddr;
    int scaned_size;
    int *inst;
    
    init_map_nofile();
    
    paddr = (phys_addr_t)((page_to_pfn(page)) << PAGE_SHIFT);
    vaddr = __phys_to_virt(paddr);

    //INFO("paddr = %llx\n",(unsigned long long)paddr);
    //INFO("vaddr = %llx\n",(unsigned long long)vaddr);

    scaned_size = 0;
    //INFO("PAGE_SIZE = %llx\n", (unsigned long long)PAGE_SIZE);
    for (inst = (int*)vaddr; scaned_size < PAGE_SIZE; inst++, scaned_size += 4) {
            if (is_privilege(*inst)) {
                    // printf("is privilege\n");
                    int index = hash(*inst);
                    /*if index out of Map, can not be a map inst*/
                    if ((index >= MAP_SIZE) | (index < 0)) continue;
                    if (Map[index] && hash_varify(*inst, Map[index],FIX_LENGTH)){
                            //INFO("\tgood inst: %s\n", itoa_ignore_low(*inst));
	    	    }else{
                            INFO("\tbad inst: %s\n", itoa_ignore_low(*inst));
		    }
	    }
    }
}
