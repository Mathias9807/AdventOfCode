#!/usr/bin/python3

import fileinput

buffer = []
for line in fileinput.input():
	buffer.append(line.rstrip())
print(buffer)

answer = 0

def checkIfGear(x, y):
	global answer
	neighbours = []

	for nY in range(-1, 2):
		if y + nY < 0 or y + nY >= len(buffer):
			continue
		coveredX = -999
		for nX in range(-1, 2):
			if x + nX < 0 or x + nX >= len(buffer[y + nY]):
				continue
			if nX == 0 and nY == 0:
				continue
			if nX <= coveredX:
				continue

			if isDigit(buffer[y + nY][x + nX]):
				print('Found digit', buffer[y + nY][x + nX])
				firstIndex = nX
				while x + firstIndex > 0 and isDigit(buffer[y + nY][x + firstIndex - 1]):
					firstIndex -= 1
				print('First index', firstIndex)

				length = 0
				while x + firstIndex + length < len(buffer[y + nY]) and isDigit(buffer[y + nY][x + firstIndex + length]):
					length += 1
				print(buffer[y + nY][x + firstIndex:x + firstIndex + length])
				neighbours.append(int(buffer[y + nY][x + firstIndex:x + firstIndex + length]))

				coveredX = nX + firstIndex + length

	print('neighbours', neighbours)
	if len(neighbours) == 2:
		ratio = neighbours[0] * neighbours[1]
		print('ratio', ratio)
		answer += ratio


def isDigit(char):
	if ord(char) < ord('0'):
		return False
	if ord(char) > ord('9'):
		return False

	return True


for y in range(len(buffer)):
	for x in range(len(buffer[y])):
		if buffer[y][x] == '*':
			checkIfGear(x, y)
			print(buffer[y])

print('Answer:', answer)

