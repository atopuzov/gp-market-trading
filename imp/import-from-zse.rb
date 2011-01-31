#!/usr/bin/ruby
# (c) 2008-2011 Aleksandar Topuzović <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>
# Downlads data from www.zse.hr and imports it to a SQLite3 database file

require 'rubygems'
require 'nokogiri'  # parsing HTML table
require 'sqlite3'   # Sqlite support
require 'open-uri'  # support for open http://
require 'optparse'  # command line options

options = {}
optparse = OptionParser.new do |opts|
  opts.banner = "Usage: import-zse.rb [options]"
  
  options[:ticker] = "PBZ-R-A"
    opts.on( '-t', '--ticker TICKER', 'Ticker identifier, default: PBZ-R-A (use ZSE for trading info)' ) do|o|
        options[:ticker] = o
    end
    options[:database] = "se.db"
    opts.on( '-d', '--database DATABASE', 'Database file, default: se.db' ) do|o|
        options[:database] = o
    end
    options[:datestart] = "01.01.1990"
    opts.on( '-s', '--date_start DD.MM.YYYY', 'Start date, default: ' + options[:datestart] ) do|o|
        options[:datestart] = o
    end
    options[:dateend] = Time.now.strftime("%d.%m.%Y")
    opts.on( '-e', '--date_end DD.MM.YYYY', 'End date, default (current): ' + options[:dateend] ) do|o|
      options[:dateend] = o
    end
    opts.on( '-h', '--help', 'Display this screen' ) do
      puts opts
      exit
    end
end

optparse.parse!

if options[:ticker] == "ZSE" then                              # build the URL
  url = "http://zse.hr/export.aspx?Duzina=20950&reporttype=promet_povijest&naslov="
else
  url = "http://www.zse.hr/export.aspx?ticker=" + options[:ticker] + "&reporttype=security&DateTo=" + options[:dateend] + "&DateFrom=" + options[:datestart] + "&range="
end

db = SQLite3::Database.open( options[:database] )     # Open database

# Check if the required tables exist
sql = "select count(*) from sqlite_master where type = 'table' and name = 'ZSE';"
table_count = db.execute( sql )
if table_count[0][0] == 0 then
  f = open('create_zse.sql')
  sql = f.read
  db.execute( sql )
end

f = open(url)                                         # Download data
doc = Nokogiri::HTML(f)                               # Parse HTML data

rows = doc.xpath('//tr')
if rows.count <= 1 then
  puts "Nothing returned."
  exit
end
rows.shift  # Remove first element

# Sql statement
sql = "insert or replace into zse( DIONICA,  DATUM,  PRVA,  ZADNJA,  NAJVISA,  NAJNIZA,  PROSJECNA,  PROMJENA,  KOLICINA,  PROMET) 
                           VALUES(:dionica, :datum, :prva, :zadnja, :najvisa, :najniza, :prosjecna, :promjena, :kolicina, :promet);"

db.transaction  # Start a database transaction

rows.collect do |row|
  if row.at_xpath('td[2]').inner_text == 'B' then next end    # Trgovanje blokirano

  datum = row.at_xpath('td[1]').inner_text
  if options[:ticker] != "ZSE" then 
    prva      = row.at_xpath('td[3]' ).inner_text.gsub(".","").gsub(",",".")
    zadnja    = row.at_xpath('td[4]' ).inner_text.gsub(".","").gsub(",",".")
    najvisa   = row.at_xpath('td[5]' ).inner_text.gsub(".","").gsub(",",".")
    najniza   = row.at_xpath('td[6]' ).inner_text.gsub(".","").gsub(",",".")
    prosjecna = row.at_xpath('td[7]' ).inner_text.gsub(".","").gsub(",",".")
    promjena  = row.at_xpath('td[8]' ).inner_text.gsub(".","").gsub(",",".").gsub(" ","")
    kolicina  = row.at_xpath('td[10]').inner_text.gsub(".","").gsub(",",".")
    promet    = row.at_xpath('td[11]').inner_text.gsub(".","").gsub(",",".")
  else
    prva      = zadnja = najvisa = najniza = prosjecna = promjena = kolicina = nil
    promet    = row.at_xpath('td[5]').inner_text.gsub(".","").gsub(",",".")
  end
  
  stmt = db.prepare( sql )
  stmt.bind_params( "dionica"   => options[:ticker],
                    "datum"     => datum,
                    "prva"      => prva,
                    "zadnja"    => zadnja,
                    "najvisa"   => najvisa,
                    "najniza"   => najniza,
                    "prosjecna" => prosjecna,
                    "promjena"  => promjena,
                    "kolicina"  => kolicina,
                    "promet"    => promet )
  stmt.execute  # Insert into database
end

db.commit       # End database transaction
