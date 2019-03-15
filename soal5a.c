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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
 	time_t jam = time(NULL);
        struct tm *waktu = localtime(&jam);
        char swaktu[256], swaktu1[256];
        sprintf(swaktu, "%d:%d:%d-%d:%d",waktu->tm_mday,waktu->tm_mon+1,waktu->tm_year+1900,waktu->tm_hour,waktu->tm_min);
	strcpy(swaktu1, "/home/rye/log/");
	strcat(swaktu1,swaktu);
        mkdir(swaktu1, 0777);
        int bil = 1;
        while(bil <= 30)
        {
                FILE *source, *target;
                char c, nfile[256];
                sprintf(nfile, "%s/log%d", swaktu1, bil);
                strcat(nfile, ".log");
                source = fopen("/var/log/syslog", "r");
                target = fopen(nfile, "w");
                while(fscanf(source, "%c", &c) != EOF)
                        fprintf(target, "%c", c);
                bil++;
                fclose(source);
                fclose(target);
                sleep(60);
        }
  }

  exit(EXIT_SUCCESS);
}
