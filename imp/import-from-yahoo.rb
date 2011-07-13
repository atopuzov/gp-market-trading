#!/usr/bin/ruby
# (c) 2008-2011 Aleksandar Topuzović <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>
# Downlads data from http://finance.yahoo.com and imports it to a SQLite3 database file

require 'rubygems'
require 'sqlite3'   # Sqlite support
require 'open-uri'  # support for open http://
require 'optparse'  # command line options

options = {}
optparse = OptionParser.new do |opts|
  opts.banner = "Usage: import-yahoo.rb [options]"
  
  options[:ticker] = "AAPL"
    opts.on( '-t', '--ticker TICKER', 'Ticker identifier, default: AAPL' ) do|o|
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

# --------------------------------------------------------------------------------

# Downloads from Yahoo
def download(options, db)
  puts "Downloading: " + options[:ticker]

  sd = /(\d{1,2})\.(\d{1,2})\.(\d{2,4})\.?/.match(options[:datestart])
  ed = /(\d{1,2})\.(\d{1,2})\.(\d{2,4})\.?/.match(options[:dateend])
  url = "http://ichart.finance.yahoo.com/table.csv?s=" + options[:ticker] + \
        "&a=" + (sd[2].to_i-1).to_s + "&b=" + sd[1] + "&c=" + sd[3] + \
        "&d=" + (ed[2].to_i-1).to_s + "&e=" + ed[1] + "&f=" + ed[3] + \
        "&g=d&ignore=.csv"

 f = open(url)                                         # Download data

  rows=[]
  f.each_line("\n") {|line| rows.push line}
  rows.shift
  
  # Sql statement
  sql = "insert or replace into zse( DIONICA,  DATUM,  PRVA,  ZADNJA,  NAJVISA,  NAJNIZA,  PROSJECNA,  PROMJENA,  KOLICINA,  PROMET) 
                             VALUES(:dionica, :datum, :prva, :zadnja, :najvisa, :najniza, :prosjecna, :promjena, :kolicina, :promet);"

  db.transaction  # Start a database transaction

  rows.collect do |row|
    c = row.split(",")
    stmt = db.prepare( sql )
    stmt.bind_params( "dionica"   => options[:ticker],
                      "datum"     => c[0],
                      "prva"      => c[1],
                      "zadnja"    => c[4],
                      "najvisa"   => c[2],
                      "najniza"   => c[3],
                      "prosjecna" => 0,
                      "promjena"  => 0,
                      "kolicina"  => c[5],
                      "promet"    => 0 )
    stmt.execute  # Insert into database
  end
  db.commit       # End database transaction
end

# --------------------------------------------------------------------------------

db = SQLite3::Database.open( options[:database] )     # Open database

# Check if the required tables exist
sql = "select count(*) from sqlite_master where type = 'table' and name = 'ZSE';"
if db.get_first_value(sql) == 0 then
  puts "Creating tables in " + options[:database]
  f = open('create_zse.sql')
  sql = f.read
  db.execute( sql )
end

download(options, db)
