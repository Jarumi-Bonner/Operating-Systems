/* -------Bugs----------
 - Check to make sure variables names are correct before storing data
 - Processes can overrun so gracefully inform user
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


// Toggle this variable if you want to print all parsed data
#define FLAG 1

// Define struct to hold each process
struct processInfo{

    char pName[256];
	int pArrivalTime;
	int pBurst;
	int pWaitTime;
	int pTurnaroundTime;
	int pBurstRemaining;
};

typedef struct processInfo ProcessInfo;

/*Function prototypes*/
int* parseProcessInfo(FILE *file);
ProcessInfo* getProcessInfo(FILE *file, int numberOfProcesses);
void shortestJobFirst(FILE* out, int* processVariables, ProcessInfo* allProcesses);
void rr(FILE* out, int* processVariables, ProcessInfo* allProcesses);
void firstComeFirstServe(FILE *out ,int* processVariables, ProcessInfo* allProcesses );
void swap(int *a, int *b);
void bubbles(int array[], int length);

//Possible Struct for processes info and Queue

/*
struct Queue {

	struct Queue *next;
}tyepedef Queue;
*/

/* Gobal Variables*/

int timer = 0,
currentprocess = -1,
processCount;

int main (void){

	/*Declare Variables */
	FILE *file, *out;
	int i, x, y, z;

	/* Open file for processing data */
	file = fopen ("set2_process.in", "r");
	out = fopen("process.out", "w+");

	//Contains all data related to the scheduler
	int* processVariables = parseProcessInfo(file);
	processCount = processVariables[0];

	if(FLAG){
		for(x = 0; x < 4; x++){
			printf("%d\n", processVariables[x]);
		}
	}

	// declare array to contain each process and its properties
	ProcessInfo* allProcesses = malloc(processCount *sizeof(ProcessInfo));

	// Call method to read the rest the file and get the processes info
	allProcesses = getProcessInfo(file, processCount);

	//Prints data for troubleshooting
	if(FLAG){
		for (i = 0 ; i < processCount ; i++){
			printf("%s %d %d %d\n", allProcesses[i].pName , allProcesses[i].pArrivalTime, allProcesses[i].pBurst,allProcesses[i].pWaitTime);
		}
	}

	/* Closes file*/
	fclose(file);

	//* Insert code for scheduling methods*/
	switch(processVariables[2]){


		case 0: // First Come First Serve
			firstComeFirstServe(out,processVariables, allProcesses);
			break;


		case 1: // Shortest Job First
            shortestJobFirst(out, processVariables, allProcesses);
            break;

		case 2: // Round Robin
		        rr(out,processVariables, allProcesses);
            break;
		// Call Function to Round-Robin

		break;

	}
    fclose(out);
    // Ayee! Free Kodak
    free(allProcesses);
	return 0;

}
// Read through the rest of the file and return an array that contains
// each process's properties
ProcessInfo* getProcessInfo(FILE *file, int numberOfProcesses)
{
    ProcessInfo* temp;
    char garbage[25];
    char line[100];
    char pName[20];
    int i = 0;
    int arrivalTime;
    int burst;

    // declare temp array
    temp = malloc(sizeof(ProcessInfo)* numberOfProcesses);

    while(fgets(line, sizeof line, file) && i < numberOfProcesses ) {
			if(!(strcmp(line, "end\n") == 0)){
				sscanf(line, "%s %s %s %s %d %s %d", garbage, garbage, temp[i].pName, garbage, &arrivalTime, garbage, &burst);
				temp[i].pArrivalTime = arrivalTime;
				temp[i].pBurst = burst;
				temp[i].pTurnaroundTime = 0;
				temp[i].pWaitTime = 0;
				temp[i].pBurstRemaining = temp[i].pBurst;
				i++;
			}
	}

	return temp;
}
int* parseProcessInfo(FILE *file){

	int* processVariables;
	/* Process Variables array
	// 0			            	1  			     	2			       	3
	// +-------------+  +-----------+	+-----------+	+---------------+
	// | ProcessCount|  | runTime	|	| Method	|	|QuantumTime	|
	// +-------------+  +-----------+	+-----------+	+---------------+
	*/
	int i= 0;
	int j = 0;
	char line[100];
	char garbage[25];
	char temp[5];
	char tempChar;
	/* method[] Array to digitilize process method
		fcfs = 0
		sjf = 1
		rr = 2
	*/
	char* method[] = {"fcfs", "sjf", "rr"};


	//malloc size of array to hold variables
	processVariables = malloc(sizeof(int) *4);

	/* Parses file and stores data accordingly */
	while( i < 4 ) {

		fgets(line, sizeof line, file);

		/* store variable names to garbage, store respective valuable data to corresponding postion in processVariables[]
		else condition handles string comparison to assignment process method as a digit*/
		if(i != 2 && i != 3){
			sscanf(line, "%s %d", garbage, &processVariables[i]);
	  	}//ends if statement

	   if (i == 2){
	  	sscanf(line, "%s %s", garbage, temp);
	  	//compares strings to method array and assigns correct number
		  	for(j = 0 ; j < 3 ; j++ ){
		  		if(!strcmp(temp,method[j])){
	            	processVariables[i] = j;
	        	}//ends if statement
	        }//end for loop
	  	}//end if statement

	  	if(i == 3 && processVariables[2] == 2 ){
	  		sscanf(line, "%s %d", garbage, &processVariables[3]);
	  	}//end if statement
	  	else
	  		processVariables[3] = 0;

	  	i++;

	}//ends while loop

	return processVariables;
}// End parseProcessInfo() function


