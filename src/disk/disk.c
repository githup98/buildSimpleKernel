#include "io/io.h"
#include "disk.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"

struct disk disk;

int diskReadSector(int lba, int total, void* buf)
{
	//osdev ATA command matrix for detail
	outb(0x1f6, (lba >>24) | 0xe0); //send lba , 0xe0 indecated master
	outb(0x1f2, total); //total sector to read
	outb(0x1f3,(unsigned char)(lba & 0xff)); // 0x1f3->5 is send more lba
	outb(0x1f4,(unsigned char)(lba >> 8));
	outb(0x1f5,(unsigned char)(lba >> 16));
	outb(0x1f7, 0x20);  // read command

	unsigned short* ptr = (unsigned short*)buf;
	for(int i = 0; i < total; i++)
	{
		char c = insb(0x1f7); //read 1 byte from port 0x1f7
		while(!(c & 0x08))  //continue read
		{
			c = insb(0x1f7);
		}

		//
		for(int j = 0; j < 256; j++) // read 1 sector, 512 bytes
		{
			*ptr = insw(0x1F0); //read 2 byte, this 0x1f0 is refresh hard disk also
			ptr ++;
		}
	}
	return 0;
}

// update to read primary disk (0)
void diskSearchAndInit()
{
	memset(&disk, 0, sizeof(disk));
	disk.type = TDOS_DISK_TYPE_REAL;
	disk.sectorSize = TDOS_SECTOR_SIZE;
	disk.filesystem = fsResolve(&disk);
}

struct disk* diskGet(int index)
{
	// only access 0 (primary disk)
	if(index != 0)
	{
		return 0;
	}
	return &disk;
}

int diskReadBlock(struct disk* idisk, unsigned int lba, int total, void* buf)
{
	if(idisk != &disk)
	{
		return -EIO; // deny read disk
	}

	return diskReadSector(lba, total, buf);

}
