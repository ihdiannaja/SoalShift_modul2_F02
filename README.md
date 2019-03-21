# Soal Shift Modul 2 F02
Pertanyaan, Jawaban dan Penjelasan Praktikum Modul 2 Sistem Operasi 2019.

Oleh :
* Wasilatul Dewi Ningrum (05111740000004)
* Ihdiannaja (05111740007005)

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
* Pada bagian ini, merupakan proses untuk membuat daemon.
* Langkah pertama adalah menspawn proses menjadi induk dan anak dengan melakukan forking, kemudian membunuh proses induk. Proses induk yang mati akan menyebabkan sistem operasi mengira bahwa proses telah selesai.
* Untuk menulis beberapa file (termasuk logs) yang dibuat oleh daemon, mode file harus diubah untuk memastikan bahwa file tersebut dapat ditulis dan dibaca secara benar. Pengubahan mode file menggunakan implementasi umask().
* Child Process harus memiliki unik SID yang berasal dari kernel agar prosesnya dapat berjalan. Child Process menjadi Orphan Process pada system. Tipe pid_t juga digunakan untuk membuat SID baru untuk Child Process. Setsid() digunakan untuk pembuatan SID baru. Fungsi setsid() memiliki return tipe yang sama dengan fork().
* Directori kerja yang aktif harus diubah ke suatu tempat yang telah pasti akan selalu ada. Pengubahan tempat direktori kerja dapat dilakukan dengan implementasi fungsi chdir(). Fungsi chdir() mengembalikan nilai -1 jika gagal.
* Langkah terakhir dalam men-set daemon adalah menutup file desciptor standard dengan menggunakan STDIN, STDOUT, dan STDERR. Dikarenakan oleh daemon tidak menggunakan terminal, maka file desciptor dapat terus berulang dan berpotensi berbahaya bagi keamanan. Untuk mengatasi hal tersebut maka harus menggunakan fungsi close().
```
  while(1) {
    DIR *d;
```
* Daemon bekerja dalam jangka waktu tertentu, sehingga diperlukan sebuah looping.
* `DIR *d` Mendeklarasikan direktori dengan nama d.
```
struct dirent *dir;
```
* `struct dirent * dir` digunakan untuk memberikan informasi mengenai isi dalam direktori bernama dir.
```
d = opendir("/home/rye/Pictures/modul2/");
```
* Membuka direktori d yang merupakan direktori /home/rye/Pictures/modul2/.
```
    while((dir = readdir(d)) != NULL)
    {
```
* Perulangan ini untuk membaca seluruh isi dalam direktori dir yang sebelumnya telah dibuka.
```
char *filename = dir->d_name;
```
* Jika memenuhi ketentuan perulangan tersebut (direktori tidak null atau memiliki isi), maka kita membuat variabel bertipe char dengan nama * filename yang dideklarasikan = dir->d_name. `dir->d_name` digunakan untuk mendapatkan nama file dalam direktori dir.
```
char *dot = strrchr(filename, '.');
```
* Membuat variabel bertipe char bernama * dot yang dideklarasikan untuk mendapatkan string dalam filename setelah tanda titik “.” <dot> (tanda titik juga akan tertampung dalam variabel dot) .
```
	if(!dot)
     	continue;
```
*	Jika ternyata tidak terdapat tanda titik pada variabel filename, maka lanjutkan proses.
```
	      char *ext = strdup(dot + 1);
```
*	Mendeklarasikan variabel *ext sebagai duplikasi dari (dot + 1), yang berarti *ext membaca string mulai posisi ke 1 pada variabel dot. sehingga variabel ext akan menampung string setelah tanda titik (tanda titik terletak pada array ke 0 pada variabel dot.
```
	if(strcmp(ext, "png") == 0){
```
*	Jika variabel *ext berisi string png, maka :
```
		char newdir[256], olddir[256];
```
*	Membuat variabel bertipe char yang bernama newdir dan olddir.
```
		strcpy(newdir, "/home/rye/modul2/gambar/");
```
*	Mengisi variabel newdir dengan string /home/rye/modul2/gambar/.
```
		strcpy(olddir, "/home/rye/Pictures/modul2/");
```
*	Mengisi variabel olddir dengan string /home/rye/Pictures/modul2/.
```
		strcat(olddir,filename);
```
*	Menggabungkan string pada variabel olddir dan filename dan disimpan pada variabel olddir. sehingga variabel olddir sekarang berisi string `/home/rye/Pictures/modul2/nama_file`
```
		char *namadepan = strtok(filename, ".");
```
*	Membuat variabel bertipe char bernama *namadepan yang berisi character setelah tanda titik pada string filename.
```
		strcat(namadepan, "_grey.");
```
*	Menggabungkan string pada variabel namadepan dengan “_grey.” dan disimpan pada variabel namadepan. Sehingga variabel namadepan sekarang berisi string `nama_file_grey.`
```
		strcat(namadepan, ext);
```
*	Menggabungkan string pada variabel namadepan dengan string pada variabel ext dan disimpan pada variabel namadepan. Sehingga variabel namadepan sekarang berisi string `nama_file_grey.png`
```
		strcat(newdir, namadepan);
```
*	Menggabungkan string pada variabel newdir dengan string pada variabel namadepan dan disimpan pada variabel newdir. Sehingga variabel newdir sekarang berisi string `/home/rye/modul2/gambar/nama_file_grey.png`
```	
		rename(olddir, newdir);
	}
    }  
```
*	Mengubah nama file dari olddir menjadi newdir.
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
*	Program dicompile dengan menggunakan perintah `gcc -o nama nama_program.c`
*	Program dijalankan dengan menggunakan perintah `./nama`
*	File pada folder asal
	<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal1/soal1%20belum%20grey.png" width="600">
*	Hasil setelah program berjalan
	<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal1/soal1%20sudah%20grey.png" width="600">
*	Untuk mengakiri program dapat dilakukan perintah `kill [PID]`
*	Untuk mendapatkan PID dapat dilakukan perintah `ps -e | grep nama`
	
	<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal1/soal1.png" width="600">

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
* Pada bagian ini, merupakan proses untuk membuat daemon.
* Langkah pertama adalah menspawn proses menjadi induk dan anak dengan melakukan forking, kemudian membunuh proses induk. Proses induk yang mati akan menyebabkan sistem operasi mengira bahwa proses telah selesai.
* Untuk menulis beberapa file (termasuk logs) yang dibuat oleh daemon, mode file harus diubah untuk memastikan bahwa file tersebut dapat ditulis dan dibaca secara benar. Pengubahan mode file menggunakan implementasi umask().
* Child Process harus memiliki unik SID yang berasal dari kernel agar prosesnya dapat berjalan. Child Process menjadi Orphan Process pada system. Tipe pid_t juga digunakan untuk membuat SID baru untuk Child Process. Setsid() digunakan untuk pembuatan SID baru. Fungsi setsid() memiliki return tipe yang sama dengan fork().
* Directori kerja yang aktif harus diubah ke suatu tempat yang telah pasti akan selalu ada. Pengubahan tempat direktori kerja dapat dilakukan dengan implementasi fungsi chdir(). Fungsi chdir() mengembalikan nilai -1 jika gagal.
* Langkah terakhir dalam men-set daemon adalah menutup file desciptor standard dengan menggunakan STDIN, STDOUT, dan STDERR. Dikarenakan oleh daemon tidak menggunakan terminal, maka file desciptor dapat terus berulang dan berpotensi berbahaya bagi keamanan. Untuk mengatasi hal tersebut maka harus menggunakan fungsi close().
```
  char file[256] = "/home/rye/modul2/hatiku/elen.ku";
```
*	Deklarasi variabel bertipe char bernama file yang dideklarasikan = file elen.ku pada direktori /home/rye/modul2/hatiku/.
```
  while(1) {
```
*	Melakukan perulangan while (true).
```
	if (cfileexists(file)){
```
*	Melakukan pengecekan apakah terdapat file seperti pada variabel file.
```
	char mode[] = "0777";
```
*	Membuat array of char bernama mode yang berisi “0777”.
```
    	int i;
    	i = strtol(mode, 0, 8);
```
*	Membuat variabel i bertipe integer yang mengubah tipedata variabel mode dari string menjadi long integer.
```
	chmod (file,i);
```
*	Mengganti permission file dengan 0777.
```
	struct stat elenku;
```
*	Membuat struct stat yang berfungsi untuk menyimpan informasi tentang file elenku.
```
	stat(file, &elenku); 
```
*	Mengambil informasi dari file yang pathnya ditunjukkan oleh variabel file. Informasi tersebut disimpan pada struct stat elenku.
```
	struct passwd *user = getpwuid(elenku.st_uid);
```
*	Membuat struct passwd bernama * user yang berisi user id dari file elenku. Struct stat menyimpan informasi mengenai user id. Namun kita perlu tidak hanya user id melainkan juga owner sehingga user id disimpan pada struct passwd. Untuk dapat menggunakan struct ini diperlukan include pwd.h.
```
	struct group  *group = getgrgid(elenku.st_gid);
```
*	Membuat struct group bernama * group yang berisi group id dari file elenku. Struct stat menyimpan informasi mengenai group id. Namun kita perlu tidak hanya group id melainkan juga group name sehingga group id disimpan pada struct group. Untuk dapat menggunakan struct ini diperlukan include grrp.h.
```
	int usercomp = strcmp (user->pw_name, "www-data");
```
*	Deklarasi variabel usercomp dan grupcomp yang bertipe int.
*	Usercomp digunakan untuk membandingkan apakah owner milik id user file elen.ku adalah www-data.
```
	int grupcomp = strcmp (group->gr_name, "www-data"); 
```
*	grupcomp digunakan untuk membandingkan apakah group name milik id group file elen.ku adalah www-data.
```
	if (usercomp == 0 && grupcomp == 0 ){
		remove (file);
	}
  }
```
*	Jika benar, maka file elen.ku dihapus.
```
    sleep(3);
  }
  exit(EXIT_SUCCESS);
}
```
*	Melakukan pause pada program setiap 3 detik. Sehingga pengecekan file elen.ku akan dilakukan setiap 3 detik.
*	Mengakhiri proses.
*	Program dicompile dengan menggunakan perintah `gcc -o nama nama_program.c`
*	Program dijalankan dengan menggunakan perintah `./nama`
*	Untuk menguji coba program, kita membuat file elen.ku pada direktori /home/rye/modul2/hatiku/ dengan perintah `touch elen.ku`
*	Untuk mengubah owner dan group name file elen.ku agar menjadi www-data dapat dilakukan dengan perintah `sudo chown www-data:www-data elen.ku`
*	Untuk mengakiri program dapat dilakukan perintah `kill [PID]`
*	Untuk mendapatkan PID dapat dilakukan perintah `ps -e | grep nama`

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal2/soal2.png" width="600">

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
* Deklarasi variabel yang diperlukan. child_id1 dan child_id2 digunakan untuk menampung id dari proses anak. fd[2] digunakan sebagai file descriptor.
```
if(pipe(fd) < 0) exit(1);
```
* Digunakan untuk melakukan pipe. Variabel fd merupakan file descriptor. fd[0] untuk proses read dan fd[1] untuk proses write. Jika pipe tidak berhasil dilakukan, program akan exit.
```
  child_id1 = fork();
```
* Digunakan untuk membuat proses anak. PID yang dihasilkan dari proses fork() ditampung pada variabel child_id1.
```
  if (child_id1 == 0) {
    char *argv[5] = {"unzip", "/home/rye/modul2/campur2.zip", "-d", "/home/rye/modul2/", NULL};
    execv("/usr/bin/unzip", argv);
  }
```
* Dalam proses child dilakukan exec.
* Proses yang dilakukan berfungsi untuk meng unzip folder campur2.zip dalam direktori /home/rye/modul2/. Hasil extract disimpan ke dalam direktori /home/rye/modul2/.

```
  else {
	  while ((wait(&status)) > 0);
	  child_id2 = fork();
```
* Setelah proses anak pertama telah selesai, maka dilakukan fork lagi untuk membuat proses anak kedua. PID dari hasil proses fork() tersebut disimpan pada variabel child_id2.
```
  	if (child_id2 == 0) {
		close(fd[0]);
    		dup2(fd[1], STDOUT_FILENO);
    		char *argv[3] = {"ls", "/home/rye/modul2/campur2/", NULL};
   		 execv("/bin/ls", argv);
  	}
```
* Dalam proses child dilakukan beberapa proses yaitu :
* Menutup fd[0]. File descriptor untuk read ditutup karena file descriptor untuk write akan digunakan.
* Fungsi `dup2(fd[1], STDOUT_FILENO)` digunakan untuk menduplikasi STDOUT_FILENO ke dalam file descriptor. Hal ini dikarenakan pada proses anak akan dilakukan exec `ls`. Hasil dari eksekusi `ls` akan ditampilkan pada console melalui STDOUT_FILENO. Dengan adanya fungsi `dup2(fd[1], STDOUT_FILENO)`, hasil eksekusi `ls` tidak akan muncul ke console, melainkan akan ditampung pada file descriptor.
* Mengeksekusi `ls` untuk melihat isi file pada direktori `/home/rye/modul2/campur2/`.
```
	else {
      		while((wait(&status)) > 0);
			close(fd[1]);
```
* Setelah proses anak kedua selesai, maka akan kembali ke proses parent. Pada proses ini dilakukan :
* melakukan perulangan untuk memastikan bahwa proses child sebelumnya sudah berakhir.
* menutup fd[1]. File descriptor untuk read ditutup karena file descriptor untuk read akan digunakan.
```
			dup2(fd[0], STDIN_FILENO);
```
* Fungsi `dup2(fd[0], STDIN_FILENO)` digunakan untuk menduplikasi isi file descriptor ke STDIN_FILENO. Hal tersebut dilakukan karena pada proses ini akan dilakukan eksekusi `grep` yang membutuhkan hasil eksekusi `ls` yang ditampung pada file descriptor.
```
			close(fd[0]);
```
* Setelah proses read file descriptor dijalankan, fd[0] ditutup karena file descriptor untuk write akan digunakan kembali.
```
			int fileopen = open("/home/rye/modul2/daftar.txt",O_WRONLY | O_CREAT);
```
* membuka file daftar.txt dengan mode write only (O_WRONLY). Jika belum ada file daftar.txt pada direktori /home/rye/modul2/, maka file daftar.txt akan dibuat (O_CREAT).
```
			dup2(fileopen, STDOUT_FILENO);
			char *argv[3] = {"grep", ".txt$", NULL};
			execv("/bin/grep", argv);
    	}
  }
}	
```
* Fungsi `dup2(fileopen, STDOUT_FILENO)` digunakan untuk menduplikasi STDOUT_FILENO ke dalam file yang sedang dibuka yaitu daftar.txt. Hal ini dikarenakan pada proses parent akan dilakukan exec `grep` untuk mendapatkan daftar file yang berekstensi .txt. Hasil dari eksekusi `grep` akan ditampilkan pada console melalui STDOUT_FILENO. Dengan adanya fungsi `dup2(fileopen, STDOUT_FILENO)`, hasil eksekusi `grep` tidak akan muncul ke console, melainkan akan tertampung pada fileopen yaitu daftar.txt pada direktori /home/rye/modul2/.
* proses berakhir.
*	Program dicompile dengan menggunakan perintah `gcc -o nama nama_program.c`
*	Program dijalankan dengan menggunakan perintah `./nama`
*	Hasil setelah program berjalan

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal3/soal3%20list.png" width="600">

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
* Pada bagian ini, merupakan proses untuk membuat daemon.
* Langkah pertama adalah menspawn proses menjadi induk dan anak dengan melakukan forking, kemudian membunuh proses induk. Proses induk yang mati akan menyebabkan sistem operasi mengira bahwa proses telah selesai.
* Untuk menulis beberapa file (termasuk logs) yang dibuat oleh daemon, mode file harus diubah untuk memastikan bahwa file tersebut dapat ditulis dan dibaca secara benar. Pengubahan mode file menggunakan implementasi umask().
* Child Process harus memiliki unik SID yang berasal dari kernel agar prosesnya dapat berjalan. Child Process menjadi Orphan Process pada system. Tipe pid_t juga digunakan untuk membuat SID baru untuk Child Process. Setsid() digunakan untuk pembuatan SID baru. Fungsi setsid() memiliki return tipe yang sama dengan fork().
* Directori kerja yang aktif harus diubah ke suatu tempat yang telah pasti akan selalu ada. Pengubahan tempat direktori kerja dapat dilakukan dengan implementasi fungsi chdir(). Fungsi chdir() mengembalikan nilai -1 jika gagal.
* Langkah terakhir dalam men-set daemon adalah menutup file desciptor standard dengan menggunakan STDIN, STDOUT, dan STDERR. Dikarenakan oleh daemon tidak menggunakan terminal, maka file desciptor dapat terus berulang dan berpotensi berbahaya bagi keamanan. Untuk mengatasi hal tersebut maka harus menggunakan fungsi close().
```
       int bil=1;
	FILE *fp;
	char file[256];
```
* Membuat variabel bil bertipe int = 1. Variabel bil akan digunakan sebagai urutan penamaan file.
* Membuat variabel * fp bertipe file.
* Membuat variabel file bertipe char.
```
	strcpy(file,"/home/rye/Documents/makanan/makan_enak.txt");
```
* Mengcopy string /home/rye/Documents/makanan/makan_enak.txt ke dalam variabel file.
```
  while(1) {
		struct stat makanenak;
```
* Membuat struct stat bernama makanenak.
```
		stat(file,&makanenak);
```
* Mengambil informasi dari file yang ditunjukkan oleh isi variabel file dan informasi tersebut disimpan pada struct stat makanenak.
```
	if(difftime(time(NULL),makanenak.st_atime) <= 30){
```
* Fungsi `difftime` digunakan untuk mencari selisih waktu antara waktu pada parameter pertama dengan waktu pada parameter kedua.
* Fungsi time(NULL) digunakan untuk mendapatkan waktu saat program dijalankan.
* `makanenak.st_atime` digunakan untuk mendapatkan waktu terakhir akses yang tersimpan pada struct stat makanenak.
* Melakukan pengecekan selisih waktu sekarang dengan waktu akses.
* Jika selisihnya <= 30, maka :
```
		char nfile[256];
        	strcpy(nfile,"/home/rye/Documents/makanan/makan_sehat");
```
* Membuat variabel nfile bertipe char.
* Mengcopy /home/rye/Documents/makanan/makan_sehat ke nfile.
```
		char c[256];
		sprintf(c, "%d", bil);
```
* Membuat variabel c bertipe char.
* Varibel bil yang tipe datanya integer diubah menjadi variabel c yang tipe datanya char.
```
		strcat(nfile, c);
```
* Menggabungkan string nfile dengan c dan disimpan pada variabel nfile. Sehingga variabel nfile sekarang berisi string /home/rye/Documents/makanan/makan_sehat[bil].
```
		strcat(nfile, ".txt");
```
* Menggabungkan string nfile dengan .txt dan disimpan pada variabel nfile. Sehingga variabel nfile sekarang berisi string /home/rye/Documents/makanan/makan_sehat[bil].txt.
```
		fp = fopen(nfile, "w");
```
* Membuka file sesuai dengan path nfile dengan mode write. Fungsi ini dilakukan untuk mencreate file sesuai dengan path pada nfile.
```
		fclose(fp);
		bil++;
	}
```
* Menutup file fp yang telah dibuka.
* Increment variabel bil.
```
sleep(5);
  }
  exit(EXIT_SUCCESS);
  }
```
* Melakukan pause setiap 5 detik. Sehinggga program akan melakukan pengecekan (berjalan) setiap 5 detik.
* Mengakhiri program.
*	Program dicompile dengan menggunakan perintah `gcc -o nama nama_program.c`
*	Program dijalankan dengan menggunakan perintah `./nama`
*	Untuk melakukan uji coba pada program, dijalankan perintah `touch -a makan_enak.txt` untuk mengubah access time pada file makan_enak.txt
*	Hasil setelah program berjalan

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal4/soal4%20pt%201.png" width="600">

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal4/soal4%20pt%202.png" width="600">

*	Untuk mengakiri program dapat dilakukan perintah `kill [PID]`
*	Untuk mendapatkan PID dapat dilakukan perintah `ps -e | grep nama`

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal4/soal4.png" width="600">

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
* Pada bagian ini, merupakan proses untuk membuat daemon.
* Langkah pertama adalah menspawn proses menjadi induk dan anak dengan melakukan forking, kemudian membunuh proses induk. Proses induk yang mati akan menyebabkan sistem operasi mengira bahwa proses telah selesai.
* Untuk menulis beberapa file (termasuk logs) yang dibuat oleh daemon, mode file harus diubah untuk memastikan bahwa file tersebut dapat ditulis dan dibaca secara benar. Pengubahan mode file menggunakan implementasi umask().
* Child Process harus memiliki unik SID yang berasal dari kernel agar prosesnya dapat berjalan. Child Process menjadi Orphan Process pada system. Tipe pid_t juga digunakan untuk membuat SID baru untuk Child Process. Setsid() digunakan untuk pembuatan SID baru. Fungsi setsid() memiliki return tipe yang sama dengan fork().
* Directori kerja yang aktif harus diubah ke suatu tempat yang telah pasti akan selalu ada. Pengubahan tempat direktori kerja dapat dilakukan dengan implementasi fungsi chdir(). Fungsi chdir() mengembalikan nilai -1 jika gagal.
* Langkah terakhir dalam men-set daemon adalah menutup file desciptor standard dengan menggunakan STDIN, STDOUT, dan STDERR. Dikarenakan oleh daemon tidak menggunakan terminal, maka file desciptor dapat terus berulang dan berpotensi berbahaya bagi keamanan. Untuk mengatasi hal tersebut maka harus menggunakan fungsi close().
```
  while(1) {
 	time_t jam = time(NULL);
```
* Selama daemon berjalan, dilakukan :
* deklarasi variabel jam yang bertipe time_t.
* Fungsi `time(NULL)` digunakan untuk mendapatkan time saat ini yang disimpan pada variabel jam.
```
        struct tm *waktu = localtime(&jam);
```
* membuat struct tm baru bernama waktu yang berisi waktu dari jam yang formatnya telah terpisah-pisah. sehingga kita bisa mendapatkan tiap format waktu (jam,menit,dst).
```
        char swaktu[256], swaktu1[256];
```
* Membuat variabel bertipe char bernama swaktu dan swaktu1.
```
        sprintf(swaktu, "%d:%d:%d-%d:%d",waktu->tm_mday,waktu->tm_mon+1,waktu->tm_year+1900,waktu->tm_hour,waktu->tm_min);
```
* merubah tipe data integer ke dalam char dari variabel swaktu. dibuat dalam format tanggal:bulan:tahun:jam:menit. 
* berikut adalah tabel untuk struct tm

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal5/soal5%20tm.jpg" width="600">

* tm_mon ditambah satu karena tm_mon dimulai dari 0 (Januari adalah bulan ke 0)
* tm_year ditambah 1900 karena tm_year dimulai dari 1900 (1900 adalah tahun ke 0)
```
	strcpy(swaktu1, "/home/rye/log/");
```
* mengcopy string /home/rye/log/ ke dalam variabel swaktu1.
```
	strcat(swaktu1,swaktu);
```
* menggabungkan nama file swaktu1 dengan swaktu yang disimpan dalam swaktu1. Sehingga variabel swaktu1 berisi string /home/rye/log/tanggal:bulan:tahun:jam:menit.
```
        mkdir(swaktu1, 0777);
```
* membuat direktori dengan nama sesuai swaktu dan permission 777.
```
        int bil = 1;
```
* deklarasi variabel bil bertipe integer = 1. Variabel bil digunakan untuk urutan penamaan file.
```
        while(bil <= 30){
```
* melakukan perulangan sebanyak 30 kali.
```
                FILE *source, *target;
```
* membuat variabel source dan target bertipe file.
```
                char c, nfile[256];
                sprintf(nfile, "%s/log%d", swaktu1, bil);
```
* membuat variabel bernama c dan nfile bertipe char.
* mengubah variabel bil yang mempunyai tipe data int menjadi string sekaligus menggabungkannya dengan variabel swaktu1 dan disimpan pada variabel nfile. Sehingga variabel nfile sekarang berisi string /home/rye/log/tanggal:bulan:tahun:jam:menit/log[bil]
```
                strcat(nfile, ".log");
```
* menggabungkan string pada variabel nfile dengan .log dan disimpan pada variabel nfile. Sehingga variabel nfile sekarang berisi string /home/rye/log/tanggal:bulan:tahun:jam:menit/log[bil].log
```
                source = fopen("/var/log/syslog", "r");
```
* mendeklarasikan source sebagai pembaca (scan) isi folder syslog.
```
                target = fopen(nfile, "w");
```
* mendeklarasikan target sebagai penulis (print) file dari folder syslog  ke folder nfile.
```
                while(fscanf(source, "%c", &c) != EOF)
                        fprintf(target, "%c", c);
```
* melakukan perulangan untuk membaca file dalam folder source ketika c != EOF (akhir file).
* jika memenuhi, maka file yang telah terbaca dicopy dalam folder target.
```
                bil++;
```
* increment variabel bil.
```
                fclose(source);
                fclose(target);
                sleep(60);
        }
  }

  exit(EXIT_SUCCESS);
}
```
* menutup file source dan target.
* pause setiap 60 detik. sehingga dalam 30 menit didapatkan 30 file log.
*	Program dicompile dengan menggunakan perintah `gcc -o nama nama_program.c`
*	Program dijalankan dengan menggunakan perintah `./nama`
*	Hasil setelah program berjalan

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal5/soal5.png" width="600">

*	Folder yang dibuat

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal5/soal5%20folder.png" width="600">

*	Isi dari folder yang dibuat

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal5/soal5%20daftar%20log.png" width="600">

*	Isi dari file .log yang dibuat

<img src="https://github.com/ihdiannajaa29/SoalShift_modul2_F02/blob/master/soal5/soal5%20log.png" width="600">

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
```
* membuat variabel perintah bertipe file yang berisi pid dari proses yang berjalan pada proses 5a.
```
	FILE *perintah = popen("pidof soal5a","r");
	fgets(pid_string,5,perintah);
	pid_t pid = strtoul(pid_string,NULL,10);

```
* hasil pid yang didapatkan dari perintah disimpan pada pid_string.
```
	kill(pid,SIGKILL);
	pclose(perintah);
}
```
* membunuh proses dengan pid yang sesuai dengan pid_string.
* menutup file perintah.
* proses selesai.
*	Program dicompile dengan menggunakan perintah `gcc -o nama nama_program.c`
*	Program dijalankan dengan menggunakan perintah `./nama`
