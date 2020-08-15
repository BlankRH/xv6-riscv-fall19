#include "user.h"
#include "kernel/param.h"

#define MAXARGLEN 51

#define EXEC 1
#define REDIR 2
#define PIPE 3

struct cmd {
	int type;
};

struct execcmd {
	int type;
	char *argv[MAXARGS];
};

struct pipecmd {
	int type;
	struct cmd *left;
	struct cmd *right;
}

struct redircmd {
	int type;
	struct cmd *cmd; 
	char *filename;
	FILE fd;
	int mode;
}

void
runcmd(struct cmd *cmd) {

switch

case EXEC:
	ecmd = (struct execcmd *) cmd;
	if(ecmd -> argv[0] == '\0')
		exit();
	exec(ecmd -> argv[0], ecmd -> argv);
	fprintf(2, "exec %s failed", argv[0]);
	break;

case PIPE:
	pcmd = (struct pipe *)cmd;
    int p[2];
	pipe(p);

	if(fork() == 0) {
		close(1);
		dup(p[1]);
		close(p[0]);
		close(p[1]);
		runcmd(pcmd->left);
	}

	if(fork() == 0) {
		close(0);
		dup(p[0]);
		close(p[1]);
		close(p[0]);
		runcmd(pcmd->right);
	}
	
	close(p[1]);
	close(p[0]);
	wait();
	wait();
	break;

exit();

}

struct cmd* parseline(const char *s) {
	char *ps = s;
	char *start[MAXARG];
	int argc = 1;
	start[0] = s;
	struct cmd* cmd;
	while(*ps) {
		if(*ps == '|') {
			start[argc++] = ps + 1;
			*(ps-1) = '\0';
		}
		ps++;
	}
	for(int i=1; i<argc; i++) {
		struct cmd* ncmd;
		struct pipecmd p;
		ncmd = &p;
		ncmd->left = cmd;
		ncmd->right = parsecmd(start[i]);
		ncmd->type = PIPE;
		cmd = ncmd;
	}
}

struct cmd* parsecmd(const char *st) {

	char *op;
	struct redircmd r;
	memset(r, 0, sizeof(struct redircmd));
	if((op = strchr(st, '<'))) {
		r->type = REDIR;
		r->file = op+1;
		r->mode = O_RDONLY;
		r->fd = 0;
		(*op) = '\0';
	}
	else if((op = strchr(st, '>'))) {
		r->type = REDIR;
		r->file = op+1;
		r->mode = O_WRONLY|O_CRETE:
		r->fd = 1;
		(*op) = '\0';
	}

	struct execcmd e;
	int argc = 1;
	e->argv[0] = st; 
	char *tmp;
	while(tmp = strchr(argv[argc-1], ' ')) {
		argv[argc++] = tmp+1;
		(*tmp) = '\0';
	}

	if(r) {
		r->cmd = e;
		return r;
	}
	return e;

}


int
main() {

    printf("@ ");
	char bufline[MAXARG*MAXARGLEN];
	while(1) {
		memset(bufline, 0, sizeof(bufline));
		gets(bufline);
		if(strlen(bufline) == 0) {
			exit();
		}
		bufline[strlen(bufline)-1] = '\0';
		if(fork() == 0) {
			runcmd(parsecmd(bufline));
		} else {
			wait();
		}
	}

	exit();
}
