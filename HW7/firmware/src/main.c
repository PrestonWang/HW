/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes
#include "i2c_display.h"
#include "i2c_master_int.h"
#include "accel.h"

#define width (10)
#define max_height (32)
#define row (32)
#define col (64)
#define max 32767

short accels[3];
void read_accel();
void printbar(int x, int y);

int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );

    int d = 0;
    while(d < 2000000){
    d = d+1;
    }

    // Initializing Display

    i2c_master_setup();
    display_init();
    display_clear();
    acc_setup();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        //read_accel();


    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

void read_accel(){
    int xaccel, yaccel;
    acc_read_register(OUT_X_L_A, (unsigned char *) accels, 6);
    xaccel = -accels[0]*2*max_height/max;
    yaccel = -accels[1]*2*max_height/max;
    printbar(xaccel, yaccel);
};

void printbar(int x,int y){
    int X, Y,w, on;
    display_clear();
    for(X = 0; X <= x; X = X + 1){
        for(w = 0; w <= width; w = w+1){
                display_pixel_set(row - width/2 + w, col + X ,1);
        };
    };
    for(X = 0; X >= x; X = X - 1){
        for(w = 0; w <= width; w = w+1){
                display_pixel_set(row - width/2 + w, col + X ,1);
        };
    };
    for(Y = 0; Y <= y; Y = Y + 1){
        for(w = 0; w <= width; w = w+1){
            display_pixel_set(row + Y, col - width/2 + w ,1);
        };
    };
    for(Y = 0; Y >= y; Y = Y - 1){
        for(w = 0; w <= width; w = w+1){
            display_pixel_set(row + Y, col - width/2 + w ,1);
        };
    };
    display_draw();
};

