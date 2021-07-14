#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <unistd.h>
#include <cstdlib>

void easytest(BuddyAllocator* ba){
  ba->printlist();

  char * mem = ba->alloc (1);
  ba->printlist ();

  ba->free (mem); 
  ba->printlist(); 
}

int main(int argc, char ** argv) {

  int basic_block_size = 128, memory_length = 128 * 1024 * 1024;
  int c;
  try{
    while((c=getopt(argc,argv,"b:s:"))!=-1){
      switch(c){
      case 'b':{
        basic_block_size = atoi(optarg);
        break;
      }

      case 's':{
        memory_length = atoi(optarg);
        break;
      }
      }
    }
  }
  catch(...){
    cout<<"An error has occured"<<endl;
    return 0;
  }
  // create memory manager
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);

  easytest (allocator);

  
  // stress-test the memory manager, do this only after you are done with small test cases
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test. 
  
  // destroy memory manager
  delete allocator;
}
