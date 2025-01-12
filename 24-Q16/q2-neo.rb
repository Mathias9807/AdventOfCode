#!/usr/bin/ruby

# New attempt. Make a map of path nodes with cumulative cost and parent nodes. Evaluate colliding paths with different costs. Modify the map live to remove useless paths.


# We read the map as a 2D array of chars
# Pathfinding starts from S and continues until every node has been searched

# We keep a list of candidate nodes left to search and iterate until it is empty. For every node we investigate we add new neighbour nodes to this list
# For every node we evaluate, a structure is created and added to a map of all coordinates+directions. This map can be consulted to check if a new path is colliding with an old path.

# If that happens, and one of the paths is cheaper than the other, that path is invalidated and its route in the char map is filled in with walls.
# The same thing happens with dead end paths.

$stdout.sync = true

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

  def ==(other)
    return false unless other.is_a?(Vec)
    @x == other.x && @y == other.y
  end

  # Override `eql?` for value-based equality in hash contexts
  def eql?(other)
    self == other
  end

  # Override `hash` to ensure objects with the same value are treated equally in hashes
  def hash
    [@x, @y].hash
  end

  def to_s
    "(#{@x}, #{@y})"
  end
end

def dirToVec(dir)
  return Vec.new(1, 0).rotate(dir)
end

def dirToChar(dir)
  dir = dir % 4
  if dir == 0 then return '>' end
  if dir == 1 then return 'v' end
  if dir == 2 then return '<' end
  if dir == 3 then return '^' end
  return '?'
end

$map = File.read('input.txt').lines.map{|line| line.chars}

$map.each_with_index do |r, y|
  r.each_with_index do |c, x|
    if $map[y][x] == 'S' then S = Vec.new(x, y) end
  end
end

$map.each_with_index do |r, y|
  r.each_with_index do |c, x|
    if $map[y][x] == 'E' then E = Vec.new(x, y) end
  end
end

def mapGet(v)
  return $map[v.y][v.x]
end

Loc = Struct.new(:p, :dir)

class Node
  attr_accessor :p, :dir, :cost, :parents, :children

  def initialize(p, dir)
    @p, @dir = p, dir
    @cost = 0
    @parents = []
    @children = []
  end

  def getNeighbours()
    nbs = []
    for i in 0..3
      if (i + 2) % 4 == dir then next end # Don't turn around on the spot

      w = p + dirToVec(i)
      if @parents.any?{|pt| pt.p == w} then next end

      if mapGet(w) == '#' then next end

      estCost = @cost + 1
      if i != @dir then estCost += 1000 end  # Can never do a 180

      if mapGet(w) == 'E'
        goal = Node.new(w, dir)
        goal.parents.push(self)
        goal.cost = estCost
        return [goal]
      end

      # Here's trouble

      # Check existing nodes we crash into and handle appropriately
      # Also check rotations of this node. If we could spin in place and meet an existing path then the paths need to be merged/pruned
      walls = 0
      for dDir in -1..1
        dir = (i + dDir + 4) % 4
        w2 = w + dirToVec(dir)
        if mapGet(w2) == '#' then walls += 1; next end

        nb = Node.new(w, dir)
        nb.parents.push(self)
        nb.cost = estCost + dDir.abs * 1000
        action = processNewPath(nb)

        old = $nodeMap[Loc.new(w, dir)]
        if action == :merge # Peaceful end
          old.parents.push(self)
          @children.push(old)
          nbs.push(old)
        end

        if action == :overwrite
          @children.push(nb)
          nbs.push(nb)
        end

        if action == :abort
          $markedForPruning.push(nb)
          next
        end

        if action == :clear
          # Create new node
          @children.push(nb)
          nbs.push(nb)
        end
      end
      if walls == 3 # If this direction was all walls, block it off
        $map[w.y][w.x] = '#'
      end
    end
    return nbs
  end

  def murderChildren()
    for c in @children
      $nodeMap[Loc.new(c.p, c.dir)] = nil
      $candidates.delete(c)
      c.murderChildren()
    end
  end

  # Call to delete this path head, as well as all parents until one with other children
  def destroyLineage()
    # puts "DestroyLineage #{@p}"
    if @children.size > 0 then return end
    # printNodeMap()

    # Remove self from map
    if $nodeMap[Loc.new(@p, @dir)] == self
      $nodeMap[Loc.new(@p, @dir)] = nil
    end

    # If we removed everything from out node, then that node should be useless. Turn it into a wall
    if (0..3).all?{|dir| $nodeMap[Loc.new(@p, dir)] == nil}
      # gets
      $map[@p.y][@p.x] = '#'
      # printNodeMap()
      # gets
    end

    # sleep(0.1)

    # Remove parents
    @parents.each{|p| p.children.delete(self)}
    @parents.each{|p| p.destroyLineage()}
  end

  def to_s
    "Node(#{@p}, #{@dir}, #{@cost}, #{@parents})"
  end
