#include "sk9822.h"

#define SK9822_START_FRAME_SIZE          4U
#define SK9822_BYTES_PER_LED             4U
#define SK9822_END_FRAME_SIZE            4U
#define SK9822_FRAME_SIZE                (SK9822_START_FRAME_SIZE + \
                                           (SK9822_LED_COUNT * SK9822_BYTES_PER_LED) + \
                                           SK9822_END_FRAME_SIZE)
#define SK9822_SPI_TIMEOUT_MS            100U
#define SK9822_PATTERN_COLOR_COUNT       6U

typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} SK9822_Color;

static const SK9822_Color test_pattern[SK9822_PATTERN_COLOR_COUNT] =
{
  {SK9822_TEST_CHANNEL_LEVEL, 0U, 0U},
  {0U, SK9822_TEST_CHANNEL_LEVEL, 0U},
  {0U, 0U, SK9822_TEST_CHANNEL_LEVEL},
  {SK9822_TEST_CHANNEL_LEVEL, SK9822_TEST_CHANNEL_LEVEL, 0U},
  {0U, SK9822_TEST_CHANNEL_LEVEL, SK9822_TEST_CHANNEL_LEVEL},
  {SK9822_TEST_CHANNEL_LEVEL, 0U, SK9822_TEST_CHANNEL_LEVEL}
};

static uint8_t frame[SK9822_FRAME_SIZE];

static void SK9822_SetPixel(uint8_t *pixel,
                            uint8_t brightness,
                            SK9822_Color color)
{
  pixel[0] = 0xE0U | (brightness & 0x1FU);
  /* The supplied SK9822 specification defines the color byte order as GRB. */
  pixel[1] = color.green;
  pixel[2] = color.red;
  pixel[3] = color.blue;
}

HAL_StatusTypeDef SK9822_ShowFixedPattern(SPI_HandleTypeDef *hspi)
{
  uint32_t led_index;
  uint32_t byte_index;

  if (hspi == NULL)
  {
    return HAL_ERROR;
  }

  for (byte_index = 0U; byte_index < SK9822_START_FRAME_SIZE; byte_index++)
  {
    frame[byte_index] = 0x00U;
  }

  for (led_index = 0U; led_index < SK9822_LED_COUNT; led_index++)
  {
    uint32_t pixel_offset = SK9822_START_FRAME_SIZE +
                            (led_index * SK9822_BYTES_PER_LED);

    SK9822_SetPixel(&frame[pixel_offset],
                    SK9822_TEST_GLOBAL_BRIGHTNESS,
                    test_pattern[led_index % SK9822_PATTERN_COLOR_COUNT]);
  }

  for (byte_index = SK9822_START_FRAME_SIZE +
                    (SK9822_LED_COUNT * SK9822_BYTES_PER_LED);
       byte_index < SK9822_FRAME_SIZE;
       byte_index++)
  {
    frame[byte_index] = 0xFFU;
  }

  return HAL_SPI_Transmit(hspi,
                          frame,
                          (uint16_t)sizeof(frame),
                          SK9822_SPI_TIMEOUT_MS);
}
