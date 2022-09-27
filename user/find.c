#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  return p+1;
  
}

void find(char *path,char *filename )
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

    //打开path所指的文件描述符
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    //查看相关文件信息并存入st中
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    
    case T_FILE:
        if(strcmp(fmtname(path),filename)==0) 
          printf("%s\n",path);
        break;
    case T_DIR:
    //  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    //   printf("find: path too long\n");
    //   break;
    // }
    //将path拷贝到buf中
    strcpy(buf, path);
    // 移动指针
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0||strcmp(de.name,".")==0||strcmp(de.name,"..")==0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      // if(stat(buf, &st) < 0){
      //   printf("find: cannot stat %s\n", buf);
      //   continue;
      // }
    // 递归找
    find(buf,filename);     
    }
        break;
  }
  close(fd);
}
int main(int argc, char *argv[])
{

  if(argc < 3){
    fprintf(2,"usage: find path filename.\n");
    exit(0);
  }
  find(argv[1],argv[2]);
  exit(0);
}