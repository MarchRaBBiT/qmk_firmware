ifndef QUANTUM_DIR
	include ../../../../Makefile
endif
COMMAND_ENABLE          = yes # Commands for debug and configuration
TAP_DANCE_ENABLE		= yes # Tap Dance feature
MOUSEKEY_ENABLE			= no  # This keymap dont use mousekey.
RGBLIGHT_ENABLE			= yes # Enable WS2812 RGB underlight.  Do not enable this with audio at the same time.

SRC += action_pseudo_lut.c
