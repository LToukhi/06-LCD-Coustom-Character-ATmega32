#include <util/delay.h>
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../HAL/LCD/LCD.h"
#include"../MCAL/DIO/DIO.h"

void main()
{
	#define LDMTRX_Row  8
	#define LDMTRX_Col  8

	MDIO_voidPinMode(PORTC , PIN2 , OUTPUT);
	MDIO_voidPinMode(PORTC , PIN3 , OUTPUT);
	MDIO_voidPinMode(PORTC , PIN4 , OUTPUT);
	MDIO_voidPortMode(PORTD , OUTPUT) ;
	HLCD_voidLcdInit() ;
	MDIO_voidPortMode(PORTA , OUTPUT) ;
	MDIO_voidPortMode(PORTB , INPUT)  ;
	MDIO_voidPinMode(PORTC ,PIN0 , INPUT) ;
	MDIO_voidPinMode(PORTC ,PIN1 , INPUT) ;
	MDIO_voidPortValue(PORTA , HIGH);
	MDIO_voidPortValue(PORTB , HIGH);
	MDIO_voidPinValue(PORTC ,PIN0 , HIGH) ;
	MDIO_voidPinValue(PORTC ,PIN1 , HIGH) ;


	u8 row , col ;
	u8 LDMTRX_Matrix[LDMTRX_Row]={0} ;

	while(1)
	{
		for(col = 0 ; col < LDMTRX_Col ; col++)
		{
			MDIO_voidPinValue(PORTA , col , LOW) ;
			for(row = 0 ; row < LDMTRX_Row ; row++)
			{
				if(MDIO_u8PinRead(PORTB , row) == 0)
				{
					LMTH_ToggleBit(LDMTRX_Matrix[row], col) ;
					_delay_ms(20) ;
					while(MDIO_u8PinRead(PORTB , row) == 0){};
					_delay_ms(20) ;
				}
			}
		MDIO_voidPinValue(PORTA , col , HIGH) ;
		}
		//clear character
		if(MDIO_u8PinRead(PORTC ,PIN0) == 0)
		{
			_delay_ms(20) ;
			while(MDIO_u8PinRead(PORTC , PIN0) == 0){};
			_delay_ms(20) ;
			for(col = 0 ; col < LDMTRX_Col ; col++)
			{
				for(row = 0 ; row < LDMTRX_Row ; row++)
				{
					LMTH_ClrBit(LDMTRX_Matrix[row] , col);
				}
			}
			HLCD_voidLCDClr() ;
		}

		//save character and display
		if(MDIO_u8PinRead(PORTC ,PIN1) == 0)
		{
			_delay_ms(20) ;
			while(MDIO_u8PinRead(PORTC , PIN1) == 0){};
			_delay_ms(20) ;
			HLCD_voidSaveCustomChar(1,LDMTRX_Matrix) ;
			HLCD_voidDisplayCustomChar(1) ;
		}
	}
}
