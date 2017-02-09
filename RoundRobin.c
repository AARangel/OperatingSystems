#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int head; 
int tail; 

struct process{
	int burst;
	int arrival;
	char name[14];
};

void printMe(struct process* myProcesses); 
void enq(struct process* Queue,int processCount, struct process currentProcesses); 


int main(){ 
	int processCount = 2; 
	int runFor = 15; 
	int quantum = 2; 
	
	// array of struct objects
	struct process* myProcesses = (struct process*) malloc (sizeof(struct process) * processCount); 

	// insertting into array of processes
	myProcesses[0].burst = 5; 
	myProcesses[0].arrival = 3; 
	strcpy(myProcesses[0].name, "P1");  
											   
	myProcesses[1].burst = 9; 
	myProcesses[1].arrival = 0; 
	strcpy(myProcesses[1].name, "P2");
			
	// print current processes in array 
	printMe(myProcesses); 
	
	//circular array 
	int* waitingLine = (int*)malloc(sizeof(int) * processCount); 
	int head =1; 
	int tail; 
	
	//set elements in the waitingLine to -1 for not being used 
	for(int i = 0; i < processCount; i++){
		waitingLine[i] = -1; 
	}
	
	//Start Round Robin 
	for(int i = 0; i < runFor; i++){
	
		//Check for new arrivals
		for(int j = 0; j < processCount; j++){

			if(myProcesses[j].arrival == i){
				// check if we are in the last element of the array and set it up to the first element
				if(tail+1 == processCount && waitingLine[0] != -1){
					tail = 0; 
					// save possition of the processes we want to put in line
					waitingLine[tail] = j; 	
					printf("%s has just arrived\n", myProcesses[waitingLine[head]].name);  
				}
				else if(tail+1 < processCount){
					// save possition of the processes we want to put in line
					waitingLine[tail+1] = j; 
					printf("%s has just arrived\n",myProcesses[waitingLine[head]].name); 
					tail++; 
				}
				else 
				{
					printf("Error Line 71 Tail: %d ProcessesCount: %d\n ", tail, processCount); 
				}
			}
		} // end of arrivals 
		
		// if we have reached are quatum point move the current process to the back of the line if unfinished
		if(i%quantum == 0){
				
			if(myProcesses[waitingLine[head]].burst <= 0){
				printf("%s has now been completed\n", 	myProcesses[waitingLine[head]].name);
				head++; 
			}
			
			// check if we are in the last element of the array and set it up to the first element
				if(tail+1 == processCount && waitingLine[0] != -1){
					tail = 0; 
					// save possition of the processes we want to put in line
					waitingLine[tail] = waitingLine[head];
					
					if(head+1 == processCount && waitingLine[0] != -1){
						head = 0;	
					}
					else if( head+1 < processCount){
						head++;
					}
					else
						printf("Error line 97"); 
				}
				else if(tail+1 < processCount){
					// save possition of the processes we want to put in line
					waitingLine[tail+1] = waitingLine[head]; 
					tail++; 
					
					if(head+1 == processCount && waitingLine[0] != -1){
						head = 0;	
					}
					else if( head+1 < processCount){
						head++;
					}
					else
						printf("Error line 97\n"); 
				}
				else 
				{
					printf("Error Line 115 Tail: %d modules: %d i = %d quantum = %d\n", tail, i%quantum, i, quantum );  
				}
		} // end of quantum 
		
		
		// check if the current process has finished 
		if(myProcesses[waitingLine[head]].burst <= 0){
				printf("%s has now been completed\n", 	myProcesses[waitingLine[head]].name);
				head++; 
		}	
		else{
			myProcesses[waitingLine[head]].burst--;	
		}
				
	} // end of run time 
	
	// free the memory
	free(myProcesses); 
	free(waitingLine); 
													   
}

// Print current elements in the queue 
void printMe(struct process* myProcesses){
	printf("%s burst: %d	arrival: %d\n", myProcesses[0].name, myProcesses[0].burst, myProcesses[0].arrival);	
	printf("%s burst: %d	arrival: %d\n", myProcesses[1].name, myProcesses[1].burst, myProcesses[1].arrival);	
}










































