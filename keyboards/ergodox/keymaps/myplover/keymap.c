#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"

#include "sendchar.h"
#include "virtser.h"

#include "version.h"

#ifdef MOUSEKEY_ENABLE
#include "mousekey.h"
#endif

#include "keycode.h"
#include "timer.h"

#include <avr/pgmspace.h>
#include "stenodictionary.h"

#define BASE 0 // default layer
#define CPLK 1
#define NENT 2  // symbols
#define PLVR 4 // plover layer
#define TXBOLT 5
#define MDIA 6  // media keys
#define NUMP 7 // directional keypad
#define DRNL 8 // directional keypad
#define LEAN 9 // keys you can lean on
#define SYMB 10//
#define FUNX 11//


#define MSPD1 9
#define MSPD2 10
#define MSPD3 12
#define MSPD4 13 // no binary stuff here
#define MSSLW 14 // no binary stuff here
#define MSJS1 15 // no binary stuff here
#define MSAC0 17 // no binary stuff here
#define MSPDp 58
#define ROLLBACK 99
#define DEBUG_DICTIONARY 254 

#define SYMBMOD 60
#define OUTEST 61
#define THUMB_SHIFT_TAB 62
#define THUMB_SHIFT_BS 63

#define EPRM M(1) // Macro 1: Reset EEPROM

/* ZSTK PWHR AO*# 1234 *
 * * * * * * * * * * * * 6789 #*UE FRPB LGTS
 * STGL BPRF UE*# 9876 *
 */

// TxBolt Codes

#define Sl 0b00000001
#define Tl 0b00000010
#define Kl 0b00000100
#define Pl 0b00001000
#define Wl 0b00010000
#define Hl 0b00100000

#define Rl 0b01000001
#define Al 0b01000010
#define Ol 0b01000100
#define X  0b01001000
#define Er 0b01010000
#define Ur 0b01100000

#define Fr 0b10000001
#define Rr 0b10000010
#define Pr 0b10000100
#define Br 0b10001000
#define Lr 0b10010000
#define Gr 0b10100000

#define Tr 0b11000001
#define Sr 0b11000010
#define Dr 0b11000100
#define Zr 0b11001000
#define NM 0b11010000

#define GRPMASK 0b11000000
#define GRP0 0b00000000
#define GRP1 0b01000000
#define GRP2 0b10000000
#define GRP3 0b11000000

//Split asterisk and 8 number bar key tracking constants
#define GRPX 0b11100000
                     
#define N1 0b11100011
#define N2 0b11100010
#define N3 0b11100001
#define N4 0b11100000
#define N6 0b11110000
#define N7 0b11110001
#define N8 0b11110010
#define N9 0b11110011
// Zl and Xr handled on their own with ifs, doesn't matter what the actual value is really.
#define Zl 0b11110100  //For split left S tracking
#define Xr 0b11110101


// Bit locations of keys on a mirror-image-consistent half steno keyboard
#define mZ 0x8000
#define mS 0x4000
#define mT 0x2000
#define mK 0x1000
#define mP 0x0800
#define mW 0x0400
#define mH 0x0200
#define mR 0x0100
#define mA 0x0080
#define mO 0x0040
#define mX 0x0020
#define mN 0x0010
#define m8 0x0008
#define m4 0x0004
#define m2 0x0002
#define m1 0x0001

uint16_t mCTL = mO;
uint16_t mALT = mA;
#define KC_LALF 0 //dummy define to prevent keypress but reuse modifier chord code for mode switching
uint16_t mSFT = m2;
uint16_t mModtoggle = m4;
uint16_t mALF = m4;
uint16_t mModlock = m8;
uint16_t mGUI = mZ;
uint16_t mod_mask = 0x80C3; //not sure if I need this...
/*0x0040
0x0080
0x0002
0x0001
0x80C3*/

#define green4   When a modifier chord is first pressed, its flag is set and its key registered.
#define green1   All modifier flags are checked on the release of a normal (translated) key, and any that are both not locked and not held down
#define green2   are unregistered. Not locked but held down, the flag is cleared, and if the flag is clear and not locked when the modifier
#define green3   key is released (not the whole chord just the modifier) then the key is also unregistered.
//
//To make this do a real capslock, it would have to be released and reinstated before number or symbol
// key presses occur. Can ergodox push an actual capslock? Could always have a capslock dictionary too, if things are done in such a way
// as to save space.

// temp_mod is simultaneous with a key or sticky for the next key,
// lock_mod holds the modifier key until a repeat of the chord triggers its release (like a capslock)
#define temp_modSFT               0x0001
#define temp_modCTL               0x0002
#define temp_modALT               0x0004
#define temp_modGUI               0x0008
#define temp_modNAV               0x0010
#define temp_modNUM               0x0020
#define temp_modMOS               0x0040
#define temp_modALF               0x0080

#define lock_modSFT               0x0100
#define lock_modCTL               0x0200
#define lock_modALT               0x0400
#define lock_modGUI               0x0800
#define lock_modNAV               0x1000
#define lock_modNUM               0x2000
#define lock_modMOS               0x4000
#define lock_modALF               0x8000
                            

