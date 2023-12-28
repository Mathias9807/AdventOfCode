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

# List of node tuples (vector, direction)
path = []
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
      path.push([c, dir])
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

def turnRight(dir)
  return {
    up: :right,
    down: :left,
    left: :up,
    right: :down,
  }[dir]
end

def turnLeft(dir)
  return {
    up: :left,
    down: :right,
    left: :down,
    right: :up,
  }[dir]
end

for y in 0..map.length()-1 do
  line = map[y]
  for x in 0..line.length()-1 do
    c = Vec.new(x, y)
    if path.all?{|m| m[0] != c}
      map[c.y][c.x] = '.'
    end
  end
end


# To check how many tiles are contained inside the path, change all non-path tiles to the right of the path into '#'
path.each do |node|
  c = node[0]
  dir = node[1]
  neigh = c + dirToCoords(turnRight(dir))
  # If neighbour isn't in path
  if path.all?{|m| m[0] != neigh}
    # Convert to hashtag
    map[neigh.y][neigh.x] = '#'
  end

  # Also check the forward diagonal
  neighForw = c + dirToCoords(dir) + dirToCoords(turnRight(dir))
  if path.all?{|m| m[0] != neighForw}
    map[neighForw.y][neighForw.x] = '#'
  end
end

for line in map
  puts line
end

# Then iteratively spread '#' into their own neighbours, bounded by the path, until no more can be found
for i in 0..10
  newHashes = 0
  for y in 0..map.length()-1 do
    line = map[y]
    if y % 20 == 0
      puts "y: #{y}"
    end

    for x in 0..line.length()-1 do
      for dir in [:up, :down, :left, :right] do
        c = Vec.new(x, y)
        looking = c + dirToCoords(dir)
        # If our coord isn't on the path and we're looking at a '#' and we're not a '#' ourselves
        if path.all?{|m| m[0] != c} && map[looking.y] && map[looking.y][looking.x] == '#' && map[c.y][c.x] != '#'
          map[c.y][c.x] = '#'
          newHashes += 1
        end
      end
    end
  end
  for line in map
    puts line
  end

  puts "spread #{i}, added #{newHashes} hashes"
  if newHashes == 0
    puts "All hashes placed, continuing"
    break
  end
end

# Count hashtags in map
hashtags = 0
for line in map
  hashtags += line.count('#')
end

puts "Answer: #{hashtags}"

