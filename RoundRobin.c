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
    int selected;
    int quantumCounter;
	int turnaround; 
	int wait;
};

void printMe(struct process* myProcesses);
void enq(struct process* Queue,int processCount, struct process currentProcesses);


int main(){
	int processCount = 2+1;
	int runFor = 15;
	int quantum = 2;

	// array of struct objects
	struct process* myProcesses = (struct process*) malloc (sizeof(struct process) * processCount);

	// insertting into array of processes
	myProcesses[0].burst = 5;
	myProcesses[0].arrival = 3;
	strcpy(myProcesses[0].name, "P1");
    myProcesses[0].selected = 0;
    myProcesses[0].quantumCounter = 0;
	myProcesses[0].turnaround = 0;
	myProcesses[0].wait = 0;

	myProcesses[1].burst = 9;
	myProcesses[1].arrival = 0;
	strcpy(myProcesses[1].name, "P2");
    myProcesses[1].selected = 0;
    myProcesses[0].quantumCounter = 0;
	myProcesses[1].turnaround = 0;
	myProcesses[1].wait = 0;

    // print current processes in array
	//printMe(myProcesses);

	//circular array
	int* waitingLine = (int*)malloc(sizeof(int) * processCount);
	int head =0;
	int tail =0;

	//set elements in the waitingLine to -1 for not being used
	for(int i = 0; i < processCount; i++){
		waitingLine[i] = -1;
	}

	printf("%d processes\n", processCount-1); 
	printf("Using Round-Robin\n"); 
	printf("Quantum %d\n\n", quantum); 
	
	//Start Round Robin
	for(int i = 0; i < runFor+1; i++){
		//Check for new arrivals
		for(int j = 0; j < processCount-1; j++){
			if(myProcesses[j].arrival == i){

                if(waitingLine[tail] == -1){
                    // insert into the current position of tail
                    waitingLine[tail] = j;                    
                    printf("Time %d: %s arrived\n", i, myProcesses[waitingLine[tail]].name);
                    tail++;
                }

                // Current position of the tail is on the first index so nothing can be insert until it is popped of the queue
                if(tail >= processCount ){
                    tail = 0;
                }
			}
		} // end of arrivals

		//printf("++ -\n %d | %d | %d\n\n", waitingLine[0], waitingLine[1], waitingLine[2]); 
		
        if(myProcesses[waitingLine[head]].burst == 0){
            printf("Time %d: %s finished\n", i, myProcesses[waitingLine[head]].name);
			myProcesses[waitingLine[head]].turnaround = i - myProcesses[waitingLine[head]].arrival;
            waitingLine[head] = -1;
            head++;
			
			//printf("\n %d | %d | %d\n\n", waitingLine[0], waitingLine[1], waitingLine[2]); 
			
			int Counter = 0; 
			for(int j = 0; j < processCount; j++)
				if(waitingLine[Counter] == -1) 
					Counter++;
			
			
			if(Counter == processCount){
				printf("Time %d: IDLE\n", i);
				printf("Finished at time %d\n\n", ++i);  
				break;
			}
			
			if(i == 14) break; 
        }

        if(myProcesses[waitingLine[head]].quantumCounter == quantum) {
            myProcesses[waitingLine[head]].quantumCounter = 0; // set the quantum to -
            myProcesses[waitingLine[head]].selected = 0; // deselect the current process
            waitingLine[tail] = waitingLine[head];
            tail++;
            waitingLine[head] = -1;
            head++;
			
            if(tail >= processCount){
                if(head >= processCount) head = 0;
                tail = 0;
			}
            else if(head >= processCount){
                head = 0;
            }
        }

		if(myProcesses[waitingLine[head]].selected == 0){
        	printf("Time %d: %s selected (burst %d)\n", i, myProcesses[waitingLine[head]].name, myProcesses[waitingLine[head]].burst);
            myProcesses[waitingLine[head]].selected = 1;
            myProcesses[waitingLine[head]].burst--;
            myProcesses[waitingLine[head]].quantumCounter++;
        }
        else {
            myProcesses[waitingLine[head]].burst--;
            myProcesses[waitingLine[head]].quantumCounter++;
        }
		
		// increment the wait time for the processes that are not selected
		int x = head+1; 
		while(waitingLine[x] != -1){
			if(x >= processCount) x = 0; 
			if(x == head) break; 
			if( waitingLine[x] != -1){
				myProcesses[waitingLine[x]].wait++; 
				x++; 	
			}
		}
	} // end of run time
	
	for(int i = 0; i < processCount-1; i++){
		printf("%s wait %d turnaround %d\n", myProcesses[i].name, myProcesses[i].wait, myProcesses[i].turnaround);
	}
	

	// free the memory
	free(myProcesses);
	free(waitingLine);

}

// Print current elements in the queue
void printMe(struct process* myProcesses){
	printf("%s burst: %d	arrival: %d\n", myProcesses[0].name, myProcesses[0].burst, myProcesses[0].arrival);
	printf("%s burst: %d	arrival: %d\n", myProcesses[1].name, myProcesses[1].burst, myProcesses[1].arrival);
}