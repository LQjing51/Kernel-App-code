#include<elf.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
// typedef struct
// {
//   Elf64_Word	p_type;			/* Segment type */
//   Elf64_Word	p_flags;		/* Segment flags */
//   Elf64_Off	p_offset;		/* Segment file offset */
//   Elf64_Addr	p_vaddr;		/* Segment virtual address */
//   Elf64_Addr	p_paddr;		/* Segment physical address */
//   Elf64_Xword	p_filesz;		/* Segment size in file */
//   Elf64_Xword	p_memsz;		/* Segment size in memory */
//   Elf64_Xword	p_align;		/* Segment alignment */
// } Elf64_Phdr;

// typedef struct
// {
//   unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
//   Elf64_Half	e_type;			/* Object file type */
//   Elf64_Half	e_machine;		/* Architecture */
//   Elf64_Word	e_version;		/* Object file version */
//   Elf64_Addr	e_entry;		/* Entry point virtual address */
//   Elf64_Off	e_phoff;		/* Program header table file offset */
//   Elf64_Off	e_shoff;		/* Section header table file offset */
//   Elf64_Word	e_flags;		/* Processor-specific flags */
//   Elf64_Half	e_ehsize;		/* ELF header size in bytes */
//   Elf64_Half	e_phentsize;		/* Program header table entry size */
//   Elf64_Half	e_phnum;		/* Program header table entry count */
//   Elf64_Half	e_shentsize;		/* Section header table entry size */
//   Elf64_Half	e_shnum;		/* Section header table entry count */
//   Elf64_Half	e_shstrndx;		/* Section header string table index */
// } Elf64_Ehdr;
#define INST_LENTH_STR 32
#define INST_LENTH_INT 4
#define FIX_LENGTH 27
#define MAP_SIZE 1000
unsigned int Map[MAP_SIZE];
char* itoa(unsigned int inst) {
    char* str_inst = malloc(INST_LENTH_STR);
    for (int i = 0; i < INST_LENTH_STR; i++) {
        str_inst[INST_LENTH_STR-1-i] = (inst & 1) + '0';
        inst >>= 1; 
    }
    return str_inst;
}
unsigned int range_atoi(int begin, int end, char* str) {
    unsigned int value = 0;
    for (int i = begin; i <= end; i++) {
        value += str[i]-'0';
        if (i != end) value <<= 1;
    }
    return value;
}
const int LOW10 = (1 << 10) - 1;
const int LOW7 = (1 << 7) - 1;
const int LOW2 = 3;
int hash(unsigned int code) {
    int high = (code >> 12) & LOW10;
    int low = (code >> 5) & LOW7;
    return -(high ^~ low);
}
void dump_map(){
    printf("**********dump hash map**********\n");
    for (int i = 0; i < MAP_SIZE; i++) {
        if (Map[i]) {
            printf("%d\t",i);
            char* str_inst = itoa(Map[i]);
            for (int j = 0; j < INST_LENTH_STR; j++) 
                printf("%c", str_inst[j]);
            printf("\n");
        }
    }
}

