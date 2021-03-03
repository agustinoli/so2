#!/usr/bin/perl -w

use strict;
use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;
use POSIX qw(strftime);

my $query = new CGI;

print <<'START_HTML';
<!DOCTYPE html>
<html>
<title>Trabajo Práctico N°3</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="w3.css">
<link rel="stylesheet" href="font.css">
<link rel="stylesheet" href="font-awesome.min.css">
<style>
body, h1,h2,h3,h4,h5,h6 {font-family: "Montserrat", sans-serif}
.w3-row-padding img {margin-bottom: 12px}
/* Set the width of the sidebar to 120px */
.w3-sidebar {width: 120px;background: #222;}
/* Add a left margin to the "page content" that matches the width of the sidebar (120px) */
#main {margin-left: 120px}
/* Remove margins from "page content" on small screens */
@media only screen and (max-width: 600px) {#main {margin-left: 0}}
</style>
<body class="w3-black">

<!-- Icon Bar (Sidebar - hidden on small screens) -->
<nav class="w3-sidebar w3-bar-block w3-small w3-hide-small w3-center">
  <a href="index.html" class="w3-bar-item w3-button w3-padding-large w3-black">
    <p>Home</p>
  </a>
  <a href="recursos.cgi" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>Recursos</p>
  </a>
  <a href="goes16.pl" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>GOES16</p>
  </a>
  <a href="modulos.cgi" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>Módulos</p>
  </a>
</nav>

<div class="w3-padding-large" id="main">
    
<!-- Page Content -->
  <!-- Header/Home -->
  <header class="w3-container w3-padding-32 w3-center w3-black" id="home">
    <h1 class="w3-jumbo">Trabajo Práctico N°3</h1>
  </header>    


  <!--Formulario -->
  <div class="w3-padding-64 w3-content w3-text-grey" id="contact">
    <h2 class="w3-text-light-grey">GOES16</h2>   
    <p>Formulario para consulta de archivos disponibles</p>
    <form action="/goes16.pl" method="post" enctype="multipart/form-data">
      <p><input class="w3-input w3-cell" type="text" style="width:35%" placeholder="Año" required name="year">
          <input class="w3-input w3-cell" type="text" style="width:35%" placeholder="DOI" required name="doi">    
        <button class="w3-button w3-light-grey" style="width:20%" type="submit">
          BUSCAR
        </button></p>
    </form>
  <!-- End Formulario -->      
    </div>
   <div class="w3-center w3-animate-top">
        <ul class="w3-ul w3-white w3-center w3-opacity w3-hover-opacity-off">
START_HTML

my $year = $query->param("year");
my $doi = $query->param("doi");
 
if ($year eq ""){
	$year =  strftime "%Y",localtime;
}
if ($doi eq ""){
	$doi =  strftime "%j",localtime;
}else{
 	$doi = sprintf("%03d",$doi);#si el usuario no lo hizo relleno con ceros
}

print ("<li class=\"w3-dark-grey w3-xlarge w3-padding-32 \">Archivos para $year-$doi </li>\n");

my $results = `sudo rclone --config=.rclone.config ls  goes:noaa-goes16/ABI-L2-CMIPF/$year/$doi`;
my @lines = split /\n/ ,$results;

#http://noaa-goes16.s3.amazonaws.com//ABI-L2-CMIPF/$year/$doi/<Hour>/$filename

my $start_line = "<li class=\"w3-padding-16\">";
my $end_line = "</li> \n";


foreach my $line (@lines){
	if( index($line, "C13") ne -1) # C13 me da los resultados para canal/banda 13
	{
		my $filename = (split '/',$line) [-1]; 
        #el filename me indica la hora, para poder acceder al link
        my $hour = substr($filename,34,2); #hay 35 letras hasta que empieza la hora del escaneo
        my $link = "<a href=\"http://noaa-goes16.s3.amazonaws.com/ABI-L2-CMIPF/$year/$doi/$hour/$filename\" target=\"_blank\" >";
		print( $start_line,$link,$filename,"</a>",$end_line);	
	}
}

print <<END_HTML;
    </ul>
      </div>
</div>    
    
</body>
</html>
END_HTML
