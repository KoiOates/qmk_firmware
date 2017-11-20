import re

LONGEST_KEY = 1

TOGGLE_MODE_RH = ["BLSZ",
                  "BLGSZ"]
                        #Use this easy-when-shifted-right ender
                        #to toggle in and out of alphabet mode,
                        #first (or last) letter of alphabet mode
                        #on left hand is optional

############## Start of Mirror functions


lefthand =  list("ZSTKPWHRAO*-")
righthand = list("-DEURFBPGLST")
righthand.reverse()


leftnums =  (("1", "Z"), ("2", "T"), ("3", "P"), ("4", "H"), ("5", "A"), ("0", "O"))
rightnums = (("F", "6"), ("P", "7"), ("L", "8"), ("T", "9"))
reversals = (("RF","FR"), ("BP","PB"), ("GL","LG"), ("ST", "TS"))

def right2nums(right):
    for l, r in rightnums:
        right = right.replace(l, r)
    return right

def left2right(left):
    out = []
    if re.search(r"\d", left):
        numbar = True
        for n, c in leftnums:
            left = left.replace(n, c)
    else:
        numbar = False

    for char in left:
        ix = lefthand.index(char)
        out.append(righthand[ix])
    out.reverse()
    if "D" in out:
        endchar = "Z" if out[-1] == "S" else "D"
        out.remove("D")
        out.append(endchar)

    out = ''.join(out)
    for l, r in reversals:
        out = out.replace(l, r)
    if numbar:
        out = right2nums(out)
    
    return out

def mirror_lh_dict(d):
    return {left2right(x):d[x] for x in d.keys()}

def testmirror(left, correct):
    right = left2right(left)
    print('%s=>%s \t should be %s' % (left, right, correct))
    assert right == correct

########## End of mirror functions

