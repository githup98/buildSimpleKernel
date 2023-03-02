#ifndef DISK_H
#define DISK_H

typedef unsigned int TDOS_DISK_TYPE;

#define TDOS_DISK_TYPE_REAL 0

struct disk
{
	TDOS_DISK_TYPE type;
	int sectorSize;
};

void diskSearchAndInit();
struct disk* diskGet(int index);
int diskReadBlock(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif
