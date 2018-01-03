//
// CS252: MyMalloc Project
//
// The current implementation gets memory from the OS
// every time memory is requested and never frees memory.
//
// You will implement the allocator as indicated in the handout.
// 
// Also you will need to add the necessary locking mechanisms to
// support multi-threaded programs.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "MyMalloc.h"

#define ALLOCATED 1
#define NOT_ALLOCATED 0
#define ARENA_SIZE 2097152

pthread_mutex_t mutex;

static bool verbose = false;

extern void atExitHandlerInC()
{
  if (verbose)
    print();
}

static void * getMemoryFromOS(size_t size)
{
  // Use sbrk() to get memory from OS
  _heapSize += size;
 
  void *mem = sbrk(size);

  if(!_initialized){
      _memStart = mem;
  }

  return mem;
}


/*
 * @brief retrieves a new 2MB chunk of memory from the OS
 * and adds "dummy" boundary tags
 * @param size of the request
 * @return a FreeObject pointer to the beginning of the chunk
 */
static FreeObject * getNewChunk(size_t size)
{
  void *mem = getMemoryFromOS(size);

  // establish fence posts
  BoundaryTag *fencePostHead = (BoundaryTag *)mem;
  setAllocated(fencePostHead, ALLOCATED);
  setSize(fencePostHead, 0);

  char *temp = (char *)mem + size - sizeof(BoundaryTag);
  BoundaryTag *fencePostFoot = (BoundaryTag *)temp;
  setAllocated(fencePostFoot, ALLOCATED);
  setSize(fencePostFoot, 0);
 
  return (FreeObject *)((char *)mem + sizeof(BoundaryTag));
}

/**
 * @brief If no blocks have been allocated, get more memory and 
 * set up the free list
 */
static void initialize()
{
  verbose = true;

  pthread_mutex_init(&mutex, NULL);

  // print statistics at exit
  atexit(atExitHandlerInC);

  FreeObject *firstChunk = getNewChunk(ARENA_SIZE);

  // initialize the list to point to the firstChunk
  _freeList = &_freeListSentinel;
  setSize(&firstChunk->boundary_tag, ARENA_SIZE - (2*sizeof(BoundaryTag))); // ~2MB
  firstChunk->boundary_tag._leftObjectSize = 0;
  setAllocated(&firstChunk->boundary_tag, NOT_ALLOCATED);

  // link list pointer hookups
  firstChunk->free_list_node._next = _freeList;
  firstChunk->free_list_node._prev = _freeList;
  _freeList->free_list_node._prev = firstChunk;
  _freeList->free_list_node._next = firstChunk;

  _initialized = 1;
}

/**
 * @brief TODO: PART 1
 * This function should perform allocation to the program appropriately,
 * giving pieces of memory that large enough to satisfy the request. 
 * Currently, it just sbrk's memory every time.
 *
 * @param size size of the request
 *
 * @return pointer to the first usable byte in memory for the requesting
 * program
 */
static void * allocateObject(size_t size)
{
  pthread_mutex_unlock(&mutex); 
  // Make sure that allocator is initialized
  if (!_initialized) {
    initialize();
  }
  if (size <= 0 || size >= (ARENA_SIZE  - (3 * sizeof(BoundaryTag)))) {
	errno = ENOMEM; 
	return NULL;
  }
  size_t offset = (8 - (size % 8));
  if ((size % 8) == 0) {
    offset = 0;
  }
  size_t modified_size = size + offset + sizeof(BoundaryTag);
  if (modified_size < sizeof(FreeObject)) {
    modified_size = sizeof(FreeObject);
  }
  FreeObject * traverse = _freeList->free_list_node._next;
  pthread_mutex_lock(&mutex);
  while (traverse != _freeList) {
    if (getSize(&traverse->boundary_tag) >= modified_size) {
	int headerLoc = getSize(&traverse->boundary_tag) - modified_size;
	if (headerLoc >= sizeof(FreeObject)) {
      		FreeObject * f = (FreeObject *)((char *)traverse + headerLoc);
      		setSize(&f->boundary_tag, modified_size);
		setAllocated(&f->boundary_tag, ALLOCATED);
      		f->boundary_tag._leftObjectSize = headerLoc;
		FreeObject * right = (FreeObject *)((char *)f + modified_size);
		right->boundary_tag._leftObjectSize = modified_size;
		setSize(&traverse->boundary_tag, headerLoc);
		pthread_mutex_unlock(&mutex);
      		return (char *) f + sizeof(BoundaryTag);
	}
	else {
	        FreeObject * f = traverse;
		setSize(&f->boundary_tag, modified_size);
		setAllocated(&f->boundary_tag, ALLOCATED);
		f->free_list_node._prev->free_list_node._next = f->free_list_node._next;
		f->free_list_node._next->free_list_node._prev = f->free_list_node._prev;
		pthread_mutex_unlock(&mutex);
		return (char *) f + sizeof(BoundaryTag);	
	}
    }
    traverse = traverse->free_list_node._next;
  }
  FreeObject * newChunk = getNewChunk(ARENA_SIZE);
  setSize(&newChunk->boundary_tag, ARENA_SIZE - (2 * sizeof(BoundaryTag)));
  FreeObject * temp = _freeListSentinel.free_list_node._next;
  _freeListSentinel.free_list_node._next = newChunk;
  newChunk->free_list_node._prev = &_freeListSentinel;
  newChunk->free_list_node._next = temp;
  temp->free_list_node._prev = newChunk;
  pthread_mutex_unlock(&mutex);
  return allocateObject(size);
}