KEYNAME_TO_CHAR = {
    # Generated using:
    #
    # from Xlib import XK
    # from plover.oslayer.xkeyboardcontrol import keysym_to_string
    # for kn, ks in sorted({
    #     name[3:].lower(): getattr(XK, name)
    #     for name in sorted(dir(XK))
    #     if name.startswith('XK_')
    # }.items()):
    #     us = keysym_to_string(ks)
    #     if us == kn or not us:
    #         continue
    # print '    %-20r: %8r, # %s' % (kn, us, us)
    'aacute'            :  u'\xe1', # á
    'acircumflex'       :  u'\xe2', # â
    'acute'             :  u'\xb4', # ´
    'adiaeresis'        :  u'\xe4', # ä
    'ae'                :  u'\xe6', # æ
    'agrave'            :  u'\xe0', # à
    'ampersand'         :     u'&', # &
    'apostrophe'        :     u"'", # '
    'aring'             :  u'\xe5', # å
    'asciicircum'       :     u'^', # ^
    'asciitilde'        :     u'~', # ~
    'asterisk'          :     u'*', # *
    'at'                :     u'@', # @
    'atilde'            :  u'\xe3', # ã
    'backslash'         :    u'\\', # \
    'bar'               :     u'|', # |
    'braceleft'         :     u'{', # {
    'braceright'        :     u'}', # }
    'bracketleft'       :     u'[', # [
    'bracketright'      :     u']', # ]
    'brokenbar'         :  u'\xa6', # ¦
    'ccedilla'          :  u'\xe7', # ç
    'cedilla'           :  u'\xb8', # ¸
    'cent'              :  u'\xa2', # ¢
    'clear'             :   '\x0b', # 
    'colon'             :     u':', # :
    'comma'             :     u',', # ,
    'copyright'         :  u'\xa9', # ©
    'currency'          :  u'\xa4', # ¤
    'degree'            :  u'\xb0', # °
    'diaeresis'         :  u'\xa8', # ¨
    'division'          :  u'\xf7', # ÷
    'dollar'            :     u'$', # $
    'eacute'            :  u'\xe9', # é
    'ecircumflex'       :  u'\xea', # ê
    'ediaeresis'        :  u'\xeb', # ë
    'egrave'            :  u'\xe8', # è
    'equal'             :     u'=', # =
    'eth'               :  u'\xf0', # ð
    'exclam'            :     u'!', # !
    'exclamdown'        :  u'\xa1', # ¡
    'grave'             :     u'`', # `
    'greater'           :     u'>', # >
    'guillemotleft'     :  u'\xab', # «
    'guillemotright'    :  u'\xbb', # »
    'hyphen'            :  u'\xad', # ­
    'iacute'            :  u'\xed', # í
    'icircumflex'       :  u'\xee', # î
    'idiaeresis'        :  u'\xef', # ï
    'igrave'            :  u'\xec', # ì
    'less'              :     u'<', # <
    'macron'            :  u'\xaf', # ¯
    'masculine'         :  u'\xba', # º
    'minus'             :     u'-', # -
    'mu'                :  u'\xb5', # µ
    'multiply'          :  u'\xd7', # ×
    'nobreakspace'      :  u'\xa0', #  
    'notsign'           :  u'\xac', # ¬
    'ntilde'            :  u'\xf1', # ñ
    'numbersign'        :     u'#', # #
    'oacute'            :  u'\xf3', # ó
    'ocircumflex'       :  u'\xf4', # ô
    'odiaeresis'        :  u'\xf6', # ö
    'ograve'            :  u'\xf2', # ò
    'onehalf'           :  u'\xbd', # ½
    'onequarter'        :  u'\xbc', # ¼
    'onesuperior'       :  u'\xb9', # ¹
    'ooblique'          :  u'\xd8', # Ø
    'ordfeminine'       :  u'\xaa', # ª
    'oslash'            :  u'\xf8', # ø
    'otilde'            :  u'\xf5', # õ
    'paragraph'         :  u'\xb6', # ¶
    'parenleft'         :     u'(', # (
    'parenright'        :     u')', # )
    'percent'           :     u'%', # %
    'period'            :     u'.', # .
    'periodcentered'    :  u'\xb7', # ·
    'plus'              :     u'+', # +
    'plusminus'         :  u'\xb1', # ±
    'question'          :     u'?', # ?
    'questiondown'      :  u'\xbf', # ¿
    'quotedbl'          :     u'"', # "
    'quoteleft'         :     u'`', # `
    'quoteright'        :     u"'", # '
    'registered'        :  u'\xae', # ®
    'return'            :     '\r', # 
    'section'           :  u'\xa7', # §
    'semicolon'         :     u';', # ;
    'slash'             :     u'/', # /
    'space'             :     u' ', #  
    'ssharp'            :  u'\xdf', # ß
    'sterling'          :  u'\xa3', # £
    'tab'               :     '\t', # 	
    'thorn'             :  u'\xfe', # þ
    'threequarters'     :  u'\xbe', # ¾
    'threesuperior'     :  u'\xb3', # ³
    'twosuperior'       :  u'\xb2', # ²
    'uacute'            :  u'\xfa', # ú
    'ucircumflex'       :  u'\xfb', # û
    'udiaeresis'        :  u'\xfc', # ü
    'ugrave'            :  u'\xf9', # ù
    'underscore'        :     u'_', # _
    'yacute'            :  u'\xfd', # ý
    'ydiaeresis'        :  u'\xff', # ÿ
    'yen'               :  u'\xa5', # ¥
}

CHAR_TO_KEYNAME = {x[1]:x[0] for x in KEYNAME_TO_CHAR.items()}
CHAR_TO_KEYNAME["^ ^"] = "space"
########## Begin proto-ultimate-alphabet mode stuff ##########

Steno = "Steno"
NormalAlpha = "NormalAlpha"
NonBsAlpha = "NonBsAlpha"
Capslock = "Capslock"
VimNormal = "VimNormal"

mode = Steno
####################################################################
STENALPHABET =  dict(zip(\
                    ["A","PW","KR","TK","PR","TP","TKPW",
                     "H","TPR","SKWR","K","HR","PH","TPH","O","P",
                     "KW","R","S","T","SKW","SR","W","KP","KWR","STKPW"],
                    list("abcdefghijklmnopqrstuvwxyz")))
