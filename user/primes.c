#include "user.h"

int
main() {
	
	int pid;
	int pipefd[2];
	int pipe2[2];

	pipe(pipefd);
	pipe(pipe2);

	for(int i = 2; i <= 35; i++) {
		write(pipefd[1], &i, 1);
	}
	close(pipefd[1]);
	
	pid = fork();
	while(1) {
		if(pid == 0) {
			int p;
	    	read(pipefd[0], &p, 1);
	    	printf("prime %d\n", p);
			int n;
			while(read(pipefd[0], &n, 1)) {
				if(n % p != 0) {
					write(pipe2[1], &n, 1);
				}
			}		
			close(pipe2[1]);
			exit();
		} 
		else {			
			int n = 0;
	    	wait();
			close(pipe2[1]);
			close(pipefd[1]);
			pipe(pipefd);
	    	while(read(pipe2[0], &n, 1)) {
		    	write(pipefd[1], &n , 1);
		    }
			close(pipefd[1]);
			close(pipe2[0]);
			pipe(pipe2);
			if(n == 0) 
				exit();
		   	pid = fork();
		}
	}	
}