end

# Map from Loc to Node
$nodeMap = {}

def printNodeMap()
  text = ''
  h = $map.size()
  w = $map[0].size()
  for y in 0..h-1
    line = Array.new(w)
    for x in 0..w-1
      c = mapGet(Vec.new(x, y))
      if c == '#'
        line[x] = '#'
        next
      elsif c == 'E'
        line[x] = 'E'
        next
      elsif c == 'S'
        line[x] = 'S'
        next
      end
      for i in 0..4
        if i == 4
          line[x] = ' '
          break
        end
        n = $nodeMap[Loc.new(Vec.new(x, y), i)]
        if $candidates.include? n
          line[x] = 'C'
          break
        end
        if n != nil
          line[x] = dirToChar(i)
          break
        end
      end
    end
    text += line.join() + "\n"
  end
  print text
end

def printCandidates()
  text = ''
  for y in 0..$map.size()-1
    line = ''
    for x in 0..$map[0].size()-1
      block = mapGet(Vec.new(x, y))
      if ['#', 'E', 'S'].include? block
        line += block
        next
      end
      c = ' '
      cand = $candidates.find{|cand| cand.p == Vec.new(x, y)}
      if cand then c = dirToChar(cand.dir) end
      line += c
    end
    text += line + "\n"
  end
  print text
  $stdout.flush
end

$candidates = []
$markedForPruning = []
def searchFromNode(start)
  $candidates = [start]
  $markedForPruning = []
  printTimer = 0
  while $candidates.size > 0
    $candidates.sort_by!(&:cost)
    # puts 'Candidates: ' + $candidates.map{|c| "<#{c.p}, #{c.dir}>"}.join(', ')
    # print 'Candidates: ' + $candidates.map{|c| "<#{c.p}, #{c.dir}>"}.join(', ') + "\n"

    # Select candidate
    node = $candidates.delete_at(0)
    # puts "Selected #{node.p} #{node.dir}"
    # print "Selected #{node.p} #{node.dir}\n"

    nbs = node.getNeighbours()

    # Check if any neighbour is new, if so add to candidates
    $nodeMap[Loc.new(node.p, node.dir)] = node
    for nb in nbs
      if $nodeMap[Loc.new(nb.p, nb.dir)] != nil and nb.p != E then next end

      isDup = false
      for c in $candidates
        if c.p == nb.p and c.dir == nb.dir
          isDup = true
        end
      end
      if isDup then next end

      nb.parents.each{|p| p.children.push(nb)}
      if nb.p == E
        print "Reached goal!\n"
        $endNode.parents.push(nb)
      else
        # print "Added #{nb.p}, #{nb.dir}\n"
        $nodeMap[Loc.new(nb.p, nb.dir)] = nb
        $candidates.push(nb)
        # printCandidates()
        # gets
      end
    end

    printTimer += 1
    if printTimer > 100 then printNodeMap(); printTimer = 0 end

    if nbs.size == 0
      $markedForPruning.push(node)
    end

    while $markedForPruning.size > 0
      p = $markedForPruning.delete_at(0)
      p.destroyLineage()
    end

    # sleep(0.1)
  end
