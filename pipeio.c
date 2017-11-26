#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


char num[80]; //global char array - for easier work.
char tempFileBuf[512];
int tempFileFD;




void toArray(int number, char* numberArray)
    {
        int i=0, j=0;
	int num;
	if (number < 0) {
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
	for(int k=j; i >= 0 ; k++) {
		numberArray[k] = tempArr[i--];
	}
     }






void
pipeio(int fd, int factor, char* filename)	{
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
		read(p0[0], bufin, 256);

		if(bufin[0] == '-')
			intbuf=-(atoi(&bufin[1]));
		else
			intbuf = atoi(bufin);
		
		intbuf = intbuf + factor;
		printf(1, "child 1 bufout before: %s\n", bufout);
		toArray(intbuf, bufout);	
		printf(1, "child 1 bufout after: %s\n", bufout);
		write(p1[1], bufout,256);
		close(p0[0]);
		close(p1[1]);
	}
	else 	{
		if (fork() == 0) {
			//second child
			read(p1[0], bufin, 256);
			if(bufin[0] == '-')
				intbuf=-(atoi(&bufin[1]));
			else
				intbuf = atoi(bufin);
		
			intbuf = intbuf + factor;
			printf(1, "child 2 bufout before: %s\n", bufout);
			toArray(intbuf, bufout);	
			printf(1, "child 2 bufout after: %s\n", bufout);		
			write(p2[1],bufout, 256);		
			close(p1[0]);
			close(p2[1]);	
		}
		else 	{
			if (fork() == 0) {
				//third child
				read(p2[0], bufin, 256);
				if(bufin[0] == '-')
					intbuf=-(atoi(&bufin[1]));
				else
					intbuf = atoi(bufin);
		
				intbuf = intbuf + factor;
				printf(1, "child 3 bufout before: %s\n", bufout);
				toArray(intbuf, bufout);	
				printf(1, "child 3 bufout after: %s\n", bufout);			
				write(p3[1],bufout, 256);	
				close(p2[0]);
				close(p3[1]);
			}
			else {
				//parent
				read(fd,bufin, 256);
				int k = 0;
				int i;
				for (i= 0; i < strlen(bufin); i++) {
					if ((int)(bufin[i]) < 58 && (int)(bufin[i]) > 47) {
						k++;
					}
					else if (k>0)
						break;
				}
				int value = atoi(&bufin[i-k]);
				if(bufin[i-k-1] == '-') {
					value = -value;
					k++;
				}
				printf(1, "bufout before: %s\n", bufout);
				toArray(value, bufout);
				printf(1, "value is: %d\n", value);
				printf(1, "bufout after: %s\n", bufout);	
				write(p0[1], bufout, 256);
				//wait for children				
				wait(&status1);
				wait(&status2);	
				wait(&status3);
				strcpy(bufout,bufin); bufout[i-k] = '\0';
				char tempArr[256];
			
				read(p3[0], tempArr, 256 );
				strcpy(bufout + strlen(bufout), tempArr);
				strcpy(bufout + strlen(bufout), &bufin[i]);

				close(p3[0]);
				close(p0[1]);

				write(1, bufout, strlen(bufout));			
						
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
		if((fd = open(argv[1], 0)) < 0){
			printf(1, "cat: cannot open %s\n", argv[1]);
			exit(0);  
		}
		if(argv[2][0] == '-')
			factor=-(atoi(&argv[2][1]));
		else
			factor=atoi(argv[2]);
		
		printf(1, "factor : %d\n", factor);
		pipeio(fd,factor, argv[1]);
	}
	else
		printf(1, "bad arg count \n");
	exit(0);
}        
