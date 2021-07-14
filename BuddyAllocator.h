#ifndef _BuddyAllocator_h_                  
#define _BuddyAllocator_h_
#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int uint;

class BlockHeader{
public:
	int block_size;  // size of the block
	BlockHeader* next; // pointer to the next block
	bool free;
};

class LinkedList{
public:
	BlockHeader* head;		
public:
	void insert(BlockHeader* b){
		if(!head){
			b->next = nullptr;
		}
		else{
			b->next = head;
		}
		head = b;
		b->free = true;
	}

	void remove(BlockHeader* b){
		BlockHeader *temp = head;
		if(!temp)return;
		if(head==b){
			if(head->next == nullptr){
				head = nullptr;
			}
			else{
				head=head->next;
				b->free=false;
			}
		}
		else{
			while(temp->next != nullptr){
				if(temp->next == b){
					if(temp->next->next != nullptr){
						temp->next=temp->next->next;
					}
					else{
						temp->next=nullptr;
					}
				}
				else{
					temp = temp->next;
				}
			}
		}
		b->next = nullptr;
	}
};


class BuddyAllocator{
private:
	char *begin;
	vector<LinkedList> FreeList;
	int basic_block_size;
	int total_memory_size;

private:
	BlockHeader* getbuddy (BlockHeader * addr); 
	// given a block address, this function returns the address of its buddy 
	
	bool arebuddies (BlockHeader* block1, BlockHeader* block2);
	// checks whether the two blocks are buddies are not

	BlockHeader* splice (BlockHeader* block1, BlockHeader* block2);
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

	BlockHeader* cut (BlockHeader* block);
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected
	int getIndex(int neededSize);

public:
	BuddyAllocator (int _basic_block_size, int _total_memory_length); 
	/* This initializes the memory allocator and makes a portion of 
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
	   its minimal unit of allocation. The function returns the amount of 
	   memory made available to the allocator. If an error occurred, 
	   it returns 0. 
	*/ 

	~BuddyAllocator(); 
	/* Destructor that returns any allocated memory back to the operating system. 
	   There should not be any memory leakage (i.e., memory staying allocated).
	*/ 

	char* alloc(int _length); 
	/* Allocate _length number of bytes of free memory and returns the 
		address of the allocated portion. Returns 0 when out of memory. */ 

	int free(char* _a); 
	/* Frees the section of physical memory previously allocated 
	   using ’my_malloc’. Returns 0 if everything ok. */ 
   
	void printlist ();
	/* Mainly used for debugging purposes and running short test cases */
	/* This function should print how many free blocks of each size belong to the allocator
	at that point. The output format should be the following (assuming basic block size = 128 bytes):

	[0] (128): 5
	[1] (256): 0
	[2] (512): 3
	[3] (1024): 0
	....
	....
	 which means that at this point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif 
