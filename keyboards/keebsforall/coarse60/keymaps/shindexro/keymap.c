#include QMK_KEYBOARD_H
#include "features/select_word.h"


enum custom_keycodes {
    ALT_TAB = SAFE_RANGE,
    GUI_SPACE,
};

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
                 KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,     KC_UNDS, KC_N,    KC_M,    KC_COMM, KC_DOT,         KC_SLSH, KC_RSFT, KC_RGUI,
                 KC_CAPS,          KC_LALT, KC_SPC,  LT(LY_NAV, KC_ESC),                  MO(LY_NAV),       KC_RGUI,                 KC_RCTL
  ),

  [LY_SYM] = LAYOUT_alice(
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  _______, KC_QUOT, KC_DOT,  KC_PERC, KC_DQUO, KC_GRV,           KC_AMPR, KC_LT  , KC_LBRC, KC_RBRC, KC_GT  , _______, _______, _______,
    _______,  _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL,  KC_HASH,          KC_PIPE, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, _______,          _______,
              _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, KC_DOT,  KC_TILD, KC_DLR , KC_COLN, KC_LCBR, KC_QUES, KC_AT  , _______, _______,
              _______,          _______, _______, _______,                            _______,          _______,          _______
  ),

  [LY_NAV] = LAYOUT_alice(
    _______,  _______, KC_F1,     KC_F2,        KC_F3,       KC_F4,     KC_F5,       KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_F11,   KC_F12,  KC_DEL,
    _______,  _______, A(KC_F4),  KC_MPRV,      KC_MPLY,     KC_MNXT,   C(S(KC_T)),           _______, KC_ESC , KC_UP   , _______, _______,  _______, _______, _______,
    _______,  _______, _______,   G(S(KC_S)),   ALT_TAB,     GUI_SPACE, _______,              KC_HOME, KC_LEFT, KC_DOWN , KC_RGHT, KC_END ,  _______,          _______,
              _______, _______,   _______,      _______,     _______,   _______,     _______, _______, _______,  _______, _______,  _______, _______, _______,
              _______,            _______,      _______,     _______,                                  _______,                    _______,           QK_BOOT
  ),

  [LY_NUM] = LAYOUT_alice(
    _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PLUS, KC_MINS, KC_ASTR, KC_SLSH, XXXXXXX, XXXXXXX, XXXXXXX,
    _______,  XXXXXXX, KC_TAB , KC_UP  , KC_ENT , XXXXXXX, XXXXXXX,          KC_7   , KC_8   , KC_9   , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______,  XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,          KC_4   , KC_5   , KC_6   , KC_TAB , XXXXXXX, XXXXXXX,          KC_ENT ,
              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DOT , KC_1   , KC_2   , KC_3   , KC_ENT , XXXXXXX, XXXXXXX, XXXXXXX,
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
