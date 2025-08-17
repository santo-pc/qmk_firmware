#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"

enum combos {
  WF_ESC,
  XC_TAB,
};

const uint16_t PROGMEM wf_combo[] = {KC_W, KC_F, COMBO_END};
const uint16_t PROGMEM xc_combo[] = {KC_X, KC_C, COMBO_END};

combo_t key_combos[] = {
  [WF_ESC] = COMBO(wf_combo, KC_ESC),
  [XC_TAB] = COMBO(xc_combo, KC_TAB),
};

enum layers {
    BASE,
    SYM,
    NAV,
    NUM,
};



// Keymap helpers
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define xxxxxxx KC_NO

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN, // Switch to next window (cmd-tab)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] =
        LAYOUT_split_3x5_2(
            KC_Q,       KC_W,   KC_F,       KC_P,       KC_B,       KC_J,           KC_L,       KC_U,       KC_Y,       KC_SCLN,
            KC_A,       KC_R,   KC_S,       KC_T,       KC_G,       KC_M,           KC_N,       KC_E,       KC_I,       KC_O,
            KC_Z,       KC_X,   KC_C,       KC_D,       KC_V,       KC_K,           KC_H,       KC_COMM,    KC_DOT,     KC_SLSH,
                                                        LA_NAV,     KC_SPC,         KC_LSFT,    LA_SYM
        ),

    [SYM]  =
        LAYOUT_split_3x5_2(
            KC_EXLM,    KC_AT,      KC_HASH,    KC_DLR,     KC_PERC,        KC_CIRC,    KC_AMPR,    KC_ASTR,    KC_UNDS,    KC_SCLN,
            KC_GRV,     KC_TILD,    KC_LCBR,    KC_LPRN,    KC_LBRC,        KC_QUOT,    OS_CTRL,    OS_CMD,     OS_ALT,     OS_SHFT,
            KC_LT,      KC_GT,      KC_RCBR,    KC_RPRN,    KC_RBRC,        KC_PIPE,    KC_PMNS,    KC_EQL,     KC_PPLS,    KC_BSLS,
                                                _______,    KC_DQUO,        _______,    _______
        ),
    [NAV]  =
        LAYOUT_split_3x5_2(
            KC_ESC,     KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_UNDS,    KC_D,       KC_U,       KC_DLR,     KC_DEL,
            OS_SHFT,    OS_ALT,     OS_CMD,     OS_CTRL,    KC_NO,      KC_BSPC,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,
            KC_TAB,     KC_TILD,    KC_NO,      KC_NO,      KC_NO,      KC_ENT,     KC_HOME,    KC_END,     KC_NO,      KC_NO,
                                                _______,    _______,    _______,    _______
        ),

    [NUM]  =
        LAYOUT_split_3x5_2(
            KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F11,     KC_7,   KC_8,   KC_9,  KC_F12,
            OS_SHFT,    OS_ALT,     OS_CMD,     OS_CTRL,    KC_NO,      KC_BSPC,    KC_4,   KC_5,   KC_6,   KC_NO,
            KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_0,       KC_1,   KC_2 ,  KC_3,   KC_DOT,
                                                _______,    _______,    _______,    _______
        )
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case LA_SYM:
        case LA_NAV:
            return true;
        default:
            return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case LA_SYM:
        case LA_NAV:
        case KC_LSFT:
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_CMD:
            return true;
        default:
            return false;
    }
}

bool sw_win_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state  = os_up_unqueued;
oneshot_state os_cmd_state  = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(&sw_win_active, KC_LGUI, KC_TAB, SW_WIN, keycode, record);

    update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
    update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
    update_oneshot(&os_cmd_state, KC_LCMD, OS_CMD, keycode, record);

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
