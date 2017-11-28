
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"



void tee1(char* filename) {   //in case of one argument, open a new file and print the buffer to it
	char buf[256];
	int newFile;	

	gets(buf,256);
	printf(1,"%s\n",buf);
	if ((newFile = open(filename, O_CREATE | O_WRONLY )) < 0) {
		printf(2,"couldnt create new file\n");
		exit(0);
	}
	printf(newFile,"%s\n",buf);
	close(newFile);
}


void tee2(char* filename1, char* filename2) {  //in case of two arguments read from first first into buffer and then print it to second file
	char buf[256];
	int rdFile,wrFile;	

	if ((rdFile = open(filename1, O_RDONLY )) < 0) {
		printf(2, "read file does not exist\n");
		exit(0);
	}	
	if ((wrFile = open(filename2, O_CREATE | O_WRONLY )) < 0 ) {
		printf(2, "couldnt create new file\n");
		exit(0);
	}
	read(rdFile, buf, 256);
	printf(1,"%s\n",buf);
	printf(wrFile,"%s\n",buf);
	close(rdFile);
	close(wrFile);
}


int main(int argc, char *argv[])
{
	if(argc == 2){		//check how many agruments are given
		tee1(argv[1]);  
	    	exit(0);
	  }
	else if(argc == 3) {
		tee2(argv[1],argv[2]);
		exit(0);
	  } 
	else {
		printf(2,"Illegal command\n");	
		exit(0);
	} 
}
