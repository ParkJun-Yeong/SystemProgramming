/*mycp.c: copy files with attributes  and display the copied file. by park junyeong wndwls1024@naver.com*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_BUF 64



int main(int argc, char* argv[]){
	int fd_ori,fd_new;
	int readsize;
	int buf[MAX_BUF];
	struct stat buf2; //파일 attributes를담을 구조체

	if(argc != 3){
		printf("USAGE: %s original_filename copied_filename\n",argv[0]);
		exit(1);
	} //인자수 확인 후 사용법 알리며 프로세스 종료

	fd_ori = open(argv[1],O_RDONLY);
	if(fd_ori < 0) {
		printf("Can't open %sfile with errno %d\n", argv[1],errno);
		exit(1);
	} //open 검사
	
	fstat(fd_ori,&buf2); //원본 파일의 attributes 복사

	fd_new = open(argv[2],O_RDWR|O_CREAT|O_EXCL,buf2.st_mode);
	//원본 파일과 동일한 접근권한 부여

	if(fd_new < 0) {
		printf("Can't open %sfile with errno %d\n", argv[2],errno);
		exit(1);
	}

	while(1){
		readsize = read(fd_ori,buf,MAX_BUF);
		if(readsize == 0) break;
		write(fd_new,buf,readsize);
	} //내용복사

	close(fd_ori);
	close(fd_new);
}
	


	
