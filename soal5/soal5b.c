#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
int main() {

	char pid_string[5];
	FILE *perintah = popen("pidof soal5a","r");
	fgets(pid_string,5,perintah);
	pid_t pid = strtoul(pid_string,NULL,10);
	kill(pid,SIGKILL);
	pclose(perintah);
}
