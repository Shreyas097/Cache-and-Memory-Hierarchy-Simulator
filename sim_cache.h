#ifndef SIM_CACHE_H
#define SIM_CACHE_H

typedef struct cache_params{
    unsigned long int block_size;
    unsigned long int l1_size;
    unsigned long int l1_assoc;
    unsigned long int vc_num_blocks;
    unsigned long int l2_size;
    unsigned long int l2_assoc;
}cache_params;

typedef struct l1_cache{
	int valid;
	unsigned long int tag;
	unsigned long int tag1;
	int index;
	int lru;
	int dirty;
	int status;
	unsigned long int address;
}l1_cache;

typedef struct v_cache{
	int valid;
	unsigned long int tag;
	unsigned long int tag1;
	int index;
	int lru;
	int dirty;
	int status;
	unsigned long int address;
}v1_cache;

typedef struct l2_cache{
	int valid;
	unsigned long int tag;
	unsigned long int tag1;
	int index;
	int lru;
	int dirty;
	int status;
	unsigned long int address;
}l2_cache;

l1_cache** cache;
l2_cache** l2cache;
v_cache* vcache;

unsigned long int addr10;
char rw;

int l2_size;
int l1_sets;
int l2_sets;
unsigned long int l1_assoc;
unsigned long int l2_assoc;
int l1_index;
int l1_offset;
int l1_tag;
int l2_index;
int l2_offset;
int l2_tag;
unsigned long int l1_tagbits_number;
int l1_indexbits_number;
int l1_indexbits_number1;
unsigned long int l1_offsetbits_number;
unsigned long int l2_tagbits_number;
int l2_indexbits_number;
int l2_indexbits_number1;
unsigned long int l2_offsetbits_number;

int num_references = 0;
int read_hit = 0;
int read_miss = 0;
int write_hit = 0;
int write_miss = 0;
int l1_wb = 0;
int l2_read = 0;
int l2_write = 0;
int lru_val = 0;
int Vlru_val = 0;
int l2_lru_val = 0;
int lru_block;
int Vlru_block;
int l2_lru_block;
int write=0;
int read = 0;
int l2_write_hit = 0;
int l2_read_hit = 0;
int l2_write_miss = 0;
int l2_read_miss = 0;
int l2_wb_to_memory = 0;
int swap_req = 0;
int swap = 0;
int mem_traffic = 0;
unsigned long int vc_blocks;
unsigned long int addr5;

char *l1_offsetbits;
char *l1_indexbits;
char *l1_tagbits;
char *l2_offsetbits;
char *l2_indexbits;
char *l2_tagbits;
char *binary;
char address[25];

// Put additional data structures here as per your requirement
char *change_to_binary(char address[]);
char *l1_get_tag(int l1_tag);
char *l1_get_index(int l1_index, int l1_tag);
char *l1_get_offset(int l1_tag, int l1_index);
char *l2_get_tag(int l2_tag);
char *l2_get_index(int l2_index, int l2_tag);
char *l2_get_offset(int l2_tag, int l2_index);
l1_cache **create_cache(unsigned long int);
l2_cache **create_l2cache(unsigned long int);
int binary_decimal(int);
int write_cache(unsigned long int, int, unsigned long int);
int read_cache(unsigned long int, int, unsigned long int);
int update_lru();
int evict_block(int,unsigned long int);
void print_order(int, unsigned long int);
void l2_print_order(int, unsigned long int);

v_cache *create_vcache(unsigned long int);
void search_vc(unsigned long int, int, int);
int update_Vlru();
int l2_update_lru();
int evict_vcblock();
int l2_evict_block(int, unsigned long int);
void print_vorder();
unsigned long int get_vcache_tag();
void wb_to_l2(unsigned long int, int, int);
void read_from_l2(unsigned long int, int, int);
void wb_to_l2_vc(int);

#endif
