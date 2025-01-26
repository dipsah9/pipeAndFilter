#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PAGE_SIZE 4096
#define NUM_PAGES (1 << 20)
#define L1_SIZE 1024
#define L2_SIZE 1024
#define INVALID_ENTRY -1

//Extract bits from a 32-bit virtual address
//offset (lowest 12 bits)
#define GET_OFFSET (addr) ((addr) & 0xFFF)

//second-level index (next 10 bits)
#define GET_L2_INDEX(addr)     (((addr) >> 12) & 0x3FF)

//first-level index (top 10 bits)
#define GET_L1_INDEX(addr) (((addr) >> 22) & 0x3FFF)

typedef int page_table_entry;

page_table_entry first_level [L1_SIZE];
page_table_entry second_level[L1_SIZE][L2_SIZE];

//initialize all the entries to INVALID

void init_page_tables(){
    for(int i = 0; i < L1_SIZE; i++){
        first_level[i] = INVALID_ENTRY;
        for(int j = 0; j < L2_SIZE; j++){
            second_level[i][j] = INVALID_ENTRY;
        }
    }
}

//map a virtual page to a physical frame.
// virtual_page: the virtual page number
//physical frame: the physical frame number (just an interger in our simulation)
//in a real os, we will also store permission bits (read-only and read write both)

void map_page(unsigned int virtual_page, int physical_frame){
    //split the virtual pages into l1 and l2 indices
    unsigned int l1_index = (virtual_page  >> 10) &0x3FF; //top 10 bits of the virtual page
    unsigned int l2_index = (virtual_page & 0x3FF); //bottom 10 bits of the virtual page
    



}