/**
 * @brief TODO: PART 2
 * This funtion takes a pointer to memory returned by the program, and
 * appropriately reinserts it back into the free list.
 * You will have to manage all coalescing as needed
 *
 * @param ptr
 */
static void freeObject(void *ptr)
{
  FreeObject * f = (FreeObject *)((char *) ptr - sizeof(BoundaryTag));
  FreeObject * left = (FreeObject *)((char *) f - (f->boundary_tag._leftObjectSize));
  FreeObject * right = (FreeObject *)((char *) f + getSize(&f->boundary_tag));
  size_t merge_size;
  size_t left_size;
  if (!isAllocated(&left->boundary_tag) || !isAllocated(&right->boundary_tag)) {
    if (!isAllocated(&left->boundary_tag)) {
      merge_size = getSize(&f->boundary_tag) + getSize(&left->boundary_tag);
      left_size = left->boundary_tag._leftObjectSize;
      f = (FreeObject *)((char *) f - getSize(&left->boundary_tag));
      setSize(&f->boundary_tag, merge_size);
      f->boundary_tag._leftObjectSize = left_size;
      setAllocated(&f->boundary_tag, NOT_ALLOCATED);
    }
    if (!isAllocated(&right->boundary_tag)) {
      merge_size = getSize(&f->boundary_tag) + getSize(&right->boundary_tag);
      //left_size = f->boundary_tag._leftObjectSize;
      setSize(&f->boundary_tag, merge_size);
      //f->boundary_tag._leftObjectSize = left_size;
      setAllocated(&f->boundary_tag, NOT_ALLOCATED);
      f->free_list_node._next = right->free_list_node._next;
      right->free_list_node._next->free_list_node._prev = f;
    }
    _freeListSentinel.free_list_node._next = f;
    f->free_list_node._prev = &_freeListSentinel;
    return;
  }
  setAllocated(&f->boundary_tag, NOT_ALLOCATED);
  FreeObject * temp = _freeListSentinel.free_list_node._next;
  _freeListSentinel.free_list_node._next = f;
  f->free_list_node._prev = &_freeListSentinel;
  f->free_list_node._next = temp;
  temp->free_list_node._prev = f;
  return;
}

void print()
{
  printf("\n-------------------\n");

  printf("HeapSize:\t%zd bytes\n", _heapSize);
  printf("# mallocs:\t%d\n", _mallocCalls);
  printf("# reallocs:\t%d\n", _reallocCalls);
  printf("# callocs:\t%d\n", _callocCalls);
  printf("# frees:\t%d\n", _freeCalls);

  printf("\n-------------------\n");
}

void print_list()
{
    printf("FreeList: ");
    if (!_initialized) 
        initialize();
    FreeObject *ptr = _freeList->free_list_node._next;
    while (ptr != _freeList) {
        long offset = (long)ptr - (long)_memStart;
        printf("[offset:%ld,size:%zd]", offset, getSize(&ptr->boundary_tag));
        ptr = ptr->free_list_node._next;
        if (ptr != NULL)
            printf("->");
    }
    printf("\n");
}

void increaseMallocCalls() { _mallocCalls++; }

void increaseReallocCalls() { _reallocCalls++; }

void increaseCallocCalls() { _callocCalls++; }

void increaseFreeCalls() { _freeCalls++; }

//
// C interface
//

extern void * malloc(size_t size)
{
  pthread_mutex_lock(&mutex);
  increaseMallocCalls();
  
  return allocateObject(size);
}

extern void free(void *ptr)
{
  pthread_mutex_lock(&mutex);
  increaseFreeCalls();
  
  if (ptr == 0) {
    // No object to free
    pthread_mutex_unlock(&mutex);
    return;
  }
  
  freeObject(ptr);
}

extern void * realloc(void *ptr, size_t size)
{
  pthread_mutex_lock(&mutex);
  increaseReallocCalls();

  // Allocate new object
  void *newptr = allocateObject(size);

  // Copy old object only if ptr != 0
  if (ptr != 0) {
    
    // copy only the minimum number of by
    FreeObject *o = (FreeObject *)((char *) ptr - sizeof(BoundaryTag));
    size_t sizeToCopy = getSize(&o->boundary_tag);
    if (sizeToCopy > size) {
      sizeToCopy = size;
    }

    memcpy(newptr, ptr, sizeToCopy);

    //Free old object
    freeObject(ptr);
  }

  return newptr;
}

extern void * calloc(size_t nelem, size_t elsize)
{
  pthread_mutex_lock(&mutex);
  increaseCallocCalls();

  // calloc allocates and initializes
  size_t size = nelem * elsize;

  void *ptr = allocateObject(size);

  if (ptr) {
    // No error
    // Initialize chunk with 0s
    memset(ptr, 0, size);
  }

  return ptr;
}
    