STENALPHABET.update({"TH":"th", "SH":"sh", "KH":"ch", "WH":"wh",
                     "AO":"i"})
RH_STENALPHABET = mirror_lh_dict(STENALPHABET)
####################################################################
#DIGITS = {"4":"1",  "3":"2",  "34":"3",  "2":"4",  "24":"5",  "23":"6",  "234":"7",
#        "1":"8",  "14":"9", "SRO": "0"}   # Not sure what to do with zero exactly
#RH_DIGITS = mirror_lh_dict(DIGITS)
####################################################################
OTHERCHARS = {\
        "ZSTKPWHRAO": ".",
        "TPR":"i", "STP":"u", "STKW":"z", "STKP":"z",          #does this line belong in stenalphabet update?
        "SKR":"!",   "TWRA":"@", "SHA":"#",    "TKHR": "$", "SPRO": "%",
        "KPR":"^",   "SKP":"&",  "STRA":"*",    "PRA": "(", "PRO": ")",
        "SKWRA":"_", "SKWRO":"-","KWA":"=",     "KPRA": "<", "KPRO": ">",
        "WO": ", ",  "PHR":"+",     "PWRA":"[", "PWRO":"]",
        "KWO":",",   "TPO":".", "PO":". ",
        "SHRO":"/", "SHRA":"\\", "HO":"?",       "TPRA": "#shift(braceleft)", "TPRO": "#shift(braceright)",
        "KHR": ":", "SKHR": ";","SKWO": "\"", "SKWA": "'",
        "TPW":"`", "THR":"~", "TPWAO":"|",
        "SP": "#space",}
RH_OTHERCHARS = mirror_lh_dict(OTHERCHARS)
####################################################################
NONCHARS = {"RA":"#left", "HRA":"#up", "HRO":"#down", "RO":"#right",
        "SKPA":"#escape",   "SPWR": "#return",  "TA": "#tab",
        "SW":"#backspace", "SWA":"#delete", "SWO":"#insert",
        "PHRA":"#pageup", "PHRO":"#pagedown", "PHA":"#home", "PHO":"#end"}
        #"4*":"F1",  "3*":"F2",  "34*":"F3",  "2*":"F4",  "24*":"F5",  "23*":"F6",  "234*":"F7",
        #"1*":"F8",  "14*":"F9", "13*":"F10","134*":"F11","12*":"F12","124*":"F13","123*":"F14",
        #"1234*":"F15"}
    #Function keys can be done some other way
RH_NONCHARS = mirror_lh_dict(NONCHARS)
####################################################################
LH_MODIFIERS = {"ZTP":  "@shift",
                "ZTO":    "@control",
                "ZTA":    "@alt",
                "ZTPO": "@control,shift",
                "ZTPA": "@alt,shift",
                }
RH_MODIFIERS = mirror_lh_dict(LH_MODIFIERS)
def z2s_lh_dict(dictionary):
    d = {k.replace('Z', 'S'):v for k, v in dictionary.items()}
    return d
LH_MODIFIERS = z2s_lh_dict(LH_MODIFIERS)
#TODO Beginning of the prep translation function
#calls a do_modifiers function. The return result of which
#is which modifiers to activate for that stroke, or None.
#It can also toggle capslock, so even if a None
#is received by prep_translation, it still has to check
#if capslock is active and shift each character accordingly.
#Maybe it should be a real
#capslock? Nah, that would mess things up in the add_translation
#dialogue because plover wouldn't know about that.
LH_TRANS = {}
for d in [STENALPHABET, OTHERCHARS, NONCHARS]:
    LH_TRANS.update(d) 

RH_TRANS = {}
for d in [RH_STENALPHABET, RH_OTHERCHARS, RH_NONCHARS]:
    RH_TRANS.update(d) 

