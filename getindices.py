#!/usr/bin/env python


import sys


def main(elements, fullList):
    indices = []
    for e in elements.split():
        idx = 0
        for cur in fullList.split():
            if cur == e:
                indices.append(str(idx))
            idx += 1
    print(" ".join(indices))


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Expected two strings with space-separated words")
        exit(1)
    main(sys.argv[1], sys.argv[2])
