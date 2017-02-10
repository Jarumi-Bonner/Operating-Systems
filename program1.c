/* -------Bugs----------
 - Check to make sure variables names are correct before storing data
 - Make sure parser does not break when Quantum time is commented out for non RR
 - Processes can overrun so gracefully inform user 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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
void shortestJobFirst(int* processVariables, ProcessInfo* allProcesses);
void rr(FILE *out, int* processVariables);

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
			printf("%s %d %d\n", allProcesses[i].pName , allProcesses[i].pArrivalTime, allProcesses[i].pBurst );
		}
	}

	
	

	/* Closes file*/
	fclose(file);

	//* Insert code for scheduling methods*/
	switch(processVariables[2]){


		case 0: // First Come First Serve
			break;


		case 1: // Shortest Job First
            shortestJobFirst(processVariables, allProcesses);
            break;

		case 2: // Round Robin
		// rr(out,processVariables);
		// Call Function to Round-Robin

		break;

	}

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
				i++;
			}
	}

	return temp;
}
int* parseProcessInfo(FILE *file){

	int* processVariables;
	/* Process Variables array
	// 0				1  				2				3
	// +-------------+  +-----------+	+-----------+	+---------------+
	// | ProcessCount|  | runTime	|	| Method	|	|QuantumTime	|
	// +-------------+  +-----------+	+-----------+	+---------------+
	*/
	int i= 0;
	int j = 0;
	char line[100];
	char garbage[25];
	char temp[5];
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
		if(i < 4 && i != 2){
			sscanf(line, "%s %d", garbage, &processVariables[i++]);
	  	}//ends if statement
	  else{
	  	sscanf(line, "%s %s", garbage, temp);
	  	//compares strings to method array and assigns correct number
	  	for(j = 0 ; j < 3 ; j++ ){
	  		if(!strcmp(temp,method[j])){
            	processVariables[i++] = j;
        	}//ends if statement
        }//end for loop
	  }//end else statement
	}//ends while loop

	return processVariables;
}// End parseProcessInfo() function


//Implentation of Round-Robin
void rr(FILE* out, int* processVariables/*, insert 2d array or struct*/){

	fprintf(out, "%d proccess\n Using Round-Robin\n Quantum %d", processVariables[0], processVariables[3]);
	return;
}

void shortestJobFirst(int* processVariables, ProcessInfo* allProcesses)
{


}