/* FILENAME: Testing_Bluetooth_program 
*  Author:  Ali El Bana
*  Version:  V1.0
*  DATE:   Thu 01/05/2023
*/

/************************************************************************/
/*                        Include headers                        	    */
/************************************************************************/

#include "../../../COTS/LIB/LSTD_TYPES.h"
#include "../../../COTS/LIB/LSTD_COMPILER.h"
#include "../../../COTS/LIB/LSTD_VALUES.h"
#include "../../../COTS/LIB/LSTD_BITMATH.h"

#include "../../../COTS/MCAL/RCC/MRCC_interface.h"
#include "../../../COTS/MCAL/GPIO/GPIO_interface.h"

#include "../../../COTS/HAL/Bluetooth/Bluetooth_interface.h"
#include "../../../COTS/HAL/LED/LED_interface.h"

#include "Testing_Bluetooth_interface.h"
#include "Testing_Bluetooth_private.h"
#include "Testing_Bluetooth_config.h"

/************************************************************************/
/*                     Functions implementations                      	*/
/************************************************************************/

void THBluetooth_vLEDColor( void )
{

	c8_t L_strRecievedColor[6] 	= "0" 			;
	u8_t L_u8BlueColorCheck		= INITIAL_ZERO	;
	u8_t L_u8GreenColorCheck	= INITIAL_ZERO	;
	u8_t L_u8ExitCheck			= INITIAL_ZERO	;
	c8_t L_strBlueColor[5] 		= "BLUE" 		;
	c8_t L_strGreenColor[6] 	= "GREEN" 		;
	c8_t L_strExit[5] 			= "EXIT" 		;

	MRCC_vInit( ) ;

	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOBEN  ) ;
	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOAEN  ) ;

	MGPIOx_vLockedPins() ;

	MGPIOx_ConfigType GREEN =
	{
		.Port 			= GPIO_PORTA		, .Pin 		 	= GPIOx_PIN10	 ,
		.Mode 		 	= GPIOx_MODE_OUTPUT	, .OutputType	= GPIOx_PUSHPULL ,
		.OutputSpeed 	= GPIOx_LowSpeed	, .InputType 	= GPIOx_NoPull
	} ;

	MGPIOx_ConfigType BLUE =
	{
		.Port 			= GPIO_PORTA		, 	.Pin 		= GPIOx_PIN11	 ,
		.Mode 			= GPIOx_MODE_OUTPUT	, 	.OutputType = GPIOx_PUSHPULL ,
		.OutputSpeed 	= GPIOx_LowSpeed	,	.InputType 	= GPIOx_NoPull
	} ;

	MGPIOx_vInit( &GREEN ) ;
	MGPIOx_vInit( &BLUE  ) ;

	// Initialization of Bluetooth Module
	HBluetooth_vInit( ) ;

	MGPIOx_vSetResetAtomic( GREEN.Port, GREEN.Pin, GPIOx_LOW ) ;
	MGPIOx_vSetResetAtomic( BLUE.Port , BLUE.Pin , GPIOx_LOW ) ;


	do
	{

		HBluetooth_vSendString( "\nSelect the LED color: \n" ) ;

		/* Don't forget to enter (Enter) -> "\n" after every string you entered */
		HBluetooth_vReceiveString( L_strRecievedColor ) ;

		L_u8BlueColorCheck 	= HBluetooth_u8CompStrings( L_strRecievedColor, L_strBlueColor  ) ;
		L_u8GreenColorCheck = HBluetooth_u8CompStrings( L_strRecievedColor, L_strGreenColor ) ;
		L_u8ExitCheck 		= HBluetooth_u8CompStrings( L_strRecievedColor, L_strExit 	  ) ;

		if( L_u8GreenColorCheck == SAME_STRING )
		{
			MGPIOx_vSetResetAtomic( GREEN.Port, GREEN.Pin, GPIOx_HIGH ) ;
			MGPIOx_vSetResetAtomic( BLUE.Port , BLUE.Pin , GPIOx_LOW  ) ;
		}
		else if( L_u8BlueColorCheck == SAME_STRING )
		{
			MGPIOx_vSetResetAtomic( BLUE.Port , BLUE.Pin , GPIOx_HIGH ) ;
			MGPIOx_vSetResetAtomic( GREEN.Port, GREEN.Pin, GPIOx_LOW  ) ;
		}
		else if( L_u8ExitCheck == SAME_STRING )
		{
			MGPIOx_vSetResetAtomic( BLUE.Port , BLUE.Pin , GPIOx_LOW  ) ;
			MGPIOx_vSetResetAtomic( GREEN.Port, GREEN.Pin, GPIOx_LOW  ) ;
		}
		else
		{
			MGPIOx_vSetResetAtomic( GREEN.Port, GREEN.Pin, GPIOx_HIGH ) ;
			MGPIOx_vSetResetAtomic( BLUE.Port , BLUE.Pin , GPIOx_HIGH ) ;
		}

	}while( L_u8ExitCheck != SAME_STRING ) ;

	while( TRUE )
	{

	}

}

/**************************************************************************************/
/**************************************************************************************/

void THBluetooth_vAsynchToggleLED( void )
{

	c8_t  L_c8RecievedButton = INITIAL_ZERO ;

	LED_LEDConfiguration GREEN =
	{
		.u8Port = GPIO_PORTA ,
		.u8Pin 	= GPIOx_PIN0
	} ;

	LED_LEDConfiguration BLUE =
	{
		.u8Port = GPIO_PORTA ,
		.u8Pin 	= GPIOx_PIN1
	} ;

	LED_LEDConfiguration YELLOW =
	{
		.u8Port = GPIO_PORTA ,
		.u8Pin 	= GPIOx_PIN2
	} ;

	MRCC_vInit( ) ;

	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOAEN  ) ;
	MRCC_vEnablePeriphralCLK( RCC_AHB1, AHB1ENR_GPIOBEN  ) ;

	MGPIOx_vLockedPins( ) ;

	HLED_vInit( &GREEN  ) ;
	HLED_vInit( &BLUE   ) ;
	HLED_vInit( &YELLOW ) ;

	HLED_vTurnLightOff( &GREEN  ) ;
	HLED_vTurnLightOff( &BLUE   ) ;
	HLED_vTurnLightOff( &YELLOW ) ;

	// Initialization of Bluetooth Module
	HBluetooth_vInit( ) ;

	void ToggleBlueYellowLEDs( void )
	{

		L_c8RecievedButton = HBluetooth_u8GetDataRegister( ) ;

		switch( L_c8RecievedButton )
		{

			case 'b': HLED_vToggleLight( &BLUE   ) ; break ;

			case 'y': HLED_vToggleLight( &YELLOW ) ; break ;

			case 'e': HBluetooth_vSendString( "\nExit from blinking state\n" ) ; break ;

			default : HBluetooth_vSendString( "\nUnknown Character\n" ) ; break ;

		}

	}

	HBluetooth_u8AsynchReceiveByte( &ToggleBlueYellowLEDs ) ;

	while( TRUE )
	{

		while( L_c8RecievedButton != 'e' )
		{
			HLED_vBlinkLED( &GREEN, 200 ) ;
		}

	}

}

/**************************************************************************************/
/**************************************************************************************/











