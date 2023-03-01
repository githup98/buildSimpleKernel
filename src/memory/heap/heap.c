#include "heap.h"
#include "kernel.h"
#include "memory/memory.h"
#include "status.h"

#include <stdbool.h>


// check alignment, other hand it has multi with 4096 or not?

static bool heapValidateTable(void* ptr, void* end, struct heapTable* table)
{
	int res = 0;
	size_t tableSize = (size_t)(end - ptr);
	size_t totalBlock = tableSize / TD_OS_HEAP_BLOCK_SIZE;
	
	if(table->total != totalBlock)
	{
		res = -EINVARG;
		goto out;
	}

	out:	
		return res;
}


static bool heapValidateAlignment(void* ptr)
{
	return ((unsigned int)ptr % TD_OS_HEAP_BLOCK_SIZE) == 0;
}

int heapCreate(struct heap* heap, void* ptr, void* end, struct heapTable* table)
{
	int res = 0;
	if(!heapValidateAlignment(ptr) || !heapValidateAlignment(end))
	{
		res = -EINVARG;
		goto out;
	}
	// alignment OK
	memset(heap, 0, sizeof(struct heap));
	heap->saddr = ptr;
	heap->table = table;
	
	res = heapValidateTable(ptr, end, table);
	if(res < 0)
	{
		goto out;
	}

	// init value to 0
	size_t tableSize = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
	memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, tableSize);

	out:
	return res;
}

//modify value to fix alignment (valBefore will <= valAfter --> upper)
static uint32_t heapAlignValToUpper(uint32_t val)
{
	if((val % TD_OS_HEAP_BLOCK_SIZE) == 0)
	{
		return val;
	}

	val = val - (val % TD_OS_HEAP_BLOCK_SIZE);
	val += TD_OS_HEAP_BLOCK_SIZE;
	return val;
}

static int heapGetEntryType(HEAP_BLOCK_TABLE_ENTRY entry)
{
	return entry & 0x0f ; //get 4 lower bits
}

int heapGetStartBlock(struct heap* heap, uint32_t totalBlocks)
{
	struct heapTable* table = heap->table;
	int bs = -1; //block start
	int bc = 0; //block current (or block count)
	for(size_t i = 0; i < table->total; i++)
	{
		if(heapGetEntryType(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE)
		{
			bs = -1;
			bc = 0;
			continue;
		}	
		if(bs == -1)
		{
			bs = i;
		}
		bc++;
		if(bc == totalBlocks)
		{
			break;
		}
	}
	if(bs == -1)
	{
		return -ENOMEM;
	}
	return bs;
}

void* heapBlockToAddress(struct heap* heap, int noBlock)
{
	//start address of heap table + pos block*4096
	return heap->saddr + (noBlock * TD_OS_HEAP_BLOCK_SIZE);
}
void heapMarkBlockTaken(struct heap* heap, int startBlock, int totalBlocks)
{
	int endBlock = (startBlock + totalBlocks) - 1;

	HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
	
	if(totalBlocks > 1)
	{
		entry |= HEAP_BLOCK_HAS_NEXT;
	}

	for(int i = 0; i <=	endBlock; i++)
	{
		heap->table->entries[i] = entry;
		entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
		if(i != endBlock - 1)
		{
			entry |= HEAP_BLOCK_HAS_NEXT;
		}
	}
}

void* heapMallocBlocks(struct heap* heap, uint32_t totalBlocks)
{
	void* address = 0;
	int startBlock = heapGetStartBlock(heap, totalBlocks);
	if(startBlock < 0)
	{
		goto out;
	}
	address = heapBlockToAddress(heap, startBlock);
	// Mark the block as taken
	heapMarkBlockTaken(heap, startBlock, totalBlocks);
	out:
		return address;
}


void heapMarkBlocksFree(struct heap* heap, int startingBlock)
{
	struct heapTable* table = heap->table;
	for(int i = startingBlock; i < (int)table->total; i++)
	{
		HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
		table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
		if(!(entry & HEAP_BLOCK_HAS_NEXT))
		{
			break;
		}
	}
}

int heapAddressToBlock(struct heap* heap, void* address)
{
	return ((int)(address -heap->saddr)) / TD_OS_HEAP_BLOCK_SIZE;
}


void* heapMalloc(struct heap* heap, size_t size)
{
	size_t alignedSize = heapAlignValToUpper(size);
	uint32_t totalBlocks = alignedSize / TD_OS_HEAP_BLOCK_SIZE;
	return heapMallocBlocks(heap, totalBlocks);
}

void heapFree(struct heap* heap, void* ptr)
{
	heapMarkBlocksFree(heap, heapAddressToBlock(heap, ptr));
}
