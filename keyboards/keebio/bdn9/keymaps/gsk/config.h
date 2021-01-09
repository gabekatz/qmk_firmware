#pragma once

#ifdef RGB_MATRIX_ENABLE
//# define RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#define EECONFIG_RGB_MATRIX (uint32_t *)28
#endif
/* For USB nonsense in various OSs */
#define TAP_CODE_DELAY 10

/* For constant mouse speed */
#define MOUSEKEY_DELAY 300
#define MOUSEKEY_INTERVAL 50
#define MOUSEKEY_MAX_SPEED 1
#define MOUSEKEY_TIME_TO_MAX 0
#define MOUSEKEY_WHEEL_MAX_SPEED 1
#define MOUSEKEY_WHEEL_TIME_TO_MAX 0
