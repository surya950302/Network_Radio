#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

//CS656 Group WG2 music transmitter program

int main(int argc, char** argv) {
	struct stat fileinfo;
	struct addrinfo *addr, hints;
	if(argc<6 || atoi(argv[2])<1024 || atoi(argv[2])>65535)
		exit(1);//we need all parameters, and a valid port.
	long size = atol(argv[4]);
	useconds_t delay = atoi(argv[5]);
	FILE *audiofile = fopen(argv[3],"r");
	fstat(fileno(audiofile),&fileinfo);
	char *data = malloc(fileinfo.st_size);
	fread(data,fileinfo.st_size,1,audiofile);

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	getaddrinfo(argv[1],argv[2],&hints,&addr);
	int destsock = socket(AF_INET,SOCK_DGRAM,0);

	long pos = 0;
	while(pos<fileinfo.st_size) {
		if(pos+size>fileinfo.st_size)
			size-= (pos+size)-fileinfo.st_size;
		sendto(destsock,data+pos,size,0,addr->ai_addr,addr->ai_addrlen);
		pos+=size;
		usleep(delay);
	}
	fclose(audiofile);
	free(data);
	freeaddrinfo(addr);
	return 0;
}