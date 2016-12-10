#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "mousekey.h"
#include "keycode.h"
#include "timer.h"

#define BASE 0 // default layer
#define CPLK 1
#define NENT 2  // symbols
#define DRNL 3 // directional keypad
#define PLVR 4 // plover layer
#define MDIA 5 // media keys
#define LEAN 6 // keys you can lean on
#define SYMB 7 // keys you can lean on
#define FUNX 8//
#define SWCH 9 // switch board, might not work the way I hope
#define VIMK 10 // vim normal mode partial emulation

#define MUL   50 // mouse up left
#define MUR   51 // mouse up right
#define MDL   52 // mouse down left
#define MDR   53 // mouse down right
#define MD    54 // mouse down
#define MR    55 // mouse right
#define ML    56 // mouse left
#define MU    57 // mouse up

#define MSPD1 9
#define MSPD2 10
#define MSPD3 12
#define MSPD4 13 // no binary stuff here
#define MSSLW 14 // no binary stuff here
#define MSPDp 58
#define ROLLBACK 99

#define EPRM M(1) // Macro 1: Reset EEPROM

//SEND_STRING
uint8_t mousespeed = KC_MS_ACCEL2 ;
uint8_t mousespeedplus=0;
//uint8_t reaccelerations=0;
uint8_t acc_chord=0;
uint8_t trans_chord=0;
uint8_t mousekeys_down_c = 0;
uint16_t mdir1=0;
uint16_t mdir2=0;

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
        KC_EQL,         KC_1,         KC_2,          KC_3,   KC_4,                    KC_5,   KC_LEFT,
      LT(FUNX, KC_TAB),KC_Q,         KC_W,           KC_E,   KC_R,                    KC_T,   TO(NENT),
        TO(BASE),        KC_A,  LT(NENT,KC_S),LT(SWCH,KC_D),   LT(FUNX,KC_F),           KC_G,
        KC_LSFT,        KC_Z,         KC_X,          KC_C,   KC_V,                    KC_B,    LT(LEAN, S(KC_TAB)),
        LT(NENT,KC_GRV),KC_QUOT,    LCTL(KC_LSFT),  MT(MOD_LCTL | MOD_LALT, KC_LEFT),  ALT_T(KC_RGHT), //TG(SWCH),
                                              GUI_T(KC_APP),  KC_LGUI,
                                                              KC_HOME,
                                SFT_T(KC_SPC), CTL_T(KC_BSPC),KC_END,
        // right hand
             TO(PLVR),     KC_6,   KC_7,           KC_8,          KC_9,    KC_0,          KC_MINS,
             TO(MDIA),     KC_Y,   KC_U,           KC_I,          KC_O,    KC_P,          GUI_T(KC_BSLS),
                 LT(SYMB, KC_H),   KC_J,           LT(SYMB, KC_K),          KC_L,    LT(MDIA, KC_SCLN),  LT(FUNX, KC_QUOT),
         LT(LEAN, KC_DELT),  KC_N,   KC_M,        KC_COMM,       KC_DOT,  KC_SLSH,   KC_RSFT,
                          ALT_T(KC_UP), MT(MOD_LCTL | MOD_LALT, KC_DOWN), MT(MOD_LCTL | MOD_LSFT, KC_LBRC),   KC_RBRC,  KC_FN1,
             KC_LALT,        GUI_T(KC_ESC),
             KC_PGUP,
             KC_PGDN,           CTL_T(KC_TAB), SFT_T(KC_ENT)
    ),

