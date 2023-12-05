#!/usr/bin/env ruby

sum = 0

ARGF.each do |line|
  puts line

  colonI = line.index(':')
  numbers = line[colonI + 1..].strip
  winningNumbers = numbers.split("|")[0].strip.split(" ")
  ourNumbers = numbers.split("|")[1].strip.split(" ")
  nWinning = ourNumbers & winningNumbers
  sum += (2 ** (nWinning.length - 1)).floor
  puts "Found #{nWinning} increasing sum to #{sum}"
end

puts "Answer: #{sum}"

