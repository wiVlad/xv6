#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"
#include "stdlib.h"



int main(int argc, char argv[]) {
	int file, num2Add;
	int rdPipe[2], wrPipe[2];

	if(argc != 3){ 
		printf(1,"Illegal Command Format!\n");
		exit();
	}
	if (file = open(argv[1],0) < 0) {
		printf(1,"File not found!\n");
		exit()
	}
	num2Add = atoi(argv[2]);
	pipe(wrPipe);
	pipe(rdPipe);
	if(fork()==0) 
		childFunction(num2Add);
	else {
		wait();
		if(fork()==0)

	


}