#RH_FUNCTIONS = {} # TODO: Capitalization, backspaceableness, vim corner cases etc.
#RH_FUNCTIONS["BLSZ"] = "!!StartAlphabet"

PARTS_RE = re.compile(r"""
    ^
    (?P<lhs> \#? S? 1? T? 2? K? P? 3? W? H? 4? R?
             A? 5? O? 0? (\* | - | ))
    (?P<rhs> E? U? F? 6? R? P? 7? B? L? 8? G?
             T? 9? S? D? Z?)
    $
""", re.VERBOSE) #Thanks, @arxanas!

def lookup(key):
    global mode
    assert len(key) <= LONGEST_KEY
    stroke = key[0]
    try:
        translation = do_lookup(stroke)
    except Exception as e:
        # Don't mask KeyErrors that arise because of my programming mistakes.
        print(e)
        raise RuntimeError("couldn't translate multiword brief\nMessage was \"%s\"" % str(e)) from e
    if not translation:
        raise KeyError()
    return translation

def stroke2int(stroke, pat, nummap):
    if stroke.startswith("#"):
        stroke = stroke[1:]
    integer = sum([x[1] if x[0] else 0
           for x in zip(re.match(pat, stroke).groups(), nummap)])
    return integer

def lh2int(stroke, starred):
    if starred and not stroke.endswith("*"):
        stroke = stroke + "*"
    integer = stroke2int(stroke,
            r"#?([S1]?)([T2]?)([P3]?)([H4]?)(R?)([A5]?)([O0]?)", [8,4,2,1,64,32,16])
    if integer:
        return str(integer)
    else:
        return ""

def rh2int(stroke, starred):
    print("rh num", stroke)
    if starred and not stroke.endswith("*"):
        stroke = "*" + stroke
    integer = stroke2int(stroke, r"#?(E?)(U?)([6F]?)(R?)([7P]?)([8L]?)([9T]?)", [16,32,1,64,2,4,8])  
    if integer:
        return str(integer)
    else:
        return ""

def get_modifiers(chord, dictionary):
    control_characters = dictionary.get(chord, '')
    print(chord, control_characters)
    if control_characters:
        shift = True if "shift" in control_characters else False
        control = True if "control" in control_characters else False
        alt = True if "alt" in control_characters else False
        print(shift, control, alt)
        return "", shift, alt, control
    else:
        return chord, False, False, False
    
def get_lh_modifiers(lhs):
    return get_modifiers(lhs, LH_MODIFIERS)

def get_rh_modifiers(rhs):
    return get_modifiers(rhs, RH_MODIFIERS)

