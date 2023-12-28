#!/usr/bin/env ruby

sum = 0

def nextNum(nums)
  # Find the next number according to the current pattern, if we can't see it immediately, take the derivative and recurse
  if nums.all?{|n| n == 0}
    return 0
  else
    diff = nums.each_cons(2).map{|n, m| m - n}
    delta = nextNum(diff)
    return nums.last + delta
  end
end

ARGF.each do |line|
  puts line

  numbers = line.split(' ').map{|n| n.to_i}
  puts "numbers: #{numbers}"
  puts "next number is: #{nextNum(numbers)}"
  sum += nextNum(numbers)
end

puts "Answer: #{sum}"

