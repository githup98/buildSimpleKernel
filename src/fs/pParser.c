#include "pParser.h"
#include "kernel.h"
#include "memory/heap/kheap.h"
#include "string/string.h"
#include "memory/memory.h"
#include "status.h"

static int pathParserPathValidFormat(const char* filename)
{
	int len = strnlen(filename, TDOS_MAX_PATH);
	//filename[0],ex:  0:/
	return (len >=3 && isDigit(filename[0]) && memcmp((void*)&filename[1], ":/", 2) == 0);
}


static int pathParserGetDriveByPath(const char** path)
{
	if(!pathParserPathValidFormat(*path)) //<==> const char* filename
	{
		return -EBADPATH;
	}
	int driveNo = toNumericDigit(*path[0]);
	*path += 3; //due to 0:/ (take 3 bytes)
	return driveNo;

}

static struct pathRoot* pathParserCreateRoot(int driveNum)\
{
	struct pathRoot* pathR = kzalloc(sizeof(struct pathRoot));
	pathR->driveNo = driveNum;
	pathR->first = 0; //stop linked list
	return pathR;
}

static const char* pathParserGetPathPart(const char **path)
{
	char* resultPathPart = kzalloc(TDOS_MAX_PATH);
	int i = 0;
	while(**path != '/' && **path != 0x00)
	{
		resultPathPart[i] = **path;
		*path +=1;
		i++;
	}
	if(**path == '/')
	{
		*path +=1;
	}
	if(i == 0)
	{
		kfree(resultPathPart);
		resultPathPart = 0;
	}
	return  resultPathPart;
}

struct pathPart* pathPartParserPathPart(const char** path, struct pathPart* pathPartNext)
{
	const char* pathPartString = pathParserGetPathPart(path);
	if(!pathPartString)
	{
		return 0;
	}

	struct pathPart* part = kzalloc(sizeof(struct pathPart)); // create part node
	part->part = pathPartString;
	part->next = 0; //stop linked list
	
	if(pathPartNext)
	{
		pathPartNext->next = part;  //create linked list
	}

	return part;
}

void pathParserFree(struct pathRoot* root)
{
	struct pathPart* part = root->first;
	while(part)
	{
		struct pathPart* partNext = part->next;
		kfree((void*)part->part);
		kfree(part);
		part = partNext;
	}
	kfree(root);
}


struct pathRoot* pathParserParser(const char* path, const char** currentDirectory)
{
	int driveNo = 0;
	//backup * path val
	const char* tmpPath = path;
	
	struct pathRoot* pathRoot = 0;

	if(strlen(path) > TDOS_MAX_PATH)
	{
		return pathRoot;
	}
	
	driveNo = pathParserGetDriveByPath(&tmpPath);
	if(driveNo < 0)
	{
		return pathRoot;
	}

	pathRoot = pathParserCreateRoot(driveNo);
	
	if(!pathRoot)
	{
		return pathRoot;
	}

	struct pathPart* firstPart = pathPartParserPathPart(&tmpPath, NULL);

	if(!firstPart)
	{
		return pathRoot;
	}
	
	//create linked list : pathRoot --> firstPart --> part --> ...
	
	pathRoot->first = firstPart;
	
	struct pathPart* part = pathPartParserPathPart(&tmpPath, firstPart);

	while(part)
	{
		part = pathPartParserPathPart(&tmpPath, part);
	}

	return pathRoot;
}
