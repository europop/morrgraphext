#include "MemoryPool.h"
#include <assert.h>
#include "windows.h"

#ifdef MEMORY_POOL_LOGGING_ENABLED
	#include <sstream>
	using namespace std;
#endif

//-----------------------------------------------------------------------------
// MemoryPool
//-----------------------------------------------------------------------------

size_t MemoryPool::m_system_page_size = 0;

//-----------------------------------------------------------------------------

MemoryPool::MemoryPool(
	const char* name,
	size_t object_size,
	size_t objects_per_block
) :
	m_block_list_head(0),
	m_slice_size(object_size),
	m_slices_per_block(objects_per_block),
	m_name(name)
{

	//Get the system page size, if it hasn't been found already
	if (!m_system_page_size) {
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		m_system_page_size = (size_t)si.dwPageSize;
	}

	// Object size must be at least big enough to hold the free slice struct
	if ( m_slice_size < sizeof(FreeMemorySlice) ) {
		m_slice_size = sizeof(FreeMemorySlice);
	}

	//Allign size to 32-byte boundaries
	if ( m_slice_size % 32 != 0 ) {
		m_slice_size += 32 - (m_slice_size % 32);
	}

	// Make sure we use a block size that's padded to 4096 to match the windows
	// memory page size.
	m_block_size = m_slices_per_block * m_slice_size;

	if ( m_block_size % m_system_page_size != 0 ) {
		m_block_size +=
			m_system_page_size - (m_block_size % m_system_page_size);
	}
	
	//Don't waste any space in the blocks, so re-calculate objects per block.
	m_slices_per_block = m_block_size / m_slice_size;

	// Connect the head and tail items in the free list to eachother
	_ResetFreeList();
	
	//Allocate a single memory block to start
	_AllocNewBlock();
}

//-----------------------------------------------------------------------------

void MemoryPool::_AllocNewBlock() {

	// Allocate a new block and insert it at the head of the linked list
	MemoryBlock* new_block = new MemoryBlock();
	
	//new_block->data = (unsigned char*)malloc(m_block_size);

	new_block->data = (unsigned char*)VirtualAlloc( 
		0,
		m_block_size,
		MEM_COMMIT,
		PAGE_READWRITE
	);

	#ifdef MEMORY_POOL_LOGGING_ENABLED
		stringstream ss;
		ss  << m_name << " : Allocated " << m_block_size << " bytes @ " << hex
			<< (size_t*)new_block << endl;
		OutputDebugString( ss.str().c_str() );
	#endif

	// This will be zero if we have run out of memory
	assert( new_block->data );

	// Add each slice into the linked list of free slices
	FreeMemorySlice* prev_slice = &m_free_head;
	FreeMemorySlice* new_slice = 0;
	for ( size_t i = 0; i < m_slices_per_block; ++i ) {
		// As long as they are free, use the the memory in the slices to hold
		// pointers to the next and previous free spots in the list.
		new_slice = (FreeMemorySlice*)&new_block->data[ i * m_slice_size ];

		//Add the new slice to the list
		new_slice->next = &m_free_tail;
		new_slice->prev = m_free_tail.prev;
		m_free_tail.prev->next = new_slice;
		m_free_tail.prev = new_slice;
	}

	// Insert the new completed block at the head of the linked list
	new_block->next_block = m_block_list_head;
	m_block_list_head = new_block;
}

//-----------------------------------------------------------------------------

void* MemoryPool::Alloc() {
	//Check if there is a free slice in the list
	if ( m_free_head.next == &m_free_tail) {
		// There are no free slices remaining, so we need a new memory block
		_AllocNewBlock();
		assert( m_free_head.next != &m_free_tail );
	}

	// Remove the first free slice from the list and return it
	FreeMemorySlice* free_slice = m_free_head.next;
	m_free_head.next = free_slice->next;
	m_free_head.next->prev = &m_free_head;

	return free_slice;
}

//-----------------------------------------------------------------------------

void MemoryPool::Free(void* ptr) {
	// We assume that the the given pointer is to an address that was allocated
	// by this this pool.  If it wasn't, then cross-references could develop,
	// or overlap could occur and that would really screw things up.  Might
	// want to eventually add a pointer to the heap that allocated an object
	// as a header in every slice in debug mode so that an assert could catch
	// this error

	//Insert the newly freed slice into the head of the free list
	FreeMemorySlice* slice = (FreeMemorySlice*)ptr;
	slice->prev = &m_free_head;
	slice->next = m_free_head.next;
	m_free_head.next->prev = slice;
	m_free_head.next = slice;
}

//-----------------------------------------------------------------------------

void MemoryPool::Flush() {
	//Remove all memory blocks
	_ResetAllBlocks();
}

//-----------------------------------------------------------------------------

void MemoryPool::_FreeAllBlocks() {
	//Traverse the linked list of blocks, freeing each one
	MemoryBlock* current_block = m_block_list_head;
	MemoryBlock* next_block = 0;
	while (current_block != 0) {
		next_block = current_block->next_block;

		// Free the block memory heap
		BOOL res = VirtualFree( current_block->data, 0, MEM_RELEASE );
		assert( res );
		current_block->data = 0;

		#ifdef MEMORY_POOL_LOGGING_ENABLED
		stringstream ss;
		if (res) {
			ss << m_name << " : Freeing " << m_block_size << " bytes @ " << hex
				<< (size_t*)current_block->data << endl;
		} else {
			ss << m_name << " : FAILED TO FREE " << m_block_size << " bytes @ "
				<< hex << (size_t*)current_block->data << endl;
		}
		OutputDebugString( ss.str().c_str() );
		#endif

		// Delete the block struct
		delete current_block;
		current_block = next_block;
	}

	m_block_list_head = 0;

	// Connect the head and tail items in the free list to eachother
	_ResetFreeList();
}

//-----------------------------------------------------------------------------

void MemoryPool::_ResetFreeList() {
	m_free_head.prev = 0;
	m_free_head.next = &m_free_tail;
	m_free_tail.prev = &m_free_head;
	m_free_tail.next = 0;
}

//-----------------------------------------------------------------------------

void MemoryPool::_ResetAllBlocks() {
	//Reset the head and tail items in the slice list
	_ResetFreeList();

	// Traverse the linked list of blocks, adding all memory in them to the
	// free list
	MemoryBlock* block = m_block_list_head;
	for ( /**/; block; block = block->next_block ) {
		// Add each slice into the linked list of free slices
		FreeMemorySlice* prev_slice = &m_free_head;
		FreeMemorySlice* new_slice = 0;
		for ( size_t i = 0; i < m_slices_per_block; ++i ) {
			// As long as they are free, use the the memory in the slices to hold
			// pointers to the next and previous free spots in the list.
			new_slice = (FreeMemorySlice*)&block->data[ i * m_slice_size ];

			//Add the new slice to the list
			new_slice->next = &m_free_tail;
			new_slice->prev = m_free_tail.prev;
			m_free_tail.prev->next = new_slice;
			m_free_tail.prev = new_slice;
		}
	}
}

//-----------------------------------------------------------------------------

MemoryPool::~MemoryPool() {
	_FreeAllBlocks();
}

//-----------------------------------------------------------------------------
