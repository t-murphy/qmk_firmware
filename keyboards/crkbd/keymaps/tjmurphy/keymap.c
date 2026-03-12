#include QMK_KEYBOARD_H

extern uint8_t is_master;

#define _BASE 0
#define _LOWER 1
#define _RAISE 2

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

enum custom_keycodes {
  GUI_CTLA = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_ESC,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, LGUI(LSFT(KC_4)),\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            LOWER, KC_BSPC,  LGUI_T(KC_SPC),     KC_ENT, GUI_CTLA, RAISE \
                                      //`--------------------------'  `--------------------------'
  ),

  [_LOWER] = LAYOUT( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSLS,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, LCTL(KC_UP), LCTL(KC_LEFT), LCTL(KC_RIGHT), XXXXXXX,      KC_LEFT, KC_DOWN,   KC_UP, KC_RIGHT, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, LALT(KC_UP), LALT(KC_DOWN), XXXXXXX,             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            LOWER, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,   RAISE \
                                      //`--------------------------'  `--------------------------'
  ),

  [_RAISE] = LAYOUT( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LCBR, KC_RCBR, KC_PIPE,  KC_GRV,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LBRC, KC_RBRC, KC_BSLS, KC_TILD,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            LOWER, XXXXXXX,  KC_SPC,     KC_ENT, XXXXXXX,   RAISE \
                                      //`--------------------------'  `--------------------------'
  )
};

static bool gui_ctla_pressed = false;
static bool gui_ctla_held = false;
static uint16_t gui_ctla_timer;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If another key is pressed while GUI_CTLA is down, activate GUI
  if (gui_ctla_pressed && keycode != GUI_CTLA && record->event.pressed) {
    if (!gui_ctla_held) {
      register_code(KC_LGUI);
      gui_ctla_held = true;
    }
  }

  switch (keycode) {
    case GUI_CTLA:
      if (record->event.pressed) {
        gui_ctla_pressed = true;
        gui_ctla_held = false;
        gui_ctla_timer = timer_read();
      } else {
        gui_ctla_pressed = false;
        if (gui_ctla_held) {
          unregister_code(KC_LGUI);
        } else {
          tap_code16(LCTL(KC_A));
        }
        gui_ctla_held = false;
      }
      return false;
  }
  return true;
}

void matrix_scan_user(void) {
  if (gui_ctla_pressed && !gui_ctla_held && timer_elapsed(gui_ctla_timer) >= TAPPING_TERM) {
    register_code(KC_LGUI);
    gui_ctla_held = true;
  }
}
