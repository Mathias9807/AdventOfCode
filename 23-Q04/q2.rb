#!/usr/bin/env ruby

sum = 0

counters = []

ARGF.each do |line|
  puts line
  puts "counters: #{counters}"

  colonI = line.index(':')
  numbers = line[colonI + 1..].strip
  winningNumbers = numbers.split("|")[0].strip.split(" ")
  ourNumbers = numbers.split("|")[1].strip.split(" ")
  nWinning = ourNumbers & winningNumbers
  puts "nWinning: #{nWinning}"

  nCopies = counters.length
  puts "nCopies: #{nCopies}"
  counters.each_index do |i|
    counters[i] -= 1
  end
  counters += [nWinning.length] * (1 + nCopies)
  counters.delete_if { |n| n < 1 }

  puts "nCards: #{1 + nCopies}"
  sum += 1 + nCopies
end

puts "Answer: #{sum}"

