#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
  pid_t child_id1, child_id2, child_id3;
  int status;
  int fd[2];
if(pipe(fd) < 0) exit(1);
  child_id1 = fork();
	char coba[10000];
  if (child_id1 == 0) {
    char *argv[5] = {"unzip", "/home/rye/modul2/campur2.zip", "-d", "/home/rye/modul2/", NULL};
    execv("/usr/bin/unzip", argv);
  }

  else {
	  while ((wait(&status)) > 0);
	  child_id2 = fork();
  	if (child_id2 == 0) {
		close(fd[0]);
    		dup2(fd[1], STDOUT_FILENO);
    		char *argv[3] = {"ls", "/home/rye/modul2/campur2/", NULL};
   		 execv("/bin/ls", argv);

  	}
  	else {
      		while((wait(&status)) > 0);
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			int fileopen = open("/home/rye/modul2/daftar.txt",O_WRONLY | O_CREAT);
			dup2(fileopen, STDOUT_FILENO);
			char *argv[3] = {"grep", ".txt$", NULL};
			execv("/bin/grep", argv);
			close(fd[1]);
    	}
  }
}
