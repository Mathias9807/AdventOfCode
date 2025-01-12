#!/usr/bin/env ruby

# Optimizer script, parse the map and iteratively remove all dead ends to provide an equal version of the given map that is simpler to check

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

catch (:done) do
  while true
    catch (:found) do
      for y in 1..map.size-1
        for x in 1..map[0].size-1
          if map[y][x] == '.' and numWalls(map, x, y) >= 3
            map[y][x] = '#'
            throw :found
          end
        end
      end
      throw :done
    end

    puts map.map{|row| row.join()}

    sleep(0.05)
  end
end

File.write('input-4.txt', map.map{|row| row.join()}.join)
