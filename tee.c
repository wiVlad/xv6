
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"



void tee1(char* filename) {
	char buf[256];
	int newFile;	

	gets(buf,256);
	printf(1,"%s\n",buf);
	newFile = open(filename, O_CREATE | O_WRONLY );
	printf(newFile,"%s\n",buf);
	close(newFile);
}


void tee2(char* filename1, char* filename2) {
	char buf[256];
	int rdFile,wrFile;	

	rdFile = open(filename1, O_RDONLY );
	wrFile = open(filename2, O_CREATE | O_WRONLY );
	read(rdFile, buf, 256);
	printf(1,"%s\n",buf);
	printf(wrFile,"%s\n",buf);
	close(rdFile);
	close(wrFile);
}


int main(int argc, char *argv[])
{
	if(argc == 2){
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
