#!/usr/bin/env python

from math import factorial
from itertools import permutations

keypadPos = (2, 3)
keypadBPos = (2, 0)

def numKeypadSequences(code):
    global keypadPos

    arrangements = 1
    for c in code:
        target = {
            '1': (0, 2),
            '2': (1, 2),
            '3': (2, 2),
            '4': (0, 1),
            '5': (1, 1),
            '6': (2, 1),
            '7': (0, 0),
            '8': (1, 0),
            '9': (2, 0),
            '0': (1, 3),
            'A': (2, 3),
        }[c]

        print("Move from {} to coord {}".format(keypadPos, target))
        # Formula for number of arrangements is keystrokes! / number of same keys!
        verticalMoves = abs(keypadPos[1] - target[1])
        horizontalMoves = abs(keypadPos[0] - target[0])
        print("factorial({}) / {} / {} = {}".format(verticalMoves + horizontalMoves, max(verticalMoves, 1), max(horizontalMoves, 1),
                factorial(verticalMoves + horizontalMoves) / factorial(max(verticalMoves, 1)) / factorial(max(horizontalMoves, 1))))
        arrangements *= factorial(verticalMoves + horizontalMoves) / factorial(max(verticalMoves, 1)) / factorial(max(horizontalMoves, 1))
        keypadPos = target

    return arrangements

def keypadAInputs(code):
    global keypadPos

    inputs = []
    for c in code:
        target = {
            '1': (0, 2),
            '2': (1, 2),
            '3': (2, 2),
            '4': (0, 1),
            '5': (1, 1),
            '6': (2, 1),
            '7': (0, 0),
            '8': (1, 0),
            '9': (2, 0),
            '0': (1, 3),
            'A': (2, 3),
        }[c]

        # Directions to move to get to the next button
        movements = (
            ['>' for _ in range(0,  (target[0] - keypadPos[0]))] +
            ['<' for _ in range(0, -(target[0] - keypadPos[0]))] +
            ['^' for _ in range(0, -(target[1] - keypadPos[1]))] +
            ['v' for _ in range(0,  (target[1] - keypadPos[1]))]
        )
        perms = set(permutations(movements))
        dir = {'>': (1, 0), '<': (-1, 0), '^': (0, -1), 'v': (0, 1)}
        def isLegal(p):
            k = [keypadPos[0], keypadPos[1]]
            for move in p:
                k[0] += dir[move][0]
                k[1] += dir[move][1]
                if k[0] == 0 and k[1] == 3:
                    return False
            return True
        perms = filter(isLegal, perms)

        inputs.append(perms)
        keypadPos = target
    return inputs

def keypadBInputs(instructions):
    global keypadBPos

    inputs = []
    for seq in instructions:
        # Array of button sequences that execute the instruction
        # Contains tuples of the final keypad loc and the list of moves
        seqOptions = []
        endPos = keypadBPos
        for c in seq:
            target = {
                '^': (1, 0),
                'A': (2, 0),
                '<': (0, 1),
                'v': (1, 1),
                '>': (2, 1),
            }[c]

            # Directions to move to get to the next button
            movements = (
                ['>' for _ in range(0,  (target[0] - endPos[0]))] +
                ['<' for _ in range(0, -(target[0] - endPos[0]))] +
                ['^' for _ in range(0, -(target[1] - endPos[1]))] +
                ['v' for _ in range(0,  (target[1] - endPos[1]))]
            )
            perms = set(permutations(movements))
            dir = {'>': (1, 0), '<': (-1, 0), '^': (0, -1), 'v': (0, 1)}
            def isLegal(p):
                k = [endPos[0], endPos[1]]
                for move in p:
                    k[0] += dir[move][0]
                    k[1] += dir[move][1]
                    if k[0] == 2 and k[1] == 0:
                        return False
                return True
            perms = filter(isLegal, perms)

test = keypadAInputs('1')
print(test)
testB = keypadBInputs(test)
print(testB)

# aInputs = keypadAInputs('029A')
# keypadBInputs(aInputs)

# with open("test-input.txt", "r") as file:
#     lines = [line.strip() for line in file.readlines()]
# 
#     for line in lines:
#         print(line)
#         print(numKeypadSequences(line))
