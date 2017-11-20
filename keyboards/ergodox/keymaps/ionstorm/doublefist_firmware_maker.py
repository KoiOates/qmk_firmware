import os
import sys

from pprint import pprint

from doublefist import LH_TRANS

uppercase_definitions = \
        {"Z"+key: val.upper() for key, val in LH_TRANS.items() \
            if (len(val) == 1 and val.isalpha())}

print(uppercase_definitions)
LH_TRANS.update(uppercase_definitions)
pprint(LH_TRANS)
# Make it case agnostic
plover2qmk = """\
    #left   left
    #right  right
    #up     up
    #down   down
    #escape esc
    #return ent
    #tab    tab
    #backspace  bspc
    #delete     del
    #insert     ins
    #pageup     pgup
    #pagedown   pgdn
    #home       home
    #end        end
    #space spc
    
    #shift(braceleft)   lsft lbrc 
    #shift(braceright)  lsft rbrc 
    
    !   lsft 1
    @   lsft 2
    #   lsft 3
    $   lsft 4
    %   lsft 5
    ^   lsft 6
    &   lsft 7
    *   lsft 8
    (   lsft 9
    )   lsft 0

    _   lsft mins
    -   mins
    =   eql
    +   lsft eql
    [   lbrc
    ]   rbrc
    ,   comm
    .   dot 
    <   lsft comm
    >   lsft dot
    /   slsh
    ?   lsft slsh 
    |   pipe
    \\  lsft slsh
    ;   scln
    :   lsft scln
    '   quot
    "   lsft quot
    `   grv
    ~   lsft grv
"""
plover2qmk = [[y.strip() for y in x.strip().split(" ", 1)] \
                             for x in plover2qmk.split("\n") \
                                                 if x.strip() != ""]
plover2qmk = [[x[0],
             ["KC_%s" % y.upper()
                 for y in x[1].split(" ")]]
             for x in plover2qmk]

plover2qmk = dict(plover2qmk)
print(plover2qmk)

key2bin = {
    "Z":0b1000000000,
    "S":0b0100000000,
    "T":0b0010000000,
    "K":0b0001000000,
    "P":0b0000100000,
    "W":0b0000010000,
    "H":0b0000001000,
    "R":0b0000000100,
    "A":0b0000000010,
    "O":0b0000000001,
}

def tokeycodes2(s):
    if len(s) == 1 and s.isalpha():
        if s.lower():
            s = s.upper()
            return "KC_" + s
    else:
        return "KC_SPC"

def letter_keycode(s):
    if s.islower():
        s = s.upper()
        return ["KC_" + s,]
    else:
        return ["KC_LSFT", "KC_" + s]

def tokeycodes(s):
    if len(s) == 1 and s.isalpha():
        return letter_keycode(s)
    else:
        out = plover2qmk.get(s, None)
        if out:
            return out
        else:
            out = []
            for char in s:
                if char == " ":
                    char = "#space"
                kc = plover2qmk.get(char, None)
                if kc:
                    out.extend(plover2qmk[char])
                else:
                    out.extend(letter_keycode(char))
            if len(out) > 3:
                print("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n", out)
                print(s)
                input()
            return out

# this stuff gets reordered by the sortation
# So it's hard to see clearly what's happening without
# thought of the caliber that will be thunk tomorrow after
# rest and liver function.
binlist = []

for chord, translation in LH_TRANS.items():
    binchord = 0
    for key in chord:
        binchord |= key2bin[key]
    binlist.append([binchord, tokeycodes(translation)])
    print(bin(0b10000000000 | binchord), chord)

binlist.sort()
for i, x in enumerate(binlist):
    print(i, x[0],x[1])
print(binlist)

i = 0
master_array = []

arrays = []

for key, val in binlist:
    while True:
        if i == key:
            array_name = "array_%d" % i
            master_array.append(array_name)
            print(val)
            arrays.append("const PROGMEM uint16_t %s[] = { %s };" %
                    (array_name, ', '.join([str(len(val))] + val)))
            i += 1
            break
        else:
            master_array.append("0")
            i += 1

arrays = '\n'.join(arrays)
master_array = ', '.join(master_array)
out = "#include <avr/pgmspace.h>\n#include \"keycode.h\"\n" \
      "%s\nconst PROGMEM uint16_t chorddict[] = { %s };" % \
            (arrays, master_array)
# The bug that took so long to fix:
#       chorddict[] was just chorddict
#   The warning was excess elements in scalar something or other, did not actually prevent
#       the code from compiling.
pprint(LH_TRANS)
pprint(out)

f = open("stenodictionary.h", "w")
f.write(''.join(out))
