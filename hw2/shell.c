// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

#define DEBUG 1
#define PATHSIZE 256

const char* valid_builtin_commands[] = {"cd", "exit", NULL};
void parse( char* line, command_t* p_cmd );
int execute( command_t* p_cmd );
int find_fullpath( char* fullpath, command_t* p_cmd );
int is_builtin( command_t* p_cmd );
int do_builtin( command_t* p_cmd );
void cleanup( command_t* p_cmd );

//-------------------------------//
// 	 BEGIN IMPLEMENTATION		//
//					hw2					//
//        by Matt Piazza			//
//-------------------------------//
void parse( char* line, command_t* p_cmd ){
	int i, arglength = 0, argcount = 0, argiter = 0, j;
	for (i = 0; line[i] != '\0'; i++){ // loop through the string once
		if (line[i] == ' '){
			argcount++; // not always -- think trailing space
			while (line[i] == ' '){ // eat any extra spaces
				i++;						// what happens with a trailing space?
			}
		}
	}
	
	if (DEBUG){
		printf("line '%s has %d args\n'", line, argcount);
	}
	// now we know enough to create our argument vector
	// (but not enough to allocate the strings for it)
	char **args = (char **) malloc(sizeof(char *) * argcount);

	i = 0;
	// i holds the starting position of each arg
	while (argiter < argcount){
		// loop through the line to allocate and populate args[]
		// first: determine argument length and allocate n+1 chars
		arglength = 0;
		while (line[i + arglength] != ' '){
			arglength++;
		}
      args[argiter] = (char*) malloc(sizeof(char) * (arglength + 1));		
      for (j = i; j - i < arglength; j++){
         args[argiter][j - i] = line[j];
      }
		i += arglength; // new starting char is arglength spaces forward
		while (line[i] == ' '){ // have to eat extra spaces again
			i++;
		}
		args[argiter][arglength] = '\0';
		argiter++;
	}
	args[argcount] = NULL; // end the array
	if (DEBUG){
		for (i = 0; args[i] != NULL; i++){
			printf("args[%d] = '%s'\n", i, args[i]);
		}
	}
	p_cmd->name = args[0];
	p_cmd->argc = argcount;
	p_cmd->argv = args;
	// change here if the first element in argv should _NOT_ be the command name
}


int execute( command_t* p_cmd ){
	int found = 0;
	char *fpath = NULL;
	found = find_fullpath(fpath, p_cmd);
	pid_t pid = fork();
	if (found){
		if (pid == 0){ 
			// then this code executes in the child process
			// let the p_cmd->name command take over this process
			return execv(fpath, p_cmd->argv);
		}
		else{
			return wait(&pid);
		}
	}
	else {
		fprintf(stderr, "command %s not found in your path\n", p_cmd->name);
		return 127;
	}
	return -1;
}
int find_fullpath( char* fullpath, command_t* p_cmd){
	// first check if p_cmd->name is a fully qualified path to an executable
	char *path = getenv("PATH\0");
	char *testpath = (char *) malloc(sizeof(char) * PATHSIZE);
	int startingchar = 0, i = 0, found = 0, j, k;
	struct stat status;
	while (path[i] != '\0' && path[i - 1] != '\0' && !found){
		for (j = 0; j < PATHSIZE; j++){
			testpath[j] = '\0';
		}
		//currentpath = &(path[startingchar]);
		int j = 0;
		while(path[i] != ':' && path[i] != '\0'){
			testpath[j] = path[i];
			i++;
			j++;
		} 
		if (DEBUG){
			printf("colon found at index %d in %s\n", i, path);
		}
		testpath[j] = '/';
		// testpath[i + 1]  = '\0';
		k = 0;
		for (j += 1; p_cmd->name[k] != '\0'; j++){
			testpath[j] = p_cmd->name[k];
			k++;
		}
		if (DEBUG){ printf("testpath = %s\ni:%d\n\n", testpath, i); }
		//stat(testpath, &status);
		//if (DEBUG) { printf(); }
		//if (status.st_mode == S_IXOTH){
			// file found and is executable by 'others'
		if (fopen(testpath, "r") != NULL){
			// file is exists and is readable
			for (j = 0; testpath[j] != '\0'; j++){
				fullpath[j] = testpath[j];
			}	
			if (DEBUG) { printf("in fullpath, fullpath = '%s' @ %p\n", fullpath, fullpath); }
			return 1;
		}
		startingchar = ++i;
	}
	return 0;
}
// int is_builtin( command_t* p_cmd );
// int do_builtin( command_t* p_cmd );
// void cleanup( command_t* p_cmd );
