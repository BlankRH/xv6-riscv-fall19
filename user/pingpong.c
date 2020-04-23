#include "user.h"

int 
main(int argc, char *argv[]) {
	int pipefd[2] = {0};
	int pid;

	pipe(pipefd);

	pid = fork();
	if(pid == 0) {
		char buf[5] = {0};
		char *s = "pong";
		read(pipefd[0], &buf, 5);
		printf("%d: recieved %s\n", getpid(), buf);	
        write(pipefd[1], s, 5);
	    exit();	   // child
	}
	else {
		char buf[5] = {0};
	    char *s = "ping";
		write(pipefd[1], s, 5);
		wait();
		read(pipefd[0], buf, 5);
		printf("%d: recieved %s\n", getpid(), buf);
		exit();
	}

}
