#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


char num[80]; //global char array - for easier work.
char tempFileBuf[512];
int tempFileFD;

int isdigit (char *c){ //check if a char is a letter or number
	if (c[0]=='0'
		|| c[0]=='1'
		|| c[0]=='2'
		|| c[0]=='3'
		|| c[0]=='4'
		|| c[0]=='5'
		|| c[0]=='6'
		|| c[0]=='7'
		|| c[0]=='8'
		|| c[0]=='9'
	) return 1;
	else return 0;
}
int isdigit1 (char c){ //check if a char is a letter or number
	if (c=='0'
		|| c=='1'
		|| c=='2'
		|| c=='3'
		|| c=='4'
		|| c=='5'
		|| c=='6'
		|| c=='7'
		|| c=='8'
		|| c=='9'
	) return 1;
	else return 0;
}

long long int my_atoi(char *c) //convert char array to integer
{
	long long int value = 0;
	int sign = 1;
	if (c[0]=='-'){
		sign=-1;
		c++;
	}
   
	while (isdigit(c))
	{
		value *= 10;
		value += (int) (*c-'0');
		c++;
	}
	return (sign*value);
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
		//close(0);
		//close(1);
		//dup(p0[0]);
		//dup(p1[1]);
		//close(p0[0]);
		//close(p1[1]);
		read(p0[0], &intbuf, sizeof(int));
		intbuf = intbuf + factor;
		//snprintf(bufout, sizeof(bufout), "%d", value);
		//strcpy(bufout + strlen(bufout), " world");		
		write(p1[1], &intbuf, sizeof(int));
		close(p0[0]);
		close(p1[1]);
	}
	else 	{
		if (fork() == 0) {
			//second child
			//close(0);
			//close(1);
			//dup(p1[0]);
			//dup(p2[1]);
			//close(p1[0]);
			//close(p2[1]);
			read(p1[0], &intbuf, sizeof(int));
			
			intbuf = intbuf + factor;
			//snprintf(bufout, sizeof(bufout), "%d", value);
			//strcpy(bufout + strlen(bufout), " world");		
			write(p2[1],&intbuf, sizeof(int));		
			close(p1[0]);
			close(p2[1]);	
		}
		else 	{
			if (fork() == 0) {
				//third child
				//close(0);
				//close(1);
				//dup(p2[0]);
				//dup(p3[1]);
				//close(p2[0]);
				//close(p3[1]);
				read(p2[0], &intbuf, sizeof(int));
				
				intbuf = intbuf + factor;
				//snprintf(bufout, sizeof(bufout), "%d", value);
				//strcpy(bufout + strlen(bufout), " world");		
				write(p3[1],&intbuf, sizeof(int));	
				close(p2[0]);
				close(p3[1]);
			}
			else {
				//parent
				//close(0);
				//dup(p3[0]);
				//dup(p0[1]);
				//close(p3[0]);
				//close(p0[1]);
				read(fd,bufin, 256);
				int k = 0;
				while (isdigit1(bufin[k]) != 1) {
					k++;
				}
				strcpy(bufout,bufin); bufout[k] = '\0';
				long long int value = my_atoi(&bufin[k]);	
				write(p0[1], &value, sizeof(int));
				//wait for children				
				wait(&status1);
				wait(&status2);	
				wait(&status3);
				read(p3[0], &intbuf, sizeof(int));
				//value = my_atoi(bufout);
				printf(1, "%d\n", intbuf);
				
				strcpy(bufout+strlen(bufout), temp);
				strcpy(bufout+strlen(bufout), &(bufin[k+sizeof(value)]));
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
		factor=my_atoi(argv[2]);
		pipeio(fd,factor, argv[1]);
	}
	else
		printf(1, "bad arg count \n");
	exit(0);
}        
