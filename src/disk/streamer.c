#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

// check primary disk or not (index = 0), allocate memory for diskStreamNew
struct diskStream* diskStreamNew(int index)
{
	struct disk* disk = diskGet(index);
	if(!disk)
	{
		return 0;
	}

	struct diskStream* streamer = kzalloc(sizeof(struct diskStream));
	streamer->pos = 0;
	streamer->disk = disk;

	return streamer;
}


int diskStreamSeek(struct diskStream* stream, int pos)
{
	stream->pos = pos;
	
	return 0;
}


int diskStreamRead(struct diskStream* stream, void* out, int total)
{
	//cal sector and pos in the sector want to read
	int sector = stream->pos / TDOS_SECTOR_SIZE;
	int offset = stream->pos % TDOS_SECTOR_SIZE;
	char buf[TDOS_SECTOR_SIZE];
	
	//readlba is logic block (sector's pos start to read)
	int res = diskReadBlock(stream->disk, sector, 1, buf); 	if(res < 0)
	{
		return res;
	}
	
	//check total and TDOS_SECTOR_SIZE to copy data to mem(RAM)
	int totalRead = total > TDOS_SECTOR_SIZE ? TDOS_SECTOR_SIZE : total;

	//copy data
	for(int i = 0; i < totalRead; i++)
	{
		*(char*)out++ = buf[offset + i];  //copy from byte want tto read, not all 512
	}
	
	//for total > TDOS_SECTOR_SIZE case, read again

	stream->pos += totalRead;
	if(total > TDOS_SECTOR_SIZE)
	{
		res = diskStreamRead(stream, out, total - TDOS_SECTOR_SIZE);
	}
	return res;

}

void diskStreamClose(struct diskStream* stream)
{
	kfree(stream);
}
