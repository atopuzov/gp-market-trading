#!/usr/bin/ruby
# (c) 2008-2011 Aleksandar Topuzović <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>
# Creates data files to draw graphs with gnuplot

require 'rubygems'
require 'rexml/document'

file = File.new("beagle.log")
fitness_ts_dat = File.new("stats_fitness_ts.dat", "w")
fitness_vs_dat = File.new("stats_fitness_vs.dat", "w")
fitness_dat    = File.new("stats_fitness.dat", "w")
treesize_dat   = File.new("stats_treesize.dat", "w")
treedepth_dat  = File.new("stats_treedepth.dat", "w")
document = REXML::Document.new file

document.elements.each("/Beagle/Logger/Log/Stats") { |stats_element|
  if stats_element.attributes["id"] == "vivarium"
    generation = stats_element.attributes["generation"].to_i
    avg_ts = avg_vs = std_ts = std_vs = max_ts = max_vs = min_ts = min_vs = 0
    ts_avg = td_avg = ts_std = td_std = ts_max = td_max = ts_min = td_min = 0
    # Loop trough each measure element
    stats_element.elements.each("Measure") { |measure_element|
        # Fitness on the training set statistics
        if measure_element.attributes["id"] == "fitness"
          avg_ts = measure_element.elements["Avg"].text.to_f
          std_ts = measure_element.elements["Std"].text.to_f
          max_ts = measure_element.elements["Max"].text.to_f
          min_ts = measure_element.elements["Min"].text.to_f
        end
        # Fitness on the validation set statistics
        if measure_element.attributes["id"] == "fitness2"
          avg_vs = measure_element.elements["Avg"].text.to_f
          std_vs = measure_element.elements["Std"].text.to_f
          max_vs = measure_element.elements["Max"].text.to_f
          min_vs = measure_element.elements["Min"].text.to_f
        end
        # Treesize statistics
        if measure_element.attributes["id"] == "treesize"
          ts_avg = measure_element.elements["Avg"].text.to_f
          ts_std = measure_element.elements["Std"].text.to_f
          ts_max = measure_element.elements["Max"].text.to_f
          ts_min = measure_element.elements["Min"].text.to_f
        end
        # Treedepth statistics
        if measure_element.attributes["id"] == "treedepth"
          td_avg = measure_element.elements["Avg"].text.to_f
          td_std = measure_element.elements["Std"].text.to_f
          td_max = measure_element.elements["Max"].text.to_f
          td_min = measure_element.elements["Min"].text.to_f
        end
      }
      fitness_dat.puts    generation.to_s + "\t" + avg_ts.to_s + "\t" + avg_vs.to_s
      fitness_ts_dat.puts generation.to_s + "\t" + avg_ts.to_s + "\t" + std_ts.to_s + "\t" + max_ts.to_s + "\t" + min_ts.to_s
      fitness_vs_dat.puts generation.to_s + "\t" + avg_vs.to_s + "\t" + std_vs.to_s + "\t" + max_vs.to_s + "\t" + min_vs.to_s
      treesize_dat.puts   generation.to_s + "\t" + ts_avg.to_s + "\t" + ts_std.to_s + "\t" + ts_max.to_s + "\t" + ts_min.to_s
      treedepth_dat.puts  generation.to_s + "\t" + td_avg.to_s + "\t" + td_std.to_s + "\t" + td_max.to_s + "\t" + td_min.to_s
    end
}


