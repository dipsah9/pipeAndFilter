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
#define GET_OFFSET(addr) ((addr) & 0xFFF)

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


    // If the first_level entry is invalid, mark it valid (in reality we'd dynamically allocate a second-level table)
    if (first_level[l1_index] == INVALID_ENTRY) {
        // Just mark the index as "active" by storing a non-negative number (e.g., l1_index).
        // A real OS would store a pointer to the second-level table here.
        first_level[l1_index] = l1_index; 
    }

    // Set the second-level entry to the physical frame number
    second_level[l1_index][l2_index] = physical_frame;

}

// Translate a full virtual address to a "physical address" in our simulation
// For simplicity, our "physical address" is just (frame_number * PAGE_SIZE + offset)
int translate_address(unsigned int virtual_address, unsigned int *physical_address) {
    // Extract offset
    unsigned int offset = GET_OFFSET(virtual_address);
    // Extract indices
    unsigned int l1_idx = GET_L1_INDEX(virtual_address);
    unsigned int l2_idx = GET_L2_INDEX(virtual_address);

    // Check if there's a valid second-level table
    if (first_level[l1_idx] == INVALID_ENTRY) {
        return -1; // not mapped
    }

    // Get the physical frame from the second-level table
    int physical_frame = second_level[l1_idx][l2_idx];
    if (physical_frame == INVALID_ENTRY)
    {
        return -1; // not mapped
    }

    // Construct the simulated physical address
    // This is a simplified representation: (frame_number * PAGE_SIZE) + offset
    *physical_address = (physical_frame << 12) + offset;
    return 0; // success
}

int main() {
    // 1. Initialize the page tables
    init_page_tables();

    // 2. Let's map a few virtual pages to some physical frames
    // Example: map virtual page 0 to physical frame 100
    map_page(0, 100);
    // map virtual page 1 to physical frame 200
    map_page(1, 200);
    // map virtual page 512 to physical frame 50
    map_page(512, 50);

    // 3. Translate some virtual addresses
    // Address in virtual page 0, offset 100
    unsigned int va1 = (0 << 12) + 100;  // (page_number << 12) + offset
    unsigned int pa1;
    if (translate_address(va1, &pa1) == 0){
        printf("VA = %u (0x%x) -> PA = %u (0x%x)\n", va1, va1, pa1, pa1);
    }else{
        printf("VA = %u (0x%x) not mapped\n", va1, va1);
    }

    // Address in virtual page 1, offset 2048
    unsigned int va2 = (1 << 12) + 2048; // page_number 1, offset 2048
    unsigned int pa2;
    if (translate_address(va2, &pa2) == 0){
        printf("VA = %u (0x%x) -> PA = %u (0x%x)\n", va2, va2, pa2, pa2);
    } else{
        printf("VA = %u (0x%x) not mapped\n", va2, va2);
    }

    // Address in virtual page 512, offset 4095 (the last byte of that page)
    unsigned int va3 = (512 << 12) + 4095;
    unsigned int pa3;
    if(translate_address(va3, &pa3) == 0){
        printf("VA = %u (0x%x) -> PA = %u (0x%x)\n", va3, va3, pa3, pa3);
    }else{
        printf("VA = %u (0x%x) not mapped\n", va3, va3);
    }

    // Address in a page we did not map, e.g., page 1234
    unsigned int va4 = (1234 << 12) + 50;
    unsigned int pa4;
    if (translate_address(va4, &pa4) == 0){
        printf("VA = %u (0x%x) -> PA = %u (0x%x)\n", va4, va4, pa4, pa4);
    }else{
        printf("VA = %u (0x%x) not mapped\n", va4, va4);
    }

    return 0;
}