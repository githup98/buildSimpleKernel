#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"


// page table step of entries: 4096
// page directory step of entries: 4096*1024


static uint32_t* currentDirectory = 0;
struct paging4gbChunk* pagingNew4gb(uint8_t flags)
{
	//create page directory  (allocate 4 byte, 1024 entry points)
	uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
	uint32_t offset = 0;
	
	// create page entry from 1024 page directory
	for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
	{
		uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);

		for(int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++)
		{
			// create 4069 entries for 1 page directory, set propertise for all entries
			entry[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags ;
		}
		offset += PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
		directory[i] = (uint32_t)entry | PAGING_IS_WRITEABLE | flags;   // set properties for ent
	}
	// create chunk, (allocate memory);
	struct paging4gbChunk* _4gbChunk = kzalloc(sizeof(struct paging4gbChunk));
	_4gbChunk->directoryEntry = directory;
	return _4gbChunk;
}

void pagingSwitch(uint32_t* directory)
{
	pagingLoadDirectory(directory);
	currentDirectory = directory;
}

uint32_t* paging4gbChunkGetDirectory(struct paging4gbChunk* chunk)
{
	return chunk->directoryEntry;
}
//check alignment
bool pagingIsAligned(void* addr)
{
	return ((uint32_t)addr%PAGING_PAGE_SIZE) == 0;
}


int pagingGetIndex(void* virtualAddress, uint32_t* directoryIndexOut, uint32_t* tableIndexOut)
{
	int res = 0; // ret may be neg
	if(!pagingIsAligned(virtualAddress))
	{
		res = -EINVARG;
		return res;
	}
	//get out directory index and page table index out
	*directoryIndexOut = (uint32_t)virtualAddress /(PAGING_PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE);  //due to get 4 from 0x40xxxx = 4096_1024

	// get 5 from 0x405xxx by mod --> 5xxx --> div 4096 --> 5 (table index)
	*tableIndexOut = (uint32_t)virtualAddress % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE;  
	
	return res;
}

// at here we acctually map vir to random physical address, random phyiscal address for page entry only, not for directory entry
int pageSet(uint32_t* directory, void* virtualAddress, uint32_t physicalAddress)
{
	if(!pagingIsAligned(virtualAddress))
	{
		return -EINVARG;
	}
	
	//get directory index
	uint32_t directoryIndex = 0;
	uint32_t tableIndex = 0;
	int res = pagingGetIndex(virtualAddress, &directoryIndex, &tableIndex);
	if(res < 0)
	{
		return res;
	}
	//get directory to mask address
	uint32_t entry = directory[directoryIndex];
	uint32_t* table = (uint32_t*)(entry & 0xfffff000); //20 higher bits for address;
	table[tableIndex] = physicalAddress;


	return TD_OS_ALL_OK;
}
