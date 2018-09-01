#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "const.h"
#include "util.h"
# define _POSIX_C_SOURCE 200809L

char *socket_path = "\0hidden";
// TODO: implement
int serial_binsearch() {
    return 0;
}

// TODO: implement
int parallel_binsearch() {
    return 0;
}

int main(int argc, char** argv) {
	int num_exp = 0, num_pot = 0, num_pos = 0;
	int opt;
    /* TODO: move this time measurement to right before the execution of each binsearch algorithms
     * in your experiment code. It now stands here just for demonstrating time measurement. */
    clock_t cbegin = clock();

    printf("[binsearch] Starting up...\n");

    /* Get the number of CPU cores available */
    printf("[binsearch] Number of cores available: '%ld'\n",
           sysconf(_SC_NPROCESSORS_ONLN));

    /* TODO: parse arguments with getopt */
	while ((opt = getopt (argc, argv, "E:T:P:")) != -1)
	{
		switch (opt)
		{
			case 'E':
				num_exp = atoi(optarg);
				if(num_exp < 1) {
					printf("-E value out of range, exiting program\n");
					exit(-1);
				}
				break;
			case 'T':
				num_pot = atoi(optarg);
				if(num_pot < 3 || num_pot > 9){
					printf("-T value out of range, exiting program\n");
					exit(-1);
				}
				break;
			case 'P':
				num_pos = atoi(optarg);
				break;
			case '?':
				printf("please use -e <number of experiments> -t <exponent of size of array> -p <position to find in array>");
				break;
		}
	}
	if(num_pos < 0 || num_pos > pow(10,num_pot)){
		printf("-P value out of range, exiting program\n"); 
		exit(-1);
	}

    /* TODO: start datagen here as a child process. */
	int pid = fork();
	if(pid == 0){
		execvp("./datagen", argv);
	}
	else if (pid > 0){
		printf("datagen in action\n");
	}
	else if (pid < 0){
		fprintf(stderr, "Cannot make datagen child\n"); 
	}


    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */
	int fd;
	struct sockaddr_un addr;
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
  	if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    	perror("socket error");
    	exit(-1);
  	}

    /* TODO: connect to datagen and ask for the necessary data in each experiment round.
     * Create a Unix domain socket with DSOCKET_PATH (see const.h).
     * Talk to datagen using the messages specified in the assignment description document.
     * Read the values generated by datagen from the socket and use them to run your
     * experiments
     * */
	memset(&addr, 0, sizeof(addr));
  	addr.sun_family = AF_UNIX;
  	if (*socket_path == '\0') {
    	*addr.sun_path = '\0';
    	strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
  	} else {
    	strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
  	}

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

    /* Probe time elapsed. */
    clock_t cend = clock();

    // Time elapsed in miliseconds.
    double time_elapsed = ((double) (cend - cbegin) / CLOCKS_PER_SEC) * 1000;

    printf("Time elapsed '%lf' [ms].\n", time_elapsed);

    exit(0);
}
