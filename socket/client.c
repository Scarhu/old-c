1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <errno.h>
 4 #include <string.h>
 5 #include <netdb.h> //for gethostbyname
 6 #include <sys/types.h>
 7 #include <sys/socket.h>
 8 #include <netinet/in.h>
 9 #include <arpa/inet.h>
10 
11 #define MAX_BUF 4096
12 #define SERVER_PORT 12138
13 
14 
15 int main(int argc,char *argv[])
16 {
17     int sockfd;//socket
18     char sendBuf[MAX_BUF],recvBuf[MAX_BUF];
19     int sendSize,recvSize;//用于记录记录发送和接收到数据的大小
20     struct hostent * host;
21     struct sockaddr_in servAddr;
22     char username[32];
23     char * p;
24 
25     if(argc != 3)
26     {
27         perror("use: ./client [hostname] [username]");
28         exit(-1);
29     }
30     p=username;
31     strcpy(p,argv[2]);
32     printf("username:%s\n",username);
33     host=gethostbyname(argv[1]);
34     if(host==NULL)
35     {
36     　　perror("fail to get host by name.");
37         exit(-1);
38     }
39     printf("Success to get host by name ...\n");
40 
41     //创建socket
42     if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
43     {
44     　　perror("fail to establish a socket");
45     　　exit(1);
46     }
47     printf("Success to establish a socket...\n");
48     
49     /*init sockaddr_in*/
50     servAddr.sin_family=AF_INET;
51     servAddr.sin_port=htons(SERVER_PORT);
52     servAddr.sin_addr=*((struct in_addr *)host->h_addr);
53     bzero(&(servAddr.sin_zero),8);
54 
55     /*connect the socket*/
56     if(connect(sockfd,(struct sockaddr *)&servAddr,sizeof(struct sockaddr_in))==-1)
57     {
58     　　perror("fail to connect the socket");
59     　　exit(1);
60     }
61     printf("Success to connect the socket...\n");
62 
63     //send-recv
64     while(1)
65     {
66         printf("Input:");
67         fgets(sendBuf,MAX_BUF,stdin);
68         send(sockfd,sendBuf,strlen(sendBuf),0);
69         memset(sendBuf,0,sizeof(sendBuf));
70         recv(sockfd,recvBuf,MAX_BUF,0);
71         printf("Server:%s\n",recvBuf);
72         memset(recvBuf,0,sizeof(recvBuf));
73     }
74 
75     return 0;
76 }
