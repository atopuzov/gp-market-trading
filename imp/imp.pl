#!/usr/bin/perl
# http://www.zse.hr/export.aspx?ticker=LEDO-R-A&reporttype=security&DateTo=29.8.2008&DateFrom=1.1.1991&range=
use DBI;

main:
{
	if ($#ARGV+1<2) {
		print "DIONICA FILE.TXT\n";
		print "ZABA-R-A ZABA-R-A.TXT\n";
		die();
	}
	if (!(-e $ARGV[1])) {
		die("Ne postoji file!");
	}
	$dionica = $ARGV[0];
	$dionica =~ tr/a-z/A-Z/;
	
	$dbargs = {AutoCommit => 0, PrintError => 1};
	$dbh = DBI->connect("dbi:SQLite:dbname=se.db","","",$dbargs);
	
	$data = $ARGV[1];
	open(data) or die("Ne mogu otvoriti file!");
	foreach $line (<data>) {
		chomp($line);              # remove the newline from $line.
		# Datum,Prva,Najvisa,Najniza,Zadnja,Prosjecna,Promjena%,Kolicina,Promet
		($datum, $prva, $najvisa, $najniza, $zadnja,
		$prosjecna, $promjena, $kolicina, $promet) = split('\s+',$line);
		$prva      =~ tr/\.//d; $prva      =~ s/,/\./g;
		$najvisa   =~ tr/\.//d; $najvisa   =~ s/,/\./g;
		$najniza   =~ tr/\.//d; $najniza   =~ s/,/\./g;
		$zadnja    =~ tr/\.//d; $zadnja    =~ s/,/\./g;
		$prosjecna =~ tr/\.//d; $prosjecna =~ s/,/\./g;
		$promjena  =~ tr/\.//d; $promjena  =~ s/,/\./g;
		$kolicina  =~ tr/\.//d; $kolicina  =~ s/,/\./g;
		$promet    =~ tr/\.//d; $promet  =~ s/,/\./g;
		
		#print "|", $datum, "|", $prva, "|" , $najvisa, "|", $najniza, "|", $zadnja, "|", $prosjecna, "|", $promjena, "|", $kolicina, "|", $promet, "\n";
		($dan, $mjesec, $godina) = split('\.',$datum);
		
		$datum = sprintf("%04d-%02d-%02d",$godina,$mjesec, $dan);
	
		if($zadnja eq "Blok") {
			print "Blokirano: ", $datum, " ", $prva, " " , $najvisa, " ", $najniza, " ", $zadnja, "\n";
		} else {
			$sqlinsert = "INSERT OR REPLACE INTO ZSE(DIONICA,DATUM,PRVA,NAJVISA,NAJNIZA,ZADNJA,PROSJECNA,PROMJENA,KOLICINA,PROMET) VALUES ('$dionica', '$datum', $prva, $najvisa, $najniza, $zadnja, $prosjecna, $promjena, $kolicina, $promet);";
			#print $sqlinsert , "\n";
			$dbh->do($sqlinsert);
		}
	}
	$dbh->commit();
	$dbh->disconnect();
}
