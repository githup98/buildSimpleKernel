#ifndef PATHPARSER_H
#define PATHPARSER_H


struct pathRoot
{
	int driveNo;
	struct pathPart* first;
};

struct pathPart
{
	const char* part;
	struct pathPart* next; // this is the linked list
};

struct pathRoot* pathParserParser(const char* path, const char** currentDirectory);
void pathParserFree(struct pathRoot* root);
#endif
