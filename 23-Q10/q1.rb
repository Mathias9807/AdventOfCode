#!/usr/bin/env ruby

map = []
ARGF.each do |line|
  puts line
  map.push(line)
end

class Vec
  attr_accessor :x, :y

  def initialize(x, y)
    @x = x
    @y = y
  end

  def +(other)
    Vec.new(@x + other.x, @y + other.y)
  end

  def -(other)
    Vec.new(@x - other.x, @y - other.y)
  end

  def ==(other)
    @x == other.x && @y == other.y
  end

  def to_s
    "(#{@x}, #{@y})"
  end
end

# Find S
map.each_with_index do |line, y|
  if (x = line.index('S'))
    puts "Found S at #{x},#{y}"
    S = Vec.new(x, y)
    break
  end
end

# When moving in a direction there are only some pipes one can enter, build a map of enterable pipes for each possible direction
okPipes = {
  up: ['|', '7', 'F', 'S'],
  down: ['|', 'L', 'J', 'S'],
  left: ['-', 'L', 'F', 'S'],
  right: ['-', '7', 'J', 'S']
}

def dirToCoords(dir)
  return {
    up: Vec.new(0, -1),
    down: Vec.new(0, 1),
    left: Vec.new(-1, 0),
    right: Vec.new(1, 0)
  }[dir]
end

def opposite(dir)
  return {
    up: :down,
    down: :up,
    left: :right,
    right: :left,
  }[dir]
end

c = Vec.new(S.x, S.y)
p = Vec.new(c.x, c.y)
n = 0
loop do
  # Take one step
  for dir in [:up, :down, :left, :right] do
    v = dirToCoords(dir)
    # Don't go back to the previous square
    if c + v == p
      next
    end

    nextPipe = map[c.y + v.y][c.x + v.x]
    # Check that we can enter the pipe in this direction and that we can leave our current pipe in this direction
    if okPipes[dir].include?(nextPipe) && okPipes[opposite(dir)].include?(map[c.y][c.x])
      puts "Found pipe #{nextPipe} in direction #{dir}"
      p = c
      c = c + v
      n += 1
      break
    end
  end

  if S == c
    break
  end
end

puts "Number of steps: #{n}"
puts "Answer: #{(n / 2).floor}"

