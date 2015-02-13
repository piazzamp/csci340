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

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main( int argc, char** argv ) {

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while ( !done )
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
    //      if (parsed command is "exit")
    //      	done = true
	//  	else if ( is _builtin( ... ) )
	//			do_builtin( ... )
	//		else if ( find_fullpath( ... ) )
	//  		execute( ... )
	//		else
	//			error()
	// 		cleanup( ... )
	//
	int done = 0, i = 0;
	char line[256], path[256];
	command_t currentcmd;
	while(!done){
		printf("\nshhh~ ");
		fgets(line, 256, stdin);
		if (DEBUG){
			printf("read in line: '%s'\n", line);
		}
		if (line[0] != '\n'){
			parse(line, &currentcmd);
		
			if (samestr(currentcmd.name, "exit")) {
				done = 1;
			}
			else if (is_builtin(&currentcmd)){
				do_builtin(&currentcmd);
			}
			else if (find_fullpath(path, &currentcmd)){
				if (DEBUG) { printf("executing with currentcmd.name = %s, argv[1]==NULL? %d\n", currentcmd.name, currentcmd.argv[1] == NULL); }
				execute(&currentcmd);
			}
			else{
				fprintf(stderr, "cmd %s not found in your $PATH :(\n", currentcmd.name);
			}
			cleanup(&currentcmd);
			for (i = 0; i < 256; i++){
				path[i] = '\0';
			}
		}
	}
/*
	printf( "CSCI 340 Homework Assignment 2 - Have Fun!\n" );
	command_t cmd;
	char *path = (char *) malloc(256 * sizeof(char)); 
	parse("ls -l    ./home  \0", &cmd);
	find_fullpath(path, &cmd);
	printf("in main: found path '%s' @ %p\n", path, path);
	printf("cmd in main: name: '%s', argc: %d, argv[0]: %s, argv[1]: %s, argv[2]: %s\n ", cmd.name, cmd.argc, cmd.argv[0], cmd.argv[1], cmd.argv[2]);
	command_t cmd2;
	cmd2.name = "exit";
	printf("is_builtin(%s): %d\n", "exit", is_builtin(&cmd2));
*/
	return 0;

} // end main function
