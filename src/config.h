#ifndef CONFIG_H
#define CONFIG_H


#define TDOsTotalInterrupt 512
#define kernelCodeSelector 0x08

#define TD_OS_HEAP_SIZE_BYTES 104857600  // 100MB
#define TD_OS_HEAP_BLOCK_SIZE 4096

#define TD_OS_HEAP_ADDRESS 0x01000000 

//check osdev memory map
#define TD_OS_HEAP_TABLE_ADDRESS 0x00007E00 //to 7eff 480.5 kb enough for 25600 entries

#define TDOS_SECTOR_SIZE   512

#define TDOS_MAX_FILESYSTEM 12
#define TDOS_MAX_FILE_DESCRIPTORS 512


#endif
