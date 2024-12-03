#!/usr/bin/env ruby

require 'json'

f = File.open('pussle-input.csv')

def sign(x)
  return "±+-"[x <=> 0]
end

def testLevels(levels)
  i = 1
  while i < levels.size
    diff = levels[i] - levels[i - 1]
    if sign(levels[1] - levels[0]) != sign(diff)
      return false
    end

    if diff.abs == 0 or diff.abs > 3
      return false
    end

    i += 1
  end

  return true
end

safe = 0
f.readlines.each do |line|
  levels = line.split(' ').map{|x| x.to_i}
  savedLevels = levels.map{|x| x}
  if levels.size == 0 then break end

  if testLevels(levels)
    safe += 1
    next
  end

  # one level might be wrong, try removing each element in a row
  for i in 0..savedLevels.size-1
    levels = savedLevels.map{|x| x}
    levels.delete_at i
    if testLevels(levels)
      safe += 1
      break
    end
  end
end

p safe
