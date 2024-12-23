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
def num_arrangements(task, offset):
	if (task, offset) in cache:
		return cache[(task, offset)]

	arrangements = 0
	for pattern in patterns:
		if task[offset:] == pattern:
			arrangements += 1

		if task[offset:offset + len(pattern)] == pattern:
			arrangements += num_arrangements(task, offset + len(pattern))

	cache[(task, offset)] = arrangements
	return arrangements

sum = 0
for task in tasks:
	sum += num_arrangements(task, 0)

print("Sum: {}".format(sum))
