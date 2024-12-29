#!/usr/bin/env python3.9

from functools import lru_cache
from itertools import permutations
import sys

keypPattern = [
    ['X', '^', 'A'],
    ['<', 'v', '>'],
]

dirs = {
    '^': (0, -1),
    'v': (0,  1),
    '<': (-1, 0),
    '>': ( 1, 0),
}

@lru_cache(None)
def getKeypPos(c):
    for y in range(len(keypPattern)):
        for x in range(len(keypPattern[0])):
            if c == keypPattern[y][x]:
                return x, y
    return -1, -1

@lru_cache(None)
def keypadPresses(frm, to):
    fromPos = getKeypPos(frm)
    toPos = getKeypPos(to)
    # print('from: {}'.format(fromPos))
    # print('to: {}'.format(toPos))

    moves = ['^' for _ in range(0, fromPos[1] - toPos[1])] \
        + ['v' for _ in range(0, toPos[1] - fromPos[1])] \
        + ['<' for _ in range(0, fromPos[0] - toPos[0])] \
        + ['>' for _ in range(0, toPos[0] - fromPos[0])]
    perms = permutations(moves)
    def filterIllegal(movement):
        p = [fromPos[0], fromPos[1]]
        for move in movement:
            p[0] += dirs[move][0]
            p[1] += dirs[move][1]
            if keypPattern[p[1]][p[0]] == 'X':
                return False
        return True
    moves = filter(filterIllegal, perms)
    moves = [''.join(move) for move in moves]
    return list(set(moves))

nLevels = 26

# Which key presses are necessary to input the given code?
# Funktionen rekurserar uppåt,
@lru_cache(None)
def keyPresses(code, level=0):
    # print('keyPresses({}, {})'.format(code, level))
    # If we're on top level, we can type the code directly
    if level + 1 >= nLevels:
        # print('\tkeyPresses({}, {}) returned {}'.format(code, level, code))
        return len(code)

    pChr = 'A'
    # p = list(getKeypPos(pChr))
    moves = 0
    for c in code:
        # print('keyPresses({}, {}) evaluating char {}'.format(code, level, c))
        bestLen = 1E100
        for opt in keypadPresses(pChr, c):
            # print('evaluating {}'.format(opt))
            presses = keyPresses(opt + 'A', level + 1)
            if presses < bestLen:
                bestLen = presses
        # print('keyPresses({}, {}) best option: {}'.format(code, level, best))
        moves += bestLen
        pChr = c
        # p = getKeypPos(c)
    # print('\tkeyPresses({}, {}) returned {}'.format(code, level, moves))
    return moves

# nums = [29, 980, 179, 456, 379]
# codes = ['<A^A>^^AvvvA', '^^^A<AvvvA>A', '^<<A^^A>>AvvvA', '^^<<A>A>AvvA', '^A<<^^A>>AvvvA']
nums = [319, 985, 340, 489, 964]
codes = ['^A<<A^^>>AvvvA', '^^^A<AvAvv>A', '^A<<^A>vvA>A', '^^<<A^>A>AvvvA', '^^^AvA<<A>>vvA']
presses1 = keyPresses(codes[0])
presses2 = keyPresses(codes[1])
presses3 = keyPresses(codes[2])
presses4 = keyPresses(codes[3])
presses5 = keyPresses(codes[4])
print('Presses needed: {}'.format(presses1))
print('Presses needed: {}'.format(presses2))
print('Presses needed: {}'.format(presses3))
print('Presses needed: {}'.format(presses4))
print('Presses needed: {}'.format(presses5))

presses = [presses1, presses2, presses3, presses4, presses5]
print('Complexity: {}'.format(sum([presses[i] * nums[i] for i in range(len(nums))])))
