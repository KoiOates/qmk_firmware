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

#define SYMBMOD 60
#define OUTEST 61
#define THUMB_SHIFT_TAB 62
#define THUMB_SHIFT_BS 63

#define EPRM M(1) // Macro 1: Reset EEPROM


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
//       KC_BSPC, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
//       TG(MDIA),M(NM),   M(NM),   M(NM),   M(NM),   M(NM),  KC_NO,  
//       TO(BASE),M(Sl),   M(Tl),   M(Pl),   M(Hl),   M(X),
//       KC_NO,   M(Sl),   M(Kl),   M(Wl),   M(Rl),   M(X),   KC_NO,
//       KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
//                                           KC_NO,   KC_NO,  
//                                                    KC_NO,  
//                                  M(Al),   M(Ol),   KC_NO,  
//    // right hand
//       KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
//       KC_TRNS,  M(NM),   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),
//                 M(X),    M(Fr),   M(Pr),   M(Lr),   M(Tr),   M(Dr),
//       KC_NO,    M(X),    M(Rr),   M(Br),   M(Gr),   M(Sr),   M(Zr),
//                          KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
//       KC_NO,   KC_NO,  
//       KC_NO,  
//       KC_NO,   M(Er),   M(Ur)
       KC_BSPC, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
       KC_NO,   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),  M(X),  
       KC_NO,   KC_NO,   M(Sl),   M(Tl),   M(Pl),   M(Hl),
       KC_NO,   KC_NO,   M(Sl),   M(Kl),   M(Wl),   M(Rl),   M(X),
       KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                           KC_NO,   KC_NO,  
                                                    KC_NO,  
                                  M(Al),   M(Ol),   KC_NO,  
    // right hand
       KC_TRNS,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
       M(X),  M(NM),   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),
                M(Fr),   M(Pr),   M(Lr),   M(Tr),   M(Dr), KC_NO,
       M(X),    M(Rr),   M(Br),   M(Gr),   M(Sr),   M(Zr), KC_NO,
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

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function

      uint8_t layer = biton32(layer_state);
      if (layer==TXBOLT){
          if (record->event.pressed) {
            uint8_t grp = (id & GRPMASK) >> 6;
            chord[grp] |= id;
          }
          else {
            if (pressed_count == 0) {
              send_chord();
              chord[0] = chord[1] = chord[2] = chord[3] = 0;
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