#ifdef MOUSEKEY_ENABLE
uint8_t mousespeed = KC_MS_ACCEL2 ;
uint8_t mousespeedplus=0;
uint8_t acc_chord=0;
uint8_t trans_chord=0;
uint8_t mousekeys_down_c = 0;
uint16_t mdir1=0;
uint16_t mdir2=0;
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   =    |   1  |   2  |   3  |   4  |   5  | LEFT |           | RIGHT|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   W  |   E  |   R  |   T  |  L1  |           |  L1  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | BkSp   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L2|' / Cmd |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |Z/Ctrl|   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv/L1|  '"  |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  | ~L1  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | App  | LGui |       | Alt  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space|Backsp|------|       |------|  Tab   |Enter |
 *                                 |      |ace   | End  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_EQL,          KC_1,        KC_2,          KC_3,   KC_4,               KC_5,           KC_LEFT,
      LT(FUNX, KC_TAB),  KC_Q,        LT(NUMP,KC_W),         KC_E,   KC_R,       KC_T,           TO(NENT),
        TO(BASE), LT(NENT,KC_A),      LT(NUMP,KC_S), KC_D,   LT(FUNX,KC_F),      LT(FUNX,KC_G),
        KC_LSFT,         KC_Z,        KC_X,         KC_C,    LT(DRNL,KC_V),      LT(MDIA,KC_B),  LT(LEAN, KC_TAB),
        LT(NUMP,KC_GRV),KC_QUOT,    LCTL(KC_LSFT),  CTL_T(KC_BSPC),  ALT_T(KC_ENT), //TG(SWCH),
                                GUI_T(KC_APP),  KC_LGUI, KC_HOME,
                                M(THUMB_SHIFT_BS), CTL_T(KC_BSPC),KC_END,
                                //SFT_T(KC_BSPC), CTL_T(KC_BSPC),KC_END,
        // right hand
             TG(TXBOLT),     KC_6,             KC_7,      KC_8,          KC_9,    KC_0,          KC_MINS,
             TO(MDIA),     KC_Y,             KC_U,      KC_I,          KC_O,    KC_P,          GUI_T(KC_BSLS),
                 LT(SYMB, KC_H),             KC_J,      LT(SYMB, KC_K),          KC_L,    LT(MDIA, KC_SCLN),  LT(FUNX, KC_QUOT),
         LT(LEAN, KC_DELT),  LT(NENT,KC_N),   KC_M,        KC_COMM,       KC_DOT,  KC_SLSH,   KC_RSFT,
                          ALT_T(KC_SPC), CTL_T(KC_LEFT), MT(MOD_LCTL | MOD_LSFT, KC_DOWN),   KC_UP,  KC_RIGHT,
             KC_LALT,        GUI_T(KC_ESC),
             KC_PGUP,
             KC_PGDN,           CTL_T(KC_TAB), M(THUMB_SHIFT_TAB)
             //KC_PGDN,           CTL_T(KC_TAB), SFT_T(KC_TAB)
    ),

