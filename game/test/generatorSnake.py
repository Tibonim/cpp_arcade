#!/usr/bin/python3.3

var = {}

var["WHERE_AM_I"] = 0
var["GET_MAP"] = 1
var["GO_UP"] = 2
var["GO_DOWN"] = 3
var["GO_LEFT"] = 4
var["GO_RIGHT"] = 5
var["GO_FORWARD"] = 6
var["SHOOT"] = 7
var["ILLEGAL"] = 8
var["PLAY"] = 9

def do(val):
    print(chr(val), end="")
    print(chr(0), end="")

for i in range(1, 20):
    for j in range(0, i):
        do(var["GO_FORWARD"])
    do(var["GO_LEFT"])

# do(var["GO_FORWARD"])
# do(var["GO_RIGHT"])
# do(var["GO_FORWARD"])
# do(var["GO_RIGHT"])
# do(var["GO_FORWARD"])
# do(var["GO_RIGHT"])
# do(var["GO_FORWARD"])
# do(var["GO_FORWARD"])
# do(var["GO_FORWARD"])
