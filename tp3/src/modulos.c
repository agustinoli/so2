#include <stdio.h>

char s1[]= "<!DOCTYPE html>\n<html>\n<title>Trabajo Práctico N°3</title>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<link rel=\"stylesheet\" href=\"w3.css\">\n<link rel=\"stylesheet\" href=\"font.css\">\n<link rel=\"stylesheet\" href=\"font-awesome.min.css\">\n<style>\nbody, h1,h2,h3,h4,h5,h6 {font-family: \"Montserrat\", sans-serif}\n.w3-row-padding img {margin-bottom: 12px}\n/* Set the width of the sidebar to 120px */\n.w3-sidebar {width: 120px;background: #222;}\n/* Add a left margin to the \"page content\" that matches the width of the sidebar (120px) */\n#main {margin-left: 120px}\n/* Remove margins from \"page content\" on small screens */\n@media only screen and (max-width: 600px) {#main {margin-left: 0}}\n</style>\n<body class=\"w3-black\">\n\n\n<!-- Icon Bar (Sidebar - hidden on small screens) -->\n<nav class=\"w3-sidebar w3-bar-block w3-small w3-hide-small w3-center\">\n  <a href=\"index.html\" class=\"w3-bar-item w3-button w3-padding-large w3-black\">\n    <p>Home</p>\n  </a>\n  <a href=\"recursos.cgi\" class=\"w3-bar-item w3-button w3-padding-large w3-hover-black\">\n    <p>Recursos</p>\n  </a>\n  <a href=\"goes16.cgi\" class=\"w3-bar-item w3-button w3-padding-large w3-hover-black\">\n    <p>GOES16</p>\n  </a>\n  <a href=\"modulos.cgi\" class=\"w3-bar-item w3-button w3-padding-large w3-hover-black\">\n    <p>Módulos</p>\n  </a>\n</nav>\n\n<div class=\"w3-padding-large\" id=\"main\">\n    \n  <header class=\"w3-container w3-padding-32 w3-center w3-black\" id=\"home\">\n    <h1 class=\"w3-jumbo\">Trabajo Práctico N°3</h1>\n  </header>\n\n  \n      \n  <div class=\"w3-padding-16 w3-content w3-text-grey\" id=\"refresh\">    \n        <p> <form action=\"upload.pl\" method=\"post\" enctype=\"multipart/form-data\">\n\tSeleccione un archivo para subir:\n\t<input type=\"file\" name=\"photo\" id=\"photo\">\n\t<input type=\"submit\" value=\"Upload Image\" name=\"submit\">\n</form> </p>  </div>   \n\n      <div class=\"w3-half\">\n        <ul class=\"w3-ul w3-white w3-center w3-opacity w3-hover-opacity-off\">\n          <li class=\"w3-dark-grey w3-xlarge w3-padding-32\">Módulos</li>\n          ";
char mod_start[]="<li class=\"w3-padding-16\">";
char mod_end[]=" </li>\n";
char s2[]="\n        </ul>\n      </div>\n    </div>    \n  </div> \n\n</body>\n</html>\n";

int main(int argc,char *argv[]){

	FILE * file =popen("lsmod" ,"r");

	char lista[100];
	char str[30];
	printf("%s",s1);
	fgets(lista,100,file);
	while( fgets(lista,100,file) != NULL){
		sscanf(lista,"%s",str);
		printf("%s %s %s",mod_start,str,mod_end);
	}
	printf("%s",s2);
	pclose(file);	
	return 0;
}