// Swaps the integers pointed to by a and b.
void swap(int *a, int *b) {
     int temp = *a;
     *a = *b;
     *b = temp;
}


void bubbles(int array[], int length){
int i, j;

  for (i = length-1; i > 0; i--) {
    for (j = 0; j < i; j++)
      if (array[j] > array[j+1])
          swap(&array[j], &array[j+1]);
  }
}

//Implentation of Round-Robin
void rr(FILE* out, int* processVariables, ProcessInfo* allProcesses){

  int timeLimit = processVariables[1], time=0, i;
  int numberOfProcesses = processVariables[0];
  int quantum = processVariables[3];
  int currentprocess=0, currentBurst= 0;
  bool isFinished[numberOfProcesses];
  int finishedCount =0;
  int processOrder[numberOfProcesses];
  int complete = 0;
  int idle = 1, next=0;
  int pCounter =0;

	fprintf(out, "%d process\n Using Round-Robin\n Quantum %d\n\n", processVariables[0], processVariables[3]);

  //Sort the process by arrival time
  for(i=0; i<numberOfProcesses; i++){
    processOrder[i] = allProcesses[i].pArrivalTime;
  }
  bubbles(processOrder, numberOfProcesses);

  for (i = 0 ; i < processCount; i++){
    if(allProcesses[i].pArrivalTime == processOrder[complete]){
      currentprocess = i;
      allProcesses[i].pWaitTime = 0;
      allProcesses[i].pBurstRemaining = allProcesses[i].pBurst;
      printf("first process : %s burst %d Arrival time %d", allProcesses[currentprocess].pName, allProcesses[currentprocess].pBurst, allProcesses[currentprocess].pArrivalTime);
      pCounter++;
    }
  }

  fprintf(out, "Time %d: %s arrived\n", time, allProcesses[currentprocess].pName);
  fprintf(out,"Time %d: %s selected (burst %d)\n", time, allProcesses[currentprocess].pName, allProcesses[currentprocess].pBurst);

  while(time <= timeLimit){
    while(currentBurst < quantum){
      currentBurst++;
      time++;
      allProcesses[currentprocess].pBurstRemaining--;
      next = 1;
    //  fprintf(out, "test\n");
    //
    //if(allProcesses[currentprocess].pArrivalTime == processOrder[complete])
      //fprintf(out, "Time %d: %s arrived\n", time, allProcesses[currentprocess].pName);
    }
    complete++;

    //fprintf(out,"Time %d: %s selected (burst %d)\n", time, allProcesses[currentprocess].pName,allProcesses[currentprocess].pBurstRemaining);

    printf("Time is %d\n", time);


    if (next){
    for (i = 0 ; i < processCount; i++){
      if(allProcesses[i].pArrivalTime == processOrder[complete]){


        currentprocess = i;
        allProcesses[currentprocess].pWaitTime = time - allProcesses[currentprocess].pArrivalTime;

        fprintf(out,"Time *%d: %s selected (burst %d)\n", time, allProcesses[currentprocess].pName,allProcesses[currentprocess].pBurstRemaining);
        next = 0;
        complete = 0;
      }
    }
  }
  time++;
  }
	return;
}

