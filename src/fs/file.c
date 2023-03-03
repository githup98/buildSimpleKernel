#include "file.h"
#include "config.h"
#include "status.h"
#include "kernel.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"

struct filesystem* filesystems[TDOS_MAX_FILESYSTEM];

struct fileDescriptor* fileDescriptors[TDOS_MAX_FILE_DESCRIPTORS];


// check where filesystem is ready to use or not
static struct filesystem** fsGetFreeFilesystem()
{
	int i = 0;
	for(i = 0; i <TDOS_MAX_FILESYSTEM; i++)
	{
		if(filesystems[i] == 0)
		{
			return &filesystems[i] ; //use &filesystem dule to filesystem**
		}
	}
	return 0; //if no filesystem is free
}

void fsInsertFilesystem(struct filesystem* filesystem)
{
	struct filesystem** fs;
	fs = fsGetFreeFilesystem();
	if(!fs)
	{
		print("Failed to insert file system!");
		while(1){}
	}
	
	*fs = filesystem; //??for what?
}


static void fsStaticLoad()
{
	//fsInsertFilesystem(fat16Init());
}

void fsLoad()
{
	memset(filesystems, 0, sizeof(filesystems));
	fsStaticLoad();
}

void fsInit()
{
	memset(fileDescriptors, 0, sizeof(fileDescriptors));
	fsLoad();
}


static int fsNewDescriptor(struct fileDescriptor** descOut)
{
	int res = -ENOMEM; //respone default
	for(int i = 0; i < TDOS_MAX_FILE_DESCRIPTORS; i++)
	{
		if(fileDescriptors[i] == 0)
		{
			struct fileDescriptor* desc = kzalloc(sizeof(struct fileDescriptor));
			//descriptor start at 1
			desc->index = i + 1;
			fileDescriptors[i] = desc;
			*descOut = desc;
			res = 0;
			break;
		}
	}
	return res;
}

static struct fileDescriptor * fileGetDescriptor(int fd)
{
	if(fd <= 0 || fd >= TDOS_MAX_FILE_DESCRIPTORS)
	{
		return 0;
	}
	//descriptor start 1;
	int index = fd - 1;
	return fileDescriptors[index];
}

struct filesystem* fsResolve(struct disk* disk)
{
	struct filesystem* fs = 0;
	for(int i = 0; i < TDOS_MAX_FILESYSTEM; i++)
	{
		if(filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0) // resolve is function pointer
		{
			fs = filesystems[i];
			break;
		}

	}
	return fs;
}


int fopen(const char* filename, const char* mode)
{
	return -EIO;
}








