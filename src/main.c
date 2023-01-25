#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <xmc_gpio.h>

#include <xmc4_gpio.h>

// /* fSYS=fCPU at 12MHz */
#define fSYS_IN_HZ 12000000
#define TIMER_1S 1 * (fSYS_IN_HZ)

#define GPIO_LED1 P5_9
static const XMC_GPIO_CONFIG_t gpio_config_led1 = {.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
                                                   .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
                                                   .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE};

int main()
{
    XMC_GPIO_Init(GPIO_LED1, &gpio_config_led1);
    while (1)
    {
        // Ca. 1s blinking, depending on the oscillator setting
        for (int i = 0; i < TIMER_1S; i++)
        {
        }
        XMC_GPIO_ToggleOutput(GPIO_LED1);
    }
}