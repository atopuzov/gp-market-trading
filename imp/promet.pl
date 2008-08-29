#!/usr/bin/perl

use DBI;

main:
{
	if ($#ARGV+1<1) {
		print "FILE.TXT\n";
		die();
	}
	if (!(-e $ARGV[0])) {
		die("Ne postoji file!");
	}
	
	$dbargs = {AutoCommit => 0, PrintError => 1};
	$dbh = DBI->connect("dbi:SQLite:dbname=se.db","","",$dbargs);
	
	$data = $ARGV[0];
	open(data) or die("Ne mogu otvoriti file!");
	foreach $line (<data>) {
		chomp($line);              # remove the newline from $line.
		# Datum,Redovni,Prijavljeni,Institucionalni,Blokirani,Ukupni
		($datum, $redovni, $prijavljeni, $institucionalni, $blokirani, $ukupni) = split('\s+',$line);
		$redovni =~ tr/\.//d; $redovni =~ s/,/\./g;

		($dan, $mjesec, $godina) = split('\.',$datum);
		$datum = sprintf("%04d-%02d-%02d", $godina, $mjesec, $dan);
	
		$sqlinsert = "INSERT INTO ZSE(DIONICA,DATUM,PROMET) VALUES ('ZSE', '$datum', $redovni);";
		#print $sqlinsert , "\n";
		$dbh->do($sqlinsert);
	}
	$dbh->commit();
	$dbh->disconnect();
}
