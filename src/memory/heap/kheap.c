#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"
struct heap kernelHeap;
struct heapTable kernelHeapTable;

//initt kheap

void kheapInit()
{
	//the kernelHeap no need to init, we init kernelHeapTable only
	
	//cal total table entries point
	int totalTableEntries = TD_OS_HEAP_SIZE_BYTES / TD_OS_HEAP_BLOCK_SIZE ;
	
	// check osdev memory map for detail
	
	kernelHeapTable.entries = (HEAP_BLOCK_TABLE_ENTRY *)TD_OS_HEAP_TABLE_ADDRESS;
	kernelHeapTable.total = totalTableEntries;

	void* end = (void*)(TD_OS_HEAP_ADDRESS + TD_OS_HEAP_SIZE_BYTES);
	
	//create a heap
	int res = heapCreate(&kernelHeap, (void*)TD_OS_HEAP_ADDRESS, end, &kernelHeapTable);
	if(res < 0)
	{
		print("Failed to create heap!\n");
	}

}
void* kmalloc(size_t size)
{
	return heapMalloc(&kernelHeap, size);
}

void kfree(void* ptr)
{
	heapFree(&kernelHeap, ptr);
}
