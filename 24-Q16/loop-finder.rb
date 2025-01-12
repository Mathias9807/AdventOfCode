#!/usr/bin/env ruby

# Loop finder script, script that looks for forks and analyzes if one of the choices is strictly better than the other

inputFile = File.read('input-2.txt')

map = inputFile.lines().map{|line| line.chars}

puts map.map{|row| row.join()}

def numWalls(map, x, y)
  walls = 0
  if map[y][x - 1] == '#' then walls += 1 end
  if map[y][x + 1] == '#' then walls += 1 end
  if map[y - 1][x] == '#' then walls += 1 end
  if map[y + 1][x] == '#' then walls += 1 end
  return walls
end

class Vec
  attr_accessor :x, :y

  def initialize(x, y)
    @x, @y = x, y
  end

  # Overloading the `+` operator
  def +(other)
    Vec.new(@x + other.x, @y + other.y)
  end

  def rotate(n=1)
    v = Vec.new(@x, @y)
    for i in 1..n
      v.x, v.y = -v.y, v.x
    end
    return v
  end

  # Overloading `to_s` to print Point
  def to_s
    "(#{@x}, #{@y})"
  end
end

def dirToVec(dir)
  return Vec.new(1, 0).rotate(dir)
end

def dirToChar(dir)
  puts dir
  dir = dir % 4
  if dir == 0 then return '>' end
  if dir == 1 then return 'v' end
  if dir == 2 then return '<' end
  if dir == 3 then return '^' end
  return '?'
end

def mapGet(map, v)
  return map[v.y][v.x]
end

s = Vec.new(0, 0)
for y in 0..map.size-1
  for x in 0..map[0].size-1
    if map[y][x] == 'S'
      s = Vec.new(x, y)
    end
  end
end

# We start at S to make sure the direction is the same as when the real race goes
# Run a mouse until it hits a fork
m = Vec.new(s.x, s.y)
dir = 0

# We want to take a certain path before we stop to analyze since there are some real forks we can't eliminate
canonical_path = [1, 0, 0]
choice = -69
while true

  # Check forks
  if numWalls(map, m.x, m.y) < 2 or mapGet(map, m) == '#'
    # We've hit a fork, consult the script for how we should move
    if canonical_path.size == 0
      raise :panic
      exit
    end

    choice = canonical_path.shift
  end

  dDir = -1
  # puts "mouse: #{m}, dirToVec: #{dirToVec((dir + dDir + 4) % 4)}"
  # puts mapGet(map, m + dirToVec((dir + dDir + 4) % 4))
  choicesSeen = 0
  dDirOpts = []
  for dDirOpt in -1..1
    if mapGet(map, m + dirToVec((dir + dDirOpt + 4) % 4)) == '.'
      dDirOpts.push(dDirOpt)
    end
  end
  dDir = dDirOpts[0]
  if dDirOpts.size > 1 then dDir = dDirOpts[choice] end
  if dDir == 2 then puts "va" end
  dir = (dir + dDir + 4) % 4
  step = dirToVec(dir)

  m += step
  map[m.y][m.x] = dirToChar(dir)

  puts map.map{|row| row.join()}

  sleep(0.05)
end

# File.write('input-3.txt', map.map{|row| row.join()}.join)
