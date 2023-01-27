/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#include <string.h>
#include "ecat_slv.h"
#include "utypes.h"
#include "xmc_gpio.h"

#ifndef XMC4800_F144x2048
#    define XMC4800_F144x2048
#endif

#ifdef XMC4800_F144x2048
#    define P_LED P5_8
#    define P_BTN P15_12
#endif

#ifdef XMC4300_F100x256
#    define P_LED P4_1
#    define P_BTN P3_4
#endif

// /* fSYS=fCPU at 12MHz (from system_XMC4800.c) */
#define OSCHP_FREQUENCY 12000000U
#define TIMER_1S 1 * (OSCHP_FREQUENCY)

extern void ESC_eep_handler(void);

/* Global buffer used by SOES when no overrides are specified for Rx and TxPDO_update.
 * Needs to be decleared, since otherwise we are running into linker issues.
 */
uint8_t rxpdo[1] = {0};
uint8_t txpdo[1] = {0};

/* Application variables */
_Objects    Obj;


static const XMC_GPIO_CONFIG_t gpio_config_btn = {.mode = XMC_GPIO_MODE_INPUT_INVERTED_PULL_UP,
                                                  .output_level = 0,
                                                  .output_strength = 0};

static const XMC_GPIO_CONFIG_t gpio_config_led = {.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
                                                  .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
                                                  .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE};

void cb_get_inputs(void)
{
    Obj.Buttons.Button1 = XMC_GPIO_GetInput(P_BTN);
}

void cb_set_outputs(void)
{
    if (Obj.LEDgroup0.LED0)
    {
        XMC_GPIO_SetOutputHigh(P_LED);
    }
    else
    {
        XMC_GPIO_SetOutputLow(P_LED);
    }
}

void cb_state_change (uint8_t * as, uint8_t * an)
{
   if (*as == SAFEOP_TO_OP)
   {
      /* Enable watchdog interrupt */
      ESC_ALeventmaskwrite (ESC_ALeventmaskread() | ESCREG_ALEVENT_WD);
   }
   else if (*as == PREOP_TO_SAFEOP)
   {
      /* Write initial input data requried if an input only slave,
       * otherwise the SM3 will never occur.
       */
      DIG_process (DIG_PROCESS_INPUTS_FLAG);
   }
}

void RxPDO_update_override()
{
    ESC_read(ESC_SM2_sma, &rxpdo[0], ESCvar.ESC_SM2_sml);
    memcpy(&Obj.LEDgroup0.LED0, &rxpdo[0], sizeof(uint8_t));
}

void TxPDO_update_override()
{
    memcpy(&txpdo[0], &Obj.Buttons.Button1 , sizeof(uint8_t));
    ESC_write(ESC_SM3_sma, &txpdo[0], ESCvar.ESC_SM3_sml);
}

/* Setup of DC */
uint16_t dc_checker (void)
{
   /* Indicate we run DC */
   ESCvar.dcsync = 1;
   /* Fetch the sync counter limit (SDO10F1) */
   ESCvar.synccounterlimit = Obj.ErrorSettings.SyncErrorCounterLimit;
   return 0;
}


void soes(void* arg)
{
    /* Setup config hooks */
    static esc_cfg_t config = {.user_arg = NULL,
                               .use_interrupt = 0,
                               .watchdog_cnt = 5000,
                               .set_defaults_hook = NULL,
                               .pre_state_change_hook = NULL,
                               .post_state_change_hook = cb_state_change,
                               .application_hook = NULL,
                               .safeoutput_override = NULL,
                               .pre_object_download_hook = NULL,
                               .post_object_download_hook = NULL,
                               .rxpdo_override = RxPDO_update_override,
                               .txpdo_override = TxPDO_update_override,
                               .esc_hw_interrupt_enable = NULL,
                               .esc_hw_interrupt_disable = NULL,
                               .esc_hw_eep_handler = ESC_eep_handler,
                               .esc_check_dc_handler = dc_checker};


    DPRINT("SOES (Simple Open EtherCAT Slave)\n");

    // configure I/O
    XMC_GPIO_Init(P_BTN, &gpio_config_btn);
    XMC_GPIO_Init(P_LED, &gpio_config_led);

    ecat_slv_init(&config);

    while (1)
    {
        ecat_slv();
    }
}

int main(void)
{
    soes(NULL);
    return 0;
}
