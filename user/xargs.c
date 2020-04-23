#include "user/user.h"
#include "kernel/param.h"

int 
main(int argc, char** argv) {
	char *args[MAXARG+2], buf;
	char arg[101] = {0};
	char buf2[101];
	
	int pid = -1;
	
	while(1) {
		
		if(pid == 0) {
			exec(args[0], args);
			exit();
		}
	    else {

			if(!read(0, &buf, 1))
				break;

			if(buf != '\n') {
				buf2[strlen(buf2)] = buf;
				continue;
			}

			if(strlen(buf2) == 0)
				continue;

		    if(pid > 0) {	
		        wait();
			}

			strcpy(arg, buf2);
			memset(args, 0, 101);
			for(int i=1; i<argc; i++) {
			    args[i-1] = argv[i];
			}
			int cnt = argc-1;
			int len = strlen(arg);
			args[cnt] = arg;
			for(int i=1; i<len; i++)
				if(arg[i] == ' ') {
					arg[i] = '\0';
					if(i != len-1)
					    args[++cnt] = arg + i + 1;
				}
			args[cnt+1] = (char *)0;

			memset(buf2, 0, 101);

			pid = fork();
		}
	}
	exit();
}
