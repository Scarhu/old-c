 1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <errno.h>
  4 #include <string.h>
  5 #include <netdb.h>
  6 #include <sys/types.h>
  7 #include <sys/socket.h>
  8 #include <sys/time.h>
  9 #include <sys/un.h>
 10 #include <sys/ioctl.h>
 11 #include <sys/wait.h>
 12 #include <netinet/in.h>
 13 #include <arpa/inet.h>
 14 
 15 
 16 #define SERVER_PORT 12138
 17 #define BACKLOG 20
 18 #define MAX_CON_NO 10
 19 #define MAX_DATA_SIZE 4096
 20 
 21 int main(int argc,char *argv[])
 22 {
 23     struct sockaddr_in serverSockaddr,clientSockaddr;
 24     char sendBuf[MAX_DATA_SIZE],recvBuf[MAX_DATA_SIZE];
 25     int sendSize,recvSize;
 26     int sockfd,clientfd;
 27     int on=1;
 28     int sinSize=0;
 29     char username[32];
 30 
 31     if(argc != 2)
 32     {
 33         printf("usage: ./server [username]\n");
 34         exit(1);
 35     }
 36     strcpy(username,argv[1]);
 37     printf("username:%s\n",username);
 38 
 39     /*establish a socket*/
 40     if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
 41     {
 42         perror("fail to establish a socket");
 43         exit(1);
 44     }
 45     printf("Success to establish a socket...\n");
 46 
 47     /*init sockaddr_in*/
 48     serverSockaddr.sin_family=AF_INET;
 49     serverSockaddr.sin_port=htons(SERVER_PORT);
 50     serverSockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 51     bzero(&(serverSockaddr.sin_zero),8);
 52 
 53     setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
 54 
 55     /*bind socket*/
 56     if(bind(sockfd,(struct sockaddr *)&serverSockaddr,sizeof(struct sockaddr))==-1)
 57     {
 58         perror("fail to bind");
 59         exit(1);
 60     }
 61     printf("Success to bind the socket...\n");
 62 
 63     /*listen on the socket*/
 64     if(listen(sockfd,BACKLOG)==-1)
 65     {
 66         perror("fail to listen");
 67         exit(1);
 68     }
 69 
 70     /*accept a client's request*/
 71     if((clientfd=accept(sockfd,(struct sockaddr  *)&clientSockaddr, &sinSize))==-1)
 72     {
 73         perror("fail to accept");
 74         exit(1);
 75     }
 76     printf("Success to accpet a connection request...\n");
 77     printf(" %s join in!\n",inet_ntoa(clientSockaddr.sin_addr));
 78     while(1)
 79     {
 80         /*receive datas from client*/
 81         if((recvSize=recv(clientfd,recvBuf,MAX_DATA_SIZE,0))==-1)
 82         {
 83             perror("fail to receive datas");
 84             exit(1);
 85         }
 86         printf("%s\n",recvBuf);
 87         memset(recvBuf,0,MAX_DATA_SIZE);
 88 
 89         /*send datas to client*/
 90         printf("Server:");
 91         fgets(sendBuf,MAX_DATA_SIZE,stdin);
 92         if((sendSize=send(clientfd,sendBuf,strlen(sendBuf),0))!=strlen(sendBuf))
 93         {
 94             perror("fail to send datas");
 95             exit(1);
 96         }
 97         printf("Success to send datas\n");
 98         memset(sendBuf,0,MAX_DATA_SIZE);
 99     }
100 
101     return 0;
102 }
