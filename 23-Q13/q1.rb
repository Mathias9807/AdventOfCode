require 'levenshtein'

sum = 0

maps = []
ARGF.each do |line|
	if maps.length == 0
		maps.append([])
	end
	if line.strip.empty?
		maps.append([])
		next
	end

	maps[-1].append(line.strip().chars())
end

puts "Maps #{maps}"
puts "n maps #{maps.length}"

def lev s, t
	return Levenshtein.distance s, t
end

def findSmudge(map)
	puts "Find smudges! \n#{map.map{|line| line.join()}.join("\n")}"
	for y in 0...map.length-1
		error = 0
		errorY = [-1, -1]
		puts y
		for refl in 0..[y, map.length - 2 - y].min
			puts map[y - refl].join()
			puts map[y + 1 + refl].join()
			newError = lev(map[y - refl].join(), map[y + 1 + refl].join())
			if newError == 1
				errorY = [y - refl, y + 1 + refl]
				puts "Error: #{error}, errorY: #{errorY}, y: #{y}, refl: #{refl}"
			end
			error += newError
		end

		if error == 1
			puts "errorY: #{errorY}"
			errorX = map[errorY[0]].each_with_index.find_index{|char, i| char != map[errorY[1]][i]}
			map[errorY[0]][errorX] = map[errorY[1]][errorX]
			# map[errorY[1]][errorX] = map[errorY[0]][errorX]
			return map
		end
	end

	return false
end

def findReflections(map)
	puts "Find reflections! \n#{map.map{|line| line.join()}.join("\n")}"
	nRefl = 0
	for y in 0...map.length-1
		puts y
		catch :skip do
			for refl in 0..[y, map.length - 2 - y].min
				puts map[y - refl].join()
				puts map[y + 1 + refl].join()
				if map[y - refl].join() != map[y + 1 + refl].join()
					puts "not match"
					throw :skip
				end
			end

			nRefl += y + 1
			puts "Match on line #{y + 1}!"
			return nRefl # Apparently we only care about the first reflection?
		end
	end
	return nRefl
end

for map in maps
	corrected = findSmudge(map)
	if !corrected
		map = map.transpose
		corrected = findSmudge(map)
		corrected = corrected.transpose
	end
	if !corrected
		puts "Can't find a smudge!!"
		break
	end
	puts "Found smudge! \n#{corrected.map{|line| line.join()}.join("\n")}"
	puts

	verticalMatch = findReflections(corrected)
	sum += 100 * verticalMatch
	if verticalMatch > 0
		next
	end
	corrected = corrected.transpose
	sum += findReflections(corrected)

	puts
end

puts "Sum: #{sum}"

