#!/usr/bin/env python

patterns = []
tasks = []

with open("input.txt", "r") as file:
	lines = file.readlines()

	patterns = lines[0].split(',')
	patterns = [p.strip() for p in patterns]

	tasks = [line.strip() for line in lines[2:]]

print(patterns)
print(tasks)

cache = {}
def can_be_arranged(task, offset):
	# print('can_be_arranged({}, {})'.format(task, offset))
	if (task, offset) in cache:
		return cache[(task, offset)]
	i = 0
	for pattern in patterns:
		i += 1
		if offset == 0:
			print('\tTesting pattern {} ({}) of {}'.format(i+1, pattern, len(patterns)))
		if task[offset:] == pattern:
			return True

		if task[offset:offset + len(pattern)] == pattern:
			if can_be_arranged(task, offset + len(pattern)):
				cache[(task, offset)] = True
				return True

	cache[(task, offset)] = False
	return False

sum = 0
for task in tasks:
	if can_be_arranged(task, 0):
		sum += 1

print("Sum: {}".format(sum))
