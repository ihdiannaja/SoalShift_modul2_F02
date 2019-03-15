#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  int bil=1;
  FILE *fp;
char file[256];
strcpy(file,"/home/rye/Documents/makanan/makan_enak.txt");
  while(1) {
	struct stat makanenak;
	stat(file,&makanenak);
	if(difftime(time(NULL),makanenak.st_atime) <= 30)
	{
		char nfile[256];
        	strcpy(nfile,"/home/rye/Documents/makanan/makan_sehat");
		char c[256];
		sprintf(c, "%d", bil);
		strcat(nfile, c);
		strcat(nfile, ".txt");
		fp = fopen(nfile, "w");
		fclose(fp);
		bil++;
	}
    sleep(5);
  }
  
  exit(EXIT_SUCCESS);
}
