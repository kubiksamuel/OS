#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void
execute_xargs(char *argv[])
{
  //save every single input  of command
  char input_line[MAXARG];
  // arguments for call function "exec"
  char *exec_argu[4];
  //variable to read input for exec to variable input_line
  char ch;
  //replace pointer to actual position to input_line
  int counter = 0;

  exec_argu[0] = argv[1];
  exec_argu[1] = argv[2];
  exec_argu[3] = 0;

  while(read(0, &ch, sizeof(char))) {
    if(ch != '\n') {
      input_line[counter++] = ch;
    }
    else {
      exec_argu[2] = input_line;
      if(fork() == 0){
	exec(argv[1], exec_argu);
      }
      else{
	wait(0);
      }
      //clean string "input_line"
      memset(input_line, 0 , sizeof(input_line));
      counter = 0;
    }
  }
  return;
}

int
main(int argc, char *argv[])
{
  //check if program gets enough number of arguments
  if(argc < 3) {
    fprintf(2, "usage: xargs <command> <arguments>");
    exit(1);
  }
  execute_xargs(argv);
  exit(0);
}
