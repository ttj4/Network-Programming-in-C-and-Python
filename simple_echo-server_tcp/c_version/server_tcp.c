//Server - TCP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 4050
#define BACKLOG 5
#define MAXDATASIZE 100
int main(void)
{
	int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	char buf[MAXDATASIZE];
	int numbytes;
	int yes = 1;

	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		perror("socket");
		exit(1);
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero),'\0',8);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	
	printf("listening\n");
	if (listen(sockfd,BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	while (1) {
	sin_size = sizeof(struct sockaddr_in);
	
	if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size)) == -1) {
		perror("accept");
		exit(1);
	}
	printf("server : got connection from %s\n",inet_ntoa(their_addr.sin_addr));
	if (!fork()) {
		close(sockfd);
		while (1) {
		if ((numbytes = recv(new_fd,buf,MAXDATASIZE-1,0)) == -1) 
			perror("recv");
		buf[numbytes] = '\0';
		printf("numbytes received is %d\n",numbytes);
		
		if (send(new_fd,buf,strlen(buf),0) == -1)
			perror("send");
		printf("send %d\n",(int)strlen(buf));
		}
		close(new_fd);
		}
		
	close(new_fd);
//	close(sockfd);
	}
	
	return 0;
}
