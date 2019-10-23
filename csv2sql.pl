#!/usr/bin/perl
#USAGE : csv2sql.pl < dblp.tsv
# will generate {usr,rel,pub}.tsv
use strict;
use warnings;

my $line;
my @col;
my %usr;
my %pub;
my $usr_id = 1;
my $pub_id = 1;

open(USR, ">usr.tsv") || die;
open(PUB, ">pub.tsv") || die;
open(REL, ">rel.tsv") || die;

while(defined ($line=<>) ){
	chomp($line);
	@col = split("\t", $line);
	if( ! exists $usr{$col[3]} ){#si le user courrant n'est pas dans le hash
		$usr{$col[3]} = $usr_id++;#le pusher au hash avec un id
		print USR $usr{$col[3]}."\t".$col[3]."\n";# print it
	}
	if( ! exists $pub{$col[0]} ){#si la publi courrante n'est pas présente en hash (via son titre)
		$pub{$col[0]} = $pub_id++;##la pusher au hash avec un id
		print PUB $pub{$col[0]}."\t".$col[2]."\t".$col[1]."\n"; #la printer
	}

	print REL $usr{$col[3]}."\t".$pub{$col[0]}."\n";#printer la relation entre le user(id) et la publi(id)
}

close REL;
close PUB;
close USR;

