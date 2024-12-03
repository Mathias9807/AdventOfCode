#!/usr/bin/env ruby

require 'json'

f = File.open('./q1-input.txt')

l1 = []
l2 = []
f.readlines.each do |line|
  e1, e2 = line.split(' ')
  e1 = e1.to_i
  e2 = e2.to_i

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

sum = 0

p1 = 0
p2 = 0
while true
  val = [l1[p1], l2[p2]].min

  n1 = 0
  while l1[p1] == val
    n1 += 1
    p1 += 1
  end

  n2 = 0
  while l2[p2] == val
    n2 += 1
    p2 += 1
  end

  sum += val * n1 * n2

  if l1[p1] == nil or l2[p2] == nil
    break
  end
end

p sum
