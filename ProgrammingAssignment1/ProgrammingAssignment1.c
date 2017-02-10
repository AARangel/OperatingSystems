#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int algorithm=0; // 0=fcfs, 1=sjf 2=rr
int quantum;

int processCount;
int timeUnits;
char output[1000]; // output of entire run returned by algorithm call

#define STRUCTSIZE 128
#define MAXINT 2147482000


void incrementWaitTimes(int running);
void read(); // assigns algorithm and num processes
void firstComeFirstServed();
void shortestJobFirst();
void roundRobin();
int earliestIn();
int earliestArrival();

typedef struct process{
int burst;
int arrival;
int wait;
int turnAround;
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
	 	//printf(" %d %d", timeUnits, processCount);
		firstComeFirstServed();
	}
	else if(algorithm==1){
		strcpy(write, "Shortest job first");
		shortestJobFirst();
	}
	else{
		strcpy(write,"Round Robin");
		//roundRobin();
	}


	fprintf(f,"Using %s \n", write);
	if(algorithm==2) fprintf(f,"Quantum %d \n", quantum);

	//BREAKS IF UNCOMMENT, NEEDS FIXING. What is output? should it be out?
	//strncat(output,'\0',2);

	//strncat(output,'\0',2);
	fprintf(f,"%s\n\n\n",output);


	int i;

	for(i=0;i<processCount; i++){
		fprintf(f, "%s wait %d turnAround %d \n", processes[i].name,
		 processes[i].wait, processes[i].turnAround );
	}


	fclose(f);
	//free(output);
	//free(processes);

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
		 processes[i].wait=0;
		 processes[i].turnAround=0;
	}

	FILE *ifp;
	ifp = fopen("processes.in","r");

	if(ifp == NULL){
		printf("Problem reading file...");
		return;
	}

	i =0 ;
	int incrementProcess = 0;

	//each iteration represents parsing of one line of input file
	while(fgets(line, STRUCTSIZE, ifp) != NULL){
	// while(inputFileLine < 5){
		// Variables to identify how much of line to copy
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
int done=0;
//printf("here\n");
int currentIndex=earliestIn();
//struct process current=proccesses[earliestArrival()];
//printf("here %d \n", currentIndex);
int timeSelected=0;
while(i<=timeUnits){
	char* msg=(char*)(malloc(sizeof(char)*50));
	char* msg2=(char*)(malloc(sizeof(char)*50));
	int p=0;
	//printf("here1232");
	//printf("here1232 %d  \n", processCount);
	while(p<processCount){
		// a new process has arrived
		//printf("here1232 %d  \n", processCount);
		//printf("new process has arrived21232 %d", i);
		if(processes[p].arrival==i){
			//printf("new process has arrived %d", i);
			sprintf(msg,"Time %d: %s arrived\n",i, processes[p].name);
		}

		p++;
	}
	//printf("selecting process ");
	// no process selected
	if(i==0){
		//printf("selecting process ");

		sprintf(msg2,"Time %d: %s selected (burst %d) \n",i,processes[currentIndex].name,processes[currentIndex].burst);
	}
	else if(processes[currentIndex].burst== (timeSelected+1)){
		//printf("here at process completed\n");
		processes[currentIndex].burst=0;
		processes[currentIndex].turnAround=i;
		// pick a new process
		timeSelected=0;
		sprintf(msg,"Time %d: %s finished  \n",i, processes[currentIndex].name);
		//select new process
		currentIndex=earliestIn();
		if(currentIndex!=99){
		sprintf(msg2,"Time %d: %s selected (burst %d) \n",
			i,processes[currentIndex].name,processes[currentIndex].burst);
		}
		//}
		//else{
			//printf("idle\n");
			//done=1;
		//}

	}
	// let the process keep running 
	else{
		timeSelected++;
		
	}
		incrementWaitTimes(currentIndex);
			strcat(msg,msg2);
			strcat(output,msg);
			
		
		
		i++;

	}


	char* msg3=(char*)(malloc(sizeof(char)*50));
	sprintf(msg3,"finished at time %d\n",timeUnits);
	strcat(output,msg3);
	// subract arrival out of waits and turnaround
	int c;
	for(c=0; c<processCount;c++){
		processes[c].wait=processes[c].wait-processes[c].arrival;
		processes[c].turnAround=processes[c].turnAround-processes[c].arrival;
	}


}
// increment all wait times except for running
void incrementWaitTimes(int running){
	int i;
	for(i=0;i<processCount; i++ ){
		if(running!=i && processes[i].burst!=0){
				processes[i].wait++;
		}
	}

}
int earliestIn(){
	int i;
	int ret=99;
	int earliestArrival=100;
	for(i=0;i<processCount; i++ ){
		//printf("looping through processes\n");
		if(processes[i].arrival<earliestArrival &&processes[i].burst!=0 ){
			//printf("earliestArrival %d\n", processes[i].arrival);
			earliestArrival=processes[i].arrival;
			ret=i;
		}
	}

	printf("lowest address %d\n", ret);
	return ret;
}
void shortestJobFirst(){
	int i, j, shortestProcess, processToRun, runningProcess;

	j = 0;
	shortestProcess = MAXINT;
	processToRun = -1;
	runningProcess = MAXINT;

	for(i =0; i < timeUnits; i++){
		processToRun = MAXINT;

		for(j = 0; j<processCount; j++){
			if(processes[j].burst == 0){
				shortestProcess = MAXINT;
				continue;
			}

			if(processes[j].arrival == i){
				printf("Time %d: %s arrived\n", i, processes[j].name);
			}

			if(processes[j].arrival <= i && processes[j].burst > -1){
				if(processes[j].burst < shortestProcess){
					processToRun = j;
					shortestProcess = processes[j].burst;
				}
			}
		}

		for(j = 0; j<processCount; j++){
			if(processes[j].burst > shortestProcess && processes[j].arrival <= i && processes[j].burst > -1){
				processes[j].wait++;
				processes[j].turnAround++;
			}
			else if(processes[j].burst > -1 && processes[j].arrival <= i){
				processes[j].turnAround++;
			}
		}

		if(processToRun != MAXINT){
			processes[processToRun].burst--;
			//Second burst-- to make burst = 0, process complete
			if(processes[processToRun].burst == 0){
				printf("Time %d: %s finished\n", i+1, processes[processToRun].name);
				processes[processToRun].burst--;
				shortestProcess = MAXINT;
			}
		}
		
		if(runningProcess != processToRun){
			if(processToRun ==MAXINT){
				printf("Time %d: IDLE\n",i);
				continue;
			}
			//+1 burst since already decremented above
			printf("Time %d: %s selected (burst %d)\n",i,processes[processToRun].name, processes[processToRun].burst+1);
			runningProcess = processToRun;
		}
	}
	printf("Finished at Time %d\n", i);

	for(i = 0; i < processCount; i++){
		printf("%s Wait Time: %d Turnaround: %d\n", processes[i].name,processes[i].wait, processes[i].turnAround);
	}
}
void roundRobin(){}


