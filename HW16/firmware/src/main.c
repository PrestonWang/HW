


#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes
#include <xc.h> // processor SFR definitions
#include <sys/attribs.h>

int u1;
int u2;
    
int main ( void )
{
 

    SYS_Initialize ( NULL );
     ANSELBbits.ANSB13 = 0;
    TRISBbits.TRISB13 = 1;
    INT4Rbits.INT4R = 0b0100;

    // Set up A1 as phase select (right wheel)
    ANSELAbits.ANSA1 = 0;
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 1;

    // Set up B7 as phase select (left wheel
    ANSELBbits.ANSB0 = 0;
    TRISBbits.TRISB0 = 0;
    LATBbits.LATB0 = 1;

    // B15 for logic 3.3 v
    ANSELBbits.ANSB15 = 0;
    TRISBbits.TRISB15 = 0;
    LATBbits.LATB15 = 1;

    // set Timer2 at 1kHz
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TCKPS = 0b100;
    PR2 = 4999;
    TMR2 = 0;
    T2CONbits.ON = 1;

    // Set up B5 as OC2
    TRISBbits.TRISB5 = 0;
    RPB5Rbits.RPB5R = 0b0101;
    OC2CONbits.OCTSEL = 0;
    OC2CONbits.OCM = 0b110;
    OC2RS = 2500; // 50% duty cycle
    OC2R = 2500;
    OC2CONbits.ON = 1;

    // Set up B9 as OC3a
    //ANSELBbits.ANSB9 = 0;
    TRISBbits.TRISB9 = 0;
    RPB9Rbits.RPB9R = 0b0101;
    OC3CONbits.OCTSEL = 0;
    OC3CONbits.OCM = 0b110;
    OC3RS = 2500; // 50% duty cycle
    OC3R = 2500;
    OC3CONbits.ON = 1;

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

