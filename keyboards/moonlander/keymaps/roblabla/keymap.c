#include QMK_KEYBOARD_H
#include "version.h"
#include "keymap_german.h"
#include "keymap_nordic.h"
#include "keymap_french.h"
#include "keymap_spanish.h"
#include "keymap_hungarian.h"
#include "keymap_swedish.h"
#include "keymap_br_abnt2.h"
#include "keymap_canadian_multilingual.h"
#include "keymap_german_ch.h"
#include "keymap_jp.h"
#include "keymap_bepo.h"
#include "keymap_italian.h"
#include "keymap_slovenian.h"
#include "keymap_danish.h"
#include "keymap_norwegian.h"
#include "keymap_portuguese.h"

#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)
#define ES_LESS_MAC KC_GRAVE
#define ES_GRTR_MAC LSFT(KC_GRAVE)
#define ES_BSLS_MAC ALGR(KC_6)
#define NO_PIPE_ALT KC_GRAVE
#define NO_BSLS_ALT KC_EQUAL
#define LSA_T(kc) MT(MOD_LSFT | MOD_LALT, kc)
#define BP_NDSH_MAC ALGR(KC_8)

// Workaround moonlander bug

extern bool mcp23018_leds[3];

enum custom_keycodes {
  RGB_SLD = ML_SAFE_RANGE,
  HSV_172_255_255,
  HSV_86_255_128,
  HSV_27_255_255,
  FR_WEIRD_C,
  FR_WEIRD_A,
  FR_WEIRD_U,
  OS_CYCLE,
};

// Special key handling.
enum SPECIAL_KEYCODE {
  KEYCODE_C,
  KEYCODE_A,
  KEYCODE_E_LEFT,
  KEYCODE_E_RIGHT,
  KEYCODE_E_HAT,
  KEYCODE_U,
};

const uint8_t win_keys[] = {
  [KEYCODE_C] = 135,
  [KEYCODE_A] = 133,
  [KEYCODE_E_LEFT] = 130,
  [KEYCODE_E_RIGHT] = 138,
  [KEYCODE_E_HAT] = 136,
  [KEYCODE_U] = 151,
};

// TODO: OSX Keys

const char *unicode_keys[] = {
  [KEYCODE_C] = "ç",
  [KEYCODE_A] = "à",
  [KEYCODE_E_LEFT] = "é",
  [KEYCODE_E_RIGHT] = "è",
  [KEYCODE_E_HAT] = "ê",
  [KEYCODE_U] = "ù",
};

enum os {
  Windows,
  MacOS,
  Linux,
  OS_END
};

static enum os OS_TY = Windows;
static char BLINK = 0;
static uint32_t CHANGE_TICK = 0;

static void tap_keypad_digit(uint8_t digit) {
  if (digit == 0)
    tap_code(KC_KP_0);
  else
    tap_code(KC_KP_1 + digit - 1);
}

static void press_key_win(uint8_t code) {
  register_code(KC_LALT);
  tap_keypad_digit(code / 100);
  tap_keypad_digit((code / 10) % 10);
  tap_keypad_digit(code % 10);
  unregister_code(KC_LALT);
}

static void press_key(enum SPECIAL_KEYCODE keycode) {
  switch (OS_TY) {
    case Windows:
      if (keycode >= sizeof(win_keys) / sizeof(win_keys[0]))
        return;
      press_key_win(win_keys[keycode]);
      return;
    case MacOS:
    case Linux:
      if (keycode >= sizeof(unicode_keys) / sizeof(unicode_keys[0]))
        return;
      send_unicode_string(unicode_keys[keycode]);
      return;
    default:
      return;
  }
}

// Custom tap dance declarations
enum {
  TD_FR_E,
};

static void td_e_key(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    press_key(KEYCODE_E_LEFT);
  } else if (state->count == 2) {
    press_key(KEYCODE_E_RIGHT);
  } else if (state->count == 3) {
    press_key(KEYCODE_E_HAT);
  }
  reset_tap_dance(state);
}

