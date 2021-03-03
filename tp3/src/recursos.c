
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

char s1[]="<!DOCTYPE html>\n<html>\n<title>Trabajo Práctico N°3</title>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<link rel=\"stylesheet\" href=\"w3.css\">\n<link rel=\"stylesheet\" href=\"font.css\">\n<link rel=\"stylesheet\" href=\"font-awesome.min.css\">\n<style>\nbody, h1,h2,h3,h4,h5,h6 {font-family: \"Montserrat\", sans-serif}\n.w3-row-padding img {margin-bottom: 12px}\n/* Set the width of the sidebar to 120px */\n.w3-sidebar {width: 120px;background: #222;}\n/* Add a left margin to the \"page content\" that matches the width of the sidebar (120px) */\n#main {margin-left: 120px}\n/* Remove margins from \"page content\" on small screens */\n@media only screen and (max-width: 600px) {#main {margin-left: 0}}\n</style>\n<body class=\"w3-black\">\n\n\n<!-- Icon Bar (Sidebar - hidden on small screens) -->\n<nav class=\"w3-sidebar w3-bar-block w3-small w3-hide-small w3-center\">\n  <a href=\"index.html\" class=\"w3-bar-item w3-button w3-padding-large w3-black\">\n    <p>Home</p>\n  </a>\n  <a href=\"recursos.cgi\" class=\"w3-bar-item w3-button w3-padding-large w3-hover-black\">\n    <p>Recursos</p>\n  </a>\n  <a href=\"goes16.cgi\" class=\"w3-bar-item w3-button w3-padding-large w3-hover-black\">\n    <p>GOES16</p>\n  </a>\n  <a href=\"modulos.cgi\" class=\"w3-bar-item w3-button w3-padding-large w3-hover-black\">\n    <p>Módulos</p>\n  </a>\n</nav>\n\n<div class=\"w3-padding-large\" id=\"main\">\n  <header class=\"w3-container w3-padding-32 w3-center w3-black\" id=\"home\">\n    <h1 class=\"w3-jumbo\">Trabajo Práctico N°3</h1>\n  </header>    \n    <div class=\"w3-row-padding\" style=\"margin:0 -16px\">\n      <div class=\"w3-half w3-margin-bottom\">\n        <ul class=\"w3-ul w3-white w3-center w3-opacity w3-hover-opacity-off\">\n          <li class=\"w3-dark-grey w3-xlarge w3-padding-32\">Recursos</li>\n          <li class=\"w3-padding-16\"> ";
char res_start[]="<li ";
char res_end[]=" class=\"w3-padding-16\">   </li>\n";
char s2[]="</ul>\n       \n  </div> \n  </div>\n<!-- END PAGE CONTENT -->\n</div>\n\n</body>\n</html>\n";

void set_cpuInfo();
void set_infoMem();
void set_upTime();
char *rs_file(char path[],char info[]);

typedef struct cpuData{
	int cantCPU;
	char model[100];
	char totalMem[100];
	char freeMem[100];
} cpuData;

typedef struct tiempo{
	long unsigned int valor;
	int dias;
	int horas;
	int minutos;
	int segundos;
} tiempo;

cpuData cpu;
tiempo uptime;

void set_cpuInfo(){

	char temp[100];
	FILE *fp;
	int count=0;

	fp = fopen("/proc/cpuinfo", "r");
	if (fp==NULL) {
        fputs ("File error",stderr); exit (1);
    }

    while(!feof(fp)){

    	fgets(temp,100,fp);

    	if(strstr(temp,"processor")){
			count++;
		}
    	else if(strstr(temp,"model name")){
    		char *pch;
  			pch = strchr(temp,':');
    		strcpy(cpu.model, pch);
   		}
   	}
	fclose(fp);
	cpu.cantCPU=count;
  memmove(&cpu.model[0], &cpu.model[1], strlen(cpu.model));
  int len = strlen(cpu.model);
  if( cpu.model[len-1] == '\n' )
    cpu.model[len-1] = 0;

}

void set_infoMem(){

	char temp[100];
	FILE *fp;
	fp = fopen("/proc/meminfo", "r");
	while(!feof(fp)){
		fgets(temp,100,fp);
		if(strstr(temp,"MemTotal")){
			char *pch;
  			pch = strchr(temp,':');
    		strcpy(cpu.totalMem, pch);
    	}
    	else if (strstr(temp,"MemFree")){
    		char *pch;
  			pch = strchr(temp,':');
    		strcpy(cpu.freeMem, pch);
    	}
	}
	fclose(fp);
}

void set_upTime(){
	char temp[100];
	int count=0;
	FILE *fp;
	long int valor;
	fp = fopen("/proc/uptime", "r");
	while(!feof(fp)){
		char *str =fgets(temp,100,fp);
		char * pch;
		char * aux[2];
		  pch = strtok (str," ");
		  while (pch != NULL)
		  {
			aux[count]=pch;
		    pch = strtok (NULL, " ");
		    count++;
			//aux[0] tiene el tiempo desde el inicio del sistema
			//aux[1]	tiene el idle time
			valor= atol(aux[0]);
	   }
     //	printf("Val[0] es: %d\n",val[0]);

   	int dias,horas,mins,segs;
   	dias = valor / (60*60*24);
   	horas = (valor-dias*(60*60*24)) / (60*60);
   	mins = (valor-dias*(60*60*24)-horas*(60*60)) / (60);
   	segs = (valor-dias*(60*60*24)-horas*(60*60)-mins*60 );
   	//printf("Valor: %ld  --> %d Dias,%d horas,
   	//	%d minutos y %d segundos.\n",valor,dias,horas,mins,segs);
   	uptime.valor=valor;
   	uptime.dias=dias;
   	uptime.horas=horas;
   	uptime.minutos=mins;
   	uptime.segundos=segs;
  }
   	fclose(fp);

}

void print_tiempo(struct tiempo timepo,char nombre[]){
	printf("%s: %dD %02d:%02d:%02d\n",nombre,timepo.dias,timepo.horas,timepo.minutos,timepo.segundos);
}

char *rs_file(char path[],char info[]){
	FILE *fptr;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	if ((fptr = fopen(path,"r")) == NULL){
       perror("Error abriendo archivo.");
       return line;
	}
	while ((read = getline(&line, &len, fptr)) != -1) {
	        if( strstr(line,info) != NULL) break;
	    }
	fclose(fptr);
	return line;
}

int main(int argc,char *argv[]){

	set_cpuInfo();
	FILE * file =popen("top -b -n1 | grep -i \"Cpu(s)\"" ,"r");
	char consumo[100];
	fgets(consumo,100,file);
	pclose(file);
	set_infoMem();
	set_upTime();
	char *btime=rs_file("/proc/stat","btime");
	time_t bootTime=atol(btime+5);
	printf("%s",s1);
	printf("%s Procesador: %s x %d %s",res_start,cpu.model,cpu.cantCPU,res_end);
	printf("%s %s %s",res_start,consumo,res_end);
	printf("%s Total: %s Free: %s %s",res_start,cpu.totalMem,cpu.freeMem,res_end);
	printf("%s Uptime: %dD %dh %dm %ds %s",res_start,uptime.dias,uptime.horas,uptime.minutos,uptime.segundos,res_end);
	printf("%s %s %s",res_start,ctime (&bootTime),res_end);
	printf("%s",s2);
	return 0;
}
