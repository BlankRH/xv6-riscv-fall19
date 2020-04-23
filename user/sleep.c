#include "user/user.h"

int
main(int argc, char *argv[]) {
	unsigned int stime = 0;
	if(argc < 2) {
		fprintf(2, "Error: no argument\n");
		exit();
	}
	for(int i = 1; i < argc; i++) {
		stime += atoi(argv[i]);
	}
	sleep(stime);

	exit();
}
