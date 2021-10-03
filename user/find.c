#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void
find(char *path, char *file_name)
{
  //variable buf saves actual path to file
  //p adds gradually part of path to variable buf
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }
  if(fstat(fd, &st) <  0){
    fprintf(2, "find: cannot stat %s\n", path);
    exit(1);
  }

  switch(st.type)
  {
  case T_FILE:
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    //check if found file equals to file program search for
    if(!strcmp(p, file_name)) {
      fprintf(1, "%s\n", path);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      printf("find: path is too long\n");
      break;
    }
    strcpy(buf, path);
    //set pointer to end of actual path
    p = buf+strlen(buf);
    *p++ = '/';
    while(sizeof(de) == read(fd, &de, sizeof(de))){
      //skip directory  "..", "." and empty directories
      if(!strcmp(de.name, "..") || !strcmp(de.name, ".") || de.inum == 0)
	continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
	printf("find: cannot stat %s\n", buf);
	continue;
      }
      find(buf, file_name);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  //check if user put enough number of inputs
  if(argc < 3) {
    fprintf(2, "usage: find <path> <file>\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
