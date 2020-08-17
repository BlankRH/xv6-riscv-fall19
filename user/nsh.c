#include "user.h"
#include "kernel/param.h"
#include "kernel/fcntl.h"

#define MAXARGLEN 51

#define EXEC 1
#define REDIR 2
#define PIPE 3

char *start[MAXARG];
int argc = 1;

void execPipe(char* start[], int argc);

void
runcmd(char *start[], int argc) {
	for(int i=1; i<argc; i++)
		if(!strcmp(start[i], "|"))
			execPipe(start, argc);

	for(int i=1; i<argc; i++) {
		if(!strcmp(start[i], ">")) {
			close(1);
			open(start[i+1], O_CREATE|O_WRONLY);
			start[i] = 0;
		}
		if(!strcmp(start[i],"<")) {
			close(0);
			open(start[i+1], O_RDONLY);
			start[i] = 0;
		}
	}
	exec(start[0], start);
}

void parseline(char *s) {
	char *ps = s;
	start[0] = s;
	while(*ps) {
		if(*ps == ' ' || *ps == '\n') {
			while(*ps == ' ' || *ps == '\n') {
				*(ps++) = 0;
			}
			if(*(ps)) {
				start[argc++] = ps++;
			}
		}
		else
			ps++;
	}
}

void execPipe(char* start[], int argc) {
	int i=0;
	for(; i<argc; i++) {
		if(!strcmp(start[i], "|")) {
			start[i] = 0;
			break;
		}
	}

    int p[2];
	pipe(p);

	if(fork() == 0) {
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		runcmd(start, i);
	}

	else {
		close(0);
		dup(p[0]);
		close(p[1]);
		close(p[0]);
		runcmd(start+i+1, argc-i-1);
	}
}

int
main() {

	char bufline[MAXARG*MAXARGLEN];
	while(1) {
    	printf("@ ");
		memset(bufline, 0, sizeof(bufline));
		gets(bufline, sizeof(bufline));
		if(strlen(bufline) == 0) {
			exit();
		}
		bufline[strlen(bufline)-1] = '\0';
		if(fork() == 0) {
			parseline(bufline);
			runcmd(start, argc);
		} else {
			wait();
		}
	}

	exit();
}
