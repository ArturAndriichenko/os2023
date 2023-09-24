#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

__attribute__((noreturn))
void right_neighbour(int rfd) {
  int num;

  if(read(rfd, &num, sizeof(int)) != sizeof(int)) {
    fprintf(2, "Read error!\n");
    exit(1);
  }

  printf("prime %d\n", num);

  int next_num;
  int p[2];
  int pid = -5;

  while(1) {
    int ret = read(rfd, &next_num, sizeof(int));

    if(ret == 0) {
       close(rfd);
       break;
    } else if(ret != sizeof(int)) {
      fprintf(2, "Read error!\n");
      exit(1);
    }
    
    if(next_num % num != 0) {
      if(pid == -5) {
        pipe(p);
        pid = fork();
        if(pid == 0) {
          close(p[0]);
        } else {
          close(p[1]);
        }
      }

      if(pid == 0) {
        write(p[1], &next_num, sizeof(int));
      } else if(pid > 0) {
        right_neighbour(p[0]);
      } else {
        fprintf(2, "Fork error! No child was created!\n");
        exit(1);
      }
    }
  }
  
  if(pid == -5) {
    exit(0);
  }
 
  close(p[1]);
  wait(0);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2]; // p[0] - read fd, p[1] - write fd

  if(pipe(p) < 0) {
    fprintf(2, "Pipe error!\n");
    exit(1);    
  }
  
  int pid = fork();
  
  if(pid == 0) {
    close(p[0]);

    for(int i = 2; i < 36; i++) {
      if(write(p[1], &i, sizeof(int)) != sizeof(int)) {
        fprintf(2, "Write error!\n");
        exit(1);
      }
    }
    close(p[1]);
    wait(0);

  } else if(pid > 0) {
    close(p[1]);
    right_neighbour(p[0]);
  } else {
    fprintf(2, "Fork error! No child process is created!\n");
    exit(1);
  }

  exit(0);
}