sticky_shift = False
sticky_alt = False
sticky_ctrl = False
def prep_translation(lhs, rhs, starred, numbar, suppress_space = True):
    global sticky_shift, sticky_alt, sticky_ctrl, CHAR_TO_KEYNAME
    if suppress_space:
        wrap = "{^}%s"
    else:
        wrap = "%s"
    lhs, sticky_shift1, sticky_alt1, sticky_ctrl1 = get_lh_modifiers(lhs)
    rhs, sticky_shift2, sticky_alt2, sticky_ctrl2 = get_rh_modifiers(rhs)

    sticky_shift |= sticky_shift2 | sticky_shift1 | sticky_shift
    sticky_ctrl |= sticky_ctrl2 | sticky_ctrl1 | sticky_ctrl
    sticky_alt |= sticky_alt2 | sticky_alt1 | sticky_alt
    
    print(sticky_shift, sticky_ctrl, sticky_alt)

    ltrans = LH_TRANS.get(lhs, "") if not numbar else lh2int(lhs, starred)

    if ltrans:
        if sticky_shift:
            ltrans = ltrans.upper()
            sticky_shift = False

    hash_braces = False
    if len(ltrans) > 1 and ltrans.startswith("#") and not numbar:
        hash_braces = True
        ltrans = ltrans[1:]

    if sticky_alt or sticky_ctrl and ltrans:
        ltrans = CHAR_TO_KEYNAME.get(ltrans, ltrans)
        hash_braces = False
        if sticky_alt and sticky_ctrl:
            ltrans = "{#control(alt(%s))}" % ltrans
            sticky_alt = False
            sticky_ctrl = False
        elif sticky_alt:
            ltrans = "{#alt(%s)}" % ltrans
            sticky_alt = False
        elif sticky_ctrl:
            ltrans = "{#control(%s)}" % ltrans
            sticky_ctrl = False

    if hash_braces:
        ltrans = "{#%s}" % ltrans
    ltrans = wrap % ltrans

        
    rtrans = RH_TRANS.get(rhs, "") if not numbar else rh2int(rhs, starred)
    if rtrans:
        if sticky_shift:
            rtrans = rtrans.upper()
            sticky_shift = False

    hash_braces = False
    if len(rtrans) > 1 and rtrans.startswith("#") and not numbar:
        rtrans = rtrans[1:]
        hash_braces = True

    if sticky_alt or sticky_ctrl and rtrans:
        rtrnas = CHAR_TO_KEYNAME.get(rtrans, rtrans)
        hash_braces = False
        if sticky_alt and sticky_ctrl:
            rtrans = "{#control(alt(%s))}" % rtrans
            sticky_alt = False
            sticky_ctrl = False
        elif sticky_alt:
            rtrans = "{#alt(%s)}" % rtrans
            sticky_alt = False
        elif sticky_ctrl:
            rtrans = "{#control(%s)}" % rtrans
            sticky_ctrl = False

    if hash_braces:
        rtrans = "{#%s}" % rtrans

    translation = ("%s%s" % (ltrans, rtrans)).replace("{^} ", "{^ ^}")
    print("(%s, %s)" % (ltrans, rtrans))
    return translation

def begin_alphabets(lhs, should_suppress_space, starred, numbar):
    global mode
    #TODO: Make it possible the left hand can start things capitalized, or trigger
    # vim mode etc.
    mode = NormalAlpha
    if should_suppress_space:
        space = "{^}"
    else:
        space = "{^ ^}"

    if lhs:
        lh_translation = prep_translation(lhs, None, starred, numbar,
                should_suppress_space)
    else:
        lh_translation = ""
    translation = space + \
                  "{PLOVER:SOLO_DICT:+doublefist.py,+cwtrans.json}" + \
                  lh_translation
    return translation

def end_alphabets(lhs, should_suppress_space, starred, numbar):
    global mode
    #TODO: could also specify things like supress space and capitalize
    # for the first normal steno stroke after the alphabet mode.
    mode = Steno
    if should_suppress_space:
        space = ""
    else:
        space = "{^ ^}"

    if should_suppress_space:
        space = "{^}"
    else:
        space = "{^ ^}"
    if lhs:
        lh_translation = prep_translation(lhs, None, starred, numbar,
                should_suppress_space)
    else:
        lh_translation = ""
    translation = prep_translation(lhs, None, starred, numbar) + \
                "{PLOVER:END_SOLO_DICT}" + \
                  space
    return translation

def vim_steno_append():
    global mode
    if mode == Steno:
        return "{^}a{^}"
    else:
        mode = Steno
        return "{PLOVER:END_SOLO_DICT}{^}a{^}"

def vim_steno_cap_append():
    global mode
    if mode == Steno:
        return "{^}A{^}"
    else:
        mode = Steno
        return "{PLOVER:END_SOLO_DICT}{^}A{^}"

def vim_steno_insert():
    global mode
    if mode == Steno:
        return "{^}i{^}"
    else:
        mode = Steno
        return "{PLOVER:END_SOLO_DICT}{^}i{^}"

def vim_steno_cap_insert():
    global mode
    if mode == Steno:
        return "{^}I{^}"
    else:
        mode = Steno
        return "{PLOVER:END_SOLO_DICT}{^}I{^}"

def cwtrans_count():
    return "{# k j comma c n p}",

def vim_escape():
    global mode
    if mode == Steno:
        mode = NormalAlpha
        return "{^}kj{^}{PLOVER:SOLO_DICT:+doublefist.py,+cwtrans.json}"
    else:
        return "{^}kj{^}"


