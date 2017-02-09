#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int algorithm=0; // 0=fcfs, 1=sjf 2=rr
int quantum;

int processCount;
int timeUnits;
char output[1000]; // output of entire run returned by algorithm call

#ifndef  STRUCTSIZE
#define STRUCTSIZE 128
#endif

void read(); // assigns algorithm and num processes
void firstComeFirstServed();
void shortestJobFirst();
void roundRobin();
int earliestIn();
int earliestArrival();

typedef struct process{
int burst;
int arrival;
char name[14];

}Processes;

Processes *processes;

int main(){
	processes = malloc(sizeof(Processes)*100);
	read();

	char out[12];
	strcpy(out, "output.txt");
	FILE *f = fopen(out, "w");

	if(f==NULL){
		printf("%s","Error opening file");
//	exit(1);
	}


// write to file
	fprintf(f,"%d processes \n", processCount);
	char write[128];
	printf("%d", algorithm);
	if(algorithm==0){
	 	strcpy(write, "First Come First Served");
		//firstComeFirstServed();
	}
	else if(algorithm==1){
		strcpy(write, "Shortest job first");
	//shortestJobFirst();
	}
	else{
		strcpy(write,"Round Robin");
		//roundRobin();
	}


	fprintf(f,"Using %s", write);
	if(algorithm==2) fprintf(f,"Quantum %d", quantum);

	//BREAKS IF UNCOMMENT, NEEDS FIXING. What is output? should it be out?
	//strncat(output,'\0',2);

	//strncat(output,'\0',2);
	fprintf(f,"%s",output);

	fclose(f);

	return 0;
	


	}

// postcondition : all variables appropriatley assigned
void read(){

	int i;
	int lineStop = 0; //Indicates # comment position in a line of input
	char line[STRUCTSIZE]; //Line of input
	char * current; 
	char parsedLine[STRUCTSIZE];
	int flag;
	int inputFileLine = 0; //Which line of input currently parsing

	for(i = 0; i <100; i++){
		 processes[i].burst = 0;
		 processes[i].arrival = 0;
	}

	FILE *ifp;
	ifp = fopen("processes.in","r");

	if(ifp == NULL){
		printf("Problem reading file...");
	}

	i =0 ;
	int incrementProcess = 0;

	//each iteration represents parsing of one line of input file
	while(fgets(line, STRUCTSIZE, ifp) != NULL){
		//Variables to identify how much of line to copy
		int processHelper = 0;
		lineStop = -1;
		flag =0;
		Processes temp;

		//Identifies all of the positions of the #
		for(i = 0; i<strlen(line); i++){
			strncpy(parsedLine, "H",strlen(parsedLine));
			if(line[i] == '#'){
				if(lineStop == -1){
					lineStop = i;
					flag++;
				}
			}
		}

		//Case where Line has # in the first position
		if(lineStop == 0){
			strcpy(parsedLine,"  \0");
			lineStop++;
		}
		//Case where Line has no #
		else if(flag !=0){
			 strncpy(parsedLine,line,lineStop);
			 parsedLine[lineStop-1] = '\0';
		}
		else{
			 strncpy(parsedLine,line, strlen(line));
			 parsedLine[strlen(line)] = '\0';
		}

		 // printf("%s\n", parsedLine);

		char *p;
		 p = (char*)malloc(sizeof(char*));

		p = strtok(parsedLine,", ");
		int getNextTok = 0;

		//space seperate movement through entire file char *p is each "word"
		//Get next line shows that hit key word and next toeken is needed
		while(p!= NULL){
			if(inputFileLine == 0){
				if(getNextTok == 1){
					processCount = atoi(p);
					getNextTok = 0;
				}
				else if(strcmp(p,"processcount") == 0){
					getNextTok = 1;
				}
			}

			else if(inputFileLine == 1){
				if(getNextTok == 1){
					timeUnits = atoi(p);
					getNextTok = 0;
				}
				else if(strcmp(p,"runfor") == 0)
					getNextTok = 1;
			}

			else if(inputFileLine == 2){
				if(getNextTok == 1){
					if(strcmp(p,"fcfs") == 0){
						algorithm =0;
					}
					else if(strcmp(p,"sjf") == 0){
						algorithm = 1;
					}
					else if(strcmp(p,"rr") == 0){
						algorithm = 2;
					}
					getNextTok = 0;
				}
				if(strcmp(p,"use") == 0)
					getNextTok = 1;
			}
			
			else if(inputFileLine == 3){
				if(getNextTok == 1){
					quantum = atoi(p);
					printf("   quantum%d", quantum);
					getNextTok = 0;
				}
				else if(strcmp(p,"quantum") == 0){
					getNextTok = 1;
					}
			}

			else if(inputFileLine > 3){
				if(getNextTok == 1){
					getNextTok = 0;

					if(processHelper == 0){
						strcpy(processes[incrementProcess].name, p);
						strcpy(temp.name, p);
						processHelper++;
						}
					else if(processHelper ==1){
						processes[incrementProcess].arrival = atoi(p);
						processHelper++;
					}
					else if(processHelper ==2){
						processes[incrementProcess].burst = atoi(p);
						processHelper++;
						incrementProcess+=1;
					}
					getNextTok =0;
				}
				if((strcmp(p,"name") == 0)||(strcmp(p,"arrival")) == 0||(strcmp(p,"burst") == 0)){
					getNextTok = 1;
				}
				else if(strcmp(p,"end") == 0){
					break;
				}
			}	
			
			p = strtok(NULL, ", ");
		}
		inputFileLine++;
	}

	//Use to print out each element in the struct

	for(i = 0; i<4; i++){
		printf("%dpro%s arrival %d   bursttttt%d   \n",i,processes[i].name, processes[i].arrival, processes[i].burst);
	}
	printf("timeunits:%d\n", timeUnits);
	printf("quantum%d\n", quantum);
	printf("algorithm%d\n", algorithm);

}

void firstComeFirstServed(){
// loop through processes until time finished
int i=0;
int currentIndex=earliestArrival();
//struct process current=proccesses[earliestArrival()];
int timeSelected=0;
while(i<timeUnits){
	char* msg;
	int p;
	for(p=0; p<processCount; p++){
		// a new process has arrived
		if(processes[p].arrival==i){
			snprintf(msg,100,"Time %d: %s arrived\n",i, processes[p].name);
		}
	}
	if(i==0){
		snprintf(msg,100,"Time %d: %s selected (burst %d) \n",i,processes[currentIndex].name,processes[currentIndex].burst);
	}
	//check for process completed
	else if(processes[currentIndex].burst== timeSelected){
		processes[currentIndex].burst=0;
		// pick a new process
		timeSelected=0;
		snprintf(msg,100,"Time %d: %s finished  \n",i, processes[currentIndex].name);
		//select new process
		currentIndex=earliestArrival();
		snprintf(msg,100,"Time %d: %s selected (burst %d) \n",i,processes[currentIndex].name,processes[currentIndex].burst);


		}
		else{
			timeSelected++;
		}
		
		
		strncat(output,msg,100);
		i++;
	}
}
int earliestIn(){
	int i;
	int ret=0;
	int earliestArrival=100;
	for(i=1;i<processCount; i++ ){
		if(processes[i].arrival<earliestArrival && processes[i].burst!=0){
			earliestArrival=processes[i].arrival;
			ret=i;
		}
	}
	return ret;
}
void shortestJobFirst(){}
void roundRobin(){}
int earliestArrival(){}

