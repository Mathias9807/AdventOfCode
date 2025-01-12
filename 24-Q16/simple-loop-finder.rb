#!/usr/bin/env ruby

# Simpler loop finder script, look for any loops with only 1 exit. These are trivially useless

inputFile = File.read('input-4.txt')

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
  return Vec.new(1, 0).rotate((dir + 4) % 4)
end

def dirToChar(dir)
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

# Find a period to start checking
ps = []
for y in 0..map.size-1
  for x in 0..map[0].size-1
    if map[y][x] == '.'
      ps.push(Vec.new(x, y))
    end
  end
end

mapBkp = map.map(&:dup)
for p in ps
  # puts "start p: #{p}"
  # Trace it clockwise or anticlockwise and look for forks
  for clockwise in 0..1
    # puts "clockwise: #{clockwise}"
    map = mapBkp.map(&:dup)
    cur = Vec.new(p.x, p.y)
    dir = 0
    forks = 0
    firstMove = true
    while true
      dirOpts = []
      for i in -1..1
        if mapGet(map, cur + dirToVec(dir + i)) != '#' then dirOpts.push((dir + i + 4) % 4) end
      end
      # puts "dirOpts: #{dirOpts}"
      if dirOpts.size == 0
        # puts "Dead end!"
        break
      end
      if firstMove == false and dirOpts.size >= 2
        # puts "Found a fork"
        forks += 1
        if forks > 1
          # puts "Too many forks"
          break
        end
      end

      dir = dirOpts[clockwise - 1]
      if firstMove then dir = dirOpts[0] end
      map[cur.y][cur.x] = dirToChar(dir)
      cur += dirToVec(dir)

      firstMove = false
      # puts "cur: #{cur}, p: #{p}"
      if cur.x == p.x and cur.y == p.y
        puts map.map{|row| row.join()}
        puts "Found a loop!!!"
        sleep(5)
      end
      
      # sleep(0.05)
    end
  end
end

# File.write('input-3.txt', map.map{|row| row.join()}.join)
