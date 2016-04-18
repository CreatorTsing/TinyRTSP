#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc,char** argv)
{
	int sock;
	sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock < 0)
	{
		printf("sock failed\n");
		return -1;
	}

	/*
	struct sockaddr_in
	{
		in_port_t	sin_port;
		struct in_addr	sin_addr;

	}
	*/
	struct sockaddr_in server;
	bzero(&server,sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(8889);
	server.sin_addr.s_addr = inet_addr("192.168.199.121");

	if(bind(sock,(struct sockaddr*)&server,sizeof(struct sockaddr)) < 0)
	{
		close(sock);
		printf("bind failed\n");
		return -1;
	}

	if(listen(sock,5) == -1)
	{
		close(sock);
		printf("listen falied\n");
		return -1;
	}

	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	int conn = accept(sock,(struct sockaddr*)&client_addr,&length);

	if(conn < 0)
	{
		return -1;
	}

	printf("conn success\n");

	char buffer[64];
	while(1)
	{
		int len = recv(conn,buffer,sizeof(buffer),0);
		if(len <= 0)
		{
			printf("recv error\n");
			break;
		}
		printf("recv %s\n",buffer);
	}

	close(conn);
	close(sock);

	return 0;
}
