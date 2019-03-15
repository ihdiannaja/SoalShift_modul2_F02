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