def do_lookup(stroke):
    print("stroke", stroke)
    
    # quick kludge, do an actual top priority lookup here later
    if stroke == "R*R":
        return "\r"
    elif stroke == "SREFBG" or stroke == "SRUBGS":
        return vim_escape()
    elif stroke == "SRAPLD":
        return vim_steno_append()
    elif stroke == "SRAEPLD" or stroke == "SRA*PLD":
        return vim_steno_cap_append()
    elif stroke == "SREURPL" or stroke == "SRURPL":
        return vim_steno_insert()
    elif stroke == "SRAEURPL" or stroke == "STRAUPL":
        return vim_steno_cap_insert()
    elif stroke == "STPHOUPBT":
        return cwtrans_count()

    starred = True if "*" in stroke else False
    numbar = True if re.search(r"#|\d", stroke) else False
    print("starred", starred, "numbar", numbar)
    parts = PARTS_RE.match(stroke)
    if not parts:
        return None

    parts = parts.groupdict()
    rhs = parts["rhs"]
    lhs = parts["lhs"].replace("-","")
    if mode == Steno:
        if rhs not in TOGGLE_MODE_RH:
            return None
        elif rhs in TOGGLE_MODE_RH:
            should_suppress_space = TOGGLE_MODE_RH.index(rhs)
            chars_to_send = begin_alphabets(lhs, should_suppress_space,
                    starred, numbar)
            return chars_to_send
            # hang on...
    else: # if mode != Steno: See if we're switching back to it
        if rhs in TOGGLE_MODE_RH:
            should_suppress_space = TOGGLE_MODE_RH.index(rhs)
            chars_to_send = end_alphabets(lhs, should_suppress_space,
                    starred, numbar)
            return chars_to_send
    print(">>>>",lhs, rhs)

    # if we're still in normal alphabet mode by now:
    translation = prep_translation(lhs, rhs, starred, numbar)
    return translation
    

if __name__ == "__main__":
    print(lh2int("1234R50*", starred = True))
    print(lh2int("4", starred = True))
    print(rh2int("6", starred = True))
    print(rh2int("#EU", starred = False)) #right shape
    print(rh2int("6R", starred = False))

    testmirror("STPRA", "URPLS")
    testmirror("STKRO", "ERLGS")
    #star
    testmirror("STKRO*", "ERLGSZ") 
    #number + star
    testmirror("10", "E9")
    testmirror("10*", "E9D")
    #right order
    testmirror("ZSTKPWHRAO", "EUFRPBLGTS")
    #right order + numbers + star
    testmirror("12K3W4RAO", "EU6R7B8G9")
    testmirror("12K3W4RAO*", "EU6R7B8G9D")



# TODO: keep multistroke definitions from bleeding up from other dictionaries
#TODO: nonbackspaceable characters will need to know this
#NEEDS_SHIFT = [d[x] for x in list('!@#$%^&*()?_+{}|:\"~')]

# TODO: pull in converting everything to nonbackspaceables from the first
# vim dicitonary generator
# TODO: Expand numbers through 256
# TODO: Right hand sroke enders that end spelling mode after attaching common suffixes?
# Try a basic syllabary?

# NOT-TODO:
# Don't follow the rabbit hole of a right hand number bar substitute.
# It's assymetrical and breaks the consistency, which is unneccessary
# because any two digit number could be represented quickly on one
# hand anyway with enough practice. Digits just won't be mixed with
# other symbols in two hand strokes.
# On the other hand, using D or Z as a right hand * is going to be
# necessary unless I want to differentiate left and right *'s.
# And if I'm going to do that, might as well differentiate left and
# right number banks too, and then numbers and other characters could
# be mixed. Would only take adding maybe one more diode row to the steno
# mod if it doesn't happen to have 3 or 4 free inputs to do that and the
# left hand Z key. Which adds unpacking the soldering stuff to the list.
# Another day...
