#!/usr/bin/env ruby

$map = File.read("input.txt").split("\n")

p $map

w = $map[0].size
h = $map.size

Coord = Struct.new(:x, :y) do
  def +(c)
    return Coord.new(x + c.x, y + c.y)
  end

  def to_s()
    return "{#{x}, #{y}}"
  end
end

starts = []
$map.each_with_index do |line, y|
  $map[y].chars.each_with_index do |char, x|
    if char == '0'
      starts.push(Coord.new(x, y))
    end
  end
end

p starts

def reachesFrom(start)
  if start.x < 0 or start.y < 0 then return [] end

  n = $map[start.y][start.x].to_i
  if n == 9 then return [start] end

  reached = []
  [Coord.new(-1, 0), Coord.new(1, 0), Coord.new(0, -1), Coord.new(0, 1)].each do |c|
    if n + 1 == $map[start.y + c.y]&.[](start.x + c.x).to_i
      reached += reachesFrom(start + c)
    end
  end
  return reached
end

sum = 0
starts.each_with_index do |start, i|
  dests = reachesFrom(start)
  rating = dests.size
  score = dests.uniq.size
  sum += rating
  puts "\ttrailhead #{i} has a score of #{score} and a rating of #{rating}"
end
puts "Sum: #{sum}"