end

# Check if a new path conflicts with an existing path and if so, resolve the conflict
def processNewPath(path)
  # If we crash into an existing path there are three cases
  # 1. It's cheaper than this one
  # 2. They have the same cost
  # 3. It's more expensive than this one
  #
  # The most expensive path should self-destruct
  # We can handle self-destructs by just not adding a new neighbour, then this will be treated as a dead end and pruned
  # If we cut into a more expensive path we need to cut it, prune it, and remove any candidates added from it

  old = $nodeMap[Loc.new(path.p, path.dir)]
  if old == nil
    # Also abort if this path could've been reached by just taking an existing path and doing a 180
    flip = $nodeMap[Loc.new(path.p, (path.dir + 2) % 4)]
    # If we hit the head of another search path, neither path will lead anywhere. They're both overlapping so both should be considered dead ends
    if flip != nil and flip.children.size == 0 and flip.parents[0].dir == flip.dir
      # Remove old path
      $nodeMap[Loc.new(flip.p, flip.dir)] = nil
      $markedForPruning.push(flip)
      # puts "CLASH-ABORT"
      return :abort
    end
    if flip != nil and flip.cost < path.cost
      # puts "FLIP-ABORT"
      return :abort
    end

    return :clear
  end

  if old.cost == path.cost
    # puts "MERGE"
    return :merge
  end

  if old.cost < path.cost
    # puts "ABORT"
    return :abort
  end

  if old.cost > path.cost
    # Remove old path
    # $nodeMap[Loc.new(path.p, path.dir)] = path
    $nodeMap[Loc.new(path.p, path.dir)] = nil
    $markedForPruning.push(old)

    # Forward delete child paths and filter candidates created from this path
    old.murderChildren()
    old.destroyLineage()

    # puts "OVERWRITE"
    return :overwrite
  end

  puts "what"
  return :what
end

def printPath(pathHead)
  nextUp = [pathHead]
  map = Array.new($map.size).map{|row| [' '] * $map[0].size}
  while nextUp.size > 0
    cur = nextUp.map{|r| r.dup}.uniq
    nextUp = []
    for p in cur
      map[p.p.y][p.p.x] = dirToChar(p.dir)
      for par in p.parents
        nextUp.push(par)
      end
    end
    nextUp.uniq!
  end
  puts map.map(&:join).join("\n")
end

$startNode = Node.new(S, 0)
$nodeMap[Loc.new(S, 0)] = $startNode
$endNode = Node.new(E, 3)
$nodeMap[Loc.new(E, 0)] = $endNode
$nodeMap[Loc.new(E, 1)] = $endNode
$nodeMap[Loc.new(E, 2)] = $endNode
$nodeMap[Loc.new(E, 3)] = $endNode
searchFromNode($startNode)

# The search includes all paths to the end, we only care about the best ones
minCost = $endNode.parents.map{|p| p.cost}.min
for i in 0..$endNode.parents.size-1
  if $endNode.parents[i].cost > minCost then $endNode.parents.delete_at(i) end
end

puts $endNode.parents.map{|p| p.cost}

for i in 0..$endNode.parents.size-1
  p = $endNode.parents[i]
  while p != nil
    # puts "Trace: #{p.p}, #{p.dir}, #{p.cost}"
    p = p.parents[i]
  end
end

printPath($endNode)

$tilesCovered = [S]
$visitedInCount = []
def countTilesCovered(pathHead)
  if $visitedInCount.include? pathHead then return end

  $visitedInCount.push(pathHead)
  $visitedInCount.uniq!

  while pathHead.parents.size > 0
    $tilesCovered.push(pathHead.p)
    $visitedInCount.push(pathHead)
    if pathHead.parents.size > 1
      pathHead.parents.each{|p| countTilesCovered(p)}
      break
    else
      pathHead = pathHead.parents[0]
      next
    end
  end
end
countTilesCovered($endNode)
puts "Tiles covered: #{$tilesCovered.uniq.size}"
