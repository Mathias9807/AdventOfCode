#!/usr/bin/env ruby

map = File.read('test-input.txt').split("\n").map{|line| line.chars}
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

def add_candidates(map, node)
  # Normal pathfinding without boost
  [[1,0], [0,1], [-1,0], [0,-1]].each do |dir|
    coord = vecAdd(node.p, dir)
    if coord[0] < 0 or coord[1] < 0 or coord[0] >= $w or coord[1] >= $h
      next
    end

    if map[coord[1]][coord[0]] == '#' then next end

    prevNode = $nodeMap[coord[1]][coord[0]]
    if prevNode == nil \
        or map[coord[1]][coord[0]] == 'E'

      boostState = node.boost
      # If we were boosting then now we'll have cancelled the boost
      if boostState == :using then boostState = :used end
      newNode = Node.new(coord, 1, boostState, node.boostLeft, node)
      $candidates.push(newNode)
      if $renderMap[coord[1]][coord[0]] != 'E'
        $renderMap[coord[1]][coord[0]] = 'o'
      end
      $nodeMap[coord[1]][coord[0]] = newNode

      if coord[0] == $e_p[0] and coord[1] == $e_p[1]
        # puts "Found goal at #{node.cost + 1} steps"
        $goalNodes.push(newNode)
      end
    end
  end

  # Pathfinding with boost
  if node.boost == :used then return end
  [[1,0], [0,1], [-1,0], [0,-1]].each do |dir|
    coord = vecAdd(node.p, dir)
    if coord[0] < 0 or coord[1] < 0 or coord[0] >= $w or coord[1] >= $h
      next
    end

    if node.boostLeft == 1 and map[coord[1]][coord[0]] == '#'
      next
    end

    boostState = :using
    boostLeft = node.boostLeft - 1
    if boostLeft == 0 then boostState = :used end

    newNode = Node.new(coord, 1, boostState, boostLeft, node)
    $candidates.push(newNode)
    if $renderMap[coord[1]][coord[0]] != 'E'
      $renderMap[coord[1]][coord[0]] = 'c'
    end

    if coord[0] == $e_p[0] and coord[1] == $e_p[1]
      # puts "Found goal at #{node.cost + 1} steps"
      $goalNodes.push(newNode)
    end
  end
end

def pathfind(map, start)
  $candidates = []
  $tracked = []
  $goalNodes = []
  $nodeMap = map.map{|row| row.map{|c| nil}}

  map_copy = map.map(&:dup)
  $renderMap = map.map(&:dup)

  $candidates.push(start)
  while $candidates.size > 0
    # puts 'sorting'
    $candidates.sort_by!{|c| c.totalCost + distance(c.p)}
    # puts "after sorting: #{$candidates}"

    chosen = $candidates[0]
    $renderMap[chosen.p[1]][chosen.p[0]] = 'xyzq'.chars.sample
    # puts "picked: #{chosen}"
    add_candidates(map_copy, chosen)

    $candidates.delete_at(0)
    $tracked.push(chosen)
    puts "added candidates: #{$candidates}"

    $renderMap.each{|line| p line.join()}

    sleep(0.1)
  end
  return $goalNodes
end

saves = [0] * 1000

s = Node.new($s_p, 0, :left, 2, nil)

$goalNodes = pathfind(map, s)

$goalNodes.sort!{|a, b| a.totalCost < b.totalCost}
puts "Cheapest path: #{$goalNodes[0].totalCost}"

# if cheated <= baseline - 100
#   puts "# Cheat #{x},#{y} costs #{cheated}"
#   saves[baseline - cheated] = saves[baseline - cheated] + 1
#   n_cheats += 1
# end
# 
# puts "# Baseline = #{baseline}"
# puts "# Cheats #{n_cheats}"
