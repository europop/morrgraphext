// A basic double-linked list template class that uses the MemoryPool class for
// allocations.

#ifndef _POOLED_LIST_H_
#define _POOLED_LIST_H_

#include "MemoryPool.h"

//-----------------------------------------------------------------------------

template <class Type>
class PooledList {
public:
	PooledList( size_t pool_block_size = 512 );
	~PooledList();

	struct Item {
		Item* next;
		Item* prev;
		Type* data;
	};

	Type* CreateNewItem();

	inline bool IsEmpty();
	inline Item* GetFirstItem();
	inline Item* GetEndMarker();
	void FreeItem( Item* item );
	void SwapItems( Item* item1, Item* item2 );
	void FreeAllItems();

	void Sort( bool (*less)(const Type&, const Type&) );
protected:
	//Disable copy constructor
	PooledList( const PooledList& );

	void PooledList::QuickSort(
		Item* left,
		Item* right,
		bool (*less)(const Type&, const Type&)
	);

	MemoryPool m_data_pool;
	MemoryPool m_item_pool;
	Item m_head;
	Item m_tail;
};

//-----------------------------------------------------------------------------
// Template function definitions
//-----------------------------------------------------------------------------

template <class Type>
PooledList<Type>::PooledList( size_t pool_block_size ) :
	m_data_pool( "List Data Pool", sizeof(Type), pool_block_size ),
	m_item_pool( "List Item Pool", sizeof(Item), pool_block_size )
{

	// Connect the head and tail items to eachother
	m_head.prev = 0;
	m_head.next = &m_tail;
	
	m_tail.prev = &m_head;
	m_tail.next = 0;

	// Void their data pointers
	m_head.data = 0;
	m_tail.data = 0;
}

//-----------------------------------------------------------------------------

template <class Type>
PooledList<Type>::~PooledList() {
	FreeAllItems();
}

//-----------------------------------------------------------------------------

template <class Type>
Type* PooledList<Type>::CreateNewItem() {

	Item* new_item = (Item*)m_item_pool.Alloc();
	
	new_item->data = (Type*)m_data_pool.Alloc();

	//Use placement new to construct the data object
	::new(new_item->data) Type();

	//Add the new item to the end of the list
	new_item->next = &m_tail;
	new_item->prev = m_tail.prev;
	m_tail.prev->next = new_item;
	m_tail.prev = new_item;

	//Return the new item data
	return new_item->data;
}

//-----------------------------------------------------------------------------

template <class Type>
inline bool PooledList<Type>::IsEmpty() {
	return ( m_head.next == &m_tail );
}

//-----------------------------------------------------------------------------

template <class Type>
inline typename PooledList<Type>::Item* PooledList<Type>::GetFirstItem() {
	return m_head.next;
}

//-----------------------------------------------------------------------------

template <class Type>
inline typename PooledList<Type>::Item* PooledList<Type>::GetEndMarker() {
	return &m_tail;
}

//-----------------------------------------------------------------------------

template <class Type>
void PooledList<Type>::FreeItem( typename PooledList<Type>::Item* item ) {
	// Link up the items before and after this item to remove it from the list
	item->next->prev = item->prev;
	item->prev->next = item->next;

	// Call the destrutor explicitly
	item->data->~Type();

	// Free the memory
	m_pool.Free(item);

	// Decrement count
	--m_count;
}

//-----------------------------------------------------------------------------

template <class Type>
void PooledList<Type>::SwapItems(
	typename PooledList<Type>::Item* item1,
	typename PooledList<Type>::Item* item2
) {
	Type* temp = item1->data;
	item1->data = item2->data;
	item2->data = temp;
}

//-----------------------------------------------------------------------------

template <class Type>
void PooledList<Type>::FreeAllItems() {
	// Do nothing if the list is already empty
	if ( IsEmpty() ) {
		return;
	}

	// Look through all items and call the destructor on the data
	Item* item = GetFirstItem();
	while( item != GetEndMarker() ) {
		item->data->~Type();

		// Move the item pointer to the next item so we can free this item
		// without messing up the iteration
		Item* temp = item;
		item = item->next;

		//Free the pool memory
		m_data_pool.Free( temp->data );
		m_item_pool.Free( temp );
	}

	// Reconnect the head and tail items to eachother
	m_head.prev = 0;
	m_head.next = &m_tail;
	
	m_tail.prev = &m_head;
	m_tail.next = 0;
}

//-----------------------------------------------------------------------------

template <class Type>
void PooledList<Type>::Sort( bool (*less)(const Type&, const Type&) ) {
	// Check if there are no items
	if ( IsEmpty() ) {
		return;
	}

	//Check if there is only one item
	if ( m_head.next == m_tail.prev ) {
		return;
	}
	
	QuickSort( m_head.next, m_tail.prev, less );
}

//-----------------------------------------------------------------------------

template <class Type>
void PooledList<Type>::QuickSort(
	typename PooledList<Type>::Item* left,
	typename PooledList<Type>::Item* right,
	bool (*less)(const Type&, const Type&)
) {

	Type& pivot = *(right->data);

	// Return when the pointers meet
	if ( (right == left) || (right->next == left) ) {
		return;
	}

	Item* i = left->prev;
	Item* j = right;
	Item* p = left->prev;
	Item* q = right;

	while (true) {
		while ( less(*((i = i->next)->data), pivot) ) {}

		while ( less(pivot, *((j = j->prev)->data)) ) {
			if ( j == left ) {
				break;
			}
		}

		if ( i == j || i->prev == j ) {
			break;
		}
		SwapItems( i, j );

		if ( !less(*(i->data),pivot) && !less(pivot,*(i->data)) ) {
			p = p->next;
			SwapItems( p, i );
		}

		if ( !less(*(j->data),pivot) && !less(pivot,*(j->data)) ) {
			q = q->prev;
			SwapItems( q, j );
		}
	}

	SwapItems( i, right );
	j = i->prev;
	i = i->next;

	for (
		Item* k = left;
		(k != p) && (k->prev != p);
		k = k->next, j = j->prev
	) {
		SwapItems( k, j );
	}

	for (
		Item* k = right->prev;
		(k != q) && (k->next != q);
		k = k->prev, i = i->next
	) {
		SwapItems( k, i );
	}

	QuickSort( left, j, less );
	QuickSort( i, right, less );
}

//-----------------------------------------------------------------------------

#endif