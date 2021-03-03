#!/usr/bin/perl -T
 
use strict;
use warnings;
use CGI;
use CGI::Carp qw/fatalsToBrowser/;
use File::Basename;

$CGI::POST_MAX = 1024 * 5000; #adjust as needed (1024 * 5000 = 5MB)
$CGI::DISABLE_UPLOADS = 0; #1 disables uploads, 0 enables uploads
my $error_html = 0;

 
my $query = CGI->new;

 
unless ($CGI::VERSION >= 2.47) { 
   error('Your version of CGI.pm is too old. You must have verison 2.47 or higher to use this script.')
}
 
my $upload_dir = '../upload';
 
# a list of valid characters that can be in filenames
my $filename_characters = 'a-zA-Z0-9_.-';
 
my $file = $query->param("module") or $error_html = 2;
#my $email_address = $query->param("email") || 'Annonymous';
 
# get the filename and the file extension
# this could be used to filter out unwanted filetypes
# see the File::Basename documentation for details
my ($filename,undef,$ext) = fileparse($file,qr{\..*});
# append extension to filename
$filename .= $ext;

if ($filename =~ /.(ko)$/) { # Check for .ko extension

# convert spaces to underscores "_"
$filename =~ tr/ /_/;
    
 
my $upload_filehandle = $query->upload("module");
 


open (UPLOADFILE, ">$upload_dir/$filename") or error($!);
binmode UPLOADFILE;
while ( <$upload_filehandle> ) {
   print UPLOADFILE;
}
close UPLOADFILE;
system("sudo insmod ../upload/$filename\n");
my $url = "/cgi-bin/get_modules.cgi";
print "Location: $url\n\n";

# print $query->header(),
#       $query->start_html(-title=>'Upload Successful'),
#       $query->p('Thanks for uploading your module!'),
#       # $query->p("Your email address: $email_address"),
#       $query->p("Your module $filename:"),
#       # $query->img({src=>"../upload/$filename",alt=>''}),
#       $query->end_html;
}
else{
	$error_html = 1
	# print $html;
	# error("Extension no valida");
} 
my $html = "Content-Type: text/html

<HTML>
<head>
<meta http-equiv=\"refresh\" content=\"0; url=/cgi-bin/get_modules.cgi?m=$error_html\" />
</head>
</HTML>";

print $html;
