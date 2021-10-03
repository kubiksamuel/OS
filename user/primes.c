#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
check_primes(int parent_fd[2])
{
  //close unused pipe
  close(parent_fd[1]);
  //variable for  unchecked numbers
  int unchecked_n;
  //variable for actual prime number
  int prime;
  read(parent_fd[0], &prime, sizeof(int));
  printf("prime %d\n", prime);
  //create process if there are some numbers to check
  if(read(parent_fd[0], &unchecked_n, sizeof(int))){
    int child_fd[2];
    pipe(child_fd);

    int pid = fork();
    if(pid < 0){
      exit(1);
    }
    else if(pid == 0){
      //call function  with file desc child_fd which includes rest unchecked numbers
      check_primes(child_fd);
      exit(0);
    }
    else{
      close(child_fd[0]);
      //reduce actual set of potential prime number
      do{
	if(unchecked_n % prime != 0){
	  write(child_fd[1], &unchecked_n, sizeof(int));
	}
      }while(read(parent_fd[0], &unchecked_n, sizeof(int)));
      close(parent_fd[0]);
      close(child_fd[1]);
      wait(0);
      exit(0);
    }
  }
  //exit if all numbers were checked
  else{
    exit(0);
  }

}

int
main(int argc, char *argv[])
{
  int parent_fd[2];
  if(pipe(parent_fd) == -1){
    fprintf(2, "error during creating pipe");
    exit(1);
  }

  int pid = fork();
  if(pid < 0){
    exit(1);
  }
  else if(pid == 0){
    check_primes(parent_fd);
    exit(0);
  }
  else{
    //close unused pipes
    close(parent_fd[0]);
    //put numbers 2-35 to pipe
    for (int i = 2; i <= 35; i++){
      write(parent_fd[1], &i, sizeof(int));
    }
    close(parent_fd[1]);
    //wait until all child, grandichild etc. end
    wait(0);
    exit(0);
  }
}
