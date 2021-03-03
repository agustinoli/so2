#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define MAX 1460

struct sockaddr_in serv_addr_udp;

int open_tcp_server(char *port_str)
{
  int sockfd, newsockfd, puerto, pid;
  struct sockaddr_in serv_addr, cli_addr;
  unsigned int clilen;
  sockfd = socket( AF_INET, SOCK_STREAM, 0);
  if ( sockfd < 0 ) {
    perror( "Fallo socket.\n" );
    exit( 1 );
  }
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
  memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
  puerto = atoi( port_str );
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons( puerto );
  if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 )
  {
    perror( "Fallo bind " );
    exit( 1 );
  }
  listen( sockfd, 5 );
	clilen = sizeof( cli_addr );

  while( 1 )
	{
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
		if ( newsockfd < 0 ) {
			perror( "Fallo accept " );
			exit( 1 );
		}
		pid = fork();
		if ( pid < 0 )
    {
			perror( "Fallo fork " );
			exit( 1 );
		}
		if ( pid == 0 )  // Proceso hijo
    {
			close( sockfd );
      //printf( "SERVIDOR [%s]: newsockfd: %d\n",port_str, newsockfd );
      return newsockfd;
		}
		else
    {
			//printf( "SERVIDOR [%s]: New client,proceso hijo: %d\n",port_str, pid );
			close( newsockfd );
		}
	}
}

int open_udp_server(char * port_str)
{
  int sockfd, puerto;
	sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sockfd < 0) {
    return -3;
	}
	memset( &serv_addr_udp, 0, sizeof(serv_addr_udp) );
	puerto = atoi( port_str );
	serv_addr_udp.sin_family = AF_INET;
	serv_addr_udp.sin_addr.s_addr = INADDR_ANY;
	serv_addr_udp.sin_port = htons( puerto );
	memset( &(serv_addr_udp.sin_zero), '\0', 8 );

	if( bind( sockfd, (struct sockaddr *) &serv_addr_udp, sizeof(serv_addr_udp) ) < 0 ) {
    return -4;
	}
  return sockfd;
}

int open_tcp_client(char *ip_str,char *port_str)
{
  int sockfd, puerto;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  puerto = atoi( port_str );
  sockfd = socket( AF_INET, SOCK_STREAM, 0 );
  if ( sockfd < 0 ) {
    return -1;
  }

  server = gethostbyname( ip_str );
  if (server == NULL) {
    return -2;
  }
  memset( (char *) &serv_addr, '0', sizeof(serv_addr) );
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length );
  serv_addr.sin_port = htons( puerto );
  if ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr ) ) < 0 ) {
    return -3;
  }
  return sockfd;
}

int write_cmd(int sockfd,void *cmd_ptr)
{
  char buffer[MAX];
  strcpy(buffer,cmd_ptr);
  size_t n;
    n = write( sockfd, buffer, strlen(buffer) );
    if ( n < 0 ) {
      return -1;
    }
    return 0;
}

void read_cmd(int sockfd, void *cmd_ptr)
{
  char buffer[MAX];
  int n;
  memset( buffer, 0, MAX );
  n = recv( sockfd, buffer, MAX-1 ,0);
  if ( n < 0 ) {
    return;
  }
  strcpy(cmd_ptr,buffer);
}

int send_file(int fd, char *filename_str)
{
  long int lsize;
  FILE * pFile;
  pFile = fopen(filename_str,"rb");
  if( pFile == NULL ){
   return -1;
  }
  fseek (pFile , 0 , SEEK_END);
  lsize = ftell (pFile);
  rewind (pFile);
  size_t num = lsize;
  char snum[6];
  sprintf(snum,"%zu",num);
  write_cmd(fd,snum);
  size_t n, m;
  unsigned char buff[MAX];
   //copy the file into the socket:
   //printf("Voy a enviar %s bytes\n",snum);
  do
  {
    n = fread((void *)buff, 1, sizeof(buff), pFile);
    if (n)
    {
      m = write(fd, buff, n);
    }
    else
    {
      m=0;
    }
  } while ((n > 0) && (n == m));
  // close the file
  fclose (pFile);
  return 0;

}
int receive_file (int sockfd,char *filename_str)
{
  char snum[MAX];
  read_cmd(sockfd,snum);
  int num = atoi(snum);
  FILE *fp;
  size_t n,total;
  total=0;
  unsigned char buff[MAX];  // to store message from client
  fp=fopen(filename_str,"wb"); // stores the file content in filename_str in the program directory
  if( fp == NULL ){
   return -1;
  }
  while(total < num){
    //printf("%zu/%d\n",total,num);
    n = recv(sockfd,buff, sizeof(buff), 0);
    total+=n;
    fwrite(buff,1,n,fp);
  }
  fclose(fp);
  return 0;
}

int read_upd_socket(int sockfd,char *ptr)
{
	int n;
  unsigned int tamano_direccion = sizeof( struct sockaddr );
  char buffer[ MAX ];
  memset( buffer, 0, MAX );
  n = recvfrom( sockfd, ptr, MAX-1, MSG_DONTWAIT, (struct sockaddr *)&serv_addr_udp,&tamano_direccion );
  return n;
}

int write_udp(char *ip_str,char *port_str,char *str)
{
  int sockfd, n;
  unsigned int tamano_direccion;
  struct sockaddr_in dest_addr;
  struct hostent *server;

  server = gethostbyname( ip_str );
  if ( server == NULL )
  {
      return -1;
  }
  sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
  if (sockfd < 0)
  {
      return -2;
  }

  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons( atoi( port_str ) );
  dest_addr.sin_addr = *( (struct in_addr *)server->h_addr );
  memset( &(dest_addr.sin_zero), '\0', 8 );

  tamano_direccion = sizeof( dest_addr );
  n = sendto( sockfd, (void *)str, MAX, 0, (struct sockaddr *)&dest_addr, tamano_direccion );
  if ( n < 0 )
  {
      return -3;
  }
  return 0;
}
