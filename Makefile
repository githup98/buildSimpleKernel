FILES=./build/kernel.asm.o ./build/kernel.o ./build/idt/idt.asm.o \
./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o \
./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.o \
./build/memory/paging/paging.asm.o

INCLUDES = -I./src

FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels \
-falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions \
-Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp \
-Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin
	rm -f ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	# kenel size may be lagre
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelFull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelFull.o


./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin


./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

	# test read a sector

	# dd if=./message.txt >> ./boot.bin
	# this line is write append 1 sector
	# right after current position (1 sector == 512 byte
	#dd if=/dev/zero bs=512 count=1 >> ./boot.bin
	
	# end of testing read a sector 



./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

./build/kernel.o: ./src/kernel.c ./src/kernel.h
	# $(INCLUDES) for include lines in kernel.c, "-c" is indecate that output is .o
	i686-elf-gcc $(FLAGS) $(INCLUDES) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o


./build/idt/idt.o: ./src/idt/idt.c 
	i686-elf-gcc $(FLAGS) $(INCLUDES) -I./src/idt/ -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o


./build/memory/memory.o: ./src/memory/memory.c
	i686-elf-gcc $(FLAGS) $(INCLUDES) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/memory/heap/heap.o: ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES)  -I./src/memory/heap $(FLAGS)  -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o


./build/memory/paging/paging.o: ./src/memory/paging/paging.c
	i686-elf-gcc $(INCLUDES)  -I./src/memory/paging $(FLAGS)  -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o
clean:
	rm -f ./bin/boot.bin
	rm -f ./bin/os.bin
	rm -f ./build/kernel.asm.o
	rm -f ./build/idt/*
	rm -f ./build/io/*
	rm -f ./build/kernelFull.o
	rm -f ./bin/kernel.bin
	rm -f ./build/kernel.o
	rm -f ./build/memory/memory.o
	rm -f ./build/memory/heap/*
	rm -f ./build/memory/paging/*
