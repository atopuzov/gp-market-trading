#!/usr/bin/perl
use DBI;
@driver_names = DBI->available_drivers();
print "driver_names (apparently installed, available):\n";
foreach my $dn (@driver_names)
{
    print "$dn\n";
}
