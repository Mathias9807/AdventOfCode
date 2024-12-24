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

$candidates = []
$tracked = []
$goalNodes = []
Node = Struct.new(:p, :cost, :boost, :boostLeft, :parent) do
  def totalCost
    cost + (parent&.totalCost || 0)
  end
end

def vecAdd(a, b)
  return [a[0] + b[0], a[1] + b[1]]
end

def distance(p)
  return $w - p[0] + $h - p[1]
end


# Create a reverse map of the cost to reach the goal from every position on the map
$costMap = map.map{|row| row.map{|cell| 0}}
$costNodeMap = map.map{|row| row.map{|cell| nil}}
end_node = Node.new($e_p, 0, nil)
$costMap[$e_p[1]][$e_p[0]] = 0
$costNodeMap[$e_p[1]][$e_p[0]] = end_node
$costCandidates = [end_node]
def check_node(map, node)
  # For each direction from this node
  [[1,0], [0,1], [-1,0], [0,-1]].each do |dir|
    coord = vecAdd(node.p, dir)
    if coord[0] < 0 or coord[1] < 0 or coord[0] >= $w or coord[1] >= $h
      next
    end

    if map[coord[1]][coord[0]] == '#' then next end

    existingNode = $costNodeMap[coord[1]][coord[0]]
    if existingNode and existingNode.cost <= node.cost + 1
      next
    end

    newNode = Node.new(coord, node.cost + 1)
    $costNodeMap[coord[1]][coord[0]] = newNode
    $costMap[coord[1]][coord[0]] = newNode.cost
    $costCandidates.push(newNode)
  end
end

while $costCandidates.size > 0
  node = $costCandidates[0]
  $costCandidates.delete_at(0)
  check_node(map, node)
end


# For each possible cheat-start node (each '.' tile)
# Iterate all cheat-end nodes (all other '.' tiles in taxi cab distance from start)
# If (cheat-start normal cost - cheat-end normal cost) - taxi cab distance > 100: record

taxiCab = 20
cheats = 0
timeSaves = [0] * 10000

map.each_with_index do |row, y|
  row.each_with_index do |char, x|
    if map[y][x] != '.' && map[y][x] != 'S' then next end

    # Found a cheat start tile
    cheatStart = [x, y]
    for cx in -taxiCab..taxiCab
      for cy in -taxiCab..taxiCab
        cheatWalkCost = cx.abs + cy.abs
        if cheatWalkCost > taxiCab then next end
        coord = [cx + x, cy + y]
        if coord[0] < 0 or coord[1] < 0 or coord[0] >= $w or coord[1] >= $h
          next
        end
        if map[coord[1]][coord[0]] != '.' && map[coord[1]][coord[0]] != 'E' then next end
        if coord[0] == cheatStart[0] and coord[1] == cheatStart[1]
          next
        end

        # If this cheat doesn't even reduce cost, skip
        if $costMap[coord[1]][coord[0]] >= $costMap[cheatStart[1]][cheatStart[0]]
          next
        end

        prints = []
        baselineCost = $costMap[$s_p[1]][$s_p[0]]
        # prints.push "Baseline cost #{baselineCost}"
        cheatStartCost = $costMap[cheatStart[1]][cheatStart[0]]
        prints.push "Cheat start #{cheatStart}"
        # prints.push "Cheat start cost #{cheatStartCost}"
        cheatEndCost = $costMap[coord[1]][coord[0]]
        prints.push "Cheat end #{coord}"
        # prints.push "Cheat end cost #{cheatEndCost}"
        prints.push "Cheat walk cost #{cheatWalkCost}"

        # For each cheat end tile
        # If the cost we saved + the cheat distance < -100
        if (cheatStartCost - cheatEndCost) - cheatWalkCost >= 100
          puts prints.join("\n")
          puts "Distance saved: #{(cheatStartCost - cheatEndCost) - cheatWalkCost}"

          cheats += 1
          timeSaves[(cheatStartCost - cheatEndCost) - cheatWalkCost] += 1
        end
      end
    end
  end
end

timeSaves.each_with_index do |num, index|
  if num == 0 then next end
  puts "\tThere are #{num} cheats that save #{index} picoseconds"
end
puts "Num cheats: #{cheats}"
