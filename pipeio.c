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






void
pipeio(int fd, int factor, char* filename)	{  //create 4 pipes - cyclic communication between parent and children
	char bufin[256];
	char bufout[256];
	int intbuf;
	int status1,status2,status3;
	int p0[2];
	int p1[2];
	int p2[2];
	int p3[2];
	pipe(p0);
	pipe(p1);
	pipe(p2);
	pipe(p3);

	if (fork() == 0) { 
		//first child
		read(p0[0], bufin, 256); 			 //read from first pipe
		if(bufin[0] == '-')				//check if negative number has arrived
			intbuf= -(atoi(&bufin[1]));
		else if (bufin[0] == '0')
			intbuf = 0;
		else
			intbuf = atoi(bufin);
		intbuf = intbuf + factor;
		toArray(intbuf, bufout);	
		write(p1[1], bufout,256); 			//write to second pipe
		close(p0[0]);
		close(p1[1]);
	}
	else 	{
		if (fork() == 0) {
			//second child
			read(p1[0], bufin, 256);  		//read from second pipe
			if(bufin[0] == '-')			//check if negative number has arrived
				intbuf=-(atoi(&bufin[1]));
			else if (bufin[0] == '0')
				intbuf = 0;
			else
				intbuf = atoi(bufin);
		
			intbuf = intbuf + factor;
			toArray(intbuf, bufout);		
			write(p2[1],bufout, 256);			//write to third pipe	
			close(p1[0]);
			close(p2[1]);	
		}
		else 	{
			if (fork() == 0) {
				//third child
				read(p2[0], bufin, 256); 		//read from third pipe
				if(bufin[0] == '-')			//check if negative number has arrived
					intbuf=-(atoi(&bufin[1]));
				else if (bufin[0] == '0')
					intbuf = 0;
				else
					intbuf = atoi(bufin);
		
				intbuf = intbuf + factor;
				if (intbuf == 0) {
					bufout[0] = '0'; bufout[1] = '\0';
				}
				else
					toArray(intbuf, bufout);			
				write(p3[1],bufout, 256);		//write to fourth pipe (back to parent)
				close(p2[0]);
				close(p3[1]);
			}
			else {
				//parent
				read(fd,bufin, 256);			//read from file
				int k = 0;
				int i;
				for (i= 0; i < strlen(bufin); i++) {	//find the first digit 
					if ((int)(bufin[i]) < 58 && (int)(bufin[i]) > 47) {
						k++;
					}
					else if (k>0)
						break;
				}
				int value = atoi(&bufin[i-k]);		//conver digit string to int
				if(bufin[i-k-1] == '-') {		//check for negative value
					value = -value;
					k++;
				}
				toArray(value, bufout);			//convert to string
				write(p0[1], bufout, 256);		//write to first pipe
				//wait for children				
				wait(&status1);
				wait(&status2);	
				wait(&status3);
				strcpy(bufout,bufin); 			//save first part of text
				bufout[i-k] = '\0';
				char tempArr[256];
			
				read(p3[0], tempArr, 256 );		//read from fourth pipe
				
				strcpy(bufout + strlen(bufout), tempArr);	//concatenate new number to old first part of text
				strcpy(bufout + strlen(bufout), &bufin[i]);	//concatenate second part of text

				close(p3[0]);
				close(p0[1]);

				write(1, bufout, strlen(bufout));   	//write to standard output the new text
				int wrFile = open(filename, O_CREATE | O_WRONLY );
				printf(wrFile,"%s",bufout); 		//re-write to file the new text
				close(wrFile);			
						
			}
		}
	}					
	exit(0);

}        
        
int
main(int argc, char *argv[])	{
	int fd;
	int factor;
	if(argc == 3){
		if((fd = open(argv[1], 0)) < 0){ 			//try to open given file
			printf(1, "cat: cannot open %s\n", argv[1]);
			exit(0);  
		}
		else {
			if(argv[2][0] == '-')				//check if factor is negative
				factor=-(atoi(&argv[2][1]));		//convert it to int
			else
				factor=atoi(argv[2]);
		
			printf(1, "factor : %d\n", factor);
			pipeio(fd,factor, argv[1]);			//do work
			close(fd);
		}
	}
	else
		printf(1, "bad arg count \n");
	exit(0);
}        
