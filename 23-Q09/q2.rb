#!/usr/bin/env ruby

sum = 0

def prevNum(nums)
  # Find the next number according to the current pattern, if we can't see it immediately, take the derivative and recurse
  if nums.all?{|n| n == 0}
    return 0
  else
    diff = nums.each_cons(2).map{|n, m| m - n}
    delta = prevNum(diff)
    return nums.first - delta
  end
end

ARGF.each do |line|
  puts line

  numbers = line.split(' ').map{|n| n.to_i}
  puts "numbers: #{numbers}"
  puts "next number is: #{prevNum(numbers)}"
  sum += prevNum(numbers)
end

puts "Answer: #{sum}"

