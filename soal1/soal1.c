#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

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

  while(1) {
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/rye/Pictures/modul2/");
    while((dir = readdir(d)) != NULL)
    {
	char *filename = dir->d_name;
	char *dot = strrchr(filename, '.');
	if(!dot)
		continue;
	char *ext = strdup(dot + 1);
	if(strcmp(ext, "png") == 0)
	{
		char newdir[256], olddir[256];
		strcpy(newdir, "/home/rye/modul2/gambar/");
		strcpy(olddir, "/home/rye/Pictures/modul2/");
		strcat(olddir,filename);
		char *namadepan = strtok(filename, ".");
		strcat(namadepan, "_grey.");
		strcat(namadepan, ext);
		strcat(newdir, namadepan);
		rename(olddir, newdir);
	}
    }
    closedir(d);
    sleep(5);
  }

  exit(EXIT_SUCCESS);

}
