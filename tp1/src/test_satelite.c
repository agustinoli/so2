#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "sockets.h"
#define MAX 1460

char uf[]= "uf";
char ss[]= "ss";
char ot[]= "ot";
char version[]="2.21\n";
void telemetria(char *str);
void print_error(char * str);

int main(int argc, char *argv[] )
{
  // print_error("Inicia el programa");
  int fd = open_tcp_client(argv[1],"8080");
  if(fd <0 )
  {
    char msg[MAX];
    sprintf(msg,"Ocurrio error %d fd.",fd);
    print_error(msg);
    exit(-1);
  }
  char buffer[MAX];
  // print_error("entro al while principal");
  do
  {
    read_cmd(fd,buffer);
    strtok(buffer, "\n");
    if (strcmp(buffer,uf) == 0)
		{
      receive_file(fd,"b.out");
      /*Leo el checksum que me envian*/
      read_cmd(fd,buffer);
      popen("cp -f b.out sat.o","w");
      popen("sudo chmod 777 sat.o","w");
      close(fd);
      popen("sudo reboot","w");
		}
		else if (strcmp(buffer,ss) == 0)
		{
      sleep(1);
      send_file(fd,"file.jpg");
		}
		else if (strcmp(buffer,ot) == 0)
		{
      char *tel_ptr;
      if ((tel_ptr = (char *) malloc(MAX * sizeof(char))) == NULL) {
        exit(-1);
      }
      telemetria(tel_ptr);
      write_udp(argv[1],"8080",tel_ptr);
		}
  }
  while(strcmp(buffer,"exit") != 0);
  close(fd);
  return 1;
}

void telemetria(char *str)
{
  memset(str,0,MAX);
  strcat(str,"Id del satelite: 1\n");
  strcat(str,"Uptime: ");
  char aux[MAX];
  FILE * uptime = popen("uptime","r");
  fgets(aux,MAX,uptime);
	pclose(uptime);
  strcat(str,aux);
  strcat(str,"Version del software: ");
  strcat(str,version);
  FILE * memoria = popen("free -h | grep -i \"Mem\"","r");
  fgets(aux,MAX,memoria);
  pclose(memoria);
  strcat(str,aux);
  FILE * cpu = popen("top -b -n1 | grep -i \"Cpu(s)\"" ,"r");
  fgets(aux,MAX,cpu);
  pclose(cpu);
  strcat(str,aux);
}

void print_error(char * str)
{
  time_t now;
  time(&now);
  printf("%s: %s\n",strtok(ctime(&now), "\n"),str);
}
