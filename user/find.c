#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"

char*
fmtname(char* path) {
  char* p;
  for(p = path + strlen(path); p >= path && *p != '/'; p--)
		;
  p++;
	return p;
}

void 
find(char* path, char* object) {
	struct stat st;
	struct dirent de;
	char buf[64];
  	
	if(stat(path, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    exit(1);
	}

	switch(st.type) {
	
  case T_FILE:
    if(strcmp(fmtname(path), object) == 0)
			printf("%s\n", path);
		return;
  case T_DIR:
		int fd;

		if((fd = open(path, O_RDONLY)) < 0) {
    	fprintf(2, "find: cannot open %s\n", path);
    	exit(1);
  	}
		
		strcpy(buf, path);
		memmove(buf + strlen(buf), "/", DIRSIZ);
		while(read(fd, &de, sizeof(de)) == sizeof(de)) {
			
			if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
				continue;

			memmove(buf + strlen(buf), de.name, DIRSIZ);
			find(buf, object);
			buf[strlen(buf)-strlen(de.name)] = '\0';
		}
		close(fd);
	}
}

int
main(int argc, char *argv[]) {

	if(argc == 2)
		find(".", argv[1]);
	else if(argc == 3)
		find(argv[1], argv[2]);
	else
		fprintf(2, "Wrong arguments!\n");

	exit(0);
}	
