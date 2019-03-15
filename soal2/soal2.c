#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int cfileexists(const char *fname)

{
	FILE *file;
	if ((file = fopen(fname, "r")))
    	{
        	fclose(file);
        	return 1;
    	}
    	return 0;
}


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
  char file[256] = "/home/ihdiannaja2911/modul2/hatiku/elen.ku";
  while(1) {
	if (cfileexists(file)){
	char mode[] = "0777";
    	int i;
    	i = strtol(mode, 0, 8);
	chmod (file,i);
	struct stat elenku;
	stat(file, &elenku); 
	struct passwd *user = getpwuid(elenku.st_uid);
	struct group  *group = getgrgid(elenku.st_gid);
	int usercomp = strcmp (user->pw_name, "www-data");
	int grupcomp = strcmp (group->gr_name, "www-data"); 

	if (usercomp == 0 && grupcomp == 0 ){
		remove (file);
	}
  }
    sleep(3);
  }
  exit(EXIT_SUCCESS);
}
