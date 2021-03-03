#!/usr/bin/perl -w

use strict;
use warnings;
use CGI;
use utf8;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;

$CGI::POST_MAX = 1024 * 10000;
my $safe_filename_characters = "a-zA-Z0-9_.-";
my $upload_dir = "/var/www/upload";

my $query = new CGI;
my $filename = $query->param("photo");

if ( !$filename )
{
print $query->header ( );
print "There was a problem uploading (try a smaller file).";
exit;
}

my ( $name, $path, $extension ) = fileparse ( $filename, '..*' );
$filename = $name . $extension;
$filename =~ tr/ /_/;
$filename =~ s/[^$safe_filename_characters]//g;

if ( $filename =~ /^([$safe_filename_characters]+)$/ )
{
$filename = $1;
}
else
{
die "Filename contains invalid characters";
}

my $upload_filehandle = $query->upload("photo");

open ( UPLOADFILE, ">$upload_dir/$filename" ) or die "$!";
binmode UPLOADFILE;

while ( <$upload_filehandle> )
{
print UPLOADFILE;
}
close UPLOADFILE;

print $query->header ( );

system("sudo /sbin/insmod upload/$filename"); #intento instalar el modulo
my $module_name = substr $filename, 0, -3; #el archivo es .ko
my $is_present = `lsmod | grep $module_name`;
my ($hidden,$disabled,$disabled_string);

if ($is_present ne "") {
#Esta presente, se instalo correctamente.
	$hidden = "hidden";
	$disabled = " ";
	$disabled_string =" Se instalo correctamente."; 
}
else {
#No se instalo correctamente. Muestro info del modulo para ayudar.
	$hidden = " ";
	my $modinfo_str = `/sbin/modinfo /var/www/upload/$filename`;
	$disabled_string = "No se puede instalar el modulo. Información del módulo: $modinfo_str  \n ";
}

print <<END_HTML;
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
\@media only screen and (max-width: 600px) {#main {margin-left: 0}}
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
  <a href="goes16.cgi" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>GOES16</p>
  </a>
  <a href="modulos.cgi" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>Módulos</p>
  </a>
</nav>
<div class="w3-padding-large" id="main">    
  <header class="w3-container w3-padding-32 w3-center w3-black" id="home">
    <h1 class="w3-jumbo">Trabajo Práctico N°3</h1>
  </header>
  <div class="w3-content w3-justify w3-text-grey w3-padding-64" id="about">
      <div class="w3-padding-16 w3-center w3-content w3-text-grey">            
            <p class="w3-red" $hidden>$disabled_string</p>
        </div>     
      <div>
          <p>
          </p>
      </div>
    </div>    
  </div> 

</body>
</html>
END_HTML
