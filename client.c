#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
char buffer[256];

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void function(int sockfd)
{
   while(1)
   {
     int n=0;
     int x=0;
     bzero(buffer,256);
     printf("Message to Server:");
     fgets(buffer,256,stdin);
     n=write(sockfd,buffer,256);
     if(n<0)error("Error in Writing");
     bzero(buffer,256);
     x=read(sockfd,buffer,256);
     if(x<0)error("Error in Reading");
     printf("Message from Server:%s\n",buffer);


   }

}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    function(sockfd);
    return 0;
}
