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

	printf( "CSCI 340 Homework Assignment 2 - Have Fun!\n" );
	command_t cmd;
	char *path = (char *) malloc(256 * sizeof(char)); 
	parse("ls -l    ./home  \0", &cmd);
	find_fullpath(path, &cmd);
	printf("in main: found path '%s' @ %p\n", path, path);
	printf("cmd in main: name: '%s', argc: %d, argv[0]: %s, argv[1]: %s, argv[2]: %s\n ", cmd.name, cmd.argc, cmd.argv[0], cmd.argv[1], cmd.argv[2]);
	return 0;

} // end main function