void init_map() {
    freopen("instcode.txt", "r", stdin);
    char code[INST_LENTH_STR] = {0};
    while(scanf("%s",code) != EOF) {
        unsigned int value = range_atoi(0, 31, code);
        int index = hash(value);
        //Map[index] = (int*)malloc(INST_LENTH_INT);
        Map[index] = value;
    }
    printf("**********finish map init**********\n");
    
}
void insert_map(char* code) {
    int value = range_atoi(0, 31, code);
    int index = hash(value);
    //Map[index] = (int*)kmalloc(INST_LENTH_INT, GFP_KERNEL);
    //Map[index] = (int*)malloc(INST_LENTH_INT);
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

void dump_basic_info(Elf64_Ehdr * elf_hdr) {
    printf("**********dump elf header info**********\n");
    printf("Magic:\t");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", elf_hdr->e_ident[i]);
    }
    printf("\n");

    printf("Type:\t");
    switch (elf_hdr->e_type)
    {
    case 0:
        printf("No file type\n");
        break;
    case 1:
        printf("Relocatable file\n");
        break;
    case 2:
        printf("Executable file\n");
        break;
    case 3:
        printf("Shared object file\n");
        break;
    case 4:
        printf("Core file\n");
        break;
    default:
        printf("error\n");
        break;
    }
    printf("\n");
}
void dump_phdr_info(Elf64_Phdr* phdr) {
    printf("**********dump program header info**********\n");
    printf("Type:\t");
    switch (phdr->p_type){
        case PT_NULL:	printf("Program header table entry unused\n"); break;
        case PT_LOAD:   printf("Loadable program segment\n"); break;
        case PT_DYNAMIC:	printf("Dynamic linking information\n"); break;
        case PT_INTERP:	printf("Program interpreter\n"); break;
        case PT_NOTE:	printf("Auxiliary information\n"); break;
        case PT_SHLIB:   printf("Reserved\n"); break;
        case PT_PHDR:	printf("Entry for header table itself\n"); break;
        case PT_TLS	:	printf("Thread-local storage segment\n"); break;
        default: printf("else type\n"); break;
    }
    printf("Flags:\t");
    if (phdr->p_flags & PF_X) printf("PF_X");
    if (phdr->p_flags & PF_W) printf(" PF_W");
    if (phdr->p_flags & PF_R) printf(" PF_R");
    printf("\n");
    printf("FileSize:\t%ld\n",phdr->p_filesz);
    printf("\n");
}
bool hash_varify(int cur_inst, int bad_inst, int cmp_length) {
    if ((cur_inst) >> (INST_LENTH_STR-cmp_length) == 
                (bad_inst) >> (INST_LENTH_STR-cmp_length))
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
void text_scan(int *ph_data, long long size) {
    int *inst;
    long long scaned_size = 0;
    for (inst = ph_data; scaned_size < size; inst++, scaned_size += 4) {
        if (is_privilege(*inst)) {
            // printf("is privilege\n");
            int index = hash(*inst);
            /*if index out of Map, can not be a map inst*/
            if (index >= MAP_SIZE | index < 0) continue;
            if (Map[index] && hash_varify(*inst, Map[index],FIX_LENGTH))
                printf("find a good inst!\n");
            else 
                printf("find a bad inst!\n");
        }
    }
}   

int main(){
    
    init_map_nofile();

    FILE* fp = fopen("app","rb");

    Elf64_Ehdr* elf_hdr = (Elf64_Ehdr*)malloc(sizeof(Elf64_Ehdr));
    memset(elf_hdr, 0, sizeof(Elf64_Ehdr));
    fread(elf_hdr, sizeof(Elf64_Ehdr), 1, fp);
    
    dump_basic_info(elf_hdr);
    
    int phdrs_size = sizeof(Elf64_Phdr) * elf_hdr->e_phnum; 
    Elf64_Phdr* phdrs = (Elf64_Phdr*)malloc(phdrs_size);
    memset(phdrs, 0, phdrs_size);
    fseek(fp, elf_hdr->e_phoff, SEEK_SET);
    fread(phdrs, phdrs_size, 1, fp);

    Elf64_Phdr* phdr = (Elf64_Phdr*)malloc(sizeof(Elf64_Phdr));
    int i;
    for (i = 0, phdr = phdrs; i < elf_hdr->e_phnum; i++, phdr++) {
        if (phdr->p_type != PT_LOAD) continue;
        if (!(phdr->p_flags & PF_X)) continue;
        dump_phdr_info(phdr);
        
        int* ph_data = malloc(phdr->p_filesz);
        fseek(fp, phdr->p_offset, SEEK_SET);
        fread(ph_data, phdr->p_filesz, 1, fp);
        text_scan(ph_data,phdr->p_filesz);
        printf("finish scan a valid phdr\n");
    }    
    

}