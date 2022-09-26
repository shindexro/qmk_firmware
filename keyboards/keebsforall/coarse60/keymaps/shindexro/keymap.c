#include QMK_KEYBOARD_H
#include "features/select_word.h"


enum custom_keycodes {
    SPAM_LEFT = SAFE_RANGE,
    SPAM_RIGHT,
    ALT_TAB,
    GUI_SPACE,
};

struct SpamClick {
    int click_interval[10];
    int click_interval_index;
    uint16_t last_click_timestamp;
    bool active;
} left_spam  = {{ 78, 102, 104, 62, 64, 104, 102, 85, 105, 89 }, 0, 0, false},
  right_spam = {{ 78, 102, 104, 62, 64, 104, 102, 85, 105, 89 }, 0, 0, false};  // irregular intervals as an lazy anti-detection

static bool is_alt_tab_active = false;
static bool is_gui_space_active = false;

enum layers{
  LY_BASE,
  LY_SYM,
  LY_NAV,
  LY_NUM,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LY_BASE] = LAYOUT_alice(
    KC_ESC,      KC_TILD, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,     KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,        KC_EQL,  KC_BSPC,
    KC_VOLU,     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,           KC_LBRC, KC_RBRC, KC_BSLS,
    KC_VOLD,     KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,        KC_QUOT,          KC_ENT,
                 KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,              KC_UNDS, KC_N,    KC_M,    KC_COMM, KC_DOT,         KC_SLSH, KC_RSFT, _______,
                 KC_CAPS,          KC_LALT, KC_SPC,  LT(LY_NAV, KC_ESC),                  KC_SPC,     KC_RGUI, KC_RCTL
  ),

  [LY_SYM] = LAYOUT_alice(
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  _______, KC_QUOT, KC_DOT,  KC_PERC, KC_DQUO, KC_GRV,           KC_AMPR, KC_LT  , KC_LBRC, KC_RBRC, KC_GT  , _______, _______, _______,
    _______,  _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL,  KC_HASH,          KC_PIPE, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, _______,          _______,
              _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, KC_DOT,           KC_TILD, KC_DLR , KC_COLN, KC_LCBR, KC_QUES, KC_AT  , _______, _______,
              _______,          _______, _______, _______,                            _______,          _______,          _______
  ),

  [LY_NAV] = LAYOUT_alice(
    _______,  _______, KC_F1,     KC_F2,        KC_F3,       KC_F4,     KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,     KC_F11,   KC_F12,  KC_DEL,
    _______,  _______, A(KC_F4),  KC_MPRV,     KC_MPLY,      KC_MNXT,   C(S(KC_T)),         KC_ESC , _______, _______,  _______, _______,  _______, _______, _______,
    _______,  _______, G(S(KC_S)), G(S(KC_S)),    ALT_TAB,     GUI_SPACE, SPAM_RIGHT,       KC_LEFT, KC_DOWN, KC_UP   , KC_RGHT,    _______,  _______,          _______,
              _______, C(KC_Z),   C(KC_X),      C(KC_C),     C(KC_V),   SPAM_LEFT,          _______, _______, _______,  _______,    _______,  _______, _______, _______,
              _______,            _______,      _______,     _______,                                   _______,                  _______,           QK_BOOT
  ),

  [LY_NUM] = LAYOUT_alice(
    _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PLUS, KC_MINS, KC_ASTR, KC_SLSH, XXXXXXX, XXXXXXX, BSPC_DEL,
    _______,  XXXXXXX, KC_TAB , KC_UP  , KC_ENT , XXXXXXX, XXXXXXX,          KC_7   , KC_8   , KC_9   , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______,  XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,          KC_4   , KC_5   , KC_6   , KC_TAB , XXXXXXX, XXXXXXX,          KC_ENT ,
              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          KC_DOT , KC_1   , KC_2   , KC_3   , KC_ENT , XXXXXXX, XXXXXXX, XXXXXXX,
              XXXXXXX,          _______, KC_SPC , _______,                            KC_0   ,          XXXXXXX,          XXXXXXX
  ),
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {

        case ALT_TAB:
        if (record->event.pressed) {
            if (!is_alt_tab_active) {
              is_alt_tab_active = true;
              register_code(KC_LALT);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        break;

        case GUI_SPACE:
        if (record->event.pressed) {
            if (!is_gui_space_active) {
              is_gui_space_active = true;
              register_code(KC_LGUI);
            }
            register_code(KC_SPC);
        } else {
            unregister_code(KC_SPC);
        }
        break;

        case BSPC_DEL:
        if (record->event.pressed) {
            uint8_t mods = get_mods();
            // clear_mods();
            // clear_oneshot_mods();
            bspc_del_keycode = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT ? KC_DEL : KC_BSPC;
            register_code(bspc_del_keycode);
            // set_mods(mods);
        } else {
            unregister_code(bspc_del_keycode);
        }
        break;

        case SPAM_LEFT:
        if (record->event.pressed) {
            left_spam.active = !left_spam.active;
            left_spam.last_click_timestamp = timer_read();
        }
        break;

        case SPAM_RIGHT:
        if (record->event.pressed) {
            right_spam.active = !right_spam.active;
            right_spam.last_click_timestamp = timer_read();
        }
        break;
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case LY_BASE:
        rgblight_setrgb (0x00,  0x00, 0xFF);
        if (is_alt_tab_active) {
            is_alt_tab_active = false;
            unregister_code(KC_LALT);
        }
        if (is_gui_space_active) {
            is_gui_space_active = false;
            unregister_code(KC_LGUI);
        }
        break;
    case LY_SYM:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    case LY_NAV:
        rgblight_setrgb (0x00,  0xFF, 0x00);
        break;
    case LY_NUM:
        rgblight_setrgb (0x7A,  0x00, 0xFF);
        break;
    default:
        rgblight_setrgb (0x00,  0xFF, 0xFF);
    
    }
  return state;
}

void matrix_scan_user(void) {
    if (left_spam.active && timer_elapsed(left_spam.last_click_timestamp) >= left_spam.click_interval[left_spam.click_interval_index]) {
        left_spam.last_click_timestamp = timer_read();
        SEND_STRING(SS_TAP(X_BTN1));
        left_spam.click_interval_index++;
        left_spam.click_interval_index %= 10;
    }

    if (right_spam.active && timer_elapsed(right_spam.last_click_timestamp) >= right_spam.click_interval[right_spam.click_interval_index]) {
        right_spam.last_click_timestamp = timer_read();
        SEND_STRING(SS_TAP(X_BTN2));
        right_spam.click_interval_index++;
        right_spam.click_interval_index %= 10;
    }
    
}
