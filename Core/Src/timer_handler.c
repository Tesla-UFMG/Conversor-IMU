#include "timer_handler.h"

#include "stdbool.h"

#include "stm32f1xx.h"

/**
 * @brief Waits a specific timer to be elapsed by a desired amount. Allows
 * multiple timers by sending a different timer_start.
 *
 * @param timer_start Timer specifier.
 * @param delay Amount desired to wait, max is 2^32, which is equal to
 * approximately 50 days. Values higher than that will not work as expected!
 * @return true The desired time has elapsed.
 * @return false The desired time has not been elapsed.
 */
bool timer_wait(uint32_t timer_start, uint32_t delay) {
    const uint32_t current_time = HAL_GetTick();
    if ((current_time - timer_start) >= delay) {
        return true;
    }
    return false;
}

/**
 * @brief Restart timer to current tick value.
 *
 * @param timer_to_restart
 */
void timer_restart(uint32_t* timer_to_restart) {
    *timer_to_restart = HAL_GetTick();
}