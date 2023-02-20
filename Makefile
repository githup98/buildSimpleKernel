FILES=./build/kernel.asm.o ./build/kernel.o

INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels \
-falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions \
-Wno-unused-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp \
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


./bin/boot.bin: ./src/boot.asm
	nasm -f bin ./src/boot.asm -o ./bin/boot.bin


./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

	# test read a sector

	# dd if=./message.txt >> ./boot.bin
	# this line is write append 1 sector
	# right after current position (1 sector == 512 byte
	#dd if=/dev/zero bs=512 count=1 >> ./boot.bin
	
	# end of testing read a sector 
./build/kernel.o: ./src/kernel.c
	# $(INCLUDES) for include lines in kernel.c, "-c" is indecate that output is .o
	i686-elf-gcc $(FLAGS) $(INCLDUES) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o
clean:
	rm -f ./bin/boot.bin
	rm -f ./bin/os.bin
	rm -f ./build/kernel.asm.o
	rm -f ./build/kernelFull.o
	rm -f ./bin/kernel.bin
	rm -f ./build/kernel.o
