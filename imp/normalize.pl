#!/usr/bin/perl
# Normalizira bazu podataka sa informacijama o cjenama
# Za sve datume kada je ZSE radio a trazenom dioncom se nije trgovalo
# koristi zadju vrjednost s kojom se trgovalo
use DBI;

main:
{
	if ($#ARGV+1<1) {
		print "DIONICA\n";
		die();
	}
	
	$dbargs = {AutoCommit => 0, PrintError => 1};
	$dbh = DBI->connect("dbi:SQLite:dbname=se.db","","",$dbargs);
	
	$dionica = $ARGV[0];
	$dionica =~ tr/a-z/A-Z/;	# Sve velika slova
	# SQL Upit
	$sql = "SELECT DATUM FROM ZSE WHERE DIONICA='ZSE' \
	AND DATUM NOT IN (SELECT DATUM FROM ZSE WHERE DIONICA='$dionica') \
	AND DATUM > (select min(date(datum)) from zse where dionica='$dionica');";
	
	$sth = $dbh->prepare( $sql ) or die "Couldn't prepare statement: " . $dbh->errstr;
	$sth->execute();
	while (@data = $sth->fetchrow_array()) {
		my $datum = $data[0];
		$sql2 = "SELECT DATUM,DIONICA,ZADNJA FROM ZSE WHERE DIONICA='$dionica' AND DATUM < '$datum' ORDER BY DATUM DESC LIMIT 1;";
		$sth2 = $dbh->prepare( $sql2 ) or die "Couldn't prepare statement: " . $dbh->errstr;
		$sth2->execute();
		@data2 = $sth2->fetchrow_array();
		$datum2 = $data2[0]; $dionica2 = $data2[1]; $zadnja = $data2[2];
		$prva = $zadnja, $najvisa = $zadnja; $najniza = $zadnja; $prosjecna = $zadnja; $promjena = 0; $kolicina = 0; $promet = 0;
		$sqlinsert = "INSERT INTO ZSE(DIONICA,DATUM,PRVA,NAJVISA,NAJNIZA,ZADNJA,PROSJECNA,PROMJENA,KOLICINA,PROMET) VALUES ('$dionica', '$datum', $prva, $najvisa, $najniza, $zadnja, $prosjecna, $promjena, $kolicina, $promet);";
		print $sqlinsert , "\n";
		#$dbh->do($sqlinsert);
		$sth2->finish(); undef $sth2; # Oslobodi resurse
	}
	$sth->finish(); undef $sth;			# Oslobodi resurse
	$dbh->commit();
	$dbh->disconnect();
}
