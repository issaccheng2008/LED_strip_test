#ifndef SK9822_H
#define SK9822_H

#include "stm32h7xx_hal.h"

/* Change this value if the connected strip has a different pixel count. */
#define SK9822_LED_COUNT                 35U

/* Low-power test settings: brightness is 1/31 and each active color is 64/255. */
#define SK9822_TEST_GLOBAL_BRIGHTNESS    1U
#define SK9822_TEST_CHANNEL_LEVEL        64U

/**
 * @brief Send a repeating red, green, blue, yellow, cyan, magenta pattern.
 * @param hspi SPI handle configured for 8-bit, MSB-first, mode-0 transmission.
 * @retval HAL status returned by the blocking SPI transmission.
 */
HAL_StatusTypeDef SK9822_ShowFixedPattern(SPI_HandleTypeDef *hspi);

#endif /* SK9822_H */
