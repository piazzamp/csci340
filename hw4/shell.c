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
#include <fcntl.h>
#include <signal.h>
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
//					hw2/3					//
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
	int found1 = 0, i, pipeindex = -1, isbg = 0, redirindex = -1;
	// if we see pipeindex > 0 then we know 2 cmds exist :)
	char fpath1[256], fpath2[256], *pipe_p;
	// put a new cmd on the stack for pipes
	command_t cmd2;
	// if the first char in the last argv element is an '&'
	if (p_cmd->argv[(p_cmd->argc) - 1][0] == '&'){
		isbg = 1;
		// remove '&' from argv and make necessary, following changes
		free(p_cmd->argv[(p_cmd->argc) - 1]);
		p_cmd->argv[(p_cmd->argc) - 1] = NULL;
		p_cmd->argc--;
	}
	// fpath can be on the stack ie: char fpath[];
	for (i = 0; i < p_cmd->argc; i++){
		if (p_cmd->argv[i][0] == '>'){
			redirindex = i;
			if (DEBUG){
				printf("'>' found at index %d\n", i);
			}
		}
		if (p_cmd->argv[i][0] == '|'){
			// everything after the | is the 2nd cmd
			pipeindex = i;
			if (DEBUG) { printf("pipe found at index %d, argc = %d\n", pipeindex, p_cmd->argc); }	
			pipe_p = p_cmd->argv[i]; // so you can put it back and free it in main	
			p_cmd->argv[i] = NULL; // replace | with NULL to end argv array for execv	
			cmd2.argc = (p_cmd->argc) - i - 1;
			cmd2.argv = &(p_cmd->argv[i + 1]); // need a pointer to a pointer, one after NULL
			cmd2.name = cmd2.argv[0];
			if (DEBUG) { printf("cmd2.argc = %d\n", cmd2.argc); }
			if (DEBUG){ printf("cmd2.argc = %d\ncmd2.argv[0] = %s\n", cmd2.argc, cmd2.argv[0]); }
			cmd2.argv = &(p_cmd->argv[i + 1]);	
			break;
		}
	}
	// now we have *p_cmd and cmd2 to fork & execv() with a pipe between them
	if (pipeindex < 0 && redirindex == -1){
		// one cmd, put hw2 code here!
		found1 = find_fullpath(fpath1, p_cmd);
   	if (DEBUG) { printf("forking, hold on to ur butts\n"); }
   	pid_t pid = fork();
   	if (found1){
      	if (pid == 0){ 
         	// then this code executes in the child process
         	// let the p_cmd->name command take over this process
         	return execv(fpath1, p_cmd->argv);
      	}
      	else if (!isbg){
         	return wait(&pid);
      	}
   	}
   	else {
      	fprintf(stderr, "command %s not found in your path\n", p_cmd->name);
      	return 127;
   	}
   	return -1;
	}
	else if (pipeindex > 0 && redirindex == -1){
		// have 2 cmds, do the pipe-n-fork thing
		int status, fds[2];
		pid_t pid1, pid2;
		if (DEBUG) { printf("piping...\n"); }
		if (pipe(fds) != 0){
			// can't create pipe! bail out
			fprintf(stderr, "there was an issue piping your data\n");
			return -1;
		}
		if ((pid1 = fork()) == 0){
			if (!find_fullpath(fpath1, p_cmd)) {
				fprintf(stderr, "sorry, %s was not found in your $PATH :(", p_cmd->name);
				return -1;
			}
			if (DEBUG){ printf("p_cmd fullpath = %s\n", fpath1); }
			close(1); // stdout in this process is now NULL
			dup(fds[1]); // clone new fd to fill stdout's position
			close(fds[0]); // this is for the other process
			execv(fpath1, p_cmd->argv);
		}
		if ((pid2 = fork()) == 0){
			if (!find_fullpath(fpath2, &cmd2)){
				fprintf(stderr, "sorry, %s was not found in your $PATH :(\n", cmd2.name);	
				return -1;
			}
			if (DEBUG ){ printf("cmd2 fullpath = %s\n", fpath2); }
			close(0);
			dup(fds[0]);
			close(fds[1]);
			execv(fpath2, cmd2.argv);
		}
		close(fds[0]);
		close(fds[1]);
		waitpid(pid1, &status, 0);
		if (!isbg) {
			waitpid(pid2, &status, 0);
		}
		p_cmd->argv[pipeindex] = pipe_p;
		// point that NULL back at the pipe so everything will be freed in main
		return 0;
	}
	else if (redirindex > -1 && pipeindex == -1) {
		int outfile, child_status;
		pid_t cpid;
		outfile = open(p_cmd->argv[redirindex + 1], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP);
		// end argv with a NULL where the > was, and keep a pointer to >
      char *redir_p = p_cmd->argv[redirindex];
      p_cmd->argv[redirindex] = NULL;	

		if(outfile == -1 ){
			fprintf(stderr, "failed to open file %s for writing, sorry\n", p_cmd->argv[redirindex]);
		}
		else if ((cpid = fork()) == 0){
			// in the child process	
			if (find_fullpath(fpath1, p_cmd)){
				dup2(outfile, 1);
				execv(fpath1, p_cmd->argv);
			} else {
				fprintf(stderr, "sorry, %s was not found in your $PATH :(\n", cmd2.name);
            return -1;
			}
		}
		waitpid(cpid, &child_status, 0);
		p_cmd->argv[redirindex] = redir_p;
	}
	return -1;
/*
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
*/
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