[CPLK] = KEYMAP(  // layer 0 : default
        // left hand
        KC_EQL,         KC_1,            KC_2,         KC_3,   KC_4,   KC_5,   KC_LEFT,
        KC_DELT,        S(KC_Q),         S(KC_W),         S(KC_E),   S(KC_R),   S(KC_T),   KC_TRNS,
        TO(BASE),       S(KC_A),         S(KC_S),         S(KC_D),   S(KC_F),   S(KC_G),
        KC_LSFT,        S(KC_Z),         S(KC_X),         S(KC_C),   S(KC_V),   S(KC_B),   KC_TRNS,
        LT(NENT,KC_GRV),KC_QUOT,    LCTL(KC_LSFT),  MT(MOD_LCTL | MOD_LALT, KC_LEFT),  ALT_T(KC_RGHT), //TG(SWCH),
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
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */

// Numeric Data Entry

[NENT] = KEYMAP(
       // left hand
       KC_TRNS,      KC_F1,           KC_F2,  KC_F3,         KC_F4,      KC_F5,    KC_TRNS,
       KC_TRNS,    KC_TRNS,         KC_TRNS,  S(KC_TAB),     KC_UP,      KC_TAB,   TO(BASE),
       TO(BASE),   KC_TRNS,         KC_TRNS,  KC_LEFT,       KC_DOWN,    KC_RGHT,
       KC_TRNS,    KC_TRNS,         KC_TRNS,  KC_TRNS,       KC_TRNS,    KC_ENT,  KC_TRNS,
          KC_TRNS, KC_TRNS,         KC_TRNS,  KC_TRNS,       KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_TRNS, KC_TRNS,       TO(BASE),       KC_PSLS,          KC_PAST,           KC_TRNS,          KC_TRNS,
       TO(MDIA),KC_TRNS,          KC_7,          KC_8,             KC_9,            KC_MINS,          KC_TRNS,
              LT(SYMB, KC_TRNS),  KC_4,          KC_5,             KC_6,              S(KC_EQL),       LT(FUNX, KC_EQL), 
       KC_TRNS, TO(DRNL),         KC_1,          KC_2,             KC_3,              KC_ENT,           KC_TRNS, 
                         ALT_T(KC_0),MT(MOD_LCTL | MOD_LALT, KC_DOT),  MT(MOD_LCTL | MOD_LSFT, KC_COMM),  LT(MDIA,KC_ENT), KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
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


// MEDIA AND MOUSE
[MDIA] = KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, M(ROLLBACK), M(MUL),KC_MS_U,   M(MUR), KC_TRNS, TO(NENT),
       KC_TRNS, M(MSPDp),KC_MS_L,  KC_MS_D,    KC_MS_R,  KC_TRNS,
       KC_TRNS, KC_TRNS, M(MDL), M(MD),   M(MDR), KC_BTN3, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, 
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  CTL_T(KC_BTN2), KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS,        KC_TRNS,         KC_TRNS,        KC_TRNS, KC_TRNS,
       TO(BASE), KC_VOLU, KC_WWW_BACK,    KC_MS_WH_UP,     KC_WWW_FORWARD,    KC_TRNS, KC_TRNS,
                 KC_VOLD, KC_MS_WH_LEFT,  KC_MS_WH_DOWN,   KC_MS_WH_RIGHT,  KC_TRNS, KC_MPLY,
       //KC_TRNS,  KC_MUTE, M(MSPD1),          M(MSPD2),     M(MSPD3),       M(MSPD4),  KC_TRNS,
       KC_TRNS,  KC_MUTE, M(MSSLW),          KC_MS_ACCEL1,    KC_MS_ACCEL2,       M(MSPD4),  KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
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
       KC_TRNS,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,KC_TRNS,
       KC_TRNS,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       KC_TRNS,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,KC_TRNS,
          EPRM,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_TRNS, KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,  KC_TRNS,
       KC_TRNS, KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,
                KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS,
                         KC_TRNS,KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
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
             KC_TRNS,       KC_TRNS,      KC_F10,    KC_F11,       KC_F12,    KC_TRNS,      KC_TRNS,
             KC_TRNS,       KC_TRNS,      KC_F7,     KC_F8,        KC_F9,     KC_F11,      KC_TRNS,
                            KC_F11,      KC_F4,     KC_F5,        KC_F6,     KC_F12,      KC_TRNS,
               KC_TRNS,     KC_F12,     KC_F1,     KC_F2,        KC_F3,     KC_TRNS,      KC_TRNS,
                                        KC_F10,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS,  KC_LCTL,   SFT_T(KC_TAB)
),

[SWCH] = KEYMAP(  // Blank template keymap
        // left hand
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
        TO(BASE),    KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,     
                                      KC_NO, KC_NO,
                                           KC_NO,
                                 KC_C,   KC_V,   KC_NO,
        // right hand
             KC_TRNS,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
             KC_TRNS,       KC_NO,      KC_NO,      TO(BASE),      KC_NO,      KC_NO,      KC_TRNS,
                            KC_NO,      TO(MDIA),   TO(FUNX),   TO(CPLK),   KC_NO,      KC_NO,
               TO(PLVR),    TO(NENT),   TO(DRNL),      KC_NO,      KC_NO,      KC_NO,      KC_NO,
                                        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO,  KC_NO,   KC_NO
),


[VIMK] = KEYMAP(  // Blank template keymap
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
             KC_TRNS,       KC_NO,      KC_NO,      TO(0),      KC_NO,      KC_NO,      KC_TRNS,
                            KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,      KC_NO,      KC_NO,
               KC_NO,       KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
                            KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO,  KC_NO,   KC_NO
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
        mousekey_settimer(3);
    }
    lastmousekey = id;
}

