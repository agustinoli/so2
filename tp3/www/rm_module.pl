#!/usr/bin/perl -w
 
use strict;
use warnings;
use CGI;
use utf8;
use CGI::Carp qw ( fatalsToBrowser );

my $query = new CGI;
my $mod_name = $query->param("mod_name");

my $rmmod_output =  `sudo /sbin/rmmod $mod_name`; 

print $query->redirect($ENV{'HTTP_REFERER'}) ;