qk_tap_dance_action_t tap_dance_actions[] = {
  // Tap once for é, twice for è
  [TD_FR_E] = ACTION_TAP_DANCE_FN(td_e_key),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_moonlander(
    KC_EQUAL,       KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_LEFT,                                        KC_RIGHT,       KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_MINUS,
    TO(4),          KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           KC_BSLASH,
    MO(3),          KC_A,           KC_S,           KC_D,           KC_F,           KC_G,           KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_H,           KC_J,           KC_K,           KC_L,           LT(2,KC_SCOLON),LGUI_T(KC_QUOTE),
    KC_LSHIFT,      LCTL_T(KC_Z),   KC_X,           KC_C,           KC_V,           KC_B,                                                                           KC_N,           KC_M,           KC_COMMA,       KC_DOT,         RCTL_T(KC_SLASH),KC_RSHIFT,
    LT(1,KC_GRAVE), KC_QUOTE,       KC_LALT,        KC_LEFT,        KC_RIGHT,                                        KC_TRANSPARENT, KC_TRANSPARENT,               KC_UP,          KC_DOWN,        KC_LBRACKET,    KC_RBRACKET,    MO(1),
                                                                                    LALT_T(KC_SPACE),KC_BSPACE,      KC_ESCAPE,      KC_LGUI,        KC_TAB,         KC_ENTER
  ),
  [1] = LAYOUT_moonlander(
    KC_ESCAPE,      KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_F6,          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,
    KC_TRANSPARENT, KC_EXLM,        KC_AT,          KC_LCBR,        KC_RCBR,        KC_PIPE,        KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_UP,          KC_7,           KC_8,           KC_9,           KC_ASTR,        KC_F12,
    KC_TRANSPARENT, KC_HASH,        KC_DLR,         KC_LPRN,        KC_RPRN,        KC_GRAVE,       KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_DOWN,        KC_4,           KC_5,           KC_6,           KC_PLUS,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_PERC,        KC_CIRC,        KC_LBRACKET,    KC_RBRACKET,    KC_TILD,                                                                        KC_AMPR,        KC_1,           KC_2,           KC_3,           KC_BSLASH,      KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 RGB_VAI,        RGB_TOG,                                        KC_TRANSPARENT, KC_DOT,         KC_0,           KC_EQUAL,       KC_TRANSPARENT,
                                                                                    RGB_HUD,               RGB_VAD, RGB_HUI, TOGGLE_LAYER_COLOR,    KC_TRANSPARENT, KC_TRANSPARENT
  ),
  // French keys layout.
  [3] = LAYOUT_moonlander(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, TD(TD_FR_E),    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, FR_WEIRD_U,     KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, FR_WEIRD_A,     KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, FR_WEIRD_C,     KC_TRANSPARENT, KC_TRANSPARENT,                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                    KC_TRANSPARENT,       OS_CYCLE, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),
  // Gaming layer
  [4] = LAYOUT_moonlander(
    KC_EQUAL,       KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_LEFT,                                        KC_RIGHT,       KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_MINUS,
    TO(0),          KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           KC_BSLASH,
    KC_TRANSPARENT, KC_A,           KC_S,           KC_D,           KC_F,           KC_G,           KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_H,           KC_J,           KC_K,           KC_L,           KC_SCOLON,      KC_QUOTE,
    KC_LSHIFT,      KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,                                                                           KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       KC_RSHIFT,
    KC_LCTL,        KC_QUOTE,       KC_LALT,        KC_LEFT,        KC_RIGHT,                                        KC_HOME,       KC_PGUP,                                      KC_UP,          KC_DOWN,        KC_LBRACKET,    KC_RBRACKET,    KC_TRANSPARENT,
                                                                                    KC_SPACE,        KC_BSPACE,      KC_ESCAPE,      KC_LGUI,        KC_TAB,         KC_ENTER
  ),
};

void rgb_matrix_indicators_user(void) {
  if (g_suspend_state || keyboard_config.disable_layer_led) { return; }

  if (!BLINK)
    return;

  uint16_t delta = timer_elapsed32(CHANGE_TICK);

  if (BLINK && delta > 5000)
    BLINK = 0;

  int led_to_blink;
  switch (OS_TY) {
    case Windows:
      led_to_blink = 32;
      break;
    case MacOS:
      led_to_blink = 16;
      break;
    default:
      led_to_blink = 13;
      break;
  }

  if (delta % 2500 < 1250) {
    rgb_matrix_set_color( led_to_blink, 255, 255, 255 );
  } else {
    rgb_matrix_set_color( led_to_blink, 0, 0, 0 );
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case FR_WEIRD_C:
      if (record->event.pressed) {
        press_key(KEYCODE_C);
      }
      return false;
    case FR_WEIRD_A:
      if (record->event.pressed) {
        press_key(KEYCODE_A);
      }
      return false;
    case FR_WEIRD_U:
      if (record->event.pressed) {
        press_key(KEYCODE_U);
      }
      return false;
    case OS_CYCLE:
      if (record->event.pressed) {
        OS_TY = (OS_TY + 1) % OS_END;
        CHANGE_TICK = timer_read32();
        BLINK = 1;
      }
      return false;
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      return false;
    case HSV_172_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(172,255,255);
      }
      return false;
    case HSV_86_255_128:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(86,255,128);
      }
      return false;
    case HSV_27_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(27,255,255);
      }
      return false;
  }
  return true;
}

uint32_t layer_state_set_user(uint32_t state) {

  uint8_t layer = biton32(state);

  ML_LED_1(false);
  ML_LED_2(false);
  ML_LED_3(false);
  ML_LED_4(false);
  ML_LED_5(false);
  ML_LED_6(false);
  switch (layer) {
    case 1:
      ML_LED_1(true);
      break;
    case 2:
      ML_LED_2(true);
      break;
    case 3:
      ML_LED_3(true);
      break;
    case 4:
      ML_LED_4(true);
      break;
    case 5:
      ML_LED_5(true);
      break;
    case 6:
      ML_LED_6(true);
      break;
    default:
      break;
  }
  return state;
};
