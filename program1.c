#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Toggle this variable if you want to print all parsed data
#define FLAG 0

/*Function prototypes*/
int* parseProcessInfo(FILE *file);
int** parseProcesses(FILE *file, int numberOfProcesses);


int main (void){

	/*Declare Variables */
	FILE *file;
	int x, y, z;

	/* Open file for processing data */
	file = fopen ("set2_process.in", "r");

	//Contains all data related to the scheduler
	int* processVariables = parseProcessInfo(file);

	//Prints data for troubleshooting
	if(FLAG){
		for(x = 0; x <4; x++){
			printf("%d\n", processVariables[x]);
		}
	}
	
	/* Contains all data related to each process in 2D array. 
	Passes File and number of processes*/
	int** processes = parseProcesses(file, processVariables[0]);

	//Prints data for troubleshooting
	if(FLAG){
		for(x = 0; x < processVariables[0]; x++){
				printf("%d %d\n",processes[x][0], processes[x][1] );
		}
	}

	/* Closes file*/
	fclose(file);

	//* Insert code for scheduling methods*/





	

	return 0;
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
	//   +-----------+	+-----------+	
	//   | arrival	|	| burst		|	
	//   +-----------+	+-----------+	
	// 		(1)P2						  												
	//   +-----------+	+-----------+	
	//   | arrival	|	| burst		|	
	//   +-----------+	+-----------+
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
		processes[i] = malloc(sizeof(int) * 3);

	}

	//Scans rest of file, stores useless variables to garbage and stores data to each process 
	while(fgets(line, sizeof line, file) ) {
			if(!(strcmp(line, "end\n") == 0)){
				sscanf(line, "%s %s %s %s %d %s %d", garbage, garbage, garbage, garbage, &processes[j][0], garbage, &processes[j][1]);
				j++;
			}	
	}

	return processes;

}
