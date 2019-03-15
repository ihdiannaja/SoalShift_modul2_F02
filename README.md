# Soal Shift Modul 2 F02
Pertanyaan, Jawaban dan Penjelasan Praktikum Modul 1 Sistem Operasi 2019.

## Soal 1
#### Pertanyaan :
Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile] _ grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
##### Catatan : 
Tidak boleh menggunakan crontab.

#### Jawaban :
```
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
```
*	Pada bagian ini, menggunakan daemon.
```
  while(1) {
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/rye/Pictures/modul2/");
```
*	Mendeklarasikan direktori dengan nama d.
*	`struct dirent` * dir digunakan untuk memberikan informasi mengenai isi dalam direktori bernama dir.
*	Mendeklarasikan direktori d = direktori /home/rye/Pictures/modul2/.
```
    while((dir = readdir(d)) != NULL)
    {
	      char *filename = dir->d_name;
	      char *dot = strrchr(filename, '.');
	      if(!dot)
		        continue;
	      char *ext = strdup(dot + 1);
```
* Perulangan ini untuk membaca seluruh isi dalam direktori dir yang sebelumnya telah dideklarasikan.
* Jika memenuhi ketentuan perulangan tersebut, maka kita membuat variabel bertipe char dengan nama * filename yang dideklarasikan = dir->d_name (nama tiap file dalam direktori dir).
* Membuat variabel bertipe char bernama * dot yang dideklarasikan untuk membaca posisi terakhir dalam filename setelah tanda titik “.” <dot> .
*	Jika bukan tanda titik, maka continue.
*	Mendeklarasikan *ext sebagai duplikasi dari (dot + 1), yang berarti *ext membaca string setelah tanda titik pada filename.
```
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
```
*	Jika *ext = png, maka :
*	Membuat variabel bertipe char yang bernama newdir dan olddir.
*	Mengcopy direktori /home/rye/modul2/gambar/ ke dalam direktori newdir.
*	Mengcopy direktori /home/rye/Pictures/modul2/ ke dalam direktori olddir.
*	Menggabungkan nama olddir dan filename.
*	Membuat variabel bertipe char bernama *namadepan yang berisi character setelah tanda titik pada string filename.
*	Membagi namadepan yang dibatasi oleh “_grey.” .
*	Membagi namadepan yang dibatasi oleh ext .
*	Membagi newdir yang dibatasi oleh namadepan .
*	Mengubah nama olddir menjadi newdir.
```
    closedir(d);
    sleep(5);
  }
  exit(EXIT_SUCCESS);
}
```
*	Menutup direktori d.
*	Melakukan pause pada program setiap 5 detik.
*	Mengakhiri / EXIT program.

## Soal 2
#### Pertanyaan :
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
##### Catatan : 
Tidak boleh menggunakan crontab

#### Jawaban :
```
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
```
*	Pada bagian ini, menggunakan daemon.
```
  char file[256] = "/home/rye/modul2/hatiku/elen.ku";
```
*	Deklarasi variabel bertipe char bernama file yang dideklarasikan = file elen.ku pada direktori "/home/rye/modul2/hatiku/.
```
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
```
*	Melakukan perulangan while (true).
*	Melakukan pengecekan apakah file sudah dibuat atau belum.
*	Membuat array of char bernama mode yang berisi “0777”.
*	Membuat variabel i bertipe integer yang mengubah tipedata variabel mode dari string menjadi long integer.
*	Mengganti permission file dengan 0777.
*	Membuat struct stat yang berfungsi untuk menyimpan informasi tentang file elenku.
*	Mengambil informasi dari elenku menuju file.
*	Membuat struct passwd bernama *user yang berisi user id dari file elenku.
*	Membuat strust group bernama *group yang berisi group id dari file elenku.
*	Deklarasi variabel usercomp dan grupcomp yang bertipe int.
*	Usercomp digunakan untuk membandingkan apakah user->pw_name = www-data.
*	grupcomp digunakan untuk membandingkan apakah group->gr_name = www-data.
*	Jika benar, maka file elenku dihapus.
```
    sleep(3);
  }
  exit(EXIT_SUCCESS);
}
```
*	Melakukan pause pada program setiap 3 detik.
*	Mengakhiri proses.

## Soal 3
#### Pertanyaan :
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
1. Mengekstrak file zip tersebut.
2. Menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
##### Catatan : 
* Gunakan fork dan exec.
* Gunakan minimal 3 proses yang diakhiri dengan exec.
* Gunakan pipe
* Pastikan file daftar.txt dapat diakses dari text editor

#### Jawaban :
```
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
  pid_t child_id1, child_id2;
  int status;
  int fd[2];
```
* Deklarasi variabel yang diperlukan.
```
if(pipe(fd) < 0) exit(1);
  child_id1 = fork();
	char coba[10000];
  if (child_id1 == 0) {
    char *argv[5] = {"unzip", "/home/rye/modul2/campur2.zip", "-d", "/home/rye/modul2/", NULL};
    execv("/usr/bin/unzip", argv);
  }
```
* `(pipe(fd) < 0) exit(1)` digunakan untuk melakukan pengecekan apakah pipe dapat berfungsi atau tidak.
* Melakukan proses child yang berfungsi untuk meng unzip folder campur2.zip dalam folder /home/rye/modul2/ ke dalam folder /home/rye/modul2/.