/*Begin Shortest Job First Algorithm */
void shortestJobFirst(FILE* out, int* processVariables, ProcessInfo* allProcesses)
{
    int timeLimit = processVariables[1];
    int currentBurst = 999, nextProcessToRun = -1;
    int finishedCount =0, time, i;
    processCount = processVariables[0];
    currentprocess = -1;
    bool isFinished[processCount];

    fprintf(out, "%d processes \nUsing Shortest Job First (Pre) \n\n", processCount);

    for (time = 0; time <= timeLimit; time++)
    {
        printf("Time is %d\n", time);

        // If there's a process currently running
        if(currentprocess >= 0)
        {
            // decrease the burst
            allProcesses[currentprocess].pBurst--;
            // Process just finished
            if(allProcesses[currentprocess].pBurst == 0)
            {
                fprintf(out, "Time %d: %s Finished\n", time, allProcesses[currentprocess].pName);
                isFinished[currentprocess] = true;
                currentprocess = -1;
                currentBurst = 999;
                finishedCount++;
            }
        }

        for(i = 0; i < processCount; i++)
        {
            // New process arrived
            if(allProcesses[i].pArrivalTime == time)
            {
                fprintf(out, "Time %d: %s arrived\n", time, allProcesses[i].pName);
                if(allProcesses[i].pBurst < currentBurst)
                {
                    currentBurst = allProcesses[i].pBurst;
                    nextProcessToRun = i;
                }
            }
            if(currentprocess < 0)
            {
                if(time > allProcesses[i].pArrivalTime && !isFinished[i])
                {
                    if(allProcesses[i].pBurst < currentBurst)
                    {
                        currentBurst = allProcesses[i].pBurst;
                        nextProcessToRun = i;
                    }
                }
            }
			/* Wrong logic for wait time *Reminder*
			else
			{
				if(currentprocess != i && time > allProcesses[i].pArrivalTime)
				{
					allProcesses[i].pWaitTime++;
				}
			}
			*/
        }
        if (nextProcessToRun >=0 && nextProcessToRun != currentprocess && !isFinished[nextProcessToRun])
        {
            currentprocess = nextProcessToRun;
            fprintf(out, "Time %d: %s selected (burst %d)\n", time, allProcesses[currentprocess].pName, allProcesses[currentprocess].pBurst);
        }
        // If there's no process currently running
        if(currentprocess < 0)
        {
            fprintf(out, "Timer %d: IDLE\n", time);
        }
    }
    if(finishedCount == processCount)
    {

        fprintf(out, "Finished at time %d\n\n", timeLimit);
		for(i = 0; i < processCount; i++)
		{
			fprintf(out, "%s wait %d turnaround \n", allProcesses[i].pName, allProcesses[i].pWaitTime);
		}

    }
    else
    {
        fprintf(out, "Px did not finish");
    }

}


void firstComeFirstServe(FILE *out ,int* processVariables, ProcessInfo* allProcesses){
	int processOrder[processCount];
	int i = 0;
	int j = 0;
	int complete = 0;
	int next = 0;
	int swap = 0;
	int runTime = processVariables[1];


	//Print number of processes and schedule method
	fprintf(out,"%d Processes\n", processCount);
	fprintf(out,"Using First Come First Serve\n\n\n");

	//Sort Processes by order of arrival
	for (i = 0 ; i < processCount; i++){
		processOrder[i] = allProcesses[i].pArrivalTime;
	}

    for (i = 0; i < processCount; ++i){
		for (j = i + 1; j < processCount; ++j){
			if (processOrder[i] > processOrder[j]){
				swap =  processOrder[i];
				processOrder[i] = processOrder[j];
				processOrder[j] = swap;
			}
    	}
	}

	//Select First Process
	for (i = 0 ; i < processCount; i++){
		if(allProcesses[i].pArrivalTime == processOrder[complete])
			currentprocess = i;
			allProcesses[i].pWaitTime = 0;

	}
	// Print arrival of first process
		fprintf(out,"Time %d: %s arrived\n", timer, allProcesses[currentprocess].pName );
		fprintf(out,"Time %d: %s selected (burst %d)\n", timer, allProcesses[currentprocess].pName, allProcesses[currentprocess].pBurst);

	//While loop to emulate the total runtime of the scheduler
	while(timer <= runTime){

		//Check if new process has arrived
		for (i = processCount-1; i >= 1 ; i--){
			if(allProcesses[i].pArrivalTime == timer)
				fprintf(out,"Time %d: %s arrived\n", timer, allProcesses[i].pName );

		}

		//Current Process has finished
		if(allProcesses[currentprocess].pBurstRemaining == 0){
			next = 1;
			allProcesses[currentprocess].pTurnaroundTime = timer - allProcesses[currentprocess].pArrivalTime;
			fprintf(out,"Time %d: %s finished \n", timer, allProcesses[currentprocess].pName );
			complete++;
		}

		//if old process has finish find next process
		if (next){
			for (i = 0 ; i < processCount; i++){
				if(allProcesses[i].pArrivalTime == processOrder[complete]){
					currentprocess = i;
					allProcesses[currentprocess].pWaitTime = timer - allProcesses[currentprocess].pArrivalTime;
					fprintf(out,"Time %d: %s selected (burst %d)\n", timer, allProcesses[currentprocess].pName,allProcesses[currentprocess].pBurst );
					next = 0;
				}
			}
		}


	// 	//otherwise increase timer
	 	timer++;
		allProcesses[currentprocess].pBurstRemaining--;


	 }//ends while loop

	fprintf(out,"Finished at time %d\n\n", timer - 1);

	if (complete == processCount){
		for (i = 0 ; i < processCount ; i ++){
			fprintf(out,"%s wait %d turnaround %d\n", allProcesses[i].pName, allProcesses[i].pWaitTime, allProcesses[i].pTurnaroundTime);
		}
	}
	else
		fprintf(out,"Fail! All processes could not be completed in the given time.");

	return;
}