[CPLK] = KEYMAP(  // layer 0 : default
        // left hand
        KC_EQL,         KC_1,            KC_2,         KC_3,   KC_4,   KC_5,   KC_LEFT,
        KC_DELT,        S(KC_Q),         S(KC_W),         S(KC_E),   S(KC_R),   S(KC_T),   KC_TRNS,
        TO(BASE),       S(KC_A),         S(KC_S),         S(KC_D),   S(KC_F),   S(KC_G),
        KC_LSFT,        S(KC_Z),         S(KC_X),         S(KC_C),   S(KC_V),   S(KC_B),   KC_TRNS,
        LT(NENT,KC_GRV),KC_QUOT,    LCTL(KC_LSFT),  MT(MOD_LCTL | MOD_LALT, KC_LEFT),  ALT_T(KC_RGHT),
                                              ALT_T(KC_APP),  KC_LGUI,
                                                              KC_HOME,
                                               KC_SPC,KC_BSPC,KC_END,
        // right hand
             KC_TRNS,     KC_6,   KC_7,   KC_8,          KC_9,    KC_0,             KC_MINS,
             KC_TRNS,     S(KC_Y),   S(KC_U),   S(KC_I),          S(KC_O),    S(KC_P),             GUI_T(KC_BSLS),
                           S(KC_H),   S(KC_J),   S(KC_K),          S(KC_L),    LT(MDIA, KC_SCLN),  LT(FUNX, KC_QUOT),
             KC_TRNS, S(KC_N),   S(KC_M),   KC_COMM,       KC_DOT,  KC_SLSH,          KC_RSFT,
                          ALT_T(KC_UP), MT(MOD_LCTL | MOD_LALT, KC_DOWN), MT(MOD_LCTL | MOD_LSFT, KC_LBRC),   KC_RBRC,  KC_FN1,
             KC_LALT,        CTL_T(KC_ESC),
             KC_PGUP,
             KC_PGDN,KC_TAB, KC_ENT
    ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |Version |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | EPRM |      |      |      |      |                                       |      |    . |   0  |   =  |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
                                  |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */

// Numeric Data Entry

[NENT] = KEYMAP(
       // left hand
       KC_TRNS,    KC_TRNS,   KC_TRNS,                   KC_TRNS,          KC_TRNS,     KC_TRNS,    KC_TRNS,
       KC_TRNS,    KC_TRNS,   S(KC_TAB),                   KC_UP,          KC_TAB,    KC_TRNS,    TO(BASE),
       TO(BASE),   KC_TRNS,   KC_LEFT,                     KC_DOWN,        KC_RGHT,   KC_TRNS,    
       KC_TRNS,    KC_TRNS,   KC_TRNS,                     KC_TRNS,        KC_TRNS,   KC_TRNS,    KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, MT(MOD_LCTL | MOD_LALT, KC_BSPC), ALT_T(KC_ENT),
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_TRNS, KC_TRNS,       TO(BASE),       KC_PSLS,          KC_PAST,           KC_TRNS,          KC_TRNS,
       TO(MDIA),KC_TRNS,          KC_7,          KC_8,             KC_9,            KC_MINS,          KC_TRNS,
              LT(SYMB, KC_TRNS),  KC_4,          KC_5,             KC_6,              S(KC_EQL),       LT(FUNX, KC_EQL), 
       KC_TRNS, TO(MDIA),         KC_1,          KC_2,             KC_3,              KC_ENT,           KC_TRNS, 
                         ALT_T(KC_0),MT(MOD_LCTL | MOD_LALT, KC_DOT),  MT(MOD_LCTL | MOD_LSFT, KC_COMM),  LT(MDIA,KC_ENT), KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),



[PLVR] = KEYMAP(  // layout: layer 4: Steno for Plover
        // left hand
        KC_NO, KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
        TG(MDIA),  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_G,
        TO(BASE),KC_NO,  KC_Q,   KC_W,   KC_E,   KC_R,
        KC_NO,  KC_NO,  KC_A,   KC_S,   KC_D,   KC_F,   KC_G,
        KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO, 
                                      KC_FN4, KC_NO,
                                           KC_NO,
                                 KC_C,   KC_V,   KC_NO,
        // right hand
             KC_TRNS,  KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,   KC_TRNS,
               KC_H,    KC_6,    KC_7,    KC_8,  KC_9,    KC_0,    KC_TRNS,
                        KC_U,    KC_I,    KC_O,  KC_P,    KC_LBRC, KC_NO,
               KC_H,    KC_J,    KC_K,    KC_L,  KC_SCLN, KC_QUOT, KC_NO,
                              KC_TRNS, KC_TRNS,  KC_NO,   KC_NO,     KC_NO,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_NO,  KC_N,   KC_M
),

// TxBolt over Serial
[TXBOLT] = KEYMAP(
       M(DEBUG_DICTIONARY), KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
       KC_NO,   M(NM),   M(N1),   M(N2),   M(N3),   M(N4),  M(X),  
       KC_NO,   KC_NO,   M(Zl),   M(Tl),   M(Pl),   M(Hl),
       KC_NO,   KC_NO,   M(Sl),   M(Kl),   M(Wl),   M(Rl),   M(X),
       KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                           KC_NO,   KC_NO,  
                                                    KC_NO,  
                                  M(Al),   M(Ol),   KC_NO,  
    // right hand
       KC_TRNS,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
       M(Xr),    M(N6),   M(N7),   M(N8),   M(N9),   M(NM),   M(NM),
                 M(Fr),   M(Pr),   M(Lr),   M(Tr),   M(Dr), KC_NO,
       M(Xr),    M(Rr),   M(Br),   M(Gr),   M(Sr),   M(Zr), KC_NO,
                          KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
       KC_NO,   KC_NO,  
       KC_NO,  
       KC_NO,   M(Er),   M(Ur)
),

// MEDIA AND MOUSE
[MDIA] = KEYMAP(  
       KC_TRNS, KC_TRNS,        KC_TRNS,   KC_TRNS, KC_TRNS,      KC_TRNS,      KC_TRNS,
       KC_TRNS, M(ROLLBACK),    S(KC_TAB), KC_MS_U, KC_TAB,  KC_TRNS,      TO(NENT),
       KC_TRNS, KC_TRNS,       KC_MS_L, KC_MS_D, KC_MS_R, LT(FUNX, KC_TRNS), 
       KC_TRNS, KC_TRNS,        KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
       KC_TRNS, KC_TRNS,        KC_TRNS,   KC_BTN3, KC_BTN1, 
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                        KC_BTN2, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_VOLD,        KC_MUTE,         KC_VOLU,        KC_TRNS, KC_TRNS,
       TO(BASE), KC_TRNS, KC_WWW_BACK,    KC_MS_WH_UP,     KC_WWW_FORWARD,    KC_TRNS, KC_TRNS,
                 LT(SYMB, KC_TRNS), KC_MS_WH_LEFT,  KC_MS_WH_DOWN,   KC_MS_WH_RIGHT,  KC_TRNS, KC_MPLY,
       //KC_TRNS,  KC_MUTE, M(MSPD1),          M(MSPD2),     M(MSPD3),       M(MSPD4),  KC_TRNS,
       KC_TRNS,  TO(NENT), M(MSJS1),      M(MSAC0),    KC_MS_ACCEL2, M(MSPD4),  KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),

//modes not meant for permanent residence, accessed by momentary switch
[NUMP] = KEYMAP(
       // left hand
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,     
                                      KC_TRNS, KC_TRNS,
                                           KC_TRNS,
                                 KC_TRNS,   KC_TRNS,   KC_TRNS,
       // right hand
       KC_TRNS, KC_TRNS,        KC_TRNS,       KC_PSLS,          KC_PAST,           KC_TRNS,          KC_TRNS,
       KC_TRNS, KC_TRNS,          KC_7,          KC_8,             KC_9,            KC_MINS,          KC_TRNS,
                KC_TRNS,          KC_4,          KC_5,             KC_6,            S(KC_EQL),       LT(FUNX, KC_EQL), 
       KC_TRNS, KC_TRNS ,         KC_1,          KC_2,             KC_3,            KC_MINS,           KC_TRNS, 
                         ALT_T(KC_0),MT(MOD_LCTL | MOD_LALT, KC_DOT),  MT(MOD_LCTL | MOD_LSFT, KC_COMM),  LT(MDIA,KC_ENT), KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, SFT_T(KC_BSLS)
),

[DRNL] = KEYMAP(  // Blank template keymap
        // left hand
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,     
                                      KC_TRNS, KC_TRNS,
                                           KC_TRNS,
                                 KC_TRNS,   KC_TRNS,   KC_TRNS,
        // right hand
             KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
             KC_TRNS,       KC_TRNS,      KC_KP_7,     KC_KP_8,    KC_KP_9,    KC_TRNS,      KC_TRNS,
                        LT(SYMB, KC_TRNS),KC_KP_4,     KC_KP_5,    KC_KP_6,    KC_TRNS,      KC_TRNS,
             KC_TRNS,       TO(NENT),     KC_KP_1,     KC_KP_2,    KC_KP_3,    KC_TRNS,      KC_TRNS,
                            KC_TRNS,      KC_KP_0,     KC_DOT,  KC_EQL,  KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS,  KC_TRNS,   KC_TRNS
),

[LEAN] = KEYMAP(  // layer 0 : default
        // left hand
        KC_EQL,         KC_1,         KC_2,         KC_3,   KC_4,   KC_5,   KC_LEFT,
        KC_DELT,        KC_Q,         KC_W,         KC_E,   KC_R,   KC_T,   KC_TRNS,
        KC_TRNS,        KC_A,         KC_S,         KC_D,   KC_F,   KC_G,
        KC_LSFT,        KC_Z,         KC_X,         KC_C,   KC_V,   KC_B,   KC_TRNS,
        KC_GRV,         KC_QUOT,      LALT(KC_LSFT),  KC_LEFT,KC_RGHT,
                                                    KC_APP,  KC_LGUI,
                                                              KC_HOME,
                                               KC_SPC,KC_BSPC,KC_END,
        // right hand
             KC_TRNS,     KC_6,   KC_7,   KC_8,          KC_9,    KC_0,             KC_MINS,
             KC_TRNS,     KC_Y,   KC_U,   KC_I,          KC_O,    KC_P,             KC_BSLS,
                          KC_H,   KC_J,   KC_K,          KC_L,    KC_SCLN,          KC_QUOT,
             KC_TRNS, KC_N,   KC_M,   KC_COMM,       KC_DOT,  KC_SLSH,          KC_RSFT,
                                  KC_UP,  KC_DOWN,KC_LBRC,KC_RBRC,          KC_FN1,
             KC_LALT,        KC_ESC,
             KC_PGUP,
             KC_PGDN,KC_TAB, KC_ENT
    ),

[SYMB] = KEYMAP(
       // left hand
       M(0),   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_TRNS,
       KC_TRNS,KC_EXLM,KC_AT,  KC_LBRC,KC_RBRC,KC_PIPE,KC_TRNS,
       KC_TRNS,KC_HASH,KC_DLR, KC_9,   KC_0,   KC_GRV,
       KC_TRNS,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_BSLS,KC_TRNS,
          EPRM,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               M(THUMB_SHIFT_BS),KC_TRNS,KC_TRNS,
       // right hand
       KC_TRNS, KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,  KC_TRNS,
       KC_TRNS, KC_TRNS,   KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,
                KC_TRNS,   KC_ESC,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,   KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,
                           KC_TRNS,  KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, M(THUMB_SHIFT_TAB)
),


[FUNX] = KEYMAP(  // Blank template keymap
        // left hand
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_ESC,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,     
                                      KC_TRNS, KC_TRNS,
                                           KC_TRNS,
                       SFT_T(KC_BSPC),   KC_LCTL,   KC_TRNS,
        // right hand
             KC_TRNS,       KC_TRNS,      KC_F10,    KC_F11,       KC_F12,    KC_PSCR,      KC_TRNS,
             KC_TRNS,       KC_TRNS,      KC_F7,     KC_F8,        KC_F9,     KC_F11,      KC_TRNS,
                            KC_F11,      KC_F4,     KC_F5,        KC_F6,     KC_F12,      KC_TRNS,
               KC_TRNS,     KC_F12,     KC_F1,     KC_F2,        KC_F3,     KC_TRNS,      KC_TRNS,
                                        KC_F10,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS,  KC_LCTL,   SFT_T(KC_TAB)
),

/*[BLNK] = KEYMAP(  // Blank template keymap
        // left hand
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,     
                                      KC_NO, KC_NO,
                                           KC_NO,
                                 KC_C,   KC_V,   KC_NO,
        // right hand
             KC_TRNS,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
             KC_TRNS,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
                            KC_NO,      TO(MDIA),   TO(VIMK),   KC_NO,      KC_NO,      KC_NO,
               KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
                            KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO,  KC_NO,   KC_NO
),
*/
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

#ifdef MOUSEKEY_ENABLE
void mouseaccel_off(){
    mousekey_off(KC_MS_ACCEL0);
    mousekey_off(KC_MS_ACCEL1);
    mousekey_off(KC_MS_ACCEL2);
};

void s_chord_plus(uint8_t mspd) {
    mspd &= 7;
    acc_chord |= mspd;
    trans_chord |= mspd;
};

void mousebump(int16_t dir1, int16_t dir2, uint8_t cycles, uint8_t speed){
    int i;
    mousekey_off(mousespeed);
    mousekey_on(speed);
    if (dir2 == -1) {
        for(i=0; i < cycles; i++){
            mousekey_on(dir1);
            mousekey_send();
        }
        if (!mousekeys_down_c) mousekey_off(dir1);
    } else {
        for(i=0; i < cycles; i++){
            mousekey_on(dir1);
            mousekey_on(dir2);
            mousekey_send();
        }
        if (!mousekeys_down_c) {
            mousekey_off(dir1);
            mousekey_off(dir2);
        }
    }
    mousekey_send();
    mousekey_off(speed);
    mousekey_on(mousespeed);
    mousekey_send();
};


void s_chord_minus(uint8_t mspd) {
    mspd &= 7;
    trans_chord ^= mspd;
    if(trans_chord == 0 && acc_chord != 0) {
        if(acc_chord == 1 || acc_chord == 2 || acc_chord == 4) {
            mousekey_off(mousespeed);
            switch(acc_chord) {
                case 1: mousespeed = KC_MS_ACCEL0; break;
                case 2: mousespeed = KC_MS_ACCEL1; break;
                case 4: mousespeed = KC_MS_ACCEL2; break;
            }
            if (mousekeys_down_c) {
                mousekey_on(mousespeed);
                mousekey_send();
            }

        } else {
            switch(acc_chord){
                case 3:
                    mousebump(mdir1, mdir2, 15, KC_MS_ACCEL1); break;
                case 5:
                    mousebump(mdir1, mdir2, 20, KC_MS_ACCEL1); break;
                case 6:
                    mousebump(mdir1, mdir2, 18, KC_MS_ACCEL2); break;
                case 7:
                    mousebump(mdir1, mdir2, 30, KC_MS_ACCEL2); break;
            }
        }
        acc_chord = 0;
    }

};


void movemouse(int16_t dir1, int16_t dir2, uint8_t keyon, uint8_t speed){
    if (keyon == 1) {
        if(speed != -1) {
            mousekey_on(speed);
        }
        mousekeys_down_c += 1 + (0 ? dir2 == -1 : 1);
    } else {
        mousekeys_down_c -= (1 + (0 ? dir2 == -1 : 1));
        if (!mousekeys_down_c) mousekey_off(speed);
    }

    if (keyon == 1) mousekey_on(dir1);
              else mousekey_off(dir1);
    if (dir2 != -1) {
        if (keyon == 1) mousekey_on(dir2);
                  else mousekey_off(dir2);
    }
    mousekey_send();
};
    
uint8_t lastmousekey = 0;
uint8_t mousekeyisdown = 0;
uint8_t mousenudgeison = 0;
uint8_t mouseslowon = 0;
static uint16_t msslw_timer = 0;

void onmousedown(int8_t id){
    acc_chord += 1;
    mousekeyisdown += 1;
    if (lastmousekey != id){
        mousekey_settimer(2);
    }
    lastmousekey = id;
}

void onmouseup(int8_t id){
    acc_chord -= 1;
    mousekeyisdown -= 1;
}    

#endif //end mouse conditional block


uint8_t bs_pressed = false;
uint8_t bs_willsend = true;
static uint16_t bs_timer = -1;

uint8_t tab_pressed = false;
uint8_t tab_willsend = true;
static uint16_t tab_timer = -1;

uint8_t reset_bs_tab_ok = 1;

//  TXBOLT copy paste starts here {
uint8_t chord[4] = {0,0,0,0};
uint8_t pressed_count = 0;

#define LEFT 0
#define RIGHT 1
struct steno_halves {
    uint16_t chord;
    uint16_t curchord;
    uint16_t prevcurchord;
    uint8_t mods;  //Modifiers currently sticky, will be consumed on the next keycode translation.
    uint8_t mods_awaiting_toggle;   //Will trigger if modToggle pushed
    uint8_t mods_awaiting_opp_dsqf; //Modifiers awaiting opposite hand being disqualified to be a modifier with non mod keys {
    uint8_t mods_to_be_cleared_on_mod_up;  //Mods go here after being consumed at least once.
    uint8_t disqualified_keys_pressed;
    uint8_t pressed_count;
};

struct steno_halves halves[2] = { {0,0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0,0} };
#define lchord halves[LEFT].chord
#define rchord halves[RIGHT].chord
#define lcurchord halves[LEFT].curchord
#define rcurchord halves[RIGHT].curchord
#define lpressed_count halves[LEFT].pressed_count
#define rpressed_count halves[RIGHT].pressed_count
#define lprevcurchord halves[LEFT].prevcurchord
#define rprevcurchord halves[RIGHT].prevcurchord

uint16_t steno_state = 0;
uint8_t suppress_next_bolt = 0;
uint8_t allow_bolt = 1;
uint8_t did_firmware_translation_during_stroke = 0;

void send_chord(void)
{
  for(uint8_t i = 0; i < 4; i++)
  {
    if(chord[i])
      virtser_send(chord[i]);
  }
  virtser_send(0);
}
// }

uint16_t bin_mirror(int n, int bits) {
    int out = 0;
    int bit = 0;
    for (int i = 0; i < bits; i++) {
        bit = n & 1;
        n = n >> 1;
        if (bit == 1) out = out | (1 << (bits - i - 1));
    }
    return out;
}


#define DEBUG_TAP(kc) register_code(KC_ ## kc); wait_ms(1); unregister_code(KC_ ## kc)
#define QUICK_TAP(kc) register_code(kc); wait_ms(1); unregister_code(kc)
#define SHIFT_TAP(kc) register_code(KC_LSFT); register_code(kc); wait_ms(5); unregister_code(kc); unregister_code(KC_LSFT)
void debug_chord(uint16_t extchord)
{
    extchord = bin_mirror(extchord, 16);
    for(uint8_t i = 0; i < 16; i++)
    {
        if ((extchord & 1) == 1) {
            QUICK_TAP(KC_1);
        } else {
            QUICK_TAP(KC_0);
        }
        extchord = extchord >> 1;        
        if (i % 8 == 7) {
            QUICK_TAP(KC_SPC);
        } else if (i % 4 == 3) {
            QUICK_TAP(KC_MINS);
        }
    }
    QUICK_TAP(KC_ENT);
}

void debug_translate_and_send(uint16_t chord)
{
    uint16_t keycode = pgm_read_word_near(chorddict + chord);
    if (keycode != 0) {
       QUICK_TAP(keycode);
       QUICK_TAP(KC_ENT);
    } 
    QUICK_TAP(keycode);
    debug_chord(keycode);
    QUICK_TAP(KC_B);
    debug_chord(KC_B);
}


// Called when a translated key is sent down the USB
void handle_modifier_after_tap(uint16_t modifier_chord_key, uint16_t keycode, struct steno_halves * opst_half,
                               uint16_t temp_mod_bit, uint16_t lock_mod_bit) {
    if (temp_mod_bit & steno_state) { // the steno_state thing is removed when modifier is unregistered
        if (!(lock_mod_bit & steno_state)) {
            DEBUG_TAP(9);
            if (!(opst_half->mods & temp_mod_bit)) {//...and a mod is not being held down in the opposite hand chord:
                steno_state &= ~temp_mod_bit;
                unregister_code(keycode);
            } else {
                opst_half->mods_to_be_cleared_on_mod_up |= temp_mod_bit;
            }
        }
    }
}
#define HANDLE_MODIFIER_AFTER_TAP(mod_name, opst_half) \
        handle_modifier_after_tap(m ## mod_name, KC_L ## mod_name, opst_half, temp_mod ## mod_name, lock_mod ## mod_name)
#define HANDLE_ALL_MODIFIERS_AFTER_TAP() \
           HANDLE_MODIFIER_AFTER_TAP(SFT, opst_half); \
           HANDLE_MODIFIER_AFTER_TAP(CTL, opst_half); \
           HANDLE_MODIFIER_AFTER_TAP(ALT, opst_half); \
           HANDLE_MODIFIER_AFTER_TAP(GUI, opst_half); \

void tap_key(uint16_t * keyarray, struct steno_halves * opst_half)
{
    if (keyarray) {
       uint16_t length = pgm_read_word_near(keyarray);
       uint8_t shifted = 0;
       uint16_t keycode = 0;

       for (uint8_t i = 1; i <= length; i++) {
           keycode = pgm_read_word_near(keyarray + i);
           if (keycode == KC_LSFT) {
               register_code(KC_LSFT);  //Not sure what happens if shift is registered twice, will find out
               shifted = 1;
           } else {
               QUICK_TAP(keycode);
               did_firmware_translation_during_stroke = 1;}
       }
       if (shifted && !(steno_state & mSFT)) unregister_code(KC_LSFT);
       if (did_firmware_translation_during_stroke) {
           if (steno_state & 0x00FF) { // if any modifier key or ALF has just been pressed down
               //|   ********** Watch out for the bitmask above if you're adding new modifiers/modeswitchers
               DEBUG_TAP(8);
               HANDLE_ALL_MODIFIERS_AFTER_TAP();
               if (!(lock_modALF & steno_state)) {
                   if (opst_half->mods & temp_modALF) {
                       opst_half->mods_to_be_cleared_on_mod_up |= temp_modALF;
                       DEBUG_TAP(F);
                   } else {
                       steno_state &= ~temp_modALF;
                       DEBUG_TAP(C);
                   }
               }
           }
       }
    }
}

uint16_t * translate(uint16_t chord)
{
   uint16_t * data = pgm_read_word_near(chorddict + chord);
   if (data) {
       uint8_t length = pgm_read_word_near(data);
       if (length) {
           return data; // Changed it to return the actual pointer whose first element is length
       } else {
           return 0; // Or zero if it's a length zero record, can't remember when that would happen though
       }
   } else{
       return 0; // Zero if the chord is undefined
   }
}


// It's not enough to look for the ring finger modifier being pushed. Must also ensure that other unallowed keys are not being pressed.
//
// Called whenever a key is pressed or released with current, unreleased chord state
//0b0111 1111 0010 0000 Bits representing keys that wouldn't be held to trigger modifiers
//0x   7    F    2    0

//0b0111 1111 0010 0000 Bits representing keys that wouldn't be held to trigger modifiers
//0x   7    F    2    0
#define NON_MOD_MASK 0x7420

void control_one_modifier(uint16_t modifier_chord_key, uint16_t keycode,
                          struct steno_halves * half, struct steno_halves * opst_half,
                          uint16_t temp_mod_bit, uint16_t lock_mod_bit, uint16_t newkey, uint8_t pressed) {
    if (newkey & modifier_chord_key) {
      if (pressed) { // if newkey was pressed down
                  //it almost doesn't matter if it was a down press or not... we're not singling out the key that was just pressed,
                  // but the entire chord. Is that what I wanted to do? It would be helpful to know which key changed and whether it
                  // was up or down. 
          // big TODO: make it so a second keypress when a modifier is left sticky and unused cancels the modifier.
          // But make sure everything else works first.
        if (!(steno_state & lock_mod_bit)) { // mod was pushed, not already active
            DEBUG_TAP(1);
            if (half->mods & temp_modALF) {
                DEBUG_TAP(2);
                if (!(opst_half->mods & temp_modALF)) {
                    DEBUG_TAP(3);
                    register_code(keycode);
                    half->mods |= temp_mod_bit;
                    steno_state |= temp_mod_bit;
                } else {
                    DEBUG_TAP(4);
                    half->mods_awaiting_opp_dsqf |= temp_mod_bit; } 
            } else {
                DEBUG_TAP(5);
                half->mods_awaiting_toggle |= temp_mod_bit; }
        }
      } else {       // if newkey was just released 
        half->mods &= ~temp_mod_bit; // Remove the temp_mod_bit when it's released on any key up
                                    // That bit on a half struct just means the key is being held down.
        if (!(steno_state & lock_mod_bit) && half->mods_to_be_cleared_on_mod_up & temp_mod_bit) {
            DEBUG_TAP(6);            
            steno_state &= ~temp_mod_bit;
            unregister_code(keycode);// Only unregister the key if the modifier has been consumed at least once.
        } // else, nothing changes, modifier is stuck until a translated character consumes it
      }
    }
}

#define REGISTER_KEY_IF_IN_MASK(mod_name, mask) \
    if (m ## mod_name & mask) { register_code(KC_L ## mod_name); }

#define REGISTER_EACH_KEY_IN_MASK(mask) \
    REGISTER_KEY_IF_IN_MASK(SFT, mask); \
    REGISTER_KEY_IF_IN_MASK(CTL, mask); \
    REGISTER_KEY_IF_IN_MASK(ALT, mask); \
    REGISTER_KEY_IF_IN_MASK(GUI, mask)

// This macro has three conditions for clearing a modifier key:
//  It was not in lock mode.
//  It was being held by the current hand.
//  It was not also being held by the opposite hand.
// Modifiers in this condition are cleared of:
//   their key down state
//   the steno_state flag for the temporary modifier
#define CLEAR_MODIFIER_IF_UNLOCKED(mod_name, half, opst_half) \
    if (!(lock_mod ## mod_name & steno_state) && \
         (temp_mod ## mod_name & half->mods) && \
         !(temp_mod ## mod_name & opst_half->mods)) \
            { unregister_code(KC_L ## mod_name); \
              steno_state &= ~temp_mod ## mod_name; }

#define CLEAR_EACH_UNLOCKED_MODIFIER(half, opst_half) \
    CLEAR_MODIFIER_IF_UNLOCKED(SFT, half, opst_half); \
    CLEAR_MODIFIER_IF_UNLOCKED(CTL, half, opst_half); \
    CLEAR_MODIFIER_IF_UNLOCKED(ALT, half, opst_half); \
    CLEAR_MODIFIER_IF_UNLOCKED(GUI, half, opst_half); 

#define CONTROL_ONE_MODIFIER(mod_name, half, opst_half, newkey, pressed) \
        control_one_modifier(m ## mod_name, KC_L ## mod_name, half, opst_half, temp_mod ## mod_name, lock_mod ## mod_name, newkey, pressed)

void control_temporary_modifiers(struct steno_halves * half, struct steno_halves * opst_half, uint16_t newkey, uint8_t pressed) {
    // FIXME Modifiers aren't actually triggering
    if ((half->chord & NON_MOD_MASK) && (half->chord & mModtoggle)) {
        // clear each modifier that is not locked, not the whole keyboard
        //half->mods_awaiting_opp_dsqf      |= half->mods;  Why save state for later on this hand?
        // It's never going to be used until key up again.
        // This is where the hand becomes disqualified, if there are mods awaiting opposite disqualification, this is where they must be trigger
        //
        REGISTER_EACH_KEY_IN_MASK(opst_half->mods_awaiting_opp_dsqf);
        opst_half->mods |= opst_half->mods_awaiting_opp_dsqf;  // not sure if I need to save it in these things or not...
        steno_state |= opst_half->mods_awaiting_opp_dsqf;
        opst_half->mods_awaiting_opp_dsqf = 0;
        CLEAR_EACH_UNLOCKED_MODIFIER(half, opst_half);
        // clear out all temporary modifier held keys triggered by current hand only, but only if the opposite hand
        // is not still modifying.
        half->mods = 0;
        half->disqualified_keys_pressed = 1;
    } else {
        if (newkey & mModtoggle) {
            if (pressed) { // probably more than one way to do this would still work
                half->mods_to_be_cleared_on_mod_up = 0;
                if (half->mods_awaiting_toggle) {
                    REGISTER_EACH_KEY_IN_MASK(half->mods_awaiting_toggle);
                    half->mods |= half->mods_awaiting_toggle;
                    steno_state |= half->mods_awaiting_toggle;
                    half->mods_awaiting_toggle = 0;
                }
                if (!(steno_state & lock_modALF)) {
                    steno_state |= temp_modALF;
                    half->mods |= temp_modALF;
                }
            } else { //if key released
                //if (did_firmware_translation_during_stroke && !(steno_state & lock_modALF)) {
                half->mods &= ~temp_modALF;  //Clear that half's flag either way, because that shows that
                                            //mModtoggle, even if it did qualify, is now released.
                if (half->mods_to_be_cleared_on_mod_up & temp_modALF){
                    DEBUG_TAP(Z);
                    steno_state &= ~temp_modALF; // Unsticky the alpha mode for next press, because it has been consumed.
                }
            }
        } else {  // allow these whether or not in alf mode so they can add to waiting lists
            CONTROL_ONE_MODIFIER(SFT, half, opst_half, newkey, pressed);
            CONTROL_ONE_MODIFIER(CTL, half, opst_half, newkey, pressed);
            CONTROL_ONE_MODIFIER(ALT, half, opst_half, newkey, pressed);
            CONTROL_ONE_MODIFIER(GUI, half, opst_half, newkey, pressed);
        }
    }
}


void dump_dictionary()
{
    if (0) {  //don't actually do this for now, save for diagnostics
        uint16_t keycode = 0;
        for (uint16_t i = 0; i < 1023; i++){
            keycode = pgm_read_word_near(chorddict + i);
            if (keycode != 0) {
                QUICK_TAP(keycode);
            } else {
                QUICK_TAP(KC_0);
            }
        }
    }
}

bool process_record_user (uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        pressed_count++; //         TXBOLT line
        if (reset_bs_tab_ok) {
            tab_timer = -1;
            bs_timer = -1;
            tab_willsend = false;
            bs_willsend = false;
        }
    } else {
        pressed_count--; //         TXBOLT line
    }
    return true;
}

#define L_TRACK_COUNT() lchord = lchord | newkey; lcurchord = lcurchord ^ newkey; leftkeypressed = 1;\
                        if (newkey) { if (pressed) lpressed_count++; else lpressed_count--; }
#define R_TRACK_COUNT() rchord = rchord | newkey; rcurchord = rcurchord ^ newkey; rightkeypressed = 1;\
                        if (newkey) { if (pressed) rpressed_count++; else rpressed_count--; }
#define BOLTMASK 0b00111111

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function

      uint8_t boltstenokey = id;
      uint8_t layer = biton32(layer_state);
      uint8_t pressed = record->event.pressed;
      if (layer==TXBOLT){
          if (pressed && id == DEBUG_DICTIONARY){
                dump_dictionary();
          }
          if (id >= GRPX) {
              if (id == Xr) {
                  boltstenokey = X;
              } else if (id == Zl) {
                  boltstenokey = Sl;
              } else if (id >= N4 && id <= N9) { //N4 is actually the lowest value, makes binary stuff later cleaner
                  boltstenokey = NM;
              }
          }/* else {
              boltstenokey = id;
          } // set to id at initialization */

          uint8_t grp = (boltstenokey & GRPMASK) >> 6;
          if (pressed) chord[grp] |= boltstenokey;
          // That covers original bolt functionality, next add keys to the extended chord.
          uint16_t newkey = 0;
          uint8_t leftkeypressed = 0;
          uint8_t rightkeypressed = 0;
          if (id < NM) {
              if (id <= X) {
                  newkey = (BOLTMASK & id) << ((6 * grp)+1);
                  newkey = bin_mirror(newkey, 16);
                  L_TRACK_COUNT();
              } else if (id <= Zr) {
                  if (grp == 1) { // still an E or U)
                      newkey = (BOLTMASK & id) << 2; 
                  } else {
                      newkey = (BOLTMASK & id) << 8;
                      if (id >= Tr) newkey = newkey << 6;
                      if (id == Pr || id == Lr) {
                          newkey = newkey << 1;
                      } else if ((id % 2) == 0) {
                          newkey = newkey >> 1;
                      } else {
                          newkey = newkey << 1;
                      }
                  }
                  R_TRACK_COUNT();
              }
          } else if (id < Zl) { /** Extended keys handling: num bar first **/
                              // this \/ make sure sided num bar indicator is set too
              newkey = (1 << (0x0F & id));
              if ((0xF0 & id) == 0xF0) { R_TRACK_COUNT(); }
              else                     { L_TRACK_COUNT(); }
          } else if (id == Zl) {
              newkey = 0x8000; /*                           top left S */
              L_TRACK_COUNT();
          } else if (id == Xr) {
              newkey = 0x20;   /*                              right * */
              R_TRACK_COUNT();
          }
          /*// checking progress of l/rcurchord
          debug_chord(lcurchord); debug_chord(rcurchord); QUICK_TAP(KC_ENT);*/

          // Something around this section's messed up. When switching back to steno, if no other key is held down, it takes
          // two additional alpha keystrokes to change back to steno.
          //
          // Probable place that's wrong: control_temporary_modifiers needs to differentiate
          // between key up and key down. Locking of modifiers should happen on key up only.
          if (newkey) {
              if (leftkeypressed && !halves[LEFT].disqualified_keys_pressed)
                  control_temporary_modifiers(&halves[LEFT], &halves[RIGHT], newkey, pressed);
              if (rightkeypressed && !halves[RIGHT].disqualified_keys_pressed)
                  control_temporary_modifiers(&halves[RIGHT], &halves[LEFT], newkey, pressed);

              /*if (pressed) {
                  if (leftkeypressed) {
                      QUICK_TAP(KC_L);
                  } else {
                      QUICK_TAP(KC_R);}
                  debug_chord(newkey);
              }*/
          }

          if (!pressed) {
              //  These parts give independent chord releasing of each keyboard half
              if (steno_state & (temp_modALF | lock_modALF)) { // If any alphabet modifier was recently pushed, keep doing this
                  if (lpressed_count == 0 && lchord) {       //                                instead of sending bolt chords.
                      if (lchord == mX) { QUICK_TAP(KC_BSPC); did_firmware_translation_during_stroke = 1;
                      } else if (!(lchord & mModtoggle)) { tap_key(translate(lchord>>6), &halves[RIGHT]); }
                      //QUICK_TAP(KC_L); debug_chord(lchord); QUICK_TAP(KC_ENT);
                      lchord = 0;
                      halves[LEFT].mods_awaiting_toggle = halves[LEFT].mods_awaiting_opp_dsqf = \
                          halves[LEFT].disqualified_keys_pressed = halves[LEFT].mods_to_be_cleared_on_mod_up = 0;
                  }
                  if (rpressed_count == 0 && rchord) {
                      //TODO fix asterisk backspaces so they don't go off with locked modifiers, but do put the modifier(s) back
                      //once they're done.
                      if (rchord == mX) { QUICK_TAP(KC_BSPC); did_firmware_translation_during_stroke = 1;
                      } else if (!(rchord & mModtoggle)) { tap_key(translate(rchord>>6), &halves[LEFT]); }
                      // TODO TODO also // this // probably doesn't work the way I thought it would
                      // and then these two pieces can probably get refactored down pretty easily
                      //QUICK_TAP(KC_R); debug_chord(rchord); QUICK_TAP(KC_ENT);
                      rchord = 0;
                      halves[RIGHT].mods_awaiting_toggle = halves[RIGHT].mods_awaiting_opp_dsqf = \
                          halves[RIGHT].disqualified_keys_pressed = halves[RIGHT].mods_to_be_cleared_on_mod_up = 0;
                  }
              }
          }
          if (!pressed) {  // Does this need to be else? could also bleed through if other thing doesn't work.
              if (lpressed_count == 0) lchord = 0;
              if (rpressed_count == 0) rchord = 0;
              if (pressed_count == 0) {
                  if (did_firmware_translation_during_stroke) {
                      did_firmware_translation_during_stroke = 0;
                      // Discard bolt transmission if firmware translates some or all of the entire two handed chord
                      // TODO:
                      // Last but not least, send a suppress space chord on the way out of alphabet locked mode
                      // But not on the way out of alphabet temporary/sticky once mode.
                  } else if (!steno_state) { //if there's no modifiers to worry about, and nothing just translated through firmware...
                      send_chord();
                      DEBUG_TAP(B);
                      //QUICK_TAP(KC_B); debug_chord(lchord); QUICK_TAP(KC_B); debug_chord(rchord); QUICK_TAP(KC_ENT); // uncomment to watch chords without plover translating
                      clear_keyboard();
                      chord[0] = chord[1] = chord[2] = chord[3] = 0;
                      lchord = rchord = lcurchord = rcurchord = 0;
                  }
              }
          }
          return MACRO_NONE;

      }

      switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
        case 1:
        if (record->event.pressed) { // For resetting EEPROM
          eeconfig_init();
        }
        break;
        case THUMB_SHIFT_BS:
            if (record->event.pressed) {
                bs_pressed = true;
                if (bs_timer != -1 && timer_elapsed(bs_timer) < 150) {
                    reset_bs_tab_ok = 0;
                    //register_code (KC_BSPC);
                    register_code (KC_X);
                    bs_willsend = false;
                } else {
                    register_code (KC_LSFT);
                }
                bs_willsend = true;
            } else {
                // easier solution: other key doubles as KC_LSFT
                unregister_code (KC_LSFT);
                bs_pressed = false;
                if (bs_willsend) {
                    register_code (KC_BSPC);
                    unregister_code (KC_BSPC);
                    bs_timer = timer_read();
                }
                unregister_code (KC_BSPC);
                reset_bs_tab_ok = 1;
            }
            break;
        case THUMB_SHIFT_TAB:
            if (record->event.pressed) {
                tab_pressed = true;
                if (tab_timer != -1 && timer_elapsed(tab_timer) < 150) {
                    register_code (KC_TAB);
                    tab_willsend = false;
                } else {
                    register_code (KC_RSFT);
                }
                tab_willsend = true;
            } else {
                // easier solution: other key doubles as KC_LSFT
                unregister_code (KC_RSFT);
                tab_pressed = false;
                if (tab_willsend) {
                    register_code (KC_TAB);
                    unregister_code (KC_TAB);
                    tab_timer = timer_read();
                }
                unregister_code (KC_TAB);
            }
            break;
        case OUTEST:
            if (record->event.pressed) {
                char* stringstore = malloc(32*sizeof(char));
                snprintf(stringstore, 32, "asdf%dz%dz%d", record->event.key.row, record->event.key.col, record->event.pressed);
                //SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                send_string(stringstore);
                //SEND_STRING (itoa(32));
                free(stringstore);
            } else {
                layer_off(SYMB);
            }
            break;

        case SYMBMOD:
            if (record->event.pressed) {
                layer_on(SYMB);
            } else {
                layer_off(SYMB);
            }
            break;

#ifdef MOUSEKEY_ENABLE
        case MSJS1:
            if (record->event.pressed) {
                mousekey_on(KC_MS_ACCEL_JUST1);
            } else {
                mousekey_off(KC_MS_ACCEL_JUST1);
                mousekey_settimer(0);
            }
            break;
        case MSAC0:
            if (record->event.pressed) {
                mousekey_on(KC_MS_ACCEL0);
                mousekey_settimer(0);
            } else {
                mousekey_off(KC_MS_ACCEL0);
                mousekey_settimer(0);
            }
            break;
        case MSSLW:
            if (record->event.pressed) {
                mousekey_on(KC_MS_ACCEL0);
                mouseslowon = 1;
                msslw_timer = timer_read();
            } else {
                if(mouseslowon == 1){
                    mousekey_off(KC_MS_ACCEL0);
                    mouseslowon = 0;
                }
                if(timer_elapsed(msslw_timer) < 120){
                    mousenudgeison = ~mousenudgeison;
                    if(mousenudgeison) { // mk_delay stuff didn't work
                        mousekey_on(KC_MS_ACCEL_JUST1);
                        // TODO modify mousekey.c and .h  again, give it a skip steps parameter
                        // which does nothing if zero, returns early from mouse_task if any other
                        // value and not modulus zero of a counter variable.
                    //    mk_delay = 100;
                    } else {
                        mousekey_off(KC_MS_ACCEL_JUST1);
                    //    mk_delay = MOUSEKEY_DELAY; //in fact it doesn't seem to work anywhere, even hard coded
                    }
                }
            }
            break;
        case MSPD1:
            if (record->event.pressed) s_chord_plus(MSPD1);
                                   else s_chord_minus(MSPD1);
            break;
        case MSPD2:
            if (record->event.pressed) s_chord_plus(MSPD2);
                                   else s_chord_minus(MSPD2);
            break;
        case MSPD3:
            if (record->event.pressed) s_chord_plus(MSPD3);
                                   else s_chord_minus(MSPD3);
            break;
        case MSPD4:
            if (record->event.pressed) {
                mousekey_on(KC_MS_ACCEL_DOUBLE);
            } else {
                mousekey_off(KC_MS_ACCEL_DOUBLE);
            }
            break;
        case ROLLBACK:
            mousekey_rawreport(128,128);
            break;
        case MSPDp:
        if (record->event.pressed) {
            mouseaccel_off();
            if(mousespeedplus == 0) {
                mousespeedplus = 1;
            } else {
                mousespeedplus = 0;
            }
            //need to do something so speed updates here too
        }
#endif
        break;
      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    //if (mousekeys_down_c != 0) {
    //    if (mousekeys_down_c & 1) ergodox_right_led_1_on();
    //    if (mousekeys_down_c & 2) ergodox_right_led_2_on();
    //    if (mousekeys_down_c & 4) ergodox_right_led_3_on();

    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case BASE:
            ergodox_right_led_1_on();
            break;
        case CPLK :
            ergodox_right_led_2_on();
            break;
        case SYMB:
            ergodox_right_led_2_on();
            break;
        case NENT:
            ergodox_right_led_3_on();
            break;
        case DRNL:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        case MDIA:
            //if (acc_chord != 0) {
            //    if (acc_chord & 1) ergodox_right_led_1_on();
            //    if (acc_chord & 2) ergodox_right_led_2_on();
            //    if (acc_chord & 4) ergodox_right_led_3_on();
            //} else if (mousespeedplus == 0){
                ergodox_right_led_2_on();
                ergodox_right_led_3_on();
            //} else {
            //    ergodox_right_led_1_on();
            //    ergodox_right_led_3_on();
            //}
            break;
        case PLVR:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case FUNX:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case NUMP:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();

        default:
            // none
            break;
    }
};
