#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2]; // p[0] - read fd, p[1] - write fd

  if(pipe(p) < 0) {
    fprintf(2, "Pipe error!\n");
    exit(1);    
  }
  
  int pid = fork();
  
  if (pid > 0) {
    char child_msg;

    if(write(p[1], "p", 1) != 1) {
      fprintf(2, "Write error!\n");
      exit(1);
    }

    close(p[1]);
    wait(0);

    if(read(p[0], &child_msg, 1) != 1) {
      fprintf(2, "Read error!\n");
      exit(1);
    }
    
    printf("%d: received pong\n", getpid());

  } else if(pid == 0) {
    char parent_msg;

    if(read(p[0], &parent_msg, 1) != 1) {
      fprintf(2, "Read error!\n");
      exit(1);
    }

    close(p[0]);
    printf("%d: received ping\n", getpid());

    if(write(p[1], "c", 1) != 1) {
      fprintf(2, "Write error!\n");
      exit(1);
    }
  
  } else {
    fprintf(2, "Fork error! No child process is created!\n");
    exit(1);
  }

  exit(0);
}
