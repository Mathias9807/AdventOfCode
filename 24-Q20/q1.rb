#!/usr/bin/env ruby

map = File.read('input.txt').split("\n").map{|line| line.chars}
$w = map[0].size
$h = map.size

p map

$s_p = []
$e_p = []
map.each_with_index do |row, y|
  row.each_with_index do |char, x|
    if char == 'S' then $s_p = [x, y] end
    if char == 'E' then $e_p = [x, y] end
  end
end

p $s_p
p $e_p

$candidates = []
$tracked = []
Node = Struct.new(:p, :cost)

def vecAdd(a, b)
  return [a[0] + b[0], a[1] + b[1]]
end

def distance(p)
  return $w - p[0] + $h - p[1]
end

def add_candidates(map, node)
  [[1,0], [0,1], [-1,0], [0,-1]].each do |dir|
    coord = vecAdd(node.p, dir)
    if coord[0] < 0 or coord[1] < 0 or coord[0] >= $w or coord[1] >= $h
      next
    end

    if map[coord[1]][coord[0]] == '.' \
        or map[coord[1]][coord[0]] == 'E'

      $candidates.push(Node.new(coord, node.cost + 1))
      map[coord[1]][coord[0]] = 'o'

      if coord[0] == $e_p[0] and coord[1] == $e_p[1]
        # puts "Found goal at #{node.cost + 1} steps"
        return node.cost + 1
      end
    end
  end
  return false
end

S = Node.new($s_p, 0)
def pathfind(map, cheat=nil)
  $candidates = []
  $tracked = []

  map_copy = map.map(&:dup)
  if cheat
    map_copy[cheat[1]][cheat[0]] = '.'
  end

  $candidates.push(S)
  while true
    # puts 'sorting'
    $candidates.sort_by!{|c| c.cost + distance(c.p)}
    # puts "after sorting: #{$candidates}"

    chosen = $candidates[0]
    map_copy[chosen.p[1]][chosen.p[0]] = 'x'
    # puts "picked: #{chosen}"
    foundGoal = add_candidates(map_copy, chosen)
    if foundGoal then return foundGoal end

    $candidates.delete_at(0)
    $tracked.push(chosen)
    # puts "added candidates: #{$candidates}"

    # map_copy.each{|line| p line.join()}

    # sleep(0.1)
  end
end

baseline = pathfind(map)

saves = [0] * 10000
puts "# Cheats #{saves}"
n_cheats = 0
map.each_with_index do |row, y|
  row.each_with_index do |char, x|
    if map[y][x] != '#' then next end

    # Set this block to be a path and see if it saves performance
    cheated = pathfind(map, [x, y])
    if cheated <= baseline - 100
      puts "# Cheat #{x},#{y} costs #{cheated}"
      saves[baseline - cheated] = saves[baseline - cheated] + 1
      n_cheats += 1
    end
  end
end

puts "# Baseline = #{baseline}"
puts "# Cheats #{n_cheats}"
