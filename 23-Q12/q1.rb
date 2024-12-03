#!/usr/bin/env ruby

$sum = 0

$cache = {}

# Count number of arrangements for the first group provided in the given pattern
# Assume that there are no springs to the left of the pattern but make sure to
# consume a safe spring to the right before recursing
def countArrangements(pattern, groups, indent)
  nSprings = groups[0]

  cachePattern = pattern.sub(/^\.+/, '') # Initial '.'s can't be used

  cached = $cache[[cachePattern, groups]]
  if cached
    # puts "Cache found #{cached} arrangements in this branch!"
    $sum += cached
    return cached
  end

  localSum = 0
  for x in 0..pattern.length() do
    # If we've passed a hashtag we can't fit in the current group, exit this branch
    if pattern[0...x].chars.include?('#')
      $cache[[cachePattern, groups]] = localSum
      return localSum
    end

    if pattern[x] == '.'
      next
    end

    if pattern[x...x+nSprings].chars.all?{|c| c == '#' || c == '?'} \
        and x + nSprings <= pattern.length \
        and (x == 0 || pattern[x - 1] != '#') \
        and pattern[x + nSprings] != '#'
      newPattern = pattern[x+nSprings+1..]
      newGroups = groups[1..]
      newIndent = indent + '-' * x + '+' * nSprings + '-'
      # puts "#{newIndent + (pattern[x+nSprings+1..] or '')} #{nSprings} springs fit in index #{x}"

      if groups.length == 1
        if newPattern && newPattern.chars.include?('#')
          next
        end

        $sum += 1
        localSum += 1
        # puts "#{newIndent} All matched!"
        next
      end
      if !newPattern
        $cache[[cachePattern, groups]] = localSum
        return localSum
      end

      # Make sure there's enough of pattern left to possibly find a match
      if newGroups.sum() + newGroups.length() - 1 > newPattern.length()
        $cache[[cachePattern, groups]] = localSum
        return localSum
      end
      localSum += countArrangements(newPattern, newGroups, newIndent)
    end
  end

  $cache[[cachePattern, groups]] = localSum
  return localSum
end

# i = 0
ARGF.each do |line|
  # Need to calculate the number of possible arrangements recursively
  pattern = line.split(' ')[0]
  pattern = ([pattern] * 5).join('?')
  pattern = pattern.chomp('.')
  groups = line.split(' ')[1].split(',').map(&:to_i)
  groups = groups * 5

  puts "Counting arrangements! #{groups}"
  puts pattern
  countArrangements(pattern, groups, '')
  puts "Sum: #{$sum}"

  # if i == 5
  #   break
  # end
  # i += 1
end

# puts "Cache: #{$cache}"
puts "Sum: #{$sum}"

