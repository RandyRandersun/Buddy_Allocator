# Buddy_Allocator
Custom memory allocator in C++ using the buddy system.

# Planned Optimizations
This allocator can be sped up by replacing the linked list with a double linked list. This comes with no space disadvantage because the previous pointer is only used when the memory is not in use. You can give the user that space as well, leaving them with the same amount of space they had with the linked list, but much faster. This just requires a few changes to the pointer arithmatic when taking memory out of the freelist. 

# Project Architecture Rundown

## Ackerman.cpp & Ackerman.h

Used for stress testing the allocator.

## BuddyAllocator.cpp & BuddyAllocator.h

The allocator itself.

## Main.cpp

Driver of the allocator and unit tests.

