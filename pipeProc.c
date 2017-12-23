#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"



void toArray(int number, char* numberArray)   //Receives an integer, converts to char array and writes it into numberArray
    {
        int i=0, j=0;
	int num;
	if (number < 0) {   //check if received negative number
		j=1;
		numberArray[0] = '-';
		number = -number;
	}
	num = number;
	while (num > 0) {
		i++; num = num /10;
	}
	char tempArr[i];
	i=0;
	while(number > 0) {
        	tempArr[i] = number % 10 + '0';
		number = number/10; 
		i++;
        }
	i--;
	for(int k=j; i >= 0 ; k++) {   //rewrite in correct order
		numberArray[k] = tempArr[i--];
	}
     }


//void childProcess(int num2Add) {
//	char bufIn[256];
//	char butOut[256];
//	int num;
//
//	read(Pipe[0], bufIn, 256); //read from pipe
//	num = atoi(bufIn);
//	num = num + num2Add;
//	toArray(num, bufOut);
//	write(Pipe, bufOut, 256);
//}



int main(int argc, char *argv[]) {
	int num2Add, num;
	int Pipe[2];
	int pid;
	char bufout[256];
	char bufOut[256];
	char bufIn[256];

	if(argc != 3){ 
		printf(1,"Illegal Command Format!\n");
		exit(0);
	}

	num2Add = atoi(argv[1]);
	num = atoi(argv[2]);

	printf(2,"Num is %d, num2add is %d.\n",num, num2Add);

	pipe(Pipe);

	toArray(num, bufout);
	write(Pipe[1], bufout, 256);		//write to pc pipe

	for(int i = 0; i < 3; i++) {
		pid = fork();
		if(pid==0) {
			read(Pipe[0], bufIn, 256); //read from pipe
			num = atoi(bufIn);
			num = num + num2Add;
			toArray(num, bufOut);
			write(Pipe[1], bufOut, 256);
			exit(0);
		} else 
			wait(0);
	}

	read(Pipe[0],bufout, 256);
	num = atoi(bufout);

	printf(2,"The final num is %d\n", num);
	close(Pipe[0]);
	close(Pipe[1]);
	exit(0);
}