void onmouseup(int8_t id){
    acc_chord -= 1;
    mousekeyisdown -= 1;
}    

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
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
        // mouse diagonals
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
                // should that lessthan be adjusted whether or not mousenudgeison is 1?
                if(timer_elapsed(msslw_timer) < 120 && mousekeyisdown < 2){
                    mousenudgeison = ~mousenudgeison;
                    if(mousenudgeison) { // And we finally know what to do with this key release
                        mousekey_on(KC_MS_ACCEL_JUST1);
                        mk_delay = 100; //which gets multiplied to 300 ms when it's used in mousekey.c
                    } else {
                        mousekey_off(KC_MS_ACCEL_JUST1);
                        mk_delay = MOUSEKEY_DELAY;
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
        break;

        case MD: // mouse down
        
        if (record->event.pressed) {
            onmousedown(KC_MS_DOWN);
            return MACRO(D(MS_DOWN));
        } else {
            onmouseup(KC_MS_DOWN);
        }
        break;

        case MR: // mouse right
        
        if (record->event.pressed) {
            onmousedown(KC_MS_RIGHT);
            return MACRO(D(MS_RIGHT));
        } else {
            onmouseup(KC_MS_RIGHT);
        }
        break;

        case ML: // mouse left
        if (record->event.pressed) {
            onmousedown(KC_MS_LEFT);
            return MACRO(D(MS_LEFT));
        } else {
            onmouseup(KC_MS_LEFT);
        }
        break;

        case MU: // mouse up
        if (record->event.pressed) {
            onmousedown(KC_MS_UP);
            return MACRO(D(MS_UP));
        } else {
            onmouseup(KC_MS_UP);
        }
        break;

        case MUL: // mouse up left
        mdir1 = KC_MS_UP; mdir2 = KC_MS_LEFT;
        if (record->event.pressed)  movemouse(mdir1, mdir2, 1, mousespeed);
                               else movemouse(mdir1, mdir2, 0, mousespeed);
        break;

        case MUR: // mouse up right
        mdir1 = KC_MS_UP; mdir2 = KC_MS_RIGHT;
        if (record->event.pressed)  movemouse(mdir1, mdir2, 1, mousespeed);
                               else movemouse(mdir1, mdir2, 0, mousespeed);
        break;

        case MDL: // mouse down left
        mdir1 = KC_MS_DOWN; mdir2 = KC_MS_LEFT;
        if (record->event.pressed)  movemouse(mdir1, mdir2, 1, mousespeed);
                               else movemouse(mdir1, mdir2, 0, mousespeed);
        break;

        case MDR: // mouse down right
        mdir1 = KC_MS_DOWN; mdir2 = KC_MS_RIGHT;
        if (record->event.pressed)  movemouse(mdir1, mdir2, 1, mousespeed);
                               else movemouse(mdir1, mdir2, 0, mousespeed);
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
    if (acc_chord != 0) {
        if (acc_chord & 1) ergodox_right_led_1_on();
        if (acc_chord & 2) ergodox_right_led_2_on();
        if (acc_chord & 4) ergodox_right_led_3_on();
    //if (mousekeys_down_c != 0) {
    //    if (mousekeys_down_c & 1) ergodox_right_led_1_on();
    //    if (mousekeys_down_c & 2) ergodox_right_led_2_on();
    //    if (mousekeys_down_c & 4) ergodox_right_led_3_on();
    } else {
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case LEAN:
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
            if (mousespeedplus == 0){
                ergodox_right_led_2_on();
                ergodox_right_led_3_on();
            } else {
                ergodox_right_led_1_on();
                ergodox_right_led_3_on();
            }
            break;
        case PLVR:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case FUNX:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case SWCH:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        default:
            // none
            break;
    }
    }

};
