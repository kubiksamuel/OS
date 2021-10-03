#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int to_parent[2];
  int to_child[2];

  if(pipe(to_parent) < 0 || pipe(to_child) < 0){
    fprintf(2, "error during creating pipes");
    exit(1);
  }

  int pid = fork();

  if(fork < 0) {
    fprintf(2, "error during making processes");
    exit(1);
  }
  else if(pid == 0) {
    char received;

    //close unused pipes in child process
    close(to_parent[0]);
    close(to_child[1]);
    
    //wait until byte comes from parent
    read(to_child[0], &received, 1);
    printf("%d: received ping\n", getpid());
    write(to_parent[1], "x", 1);

    close(to_parent[1]);
    close(to_child[0]);
  } else {
    write(to_child[1], "b", 1);
    char received;

    //close unused pipes in parent process
    close(to_child[0]);
    close(to_parent[1]);

    read(to_parent[0], &received, 1);
    printf("%d: received pong\n", getpid());

    close(to_parent[0]);
    close(to_child[1]);
  }
  exit(0);
}

