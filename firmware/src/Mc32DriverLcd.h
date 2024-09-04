#ifndef __MC32DRIVERLCD_H
#define __MC32DRIVERLCD_H
/*--------------------------------------------------------*/
//	Mc32DriverLcd.h
/*--------------------------------------------------------*/
//	Description :	Driver pour LCD Pic 32
//
/*--------------------------------------------------------*/

#include <stdarg.h>
#include "system_definitions.h"

/*--------------------------------------------------------*/
// LCD
/*--------------------------------------------------------*/
//On écrit dans le latch pour éviter les problèmes de R/W
#define LCD_RS_W       LATBbits.LATB13
#define LCD_RW_W       LATBbits.LATB11
#define LCD_E_W        LATBbits.LATB12
#define LCD_BL_W       LATBbits.LATB10
#define LCD_DB4_W      LATBbits.LATB5
#define LCD_DB5_W      LATBbits.LATB6
#define LCD_DB6_W      LATBbits.LATB7
#define LCD_DB7_W      LATBbits.LATB8
//On lit directement sur le port, sinon on obtient la valeur
//précédemment écrite dans le latch!!
#define LCD_RS_R       PORTBbits.RB13
#define LCD_RW_R       PORTBbits.RB11
#define LCD_E_R        PORTBbits.RB12
#define LCD_BL_R       PORTBbits.RB10
#define LCD_DB4_R      PORTBbits.RB5
#define LCD_DB5_R      PORTBbits.RB6
#define LCD_DB6_R      PORTBbits.RB7
#define LCD_DB7_R      PORTBbits.RB8

#define LCD_RS_T     TRISBbits.TRISB13
#define LCD_RW_T     TRISBbits.TRISB11
#define LCD_E_T      TRISBbits.TRISB12
#define LCD_BL_T     TRISBbits.TRISB10
#define LCD_DB4_T    TRISBbits.TRISB5
#define LCD_DB5_T    TRISBbits.TRISB6
#define LCD_DB6_T    TRISBbits.TRISB7
#define LCD_DB7_T    TRISBbits.TRISB8

// Definitions pour fonctions PLIB_PORTS
#define LCD_RS_PORT   PORT_CHANNEL_B
#define LCD_RS_BIT    PORTS_BIT_POS_13
#define LCD_RW_PORT   PORT_CHANNEL_B
#define LCD_RW_BIT    PORTS_BIT_POS_11
#define LCD_E_PORT    PORT_CHANNEL_B
#define LCD_E_BIT     PORTS_BIT_POS_12
#define LCD_BL_PORT   PORT_CHANNEL_B
#define LCD_BL_BIT    PORTS_BIT_POS_10
#define LCD_DB4_PORT   PORT_CHANNEL_B
#define LCD_DB4_BIT    PORTS_BIT_POS_5
#define LCD_DB5_PORT   PORT_CHANNEL_B
#define LCD_DB5_BIT    PORTS_BIT_POS_6
#define LCD_DB6_PORT   PORT_CHANNEL_B
#define LCD_DB6_BIT    PORTS_BIT_POS_7
#define LCD_DB7_PORT   PORT_CHANNEL_B
#define LCD_DB7_BIT    PORTS_BIT_POS_8

// Definitions dans le fichier XML (bsp.xml)
// <function name="LCD_RS" pin="RC4" mode="digital" direction="out"/>
// <function name="LCD_RW" pin="RC5" mode="digital" direction="out"/>
// <function name="LCD_E" pin="RC3" mode="digital" direction="out"/>
// <function name="LCD_BL" pin="RA8" mode="digital" direction="out"/ latch="low">
// <function name="LCD_DB4" pin="RB5" mode="digital" direction="out" latch="high"/>
// <function name="LCD_DB5" pin="RB6" mode="digital" direction="out" latch="high"/>
// <function name="LCD_DB6" pin="RB7" mode="digital" direction="out" latch="high"/>
// <function name="LCD_DB7" pin="RB8" mode="digital" direction="out" latch="high"/>

/*--------------------------------------------------------*/
// Définition des fonctions prototypes
/*--------------------------------------------------------*/
unsigned char lcd_read_byte( void );
void lcd_send_nibble( unsigned char n );
void lcd_send_byte( unsigned char address, unsigned char n );
void lcd_init(void);
void lcd_gotoxy( unsigned char x, unsigned char y);
void lcd_putc( unsigned char c);
void lcd_put_string_ram( char *ptr_char );
void lcd_put_string_rom( const char *ptr_char );
char lcd_getc( unsigned char x, unsigned char y);
void lcd_bl_on( void );
void lcd_bl_off( void );
void printf_lcd(const char *format,  ...);
// Ajout a la demande des utilisateurs
// Auteur C. Huber 02.12.2014
void lcd_ClearLine( unsigned char NoLine);

#endif
