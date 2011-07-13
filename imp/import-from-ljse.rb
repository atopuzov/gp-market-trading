#!/usr/bin/ruby
# (c) 2008-2011 Aleksandar Topuzović <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>
# Downlads data from www.ljse.hr and imports it to a SQLite3 database file

require 'rubygems'
require 'nokogiri'  # parsing HTML table
require 'sqlite3'   # Sqlite support
require 'open-uri'  # support for open http://
require 'optparse'  # command line options

options = {}
optparse = OptionParser.new do |opts|
  opts.banner = "Usage: import-ljse.rb [options]"
  
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

# "http://www.ljse.si/cgi-bin/jve.cgi?doc=2137&date1=01.01.2001&date2=31.12.2001&SecurityId=GRVG&csv=1"
# http://www.ljse.si/cgi-bin/jve.cgi?doc=2137&csv=1&date1=01.01.2001&date2=31.12.2001&SecurityId=GRVG

date1="01.01.2000"
date2="31.01.2000"
ticker="GRVG"
url = "http://www.ljse.si/cgi-bin/jve.cgi?doc=2137&csv=1&date1=" + date1 + "&date2" + date + "&SecurityId=" + ticker
