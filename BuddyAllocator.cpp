#include "BuddyAllocator.h"
#include <iostream>
#include <math.h>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  basic_block_size = _basic_block_size;
  total_memory_size = _total_memory_length;
  begin = new char[_total_memory_length+sizeof(BlockHeader)];

  int currSize = _basic_block_size;
  int index = 0;
  while(currSize != _total_memory_length){
    index++;
    currSize *=2;
  }
  FreeList.resize(index+1);
  for(int i=0;i<index+1;++i){
    FreeList[i].head = nullptr;
  }
  FreeList[index].head = (BlockHeader*)begin;
  FreeList[index].head->block_size = _total_memory_length;
  FreeList[index].head->free = true;
  FreeList[index].head->next = nullptr;
}

BuddyAllocator::~BuddyAllocator (){
	delete [] begin;
}

int BuddyAllocator::getIndex(int neededSize){
  int index = ceil(log2(static_cast<double>(neededSize)/static_cast<double>(basic_block_size)));
  if(index<0)return 0;
  return index;
}

BlockHeader* BuddyAllocator::getbuddy(BlockHeader *block){
  return (BlockHeader*) ((((char*)block - (char*)begin) ^ block->block_size) + (char*)begin);
}

BlockHeader* BuddyAllocator::cut(BlockHeader *to_cut){
  //cast char* to the block we are cutting, move the pointer over to the middle and then recast it to blockheader for returning and management
  BlockHeader *ret = (BlockHeader*) ((char*)to_cut + (to_cut->block_size/2));
  ret->block_size = to_cut->block_size/2;
  ret->free = true;
  to_cut->block_size = ret->block_size;
  return ret;
}

char* BuddyAllocator::alloc(int _length) {
  int needSize = _length+sizeof(BlockHeader);
  int index = getIndex(needSize);
  if(FreeList[index].head != nullptr){
    BlockHeader *temp = FreeList[index].head;
    FreeList[index].remove(temp);
    temp->free = false;
    temp->next = nullptr;
    return (char*)temp+sizeof(BlockHeader);
  }
  else{
    int largerIndex = -1;
    for(int i=index;i<FreeList.size();++i){
      if(FreeList[i].head != nullptr){
        largerIndex = i;
        break;
      }
    }
    if(largerIndex == -1) return nullptr;
    BlockHeader *temp = FreeList[largerIndex].head;
    while(largerIndex > index){
      temp = FreeList[largerIndex].head;
      FreeList[largerIndex].remove(temp);
      BlockHeader *temp_buddy = cut(temp);
      temp->next = nullptr;
      FreeList[largerIndex-1].insert(temp);
      FreeList[largerIndex-1].insert(temp_buddy);
      --largerIndex;
    }
    FreeList[index].remove(temp);
    temp->next = nullptr;
    temp->free = false;
    return (char*)temp+sizeof(BlockHeader);
  }
}

BlockHeader* BuddyAllocator::splice(BlockHeader *a,BlockHeader *b){
  BlockHeader *spliceHead = b;
  if(a<b){
    spliceHead = a;
  }
  spliceHead->block_size = a->block_size*2;
  spliceHead->free = true;
  return spliceHead;
}

bool BuddyAllocator::arebuddies(BlockHeader *block1,BlockHeader *block2){
  return block1->free && block2->free && block1->block_size == block2->block_size;
}

int BuddyAllocator::free(char* _a) {
  BlockHeader *temp = (BlockHeader*)(_a-sizeof(BlockHeader));
  temp->free = true;
  int index = getIndex(temp->block_size);
  FreeList[index].insert(temp);
  while(temp->block_size<total_memory_size){
    BlockHeader *temp_buddy = getbuddy(temp);
    if(arebuddies(temp,temp_buddy)){
      FreeList[index].remove(temp);
      FreeList[index].remove(temp_buddy);
      BlockHeader* spliced = splice(temp,temp_buddy);
      FreeList[index+1].insert(spliced);
      temp = spliced;
      index++;
    }
    else{
      return 0;
    }
  }
  return 0;
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  for (int i=0; i<FreeList.size(); i++){
    cout << "[" << i <<"] (" << ((1<<i) * basic_block_size) << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    while (b){
      count ++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != (1<<i) * basic_block_size){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;  
  }
}

