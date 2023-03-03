#ifndef FILE_H
#define FILE_H

#include "pParser.h"


typedef unsigned int FILE_SEEK_MODE;
enum
{
	SEEK_SET,
	SEEK_CUR,
	SEEK_END
};

typedef unsigned int FILE_MODE;
enum
{
	FILE_MODE_READ,
	FILE_MODE_WRITE,
	FILE_MODE_APPEND,
	FILE_MODE_INVALID
};

struct disk; // to dedicate where the file descriptor run on

//declare function pointer
// where to read (which disk), path file, read mode
typedef void* (*FS_OPEN_FUNCTION)(struct disk* disk, struct pathPart path, FILE_MODE mode);

//dedicate which disk's filesy //-stem is used or not (0: yes)
typedef int   (*FS_RESOLVE_FUNCTION)(struct disk* disk); 



struct filesystem
{
	FS_RESOLVE_FUNCTION resolve;
	FS_OPEN_FUNCTION    open;
	char name[30];
};


//file descriptor consist of index, filesystem, private data, disk (where file des run)
struct fileDescriptor
{
	int index;
	struct filesystem* filesystem;
	void* privateData;
	struct disk* disk;
};


void fsInit();
int fopen(const char* filename, const char* mode);
void fsInsertFilesystem(struct filesystem* filesystem);
struct filesystem* fsResolve(struct disk* disk);
#endif
