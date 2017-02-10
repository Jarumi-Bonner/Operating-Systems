/*
	add array to store names
	check variable name before store data
	processes can overrun

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Toggle this variable if you want to print all parsed data
#define FLAG 1

// Define struct to hold each process
struct processInfo{

    char* pName;
	int pArrivalTime;
	int pBurst;
	int pWaitTime;
};

typedef struct processInfo ProcessInfo;


/*Function prototypes*/
int* parseProcessInfo(FILE *file);
int** parseProcesses(FILE *file, int numberOfProcesses);
ProcessInfo* getProcessInfo(FILE *file, int numberOfProcesses);
void shortestJobFirst(int* processVariables, ProcessInfo* allProcesses);
void rr(FILE *out, int* processVariables);

//Possible Struct for processes info and Queue

/*
struct Queue {

	struct Queue *next;
}tyepedef Queue;
*/

int timer =0, currentprocess = -1, processCount;
int main (void){

	/*Declare Variables */
	FILE *file, *out;
	int i, x, y, z;
	// Made these variable global cause they'll be used throughout the program
	//int timer = 0;
	//int currentprocess = 0;

	/* Open file for processing data */
	file = fopen ("set1_process.in", "r");

	out = fopen("process.out", "w+");
	//Contains all data related to the scheduler
	int* processVariables = parseProcessInfo(file);
	processCount = processVariables[0];

	// declare array to contain each process and its properties
	ProcessInfo* allProcesses = malloc(processCount *sizeof(ProcessInfo));
	// Call method to read the rest the file and get the processes info
	allProcesses = getProcessInfo(file, processCount);
	//Prints data for troubleshooting

	/*
	if(FLAG){
		for(x = 0; x < 4; x++){
			printf("%d\n", processVariables[x]);`
		}
	}
	*/

	/* Contains all data related to each process in 2D array.
	Passes File and number of processes*/
	int** processes = parseProcesses(file, processCount);

	//Prints data for troubleshooting
	if(FLAG){
		for(x = 0; x < processCount; x++){
				printf("%d %d %d %d\n",processes[x][0], processes[x][1],
								processes[x][2], processes[x][3]);
		}
	}

	/* Closes file*/
	fclose(file);

	//* Insert code for scheduling methods*/
	switch(processVariables[2]){


		case 0: // First Come First Serve
		if (FLAG){
			printf("Enter FCFS\n");
		}

		//find first process to arrive
			for (i = processCount -1; i >= 1 ; i--){

				if(processes[i][0] < processes[(i-1)][0])
					currentprocess = i;
			}

		//While loop to emulate the total runtime of the scheduler
		while(timer < processVariables[1]){
			//Check if new process has arrived

			//if new process arrive alert scheduler


			//Check if old process finish


			//if old process finish find next process


			//otherwise increase timer











		}


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
    temp = (ProcessInfo*)malloc(numberOfProcesses * sizeof(ProcessInfo));

    while(fgets(line, sizeof line, file) && i <= numberOfProcesses ) {
			if(!(strcmp(line, "end\n") == 0)){
				sscanf(line, "%s %s %s %s %d %s %d", garbage, garbage, pName, garbage, arrivalTime, garbage, burst);
				temp[i].pName = (char*)malloc(sizeof(char*));
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


int** parseProcesses(FILE *file, int numberOfProcesses){
	int** processes;
	/* Processes 2D array
	// 		(0)P1
	//   +-----------+	+-----------+	+----------------+	+-----------+
	//   | arrival	|	| burst		|	|Remaining burst |	|	Wait	|
	//   +-----------+	+-----------+	+----------------+	+-----------+
	// 		(1)P2
	//   +-----------+	+-----------+	+----------------+	+-----------+
	//   | arrival	|	| burst		|	|Remaining burst |	|	Wait	|
	//   +-----------+	+-----------+	+----------------+	+-----------+
	//   etc...
	*/

	char garbage[25];
	char line[100];
	int j = 0;
	int i = 0;
	int k = 0;

	//declaring array of int pointer to store each process
	processes = malloc(sizeof(int*) * numberOfProcesses);

	//declares following arrays to store individual data for each process
	for(i = 0; i < numberOfProcesses; i++){
		processes[i] = malloc(sizeof(int) * 4);

	}

	//Scans rest of file, stores useless variables to garbage and stores data to each process
	while(fgets(line, sizeof line, file) ) {
			if(!(strcmp(line, "end\n") == 0)){
				sscanf(line, "%s %s %s %s %d %s %d", garbage, garbage, garbage, garbage, processes[i][0], garbage, processes[i][1]);
				j++;
			}
	}

	return processes;

}


//Implentation of Round-Robin
void rr(FILE* out, int* processVariables/*, insert 2d array or struct*/){

	fprintf(out, "%d proccess\n Using Round-Robin\n Quantum %d", processVariables[0], processVariables[3]);
	return;
}

void shortestJobFirst(int* processVariables, ProcessInfo* allProcesses)
{


}
