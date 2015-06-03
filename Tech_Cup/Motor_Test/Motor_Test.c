
#include<xc.h> // processor SFR definitions
#include<sys/attribs.h> // __ISR macro

// DEVCFGs here

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // not boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // free up secondary osc pins
#pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = CSECME // do not enable clock switch
#pragma config WDTPS = PS1048576 // slowest wdt
#pragma config WINDIS = OFF // no wdt window
#pragma config FWDTEN = OFF // wdt off by default
//#pragma config FWDTWINSZ = WISZ_25 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 40MHz
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_20 // multiply clock after FPLLIDIV
#pragma config UPLLIDIV = DIV_2 // divide clock after FPLLMUL
#pragma config UPLLEN = ON // USB clock on
#pragma config FPLLODIV = DIV_1 // divide clock by 1 to output on pin

// DEVCFG3
#pragma config USERID = 0b11 // some 16bit userid
#pragma config PMDL1WAY = ON // not multiple reconfiguration, check this
#pragma config IOL1WAY = ON // not multimple reconfiguration, check this
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // controlled by USB module

int readADC(void);
int main() {

    // startup
    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that
    // kseg0 is cacheable (0x3) or uncacheable (0x2)
    // see Chapter 2 "CPU for Devices with M4K Core"
    // of the PIC32 reference manual
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // no cache on this chip!

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to be able to use TDI, TDO, TCK, TMS as digital
    DDPCONbits.JTAGEN = 0;

    __builtin_enable_interrupts();

    // set up USER pin as input

    ANSELBbits.ANSB13 = 0;
    TRISBbits.TRISB13 = 1;
    INT4Rbits.INT4R = 0b0100;
    
    // Set up B12 as phase select
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 0;

    // Set up B7 as phase select

    TRISBbits.TRISB7 = 0;
    LATBbits.LATB7 = 0;

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

    // Set up B15 as OC1a
    ANSELBbits.ANSB15 = 0;
    TRISBbits.TRISB15 = 0;
    RPB15Rbits.RPB15R = 0b0101;
    OC1CONbits.OCTSEL = 0;
    OC1CONbits.OCM = 0b110;
    OC1RS = 2500; // 50% duty cycle
    OC1R = 2500;
    OC1CONbits.ON = 1;

    // set up A0 as AN0

    while (1) {

    if (PORTBbits.RB13 == 1) {
        OC2RS = 5000;
        OC1RS = 5000;
    }
    else {
        OC2RS = 2500;
        OC1RS = 2500;
    }
    }
    // wait for half a second, setting LED brightness to pot angle while waiting

}

