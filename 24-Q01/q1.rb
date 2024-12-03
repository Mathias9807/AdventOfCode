#!/usr/bin/env ruby

require 'json'

f = File.open('./q1-input.txt')

l1 = []
l2 = []
f.readlines.each do |line|
  e1, e2 = line.split(' ')

  sortedIndex1 = 0
  l1.each do |x|
    if x > e1 then break end
    sortedIndex1 += 1
  end
  l1.insert(sortedIndex1, e1)

  sortedIndex2 = 0
  l2.each do |x|
    if x > e2 then break end
    sortedIndex2 += 1
  end
  l2.insert(sortedIndex2, e2)
end

p l1
p l2

distances = l1.map.with_index do |e1, i| 
  e2 = l2[i]

  tmp = (e1.to_i - e2.to_i).abs
end
p distances

p distances.sum
