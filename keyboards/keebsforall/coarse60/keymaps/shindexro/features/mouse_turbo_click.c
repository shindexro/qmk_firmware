#include "mouse_turbo_click.h"

/*

   |-> CLICK_DELAY     |-> CLICK_DELAY
   |                   |
|--------------------|--------------------|
reg      unreg       reg      unreg
|--------|-----------|--------|-----------|----> timeline
    |
    |-> CLICK_RELEASE_DELAY
*/
#define CLICK_DELAY 88
#define CLICK_RELEASE_DELAY 44

static bool is_active = false;
static bool click_registered = false;

/* return the delay in ms to repeat this function
   return 0 means that it be should unregistered */
static uint32_t turbo_click_callback(uint32_t trigger_time, void* cb_arg) {
  if (click_registered) {
    unregister_code(KC_MS_BTN1);
    click_registered = false;
    return is_active ? CLICK_DELAY - CLICK_RELEASE_DELAY : 0;
  } else {
    click_registered = true;
    register_code(KC_MS_BTN1);
    return CLICK_RELEASE_DELAY;
  }
}

/* tap behavior:  toggle turbo click 
   hold behavior: turbo click on hold and stop on release */
bool process_mouse_turbo_click(uint16_t keycode, keyrecord_t* record, uint16_t turbo_click_keycode) {
	static uint16_t initial_press_time = 0;
	if (keycode != turbo_click_keycode) {
		return true;
	}

	if (record->event.pressed) {
		if (!is_active) {
			is_active = true;
			defer_exec(1, turbo_click_callback, NULL);
			if (!initial_press_time) {
				initial_press_time = record->event.time;
			}
		} else if (!initial_press_time) {
			is_active = false;
		}
	} else {
		uint16_t hold_time_threshold = initial_press_time + TAPPING_TERM;
		if (timer_expired(record->event.time, hold_time_threshold)) {
			is_active = false;
		}
		initial_press_time = 0;
	}
	return false;
}