```
  else {
	  while ((wait(&status)) > 0);
	  child_id2 = fork();
  	if (child_id2 == 0) {
		close(fd[0]);
    		dup2(fd[1], STDOUT_FILENO);
    		char *argv[3] = {"ls", "/home/rye/modul2/campur2/", NULL};
   		 execv("/bin/ls", argv);
  	}
```
* melakukan perulangan apakah proses child sebelumya sudah selesai atau belum.
* melakukan proses child yang menampilkan isi <ls> dari folder campur2.
* hasil dari ls tersebut yang harusnya ditampilkan di console dialihkan menggunakan STDOUT_FILENO dan disimpan dalam fd.
```
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
```
* melakukan perulangan untuk memastikan bahwa proses child sebelumnya sudah berakhir.
* menutup fd[1].
* membuka file daftar.txt yang bersifat read-only.
* hasil ls dari proses sebelumnya dijadikan input di proses saat ini. sehingga digunakan STDIN_FILENO.
* melakukan pencarian nama file yang berekstensi .txt pada hasil ls yang telah didapat.
* file yang telah tersaring akan ditampilkan pada file daftar.txt.
* file daftar.txt ditutup.
* proses berakhir.

## Soal 4
#### Pertanyaan :
Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.
##### Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt
##### Catatan : 
* Dilarang menggunakan crontab
* Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

#### Jawaban :
```
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
```
* Pada bagian ini, menggunakan daemon.
```
       int bil=1;
	FILE *fp;
	char file[256];
	strcpy(file,"/home/rye/Documents/makanan/makan_enak.txt");
```
* Membuat variabel bil bertipe int = 1.
* Membuat variabel *fp bertipe file.
* Membuat variabel file bertipe char.
* Mengcopy file makan_enak.txt dalam direktori /home/rye/Documents/makanan/ ke dalam variabel file yang bertipe file.
```
  while(1) {
		struct stat makanenak;
		stat(file,&makanenak);
```
* Membuat struct stat bernama makanenak.
* Mengambil informasi dari makanenak menuju file.
```
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
```
* Melakukan pengecekan apakah ..
* Membuat variabel nfile bertipe char.
* Mengcopy /home/rye/Documents/makanan/makan_sehat ke nfile.
* Membuat variabel c bertipe char.
* Varibel bil yang tipe datanya integer diubah menjadi variabel c yang tipe datanya char.
* Menggabungkan string nfile dengan c.
* Menambahkan string .txt pada nfile.
* Membuka file sesuai dengan path nfile.
* Menutup file fp yang telah dibuka.
* Increment variabel bil.
```
sleep(5);
  }
  exit(EXIT_SUCCESS);
  }
```
* Melakukan pause setiap 5 detik.
* Mengakhiri program.

## Soal 5
#### Pertanyaan :
Kerjakan poin 1 dan 2 di bawah:
1. Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log . 
Ket:
   * Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
   * Per menit memasukkan log#.log ke dalam folder tersebut
     ‘#’ : increment per menit. Mulai dari 1
2. Buatlah program c untuk menghentikan program di atas.
##### Catatan : 
Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

#### Jawaban :
##### 5a
```
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

```
* menggunakan daemon.
```
  while(1) {
 	time_t jam = time(NULL);
        struct tm *waktu = localtime(&jam);
        char swaktu[256], swaktu1[256];
        sprintf(swaktu, "%d:%d:%d-%d:%d",waktu->tm_mday,waktu->tm_mon+1,waktu->tm_year+1900,waktu->tm_hour,waktu->tm_min);
	strcpy(swaktu1, "/home/rye/log/");
	strcat(swaktu1,swaktu);
        mkdir(swaktu1, 0777);
        int bil = 1;
```
* melakukan while(true) untuk mendapatkan time saat ini yang disimpan pada variabel jam.
* membuat variabel baru bernama waktu yang berisi waktu dari jam yang formatnya telah terpisah-pisah. sehingga kita bisa mendapatkan tiap format waktu (jam,menit,dst).
* merubah tipe data integer ke dalam char dari variabel swaktu. dibuat dalam format tanggal:bulan:tahun:jam:menit.
* mengcopy swaktu1 ke dalam folder /home/rye/log/.
* menggabungkan nama file swaktu1 dengan swaktu yang disimpan dalam swaktu1.
* membuat direktori dengan nama sesuai swaktu dan permission 777.
* deklarasi variabel bil bertipe integer = 1.
```
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
```
* melakukan perulangan sebanyak 30 kali.
* membuat variabel source dan target bertipe file, dan variabel c dan nfile bertipe char.
* mengubah tipe data int menjadi string pada nfile.
* menggabungkan nama nfile dengan .log .
* mendeklarasikan source sebagai pembaca (scan) isi folder syslog.
* mendeklarasikan target sebagai penulis (print) file dari folder syslog  ke folder nfile.
* melakukan perulangan untuk membaca file dalam folder source ketika c != EOF.
* jika memenuhi, maka file yang telah terbaca dicopy dalam folder target.
* increment bil.
* menutup file source dan target.
* pause setiap 60 detik. sehingga dalam 1 jam didapatkan 30 log.

##### 5b
```
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
```
* membuat variabel perintah bertipe file yang berisi pid dari proses yang berjalan pada proses 5a.
* hasil pid yang didapatkan dari perintah disimpan pada pid_string.
* membunuh proses dengan pid yang sesuai dengan pid_string.
* menutup file perintah.
* proses selesai.
