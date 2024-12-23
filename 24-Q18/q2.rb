#!/usr/bin/env ruby

$w = 71
$h = 71

input = File.read('input.txt').split("\n")

p input

memory = Array.new($h) { Array.new($w, '.') }
input[0..2500].each do |line|
  p line
  nums = line.split(',')
  p nums
  memory[nums[1].to_i][nums[0].to_i] = '#'
end

memory.each{|line| p line.join()}

# Path find from 0,0 to bottom right
$candidates = []
$tracked = []
Node = Struct.new(:p, :cost)

def vecAdd(a, b)
  return [a[0] + b[0], a[1] + b[1]]
end

def distance(p)
  return $w - p[0] + $h - p[1]
end

$render = memory.map(&:dup)

def add_candidates(memory, node)
  [[1,0], [0,1], [-1,0], [0,-1]].each do |dir|
    coord = vecAdd(node.p, dir)
    if coord[0] < 0 or coord[1] < 0 or coord[0] >= $w or coord[1] >= $h
      next
    end

    if memory[coord[1]][coord[0]] == '.'
      $candidates.push(Node.new(coord, node.cost + 1))
      memory[coord[1]][coord[0]] = 'o'

      if coord[0] == $w-1 and coord[1] == $h-1
        puts "Found goal at #{node.cost + 1} steps"
        return true
      end
    end
  end
  return false
end

def path_find(memory)
  $candidates = []
  $tracked = []
  s = Node.new([0, 0], 0)
  $candidates.push(s)
  while true
    # puts 'sorting'
    $candidates.sort_by!{|c| c.cost + distance(c.p)}
    # puts "after sorting: #{$candidates}"

    chosen = $candidates[0]
    if chosen == nil then return false end
    memory[chosen.p[1]][chosen.p[0]] = 'x'
    # puts "picked: #{chosen}"
    foundGoal = add_candidates(memory, chosen)
    if foundGoal then break end

    $candidates.delete_at(0)
    $tracked.push(chosen)
    # puts "added candidates: #{$candidates}"

    # memory.each{|line| p line.join()}

    # sleep(0.1)
  end
  return true
end

input.each do |line|
  p line
  nums = line.split(',')
  puts "Adding block at #{nums[0]},#{nums[1]}"
  memory[nums[1].to_i][nums[0].to_i] = '#'

  # Does path still exist?
  found_path = path_find(memory.map(&:dup))
  puts "pathfind: #{found_path}, added block #{nums[0]},#{nums[1]}"
  if !found_path then break end
end
