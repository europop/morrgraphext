#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

//#define MEMORY_POOL_LOGGING_ENABLED

//-----------------------------------------------------------------------------

// A pooled memory allocator based on the description in C++ for Game
// Programmers By Noel Llopis.
// Pooled memory allocators significantly speed up similarly sized memory
// allocations and dealocations by allocating large blocks of at a time
// instead of asking the OS to search for a new memory location every time a new
// object is needed.  It also allows you to free large chunks of memory at once
// if you don't need to call destructors on it.
class MemoryPool {
public:
	// Creates a memory pool to allocate objects of object_size or smaller.
	// Objects per block determines how much memory is allocated at a time.
	// The name can be useful for debugging.
	MemoryPool(
		const char* name,
		size_t object_size,
		size_t objects_per_block = 512
	);

	// Allocates a block of memory from the pool of the object size that was
	// passed into the constructor.  You could use it to store smaller objects,
	// but some memory would be wasted.
	// Note that no constructor will be called on objects you place into the
	// returned memory.
	void* Alloc();

	// Frees memory that was previously allocated from this pool.  It is very
	// important that the pointer that is passed in is an address that was
	// returned from the Alloc function on this pool and that is has not
	// already been freed by this function or the Flush function.  None of these
	// errors are checked for.
	// Note that no destructor will be called on any object in the memory, so if
	// that is necessary you have to do it yourself.
	void Free(void* ptr);

	// Causes the pool to forget all outstanding allocations and overwrite them.
	// This leaves the allocated memory in place for future allocations, but 
	// all objects allocated from this pool will be immediately invalidated and
	// no destructors will be called.  Only do this if you can guarantee that
	// the objects in the pool have already had their destructors called or do
	// not need to do anything in their destructors.  You should also make sure
	// nothing else contains pointers to any data that is being destroyed.
	void Flush();

	// The destructor calls Flush, so the same rules apply.  Destroying the
	// memory pool destroys all objects that were allocated by it.
	~MemoryPool();

private:
	struct MemoryBlock {
		MemoryBlock* next_block;
		unsigned char* data;
	};

	struct FreeMemorySlice {
		FreeMemorySlice* next;
		FreeMemorySlice* prev;
	};

	void _AllocNewBlock();
	void _FreeAllBlocks();
	void _ResetFreeList();
	void _ResetAllBlocks();

	const char* m_name;

	MemoryBlock* m_block_list_head;

	// A double linked list containing availiable memory slices
	FreeMemorySlice m_free_head;
	FreeMemorySlice m_free_tail;

	size_t m_slice_size;
	size_t m_block_size;
	size_t m_slices_per_block;

	static size_t m_system_page_size;
};

//-----------------------------------------------------------------------------

#endif