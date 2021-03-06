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

#define DEBUG 0
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
	int i, arglength = 0, argcount = 1, argiter = 0, j;
	for (i = 0; line[i] != '\0' && line[i] != '\n'; i++){ // loop through the string once	
		if (line[i] == ' ' /* && line[i + 1] != ' ' && line[i + 1] != '\n' && line[i + 1] != '\0' */){
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
	char **args = (char **) malloc(sizeof(char *) * (argcount + 1));

	i = 0;
	// i holds the starting position of each arg
	while (argiter < argcount){
		// loop through the line to allocate and populate args[]
		// first: determine argument length and allocate n+1 chars
		arglength = 0;
		while (line[i + arglength] != ' ' && line[i + arglength] != '\n' && line[i + arglength] != '\0'){
			arglength++;
		}
		if (DEBUG) { printf("allocating chars for argv[%d]\n", argiter); }
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
	if (DEBUG) { printf("assigning p_cmd components @ %p\n", p_cmd); }
	p_cmd->name = args[0];
	p_cmd->argc = argcount;
	p_cmd->argv = args;
	// don't double free name & argv[0]	
}


int execute( command_t* p_cmd ){
	int found = 0;
	char fpath[256];
	// fpath can be on the stack ie: char fpath[];
	found = find_fullpath(fpath, p_cmd);
	if (DEBUG) { printf("forking, hold on to ur butts\n"); }
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
		int j = 0;
		while(path[i] != ':' && path[i] != '\0'){
			testpath[j] = path[i];
			i++;
			j++;
		}
		testpath[j] = '/';	
		k = 0;
		for (j += 1; p_cmd->name[k] != '\0'; j++){
			testpath[j] = p_cmd->name[k];
			k++;
		}	
		found = stat(testpath, &status) + 1;	
		if (found && (status.st_mode & S_IFREG)){
			// file found & is accessible
			for (j = 0; testpath[j] != '\0'; j++){
				fullpath[j] = testpath[j];
			}	
			fullpath[j] = '\0';
			if (DEBUG) { 
				printf("in fullpath, fullpath = '%s' @ %p\n", fullpath, fullpath); }
			return 1;
		}
		startingchar = ++i;
	}
	free(testpath);
	return 0;
}

int samestr(const char *a, const char *b){
	int i;
	for (i = 0; a[i] != '\0' && b[i] != '\0'; i++){
		if (a[i] != b[i]){
			return 0;
		}
	}
	if (DEBUG){
		printf("in samestr: i: %d, a[i]: %c, b[i]: %c\n", i, a[i], b[i]);
	}
	if (a[i] == '\0' && b[i] == '\0'){
		return 1;
	}
	else{
		return 0;
	}
}

int is_builtin( command_t* p_cmd ){
	int i;
	for (i = 0; valid_builtin_commands[i] != NULL; i++){
		if (samestr(valid_builtin_commands[i], p_cmd->name)){
			return 1;
		}
	}
	return 0;
}

int do_builtin( command_t* p_cmd ){
	if (samestr(p_cmd->name, "cd")){
		int success = chdir(p_cmd->argv[1]);
		// chdir returns -1 on fail, 0 on success
		if (success == 0){
			return SUCCESSFUL;
		}
		return ERROR;
	}
	else if (samestr(p_cmd->name, "exit")){	
		// will never reach this since it's handled in main's while loop
		return SUCCESSFUL;
	}
	return ERROR; // ?
}
void cleanup( command_t* p_cmd ){
	// p_cmd->name == p_cmd->argv[0]
	// do not double free it!
	int i;
	for (i = 0; p_cmd->argv[i] != NULL; i++){
		// free each of the strings in argv
		free(p_cmd->argv[i]);
	}
	// then free the array of pointers itself
	free(p_cmd->argv);
}
