#!/usr/bin/env python3.9

from functools import cache
from itertools import permutations

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

@cache
def getKeypPos(c):
    for y in range(len(keypPattern)):
        for x in range(len(keypPattern[0])):
            if c == keypPattern[y][x]:
                return x, y
    return -1, -1

@cache
def keypadPresses(frm, to):
    fromPos = getKeypPos(frm)
    toPos = getKeypPos(to)
    print('from: {}'.format(fromPos))
    print('to: {}'.format(toPos))

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

nLevels = 3

# Which key presses are necessary to input the given code?
# Funktionen rekurserar uppåt, 
def keyPresses(code, level=0):
    print('keyPresses({}, {})'.format(code, level))
    # If we're on top level, we can type the code directly
    if level + 1 >= nLevels:
        print('\tkeyPresses({}, {}) returned {}'.format(code, level, code))
        return code

    pChr = 'A'
    p = list(getKeypPos(pChr))
    moves = []
    for c in code:
        print('keyPresses({}, {}) evaluating char {}'.format(code, level, c))
        best = []
        bestLen = 1E100
        for opt in keypadPresses(pChr, c):
            print('evaluating {}'.format(opt))
            presses = ''.join(keyPresses(opt + 'A', level + 1))
            if len(presses) < bestLen:
                best = presses
        print('best option: {}'.format(best))
        moves.append(best)
        pChr = c
        p = getKeypPos(c)
    print('\tkeyPresses({}, {}) returned {}'.format(code, level, moves))
    return moves

# presses1 = ''.join(keyPresses('<A^A>^^AvvvA'))  # 029A
# presses2 = ''.join(keyPresses('^^^A<AvvvA>A'))  # 980A
# presses3 = ''.join(keyPresses('^<<A^^A>>AvvvA'))  # 179A
# presses4 = ''.join(keyPresses('^^<<A>A>AvvA'))  # 456A
presses5 = ''.join(keyPresses('^A^^<<A>>AvvvA'))  # 379A
# print('Presses needed to input <^: {} of len {}'.format(presses1, len(presses1)))
# print('Presses needed to input <^: {} of len {}'.format(presses2, len(presses2)))
# print('Presses needed to input <^: {} of len {}'.format(presses3, len(presses3)))
# print('Presses needed to input <^: {} of len {}'.format(presses4, len(presses4)))
print('Presses needed: {} of len {}'.format(presses5, len(presses5)))
