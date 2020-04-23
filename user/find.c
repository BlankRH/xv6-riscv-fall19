#include "user/user.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

char* fmtname(char *path) {
	char *p;

	for(p=path+strlen(path); p >= path && *p != '/'; p--) 
		;
	p++;
	return p;
}

void
find(char* path, char* name) {
	
	int fd;
	struct stat st;
	struct dirent de;
	char buf[512], *p;

	// printf("%s\n", path);
	if((fd = open(path, 0)) < 0) {
		fprintf(2, "find cannot open %s\n", path);
		return;
	}

	if(fstat(fd, &st) < 0) {
		fprintf(2, "find cannot stat %s\n", path);
		return;
	}

	switch(st.type) {
	case T_FILE:
		//printf("%s %s %d\n", fmtname(path), name, strcmp(name, fmtname(path)));
		if(strcmp(name, fmtname(path)) == 0)
			printf("%s\n", path);
		close(fd);
		return;

	case T_DIR:
		strcpy(buf, path);
		p = buf + strlen(buf);
		*p++ = '/';
		while(read(fd, &de, sizeof(de)) == sizeof(de)) {
			if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
				continue;
			memmove(p, de.name, DIRSIZ);
		
			p[DIRSIZ] = 0;
			find(buf, name);
		}
	}
	close(fd);
}
	


int
main(int argc, char** argv) {
	if(argc != 3) {
		fprintf(2, "Usage: %s <path> <filename>", argv[0]);
		exit();
	}
	find(argv[1], argv[2]);

	exit();

}
