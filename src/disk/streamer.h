#ifndef STREAMER_H
#define STREAMER_H

#include  "disk.h"

struct diskStream
{
	int pos;
	struct disk* disk;
};

struct diskStream* diskStreamNew(int index);
int diskStreamSeek(struct diskStream* stream, int pos);
int diskStreamRead(struct diskStream* stream, void* out, int total);
void diskStreamClose(struct diskStream* stream);
#endif
