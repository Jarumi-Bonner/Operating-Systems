/* -------Bugs----------
 - Check to make sure variables names are correct before storing data
 - Make sure parser does not break when Quantum time is commented out for non RR
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
};

typedef struct processInfo ProcessInfo;

/*Function prototypes*/
int* parseProcessInfo(FILE *file);
ProcessInfo* getProcessInfo(FILE *file, int numberOfProcesses);
void shortestJobFirst(FILE* out, int* processVariables, ProcessInfo* allProcesses);
void rr(FILE* out, int* processVariables, ProcessInfo* allProcesses);

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
	file = fopen ("set4_process.in", "r");
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
			//firstComeFirstServe();
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
				temp[i].pWaitTime = 0;
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


//Implentation of Round-Robin
void rr(FILE* out, int* processVariables, ProcessInfo* allProcesses){

  int timeLimit = processVariables[1], time, i;
  int numberOfProcesses = processVariables[0];
  int quantum = processVariables[3];
  int currentprocess = -1, currentBurst= 0;
  bool isFinished[numberOfProcesses];
  int finishedCount =0;
	fprintf(out, "%d proccess\n Using Round-Robin\n Quantum %d\n\n", processVariables[0], processVariables[3]);

  for (time = 0; time < timeLimit; time++){
    printf("Time is %d\n", time);

    for(i = 0; i<numberOfProcesses; i++){
      if(allProcesses[i].pArrivalTime == time){
        fprintf(out, "Time %d: %s arrived\n", time, allProcesses[i].pName);
        currentBurst = allProcesses[i].pBurst;
        if(currentBurst > quantum)
          currentBurst = currentBurst - quantum;
      }
    }
    // If there's no process currently running
    if(currentprocess < 0){
        fprintf(out, "Timer %d: IDLE\n", time);
    }
  }
  if(finishedCount == processCount){
    fprintf(out, "Finished at time %d\n\n", timeLimit);
    for(i = 0; i < processCount; i++){
      fprintf(out, "%s wait %d turnaround \n", allProcesses[i].pName, allProcesses[i].pWaitTime);
    }
  }else{
    fprintf(out, "Px did not finish");
  }
  //
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


cmpA(const void * a, const void *b){
  ProcessInfo * processA = (ProcessInfo *)a;
  ProcessInfo * processB = (ProcessInfo *)b;

  return  (processA->pArrivalTime  - processB->pArrivalTime);
}
