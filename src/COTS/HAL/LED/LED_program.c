/* FILENAME: LDR_program
 *  Author:  Ali El Bana
 *  Version:  V1.0
 *  DATE:   Thu 12/22/2022
 */

/************************************************************************/
/*                        Include headers                        	    */
/************************************************************************/

#include "../../LIB/LSTD_TYPES.h"
#include "../../LIB/LSTD_COMPILER.h"

#include "../../MCAL/GPIO/GPIO_interface.h"

#include "LED_private.h"
#include "LED_interface.h"
#include "LED_config.h"

/************************************************************************/
/*                     Functions' implementations                      	*/
/************************************************************************/

void HLED_vInit(P2VAR(LED_LEDConfiguration) pLED_Cfg)
{
    VAR(MGPIOx_ConfigType) LED_Cfg =
        {
            .Port = pLED_Cfg->u8Port,
            .Pin = pLED_Cfg->u8Pin,
            .Mode = GPIOx_MODE_OUTPUT,
            .OutputType = GPIOx_PUSHPULL,
            .OutputSpeed = GPIOx_LowSpeed,
            .InputType = GPIOx_NoPull};

    MGPIOx_vInit(&LED_Cfg);
}

/*******************************************************************************************************************/
/******************************************************************************************************************/

void HLED_vTurnLightOn(P2VAR(LED_LEDConfiguration) pLED_Cfg)
{
    MGPIOx_vSetPinValue(pLED_Cfg->u8Port, pLED_Cfg->u8Pin, GPIOx_HIGH);
}

/*******************************************************************************************************************/
/******************************************************************************************************************/

void HLED_vTurnLightOff(P2VAR(LED_LEDConfiguration) pLED_Cfg)
{
    MGPIOx_vSetPinValue(pLED_Cfg->u8Port, pLED_Cfg->u8Pin, GPIOx_LOW);
}

/*******************************************************************************************************************/
/******************************************************************************************************************/

void HLED_vToggleLight(P2VAR(LED_LEDConfiguration) pLED_Cfg)
{
    MGPIOx_vTogglePinValue(pLED_Cfg->u8Port, pLED_Cfg->u8Pin);
}

/*******************************************************************************************************************/
/******************************************************************************************************************/
