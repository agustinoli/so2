#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "sockets.h"
#define MAX 1460

void user_login();

int main(int argc, char *argv[] )
{
  user_login();
  printf("\033[5;22;36m");
  printf("Esperando conexión...");
  printf("\033[m"); /* stop blinking attribute from further text */
  printf("\n");

  int fd = open_tcp_server("8080");
  int udp_fd = open_udp_server("8080");
  char *user_input;
  if ((user_input = (char *) malloc(MAX * sizeof(char))) == NULL)
  {
    exit(1);
  }
  char *telemetria_ptr;
  if ((telemetria_ptr = (char *) calloc(MAX , sizeof(char))) == NULL)
  {
    exit(1);
  }
  printf("\033[1A"); // Move up a line
  printf("Servidor conectado   \n");
  sleep(1);
  do
  {
		printf("#");
		fgets(user_input,MAX,stdin);
		strtok(user_input, "\n");
		if (strcmp(user_input,"uf") == 0)
		{
      /*Envio comando*/
      write_cmd(fd,"uf");
      sleep(1);
      send_file(fd,"crosscompile/sat.o");
      printf("Sale el proceso %d\n",getpid());
      sleep(10);
      exit(1);
		}
		else if (strcmp(user_input,"ss") == 0)
		{
      time_t start, end;
      double elapsed_time;
      write_cmd(fd,"ss");
      start = time(&start);
      receive_file(fd,"recibido.jpg");
      end = time(&end);
      elapsed_time = difftime(end,start);
      printf("El proceso de transferencia llevo %.0f segundos.\n", elapsed_time);
		}
		else if (strcmp(user_input,"ot") == 0)
		{
      write_cmd(fd,"ot");
      memset(telemetria_ptr,' ',MAX);
      sleep(3);
      read_upd_socket(udp_fd,telemetria_ptr);
      printf("%s\n",telemetria_ptr);
		}
		else if (strcmp(user_input, "exit") == 0)
		{
      write_cmd(fd,"exit");
		}
		else
		{
      printf("Solo se admiten 4 comandos: obtener telemetria,"
							"start scanning, update firmware o exit\n");
		}
  }
  while(strcmp(user_input,"exit") != 0);
  close(fd);
  return 1;
}


void user_login()
{
  char *u;
  if ((u = (char *) malloc(MAX * sizeof(char))) == NULL) {
  printf( "malloc failed: \n");
  exit(1);
  }

  char *p;
  if ((p = (char *) malloc(MAX * sizeof(char))) == NULL) {
  printf( "malloc failed: \n");
  exit(1);
  }

  int tries_left=3;
  _Bool auth=0;
  char passwd[]="admin";
  char user[]="admin";

  do
  {
    printf("$ Ingrese usuario: ");
    fgets(u,20,stdin);
    strtok(u, "\n");
    p=getpass("$ Ingrese contraseña: ");
    if(strcmp(user,u)==0 && strcmp(passwd,p)==0 )
    {
      auth=1;
    }else{
      printf("Mala combinacion de usuario y contraseña.\n");
      tries_left--;
    }
  }
  while(auth ==0 && tries_left!=0);

  if(tries_left==0 || auth == 0)
  {
    perror( "mala autenticacion");
    free(u);
    free(p);
    exit(1);
  }
}
