#ifndef __TYPE_H
#define __TYPE_H

/*
    "input" = 0
    "output" = 1
    "inout" = 2
    "basic" = 3
    'add': 4
    'sub': 5
    'mul': 6
    'or': 7
    'xor': 8
    'and': 9
    'not': 10
    'abs': 11
    'pass': 12
    'muladd': 13
    'mulsub': 14
    'addadd': 15
    'subsub': 16
    'addsub': 17
    'mux': 18
    'slt': 19
    'sgt': 20
    'seq': 21
    'sne': 22
    'shl': 23
    'shr': 24
    'max': 25
    'min': 26
*/

std::map<std::string, int> map_type = {{"input", 0},
                          {"output", 1},
                          {"inout", 2},
                          {"basic", 3},
                          {"add", 4},
                          {"sub", 5},
                          {"mul", 6},
                          {"or", 7},
                          {"xor", 8},
                          {"and", 9},
                          {"not", 10},
                          {"abs", 11},
                          {"pass", 12},
                          {"muladd", 13},
                          {"mulsub", 14},
                          {"addadd", 15},
                          {"subsub", 16},
                          {"addsub", 17},
                          {"mux", 18},
                          {"slt", 19},
                          {"sgt", 20},
                          {"seq", 21},
                          {"sne", 22},
                          {"shl", 23},
                          {"shr", 24},
                          {"max", 25},
                          {"min", 26}};

#define SIZE_TYPE 27

#endif