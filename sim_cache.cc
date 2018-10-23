#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <iomanip>
#include "sim_cache.h"

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim_cache 32 8192 4 7 262144 8 gcc_trace.txt
    argc = 8
    argv[0] = "sim_cache"
    argv[1] = "32"
    argv[2] = "8192"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    cache_params params;    // look at sim_cache.h header file for the the definition of struct cache_params
    char rw;                // variable holds read/write type read from input file. The array size is 2 because it holds 'r' or 'w' and '\0'. Make sure to adapt in future projects
    unsigned long int addr = 0; // Variable holds the address read from input file

    if(argc != 8)           // Checks if correct number of inputs have been given. Throw error and exit if wrong
    {
        printf("Error: Expected inputs:7 Given inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    params.block_size       = strtoul(argv[1], NULL, 10);
    params.l1_size          = strtoul(argv[2], NULL, 10);
    params.l1_assoc         = strtoul(argv[3], NULL, 10);
    params.vc_num_blocks    = strtoul(argv[4], NULL, 10);
    params.l2_size          = strtoul(argv[5], NULL, 10);
    params.l2_assoc         = strtoul(argv[6], NULL, 10);
    trace_file              = argv[7];

    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    // Print params
    printf("  ===== Simulator configuration =====\n"
            "  BLOCKSIZE:                     %lu\n"
            "  L1_SIZE:                          %lu\n"
            "  L1_ASSOC:                         %lu\n"
            "  VC_NUM_BLOCKS:                    %lu\n"
            "  L2_SIZE:                          %lu\n"
            "  L2_ASSOC:                         %lu\n"
            "  trace_file:                       %s\n"
            "  ===================================\n\n", params.block_size, params.l1_size, params.l1_assoc, params.vc_num_blocks, params.l2_size, params.l2_assoc, trace_file);
	
	l2_size = params.l2_size;
	l1_sets = params.l1_size/(params.block_size*params.l1_assoc);
	if(l2_size > 0)
		l2_sets = params.l2_size/(params.block_size*params.l2_assoc);	
	// Initializing cache
	l1_assoc = params.l1_assoc;
	l2_assoc = params.l2_assoc;
	cache = create_cache(l1_assoc);	
	l2cache = create_l2cache(l2_assoc);
	vc_blocks = params.vc_num_blocks;
	vcache = create_vcache(vc_blocks);	

// Going through file
    char str[2];
    while(fscanf(FP, "%s %lx", str, &addr) != EOF)
    {
		num_references++;
 //       rw = str[0];
 //       if (rw == 'r')
 //           printf("\n%s %lx", "read", addr);           // Print and test if file is read correctly
 //       else if (rw == 'w')
 //          printf("\n%s %lx", "write", addr);          // Print and test if file is read correctly
        /*************************************
                  Add cache code here
        **************************************/
//	char address[25];
	addr10 = addr;
	sprintf(address, "%lx", addr);
	int len;
	len = strlen(address);
	char zero[25] = {};
	if(len<8)
	{
		for(int i = 0;len<8;++i)
		{
			zero[i] = '0';
			++len;
		}
		strcat(zero,address);
		strcpy(address,zero);
	}

	binary = change_to_binary(address);


	// Finding L1 parameters
	l1_index = log2(l1_sets);
	l1_offset = log2(params.block_size);
	l1_tag = strlen(binary) - l1_index - l1_offset;

	// Finding L2 parameters
	if(l2_size > 0)
	{
		l2_index = log2(l2_sets);
		l2_offset = log2(params.block_size);
		l2_tag = strlen(binary) - l2_index - l2_offset;
	}
	if(l2_size > 0)
		addr5 = addr >> l2_offset;

	l1_tagbits = l1_get_tag(l1_tag);
	l1_indexbits = l1_get_index(l1_index, l1_tag);
	l1_offsetbits = l1_get_offset(l1_tag, l1_index);

	l1_tagbits_number = strtoul(l1_tagbits, NULL, 2);
	l1_indexbits_number = atoi(l1_indexbits);
	l1_offsetbits_number = strtoul(l1_offsetbits, NULL, 2);	

	l1_indexbits_number1 = binary_decimal(l1_indexbits_number);

	if(l2_size > 0)
	{
		addr5 = addr >> l2_offset;
		l2_indexbits_number1 = l2_index?(addr5 & (int)(pow(2,l2_index)-1)):0;
		l2_tagbits_number = addr5 >> l2_index;
	}


	rw = str[0];
	if (rw == 'r')
	{
		read++;
		read_cache(l1_tagbits_number, l1_indexbits_number1, params.l1_assoc);
	}
	else if (rw == 'w')
	{
		write++;
		write_cache(l1_tagbits_number, l1_indexbits_number1, params.l1_assoc);
	}

    }

	// Printing L1 contents
	printf("\n  ===== L1 contents =====\n");
   char dir;
   for(int i = 0; i<l1_sets; i++)
   {
	print_order(i, params.l1_assoc);
	printf("\n Set %d:", i);
	for(unsigned long int j = 0; j<params.l1_assoc; j++)
	{
		if (cache[i][j].dirty == 1) 
			dir = 'D';
		else 
			dir = ' ';
		printf(" %lx %c", cache[i][j].tag, dir); 
	}
	printf("\n");
    } 

if (vc_blocks > 0)
{	
	printf("\n  ===== VC contents =====\n");
	printf("\n Set 0 : ");
	print_vorder();
	char dir1;
       for(unsigned long int i = 0; i<vc_blocks; i++)
   {
	if (vcache[i].dirty == 1)
		dir1 = 'D';
	else
		dir1 = ' ';
		printf("%lx %c ", vcache[i].tag1, dir1);
    } 
	printf("\n");
}
	
	if(l2_size > 0)
	{
	// Printing L2 contents
	printf("\n  ===== L2 contents =====\n");
	char dir2;
   for(int i = 0; i<l2_sets; i++)
   {
	l2_print_order(i, params.l2_assoc);
	printf("\n Set %d:", i);
	for(unsigned long int j = 0; j<params.l2_assoc; j++)
	{
		if (l2cache[i][j].dirty == 1) 
			dir2 = 'D';
		else 
			dir2 = ' ';
		printf(" %lx %c", l2cache[i][j].tag, dir2); 
	}
	printf("\n");
    } 
	}
  


// Printing final parameters found
    if(l2_size > 0)
		mem_traffic = l2_read_miss+l2_write_miss+l2_wb_to_memory;
	else
		mem_traffic = read_miss+write_miss-swap+l1_wb;
	float sr,nr;
	sr = swap_req;
	nr = num_references;
	float swap_req_rate = 0.0;
	if(vc_blocks > 0)
		swap_req_rate = sr/nr;
	else
		swap_req_rate = 0.0;
	float l1_miss_rate = 0.0;
	float l2_miss_rate = 0.0;
	float rm,wm,sw,read1,write1,l2rm,l2r;
	rm = read_miss;
	wm = write_miss;
	sw = swap;
	read1 = read;
	write1 = write;
	l2rm = l2_read_miss;
	l2r = l2_read;
	l1_miss_rate = (rm + wm - sw)/(read1+write1);
	if(l2_size > 0)
		l2_miss_rate = (l2rm)/(l2r);
	else
		l2_miss_rate = 0.0;
	
	
	printf("\n  ===== Simulation results =====\n");
	printf("\n a. number of L1 reads: %d \n b. number of L1 read misses: %d \n c. number of L1 writes: %d \n d. number of L1 write misses: %d \n e. number of swap requests: %d \n f. swap request rate: %.4f \n g. number of swaps: %d \n h. combined L1+VC miss rate: %.4f \n i. number writebacks from L1/VC: %d \n j. number of L2 reads: %d \n k. number of L2 read misses: %d \n l. number of L2 writes: %d \n m. number of L2 write misses: %d \n n. L2 miss rate: %.4f \n o. number of writebacks from L2: %d\n p. total memory traffic: %d\n", read, read_miss, write , write_miss, swap_req, swap_req_rate, swap, l1_miss_rate, l1_wb, l2_read, l2_read_miss, l2_write, l2_write_miss, l2_miss_rate, l2_wb_to_memory, mem_traffic);
    return 0;

}

char *change_to_binary(char address[])
{
	char *binary = (char *)malloc(sizeof(char) * 10000);
	unsigned long int i;
	for(i=0; i < strlen(address)*10; ++i)
		binary[i] = '0';
	for(i=0; i < strlen(address); ++i)
	{
		if (address[i] == '0')
			strcat(binary, "0000");
		else if (address[i] == '1')
			strcat(binary, "0001");
		else if (address[i] == '2')
			strcat(binary, "0010");
		else if (address[i] == '3')
			strcat(binary, "0011");
		else if (address[i] == '4')
			strcat(binary, "0100");
		else if (address[i] == '5')
			strcat(binary, "0101");
		else if (address[i] == '6')
			strcat(binary, "0110");
		else if (address[i] == '7')
			strcat(binary, "0111");
		else if (address[i] == '8')
			strcat(binary, "1000");
		else if (address[i] == '9')
			strcat(binary, "1001");
		else if (address[i] == 'a' || address[i] == 'A')
			strcat(binary, "1010");
		else if (address[i] == 'b' || address[i] == 'B')
			strcat(binary, "1011");
		else if (address[i] == 'c' || address[i] == 'C')
			strcat(binary, "1100");
		else if (address[i] == 'd' || address[i] == 'D')
			strcat(binary, "1101");
		else if (address[i] == 'e' || address[i] == 'E')
			strcat(binary, "1110");
		else if (address[i] == 'f' || address[i] == 'F')
			strcat(binary, "1111");
	}
	return binary;
}

char *l1_get_tag(int l1_tag)
{
	char *l1_tagbits = (char *)malloc(sizeof(char) * 10000);
	char temp[l1_tag];
	for (int i = 0; i<l1_tag; ++i)
	{
		temp[i] = binary[i];
	}
	strcat(l1_tagbits,temp);	
	return l1_tagbits;
}


char *l1_get_index(int l1_index, int l1_tag)
{
	char *l1_indexbits = (char *)malloc(sizeof(char) * 10000);
	char temp[l1_index];
	int j = 0;
	for (int i = l1_tag; i< (l1_tag+l1_index); ++i)
	{
		temp[j] = binary[i];
		++j;
	}
	strcat(l1_indexbits,temp);	
	return l1_indexbits;
}


char *l1_get_offset(int l1_tag, int l1_index)
{
	char *l1_offsetbits = (char *)malloc(sizeof(char) * 10000);
	char temp[l1_offset];
	int j = 0;
	for (int i = (l1_tag+l1_index); i<(l1_tag+l1_index+l1_offset); ++i)
	{
		temp[j] = binary[i];
		++j;
	}	
	char temp1[l1_offset];								//Used a 2nd temp variable to avoid extra bits of 1st temp
	int k=0;
	for (int i = (l1_tag+l1_index); i<(l1_tag+l1_index+l1_offset); ++i)
	{
		temp1[k] = temp[k];
		++k;
	}	strcat(l1_offsetbits,temp1);		
	return l1_offsetbits;
}


l1_cache **create_cache(unsigned long int l1_assoc)
{
	int i=0;
	unsigned long int j=0;
	l1_cache **cache;
	cache = new l1_cache*[l1_sets];
	for(; i<l1_sets; i++)
	{
	cache[i] = new l1_cache[l1_assoc];
	for(;j<l1_assoc;j++)
	{
			cache[i][j].valid = 0;
			cache[i][j].tag = 0;
			cache[i][j].index = 0;
			cache[i][j].lru = 0;
			cache[i][j].dirty = 0;
			cache[i][j].status = 0;
			cache[i][j].tag1 = 0;
			cache[i][j].address = 0;
	}
	}	
	return cache;
}

l2_cache **create_l2cache(unsigned long int l2_assoc)
{
	int i;
	unsigned long int j;
	l2_cache **l2cache;
	l2cache = new l2_cache*[l2_sets];
	for(i=0; i<l2_sets; i++)
	{
	l2cache[i] = new l2_cache[l2_assoc];
	for(j=0;j<l2_assoc;j++)
	{
			l2cache[i][j].valid = 0;
			l2cache[i][j].tag = 0;
			l2cache[i][j].index = 0;
			l2cache[i][j].lru = 0;
			l2cache[i][j].dirty = 0;
			l2cache[i][j].status = 0;
			l2cache[i][j].tag1 = 0;
			l2cache[i][j].address = 0;
	}
	}
	return l2cache;
}


v_cache *create_vcache(unsigned long int vc_blocks)
{
	unsigned long int i=0;
	v_cache *vcache;
	vcache = new v_cache[vc_blocks];
	for(; i<vc_blocks; i++)
	{
			vcache[i].valid = 0;
			vcache[i].tag = 0;
			vcache[i].tag1 = 0;
			vcache[i].index = 0;
			vcache[i].lru = 0;
			vcache[i].dirty = 0;
			vcache[i].status = 0;
			vcache[i].address = 0;
	}
	return vcache;
}

// Binary to decimal
int binary_decimal(int n)
{
	int base = 1;
	int value = 0;
	int temp = n;
	while(temp)
	{
		int a = temp % 10;
		temp = temp/10;
		value+= a*base;
		base = base*2;
	}
	return value;
}


// Writing Cache
int write_cache(unsigned long int l1_tagbits_number, int l1_indexbits_number1, unsigned long int assoc)
{
	int index = l1_indexbits_number1;
	int i;
	unsigned long int j;
	for(i = index; i<=index; i++)
	{
		for(j = 0; j<assoc ; j++)
		{
			if(cache[i][j].valid == 1)
			{
				if(cache[i][j].tag == l1_tagbits_number)
				{
					write_hit++;
					cache[i][j].status = 1;
					cache[i][j].dirty = 1;
					cache[i][j].lru = update_lru();
					cache[i][j].tag1 = get_vcache_tag();
					cache[i][j].address = addr10;
					return j;
				}
			}
		}	
	}
	write_miss++;
	for(i=index;i<=index;i++)
	{
		for(j = 0; j<assoc ; j++)
		{
			if(cache[i][j].valid == 0)
			{	
				if(l2_size == 0)
				{
					cache[i][j].tag = 0;
					cache[i][j].valid = 0;
					cache[i][j].dirty = 0;
					cache[i][j].status = 0;
					cache[i][j].address = 0;
					cache[i][j].tag = l1_tagbits_number;
					cache[i][j].valid = 1;
					cache[i][j].dirty = 1;
					cache[i][j].status = 2;
					cache[i][j].lru = update_lru();
					cache[i][j].tag1 = get_vcache_tag();
					cache[i][j].address = addr10;
				}
				else if(l2_size > 0)
				{
					read_from_l2(l1_tagbits_number, index, j);
					cache[i][j].dirty = 1;
				}
				return j;
			}
		}
	}
	//evict block based on lru
	if(vc_blocks == 0 && l2_size == 0)
	{
		lru_block = evict_block(index, assoc);
		if (cache[index][lru_block].dirty == 1)
			l1_wb++;
		cache[index][lru_block].tag = l1_tagbits_number;
		cache[index][lru_block].tag1 = get_vcache_tag();
		cache[index][lru_block].valid = 1;
		cache[index][lru_block].dirty = 1;
		cache[index][lru_block].status = 3;
		cache[index][lru_block].lru = update_lru();
		cache[index][lru_block].address = addr10;
	}
	else if(vc_blocks > 0 && l2_size == 0)
	{
		lru_block = evict_block(index, assoc);
		search_vc(l1_tagbits_number, index, lru_block);
		cache[index][lru_block].dirty = 1;
	}
	else if(vc_blocks == 0 && l2_size > 0)
	{
		lru_block = evict_block(index, assoc);
		if (cache[index][lru_block].dirty == 1)
		{
			l1_wb++;
			wb_to_l2(l1_tagbits_number, index, lru_block);
		}
		read_from_l2(l1_tagbits_number, index, lru_block);
		cache[index][lru_block].dirty = 1;
	}
	else if(vc_blocks > 0 && l2_size > 0)
	{
		lru_block = evict_block(index, assoc);
		search_vc(l1_tagbits_number, index, lru_block);
		cache[index][lru_block].dirty = 1;
	}
	return lru_block;
}

int read_cache(unsigned long int l1_tagbits_number, int l1_indexbits_number1, unsigned long int assoc)
{
	int index = l1_indexbits_number1;
	int i;
	unsigned long int j;
	for(i = index; i<=index; i++)
	{
		for(j = 0; j<assoc ; j++)
		{
			if(cache[i][j].valid == 1)
			{
				if(cache[i][j].tag == l1_tagbits_number)
				{
					read_hit++;
					cache[i][j].status = 1;
					cache[i][j].lru = update_lru();
					cache[i][j].tag1 = get_vcache_tag();
					cache[i][j].address = addr10;
					return j;
				}
			}
		}
	}
	read_miss++;
	for(i=index;i<=index;i++)
	{
		for(j = 0; j<assoc ; j++)
		{
			if(cache[i][j].valid == 0)
			{
				if(l2_size == 0)
				{
					cache[i][j].tag = 0;
					cache[i][j].valid = 0;
					cache[i][j].dirty = 0;
					cache[i][j].address = 0;
					cache[i][j].tag = l1_tagbits_number;
					cache[i][j].valid = 1;
					cache[i][j].dirty = 0;
					cache[i][j].status = 2;
					cache[i][j].lru = update_lru();
					cache[i][j].tag1 = get_vcache_tag();
					cache[i][j].address = addr10;
				}
				else if(l2_size > 0)
				{
					read_from_l2(l1_tagbits_number, index, j);
					cache[i][j].dirty = 0;
				}
				return j;
			}
		}
	}
	//evict block based on lru
	if(vc_blocks == 0 && l2_size == 0)
	{
		lru_block = evict_block(index, assoc);
		if (cache[index][lru_block].dirty == 1)
			l1_wb++;
		cache[index][lru_block].tag = l1_tagbits_number;
		cache[index][lru_block].tag1 = get_vcache_tag();
		cache[index][lru_block].valid = 1;
		cache[index][lru_block].dirty = 0;
		cache[index][lru_block].status = 3;
		cache[index][lru_block].lru = update_lru();
		cache[index][lru_block].address = addr10;
	}
	else if(vc_blocks > 0 && l2_size == 0)
	{
		lru_block = evict_block(index, assoc);
		search_vc(l1_tagbits_number, index, lru_block);
	}
	else if(vc_blocks == 0 && l2_size > 0)
	{
		lru_block = evict_block(index, assoc);
		if (cache[index][lru_block].dirty == 1)
		{
			l1_wb++;
			wb_to_l2(l1_tagbits_number, index, lru_block);
		}
		read_from_l2(l1_tagbits_number, index, lru_block);
	}	
	else if(vc_blocks > 0 && l2_size > 0)
	{
		lru_block = evict_block(index, assoc);
		search_vc(l1_tagbits_number, index, lru_block);
	}
	return lru_block;
}

int update_lru()
{
	++lru_val;
	return lru_val;
}

int l2_update_lru()
{
	++l2_lru_val;
	return l2_lru_val;
}

int update_Vlru()
{
	++Vlru_val;
	return Vlru_val;
}

int evict_block(int index, unsigned long int assoc)
{
	int smallest = cache[index][0].lru;
	int index1 = 0;
	int i;
	unsigned long int j;
	for(i = index; i <= index; i++)
	{
		for(j=0;j<assoc;j++)
		{
			if(cache[i][j].lru < smallest)
			{
				smallest = cache[i][j].lru;
				index1 = j;
			}
		}
	}
	return index1;
}	

int l2_evict_block(int index, unsigned long int assoc)
{
	int smallest = l2cache[index][0].lru;
	int index1 = 0;
	int i;
	unsigned long int j;
	for(i = index; i <= index; i++)
	{
		for(j=0;j<l2_assoc;j++)
		{
			if(l2cache[i][j].lru < smallest)
			{
				smallest = l2cache[i][j].lru;
				index1 = j;
			}
		}
	}
	return index1;
}

void read_from_l2(unsigned long int l1_tagbits1, int l1_index1, int l1_lru_block)
{
	++l2_read;
	int index = l2_indexbits_number1;
	int i;
	unsigned long int j;
	int l2_tag2;
	unsigned long int temp1;
	for(i = index; i<=index; i++)
	{
		for(j = 0; j<l2_assoc ; j++)
		{
			if(l2cache[i][j].valid == 1)
			{
				temp1 = l2cache[i][j].address>>l1_offset;
				unsigned long int temp2;
				temp2 = addr10>>l2_offset;
				if(l2cache[i][j].tag == temp2>>l2_index)
				{

					l2_read_hit++;
					cache[l1_index1][l1_lru_block].tag = 0;
					cache[l1_index1][l1_lru_block].tag1 = 0;
					cache[l1_index1][l1_lru_block].valid = 0;
					cache[l1_index1][l1_lru_block].dirty = 0;
					cache[l1_index1][l1_lru_block].address = 0;
					cache[l1_index1][l1_lru_block].tag = l1_tagbits_number;
					cache[l1_index1][l1_lru_block].tag1 = get_vcache_tag();
					cache[l1_index1][l1_lru_block].valid = 1;
					if (rw == 'r')
						cache[l1_index1][l1_lru_block].dirty = 0;
					else if (rw == 'w')
						cache[l1_index1][l1_lru_block].dirty = 1;
					cache[l1_index1][l1_lru_block].status = 3;
					cache[l1_index1][l1_lru_block].lru = update_lru();
					l2cache[i][j].lru = l2_update_lru();
					cache[l1_index1][l1_lru_block].address = addr10;
					return;
				}
			}
		}	
	}
	l2_read_miss++;
	for(i=index;i<=index;i++)
	{
		for(j = 0; j<l2_assoc ; j++)
		{
			if(l2cache[i][j].valid == 0)
			{		
					unsigned long int temp100;
					//Allocating L2
					l2cache[i][j].tag = 0;
					l2cache[i][j].valid = 0;
					l2cache[i][j].dirty = 0;
					l2cache[i][j].status = 0;
					l2cache[i][j].tag1 = 0;
					l2cache[i][j].address = 0;
					temp100 = addr10>>l2_offset;
					l2_tag2 = temp100>>l2_index;
					l2cache[i][j].tag = l2_tag2;
					l2cache[i][j].valid = 1;
					l2cache[i][j].dirty = 0;
					l2cache[i][j].status = 2;
					l2cache[i][j].lru = l2_update_lru();
					l2cache[i][j].tag1 = get_vcache_tag();
					l2cache[i][j].address = addr10;
					
					// Allocating L1
					cache[l1_index1][l1_lru_block].tag = 0;
					cache[l1_index1][l1_lru_block].tag1 = 0;
					cache[l1_index1][l1_lru_block].valid = 0;
					cache[l1_index1][l1_lru_block].dirty = 0;
					cache[l1_index1][l1_lru_block].address = 0;
					cache[l1_index1][l1_lru_block].tag = l1_tagbits_number;
					cache[l1_index1][l1_lru_block].tag1 = get_vcache_tag();
					cache[l1_index1][l1_lru_block].valid = 1;
					if (rw == 'r')
						cache[l1_index1][l1_lru_block].dirty = 0;
					else if (rw == 'w')
						cache[l1_index1][l1_lru_block].dirty = 1;
					cache[l1_index1][l1_lru_block].status = 3;
					cache[l1_index1][l1_lru_block].lru = update_lru();
					cache[l1_index1][l1_lru_block].address = addr10;
					return;
			}
		}
	}
	l2_lru_block = l2_evict_block(index, l2_assoc);
	if(l2cache[index][l2_lru_block].dirty == 1)
		l2_wb_to_memory++;
	
	l2cache[index][l2_lru_block].tag = 0;
	l2cache[index][l2_lru_block].tag1 = 0;
	l2cache[index][l2_lru_block].valid = 0;
	l2cache[index][l2_lru_block].dirty = 0;
	l2cache[index][l2_lru_block].status = 0;
	l2cache[index][l2_lru_block].address = 0;
	//Allocating L2
	temp1 = addr10>>l2_offset;
	l2_tag2 = temp1>>l2_index;
	l2cache[index][l2_lru_block].tag = l2_tag2;
	l2cache[index][l2_lru_block].tag1 = get_vcache_tag();
	l2cache[index][l2_lru_block].valid = 1;
	l2cache[index][l2_lru_block].dirty = 0;
	l2cache[index][l2_lru_block].status = 3;
	l2cache[index][l2_lru_block].lru = l2_update_lru();
	l2cache[index][l2_lru_block].address = addr10;
	
	cache[l1_index1][l1_lru_block].tag = 0;
	cache[l1_index1][l1_lru_block].tag1 = 0;
	cache[l1_index1][l1_lru_block].valid = 0;
	cache[l1_index1][l1_lru_block].dirty = 0;
	cache[l1_index1][l1_lru_block].address = 0;
	// Allocating L1
	cache[l1_index1][l1_lru_block].tag = l1_tagbits_number;
	cache[l1_index1][l1_lru_block].tag1 = get_vcache_tag();
	cache[l1_index1][l1_lru_block].valid = 1;
	if (rw == 'r')
		cache[l1_index1][l1_lru_block].dirty = 0;
	else if (rw == 'w')
		cache[l1_index1][l1_lru_block].dirty = 1;
	cache[l1_index1][l1_lru_block].status = 3;
	cache[l1_index1][l1_lru_block].lru = update_lru();
	cache[l1_index1][l1_lru_block].address = addr10;	
	return;
}

	
void wb_to_l2(unsigned long int l1_tagbits1, int l1_index1, int l1_lru_block)
{
	++l2_write;
	unsigned long int addr6 = cache[l1_index1][l1_lru_block].address >> l2_offset;
	int index = l2_index?(addr6 & (int)(pow(2,l2_index)-1)):0;
	unsigned long int temp,temp1;
	int i;
	unsigned long int j;
	int l2_tag2;
	for(i = index; i<=index; i++)
	{
		for(j = 0; j<l2_assoc ; j++)
		{
			if(l2cache[i][j].valid == 1)
			{
				temp1 = cache[l1_index1][l1_lru_block].address>>l2_offset;
				temp = temp1>>l2_index;
				if(l2cache[i][j].tag == temp)
				{
					++l2_write_hit;
					l2cache[i][j].status = 1;
					l2cache[i][j].dirty = 1;
					l2cache[i][j].lru = l2_update_lru();
					l2cache[i][j].tag1 = get_vcache_tag();
					l2cache[i][j].address = cache[l1_index1][l1_lru_block].address;
					return;
				}
			}
		}	
	}
	++l2_write_miss;
	for(i=index;i<=index;i++)
	{
		for(j = 0; j<l2_assoc ; j++)
		{
			if(l2cache[i][j].valid == 0)
			{	
					l2cache[i][j].tag = 0;
					l2cache[i][j].valid = 0;
					l2cache[i][j].dirty = 0;
					l2cache[i][j].status = 0;
					l2cache[i][j].tag1 = 0;
					l2cache[i][j].address = 0;
					
					unsigned long int temp1;
					temp1 = cache[l1_index1][l1_lru_block].address>>l2_offset;
					l2_tag2 = temp1>>l2_index;
					l2cache[i][j].tag = l2_tag2;
					l2cache[i][j].valid = 1;
					l2cache[i][j].dirty = 1;
					l2cache[i][j].status = 2;
					l2cache[i][j].lru = l2_update_lru();
					l2cache[i][j].tag1 = get_vcache_tag();
					l2cache[i][j].address = cache[l1_index1][l1_lru_block].address;
					return;
			}
		}
	}
	l2_lru_block = l2_evict_block(index, l2_assoc);
	if(l2cache[index][l2_lru_block].dirty == 1)
		++l2_wb_to_memory;
	l2cache[index][l2_lru_block].tag = 0;
	l2cache[index][l2_lru_block].tag1 = 0;
	l2cache[index][l2_lru_block].valid = 0;
	l2cache[index][l2_lru_block].dirty = 0;
	l2cache[index][l2_lru_block].status = 0;
	l2cache[index][l2_lru_block].address = 0;
	
	temp1 = cache[l1_index1][l1_lru_block].address>>l2_offset;
	l2_tag2 = temp1>>l2_index;
	l2cache[index][l2_lru_block].tag = l2_tag2;
	l2cache[index][l2_lru_block].tag1 = get_vcache_tag();
	l2cache[index][l2_lru_block].valid = 1;
	l2cache[index][l2_lru_block].dirty = 1;
	l2cache[index][l2_lru_block].status = 3;
	l2cache[index][l2_lru_block].lru = l2_update_lru();
	l2cache[index][l2_lru_block].address = cache[l1_index1][l1_lru_block].address;
	return;
}

void wb_to_l2_vc(int vc_lru_block)
{
	++l2_write;
	unsigned long int addr6 = vcache[vc_lru_block].address >> l2_offset;
	int index = l2_index?(addr6 & (int)(pow(2,l2_index)-1)):0;
	unsigned long int temp,temp1;
	int i;
	unsigned long int j;
	int l2_tag2;
	for(i = index; i<=index; i++)
	{
		for(j = 0; j<l2_assoc ; j++)
		{
			if(l2cache[i][j].valid == 1)
			{
				temp1 = vcache[vc_lru_block].address>>l2_offset;
				temp = temp1>>l2_index;
				if(l2cache[i][j].tag == temp)
				{
					++l2_write_hit;
					l2cache[i][j].status = 1;
					l2cache[i][j].dirty = 1;
					l2cache[i][j].lru = l2_update_lru();
					l2cache[i][j].tag1 = get_vcache_tag();
					l2cache[i][j].address = vcache[vc_lru_block].address;
					return;
				}
			}
		}	
	}
	++l2_write_miss;
	for(i=index;i<=index;i++)
	{
		for(j = 0; j<l2_assoc ; j++)
		{
			if(l2cache[i][j].valid == 0)
			{	
					l2cache[i][j].tag = 0;
					l2cache[i][j].valid = 0;
					l2cache[i][j].dirty = 0;
					l2cache[i][j].status = 0;
					l2cache[i][j].tag1 = 0;
					l2cache[i][j].address = 0;
					unsigned long int temp2;
					temp2 = vcache[vc_lru_block].address>>l2_offset;
					l2_tag2 = temp2>>l2_index;
					l2cache[i][j].tag = l2_tag2;
					l2cache[i][j].valid = 1;
					l2cache[i][j].dirty = 1;
					l2cache[i][j].status = 2;
					l2cache[i][j].lru = l2_update_lru();
					l2cache[i][j].tag1 = get_vcache_tag();
					l2cache[i][j].address = vcache[vc_lru_block].address;
					return;
			}
		}
	}
	l2_lru_block = l2_evict_block(index, l2_assoc);
	if(l2cache[index][l2_lru_block].dirty == 1)
		++l2_wb_to_memory;
	l2cache[index][l2_lru_block].tag = 0;
	l2cache[index][l2_lru_block].tag1 = 0;
	l2cache[index][l2_lru_block].valid = 0;
	l2cache[index][l2_lru_block].dirty = 0;
	l2cache[index][l2_lru_block].status = 0;
	l2cache[index][l2_lru_block].address = 0;
	unsigned long int temp2;
	temp2 = vcache[vc_lru_block].address>>l2_offset;
	l2_tag2 = temp2>>l2_index;
	l2cache[index][l2_lru_block].tag = l2_tag2;
	l2cache[index][l2_lru_block].tag1 = get_vcache_tag();
	l2cache[index][l2_lru_block].valid = 1;
	l2cache[index][l2_lru_block].dirty = 1;
	l2cache[index][l2_lru_block].status = 3;
	l2cache[index][l2_lru_block].lru = l2_update_lru();
	l2cache[index][l2_lru_block].address = vcache[vc_lru_block].address;
	return;
}
	
void search_vc(unsigned long int l1_tagbits1, int l1_index1, int l1_lru_block)
{
	swap_req++;
	unsigned long int i;
	// Check for VC hit
	for (i=0; i< vc_blocks; i++)
	{
		if (vcache[i].valid == 1)
		{
			if (vcache[i].tag == addr10>>l1_offset)
			{
				swap++;
				// Swamp L1 cache & VC
				unsigned long int temp1, temp4, temp5, temp6, temp7;	
				int temp2,temp3;	
				temp6 = cache[l1_index1][l1_lru_block].address>>l1_offset;
				temp7 = vcache[i].tag>>l1_index;
				temp1  = temp6;
				cache[l1_index1][l1_lru_block].tag = temp7;
				vcache[i].tag = temp1;

				temp2  = cache[l1_index1][l1_lru_block].dirty;
				cache[l1_index1][l1_lru_block].dirty = vcache[i].dirty;
				vcache[i].dirty = temp2;

				temp3  = cache[l1_index1][l1_lru_block].valid;
				cache[l1_index1][l1_lru_block].valid = vcache[i].valid;
				vcache[i].valid = temp3;

				temp4  = cache[l1_index1][l1_lru_block].tag1;
				cache[l1_index1][l1_lru_block].tag1 = vcache[i].tag1;
				vcache[i].tag1 = temp4;
				
				temp5  = cache[l1_index1][l1_lru_block].address;
				cache[l1_index1][l1_lru_block].address = vcache[i].address;
				vcache[i].address = temp5;

				cache[l1_index1][l1_lru_block].lru = update_lru();
				vcache[i].lru = update_Vlru();
				return;
			}
		}
	}
	
	for (i=0; i<vc_blocks; i++)
	{
		if (vcache[i].valid == 0)
		{
			if(l2_size == 0)
			{
				vcache[i].tag = 0;
				vcache[i].dirty = 0;
				vcache[i].valid = 0;
				vcache[i].address = 0;
				vcache[i].valid = 1;
				vcache[i].tag = cache[l1_index1][l1_lru_block].address>>l1_offset;
				vcache[i].dirty = cache[l1_index1][l1_lru_block].dirty;
				vcache[i].lru = update_Vlru();
				vcache[i].tag1 = cache[l1_index1][l1_lru_block].tag1;
				vcache[i].address = cache[l1_index1][l1_lru_block].address;
				
				
				cache[l1_index1][l1_lru_block].tag = 0;
				cache[l1_index1][l1_lru_block].valid = 0;
				cache[l1_index1][l1_lru_block].dirty = 0;
				cache[l1_index1][l1_lru_block].status = 0;
				cache[l1_index1][l1_lru_block].address = 0;
				
				cache[l1_index1][l1_lru_block].tag = l1_tagbits_number;
				cache[l1_index1][l1_lru_block].valid = 1;
				if (rw == 'r')
					cache[l1_index1][l1_lru_block].dirty = 0;
				else if (rw == 'w')
					cache[l1_index1][l1_lru_block].dirty = 1;
				cache[l1_index1][l1_lru_block].status = 3;
				cache[l1_index1][l1_lru_block].lru = update_lru();
				cache[l1_index1][l1_lru_block].tag1 = get_vcache_tag();
				cache[l1_index1][l1_lru_block].address = addr10;
				
			}
			else if (l2_size > 0)
			{
				vcache[i].valid = 0;
				vcache[i].tag = 0;
				vcache[i].dirty = 0;
				vcache[i].tag1 = 0;
				vcache[i].address = 0;
				vcache[i].lru = 0;
				
				vcache[i].valid = 1;
				vcache[i].tag = cache[l1_index1][l1_lru_block].address>>l1_offset;
				vcache[i].dirty = cache[l1_index1][l1_lru_block].dirty;
				vcache[i].lru = update_Vlru();
				vcache[i].tag1 = cache[l1_index1][l1_lru_block].tag1;
				vcache[i].address = cache[l1_index1][l1_lru_block].address;
				// Searching L2 cache
				read_from_l2(l1_tagbits_number, l1_index1, l1_lru_block);
				if (rw == 'r')
					cache[l1_index1][l1_lru_block].dirty = 0;
				else if (rw == 'w')
					cache[l1_index1][l1_lru_block].dirty = 1;
			}
			return;
		}
	}
	// Make space for L1 cache block by evicting lru block of VC
	if(l2_size == 0)
	{
		Vlru_block = evict_vcblock();
		if(vcache[Vlru_block].dirty == 1)
			l1_wb++;
		vcache[Vlru_block].tag = 0;
		vcache[Vlru_block].dirty = 0;
		vcache[Vlru_block].valid = 0;
		vcache[Vlru_block].address = 0;
		
		vcache[Vlru_block].valid = 1;
		unsigned long int temp1;
		temp1 = cache[l1_index1][l1_lru_block].address>>l1_offset;
		vcache[Vlru_block].tag = temp1;
		vcache[Vlru_block].dirty = cache[l1_index1][l1_lru_block].dirty;
		vcache[Vlru_block].lru = update_Vlru();
		vcache[Vlru_block].tag1 = cache[l1_index1][l1_lru_block].tag1;
		vcache[Vlru_block].address = cache[l1_index1][l1_lru_block].address;

		cache[l1_index1][l1_lru_block].tag = 0;
		cache[l1_index1][l1_lru_block].valid = 0;
		cache[l1_index1][l1_lru_block].dirty = 0;
		cache[l1_index1][l1_lru_block].status = 0;
		cache[l1_index1][l1_lru_block].address = 0;		
		
		cache[l1_index1][l1_lru_block].tag = l1_tagbits_number;
		cache[l1_index1][l1_lru_block].valid = 1;
		if (rw == 'r')
			cache[l1_index1][l1_lru_block].dirty = 0;
		else if (rw == 'w')
			cache[l1_index1][l1_lru_block].dirty = 1;
		cache[l1_index1][l1_lru_block].status = 3;
		cache[l1_index1][l1_lru_block].lru = update_lru();
		cache[l1_index1][l1_lru_block].tag1 = get_vcache_tag();
		cache[l1_index1][l1_lru_block].address = addr10;
	}
	else if (l2_size > 0)
	{
		Vlru_block = evict_vcblock();
		if(vcache[Vlru_block].dirty == 1)
		{
			l1_wb++;
			wb_to_l2_vc(Vlru_block);
		}
		
		vcache[Vlru_block].tag = 0;
		vcache[Vlru_block].dirty = 0;
		vcache[Vlru_block].tag1 = 0;
		vcache[Vlru_block].address = 0;
		vcache[Vlru_block].lru = 0;
		
		vcache[Vlru_block].valid = 1;
		vcache[Vlru_block].tag = cache[l1_index1][l1_lru_block].address>>l1_offset;
		vcache[Vlru_block].dirty = cache[l1_index1][l1_lru_block].dirty;
		vcache[Vlru_block].lru = update_Vlru();
		vcache[Vlru_block].tag1 = cache[l1_index1][l1_lru_block].tag1;
		vcache[Vlru_block].address = cache[l1_index1][l1_lru_block].address;
		// Searching L2 cache
		read_from_l2(l1_tagbits_number, l1_index1, l1_lru_block);
		if (rw == 'r')
			cache[l1_index1][l1_lru_block].dirty = 0;
		else if (rw == 'w')
			cache[l1_index1][l1_lru_block].dirty = 1;
	}		
	return;
}

int evict_vcblock()
{
	int smallest = vcache[0].lru;
	int index1 = 0;
	unsigned long int i;
	for(i = 0; i < vc_blocks; i++)
	{
		if(vcache[i].lru < smallest)
		{
			smallest = vcache[i].lru;
			index1 = i;
		}
	}
	return index1;
}

unsigned long int get_vcache_tag()
{
	unsigned long int addr1 = strtoul(address, NULL, 16);
	unsigned long int addr2 = addr1 >> l1_offset;
	return addr2;
}


void print_order(int index, unsigned long int assoc)
{
	unsigned long int i,j;
	unsigned long int temp1, temp5, temp6;
	int temp2,temp3,temp4;
	for(i=0;i<assoc;i++)
	{		
		for(j=i+1;j<assoc;j++)
		{
			if(cache[index][i].lru < cache[index][j].lru)
			{
				temp1  = cache[index][i].tag;
				cache[index][i].tag = cache[index][j].tag;
				cache[index][j].tag = temp1;

				temp2  = cache[index][i].dirty;
				cache[index][i].dirty = cache[index][j].dirty;
				cache[index][j].dirty = temp2;

				temp3  = cache[index][i].valid;
				cache[index][i].valid = cache[index][j].valid;
				cache[index][j].valid = temp3;

				temp4  = cache[index][i].lru;
				cache[index][i].lru = cache[index][j].lru;
				cache[index][j].lru = temp4;

				temp5  = cache[index][i].tag1;
				cache[index][i].tag1 = cache[index][j].tag1;
				cache[index][j].tag1 = temp5;
				
				temp6  = cache[index][i].address;
				cache[index][i].address = cache[index][j].address;
				cache[index][j].address = temp6;
			}
		}
	}
}

void l2_print_order(int index, unsigned long int assoc)
{
	unsigned long int i,j;
	unsigned long int temp1, temp5, temp6;
	int temp2,temp3,temp4;
	for(i=0;i<assoc;i++)
	{		
		for(j=i+1;j<assoc;j++)
		{
			if(l2cache[index][i].lru < l2cache[index][j].lru)
			{
				temp1  = l2cache[index][i].tag;
				l2cache[index][i].tag = l2cache[index][j].tag;
				l2cache[index][j].tag = temp1;

				temp2  = l2cache[index][i].dirty;
				l2cache[index][i].dirty = l2cache[index][j].dirty;
				l2cache[index][j].dirty = temp2;

				temp3  = l2cache[index][i].valid;
				l2cache[index][i].valid = l2cache[index][j].valid;
				l2cache[index][j].valid = temp3;

				temp4  = l2cache[index][i].lru;
				l2cache[index][i].lru = l2cache[index][j].lru;
				l2cache[index][j].lru = temp4;

				temp5  = l2cache[index][i].tag1;
				l2cache[index][i].tag1 = l2cache[index][j].tag1;
				l2cache[index][j].tag1 = temp5;
				
				temp6  = l2cache[index][i].address;
				l2cache[index][i].address = l2cache[index][j].address;
				l2cache[index][j].address = temp6;
			}
		}
	}
}

void print_vorder()
{
	unsigned long int i,j;
	unsigned long int temp1, temp5, temp6;
	int temp2,temp3,temp4;
	for(i=0;i<vc_blocks;i++)
	{		
		for(j=i+1;j<vc_blocks;j++)
		{
			if(vcache[i].lru < vcache[j].lru)
			{
				temp1  = vcache[i].tag;
				vcache[i].tag = vcache[j].tag;
				vcache[j].tag = temp1;

				temp2  = vcache[i].dirty;
				vcache[i].dirty = vcache[j].dirty;
				vcache[j].dirty = temp2;

				temp3  = vcache[i].valid;
				vcache[i].valid = vcache[j].valid;
				vcache[j].valid = temp3;

				temp4  = vcache[i].lru;
				vcache[i].lru = vcache[j].lru;
				vcache[j].lru = temp4;

				temp5  = vcache[i].tag1;
				vcache[i].tag1 = vcache[j].tag1;
				vcache[j].tag1 = temp5;
				
				temp6  = vcache[i].address;
				vcache[i].address = vcache[j].address;
				vcache[j].address = temp6;
			}
		}
	}
}
