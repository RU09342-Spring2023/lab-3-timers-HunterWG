/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Hunter Geitz
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}


void gpioInit(){
    // @TODO Initialize the Red or Green LED

    // @TODO Initialize Button 2.3
    // Configure RED LED on P1.0 as Output
         P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
         P1DIR |= BIT0;                          // Set P1.0 to output direction

         // Configure Green LED on P6.6 as Output
         P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
         P6DIR |= BIT6;                          // Set P6.6 to output direction


         // Configure Button on P2.3 as input with pullup resistor
         P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
         P2REN |= BIT3;                          // P2.3 pull-up register enable
         P2IES &= ~BIT3;                         // P2.3 Low --> High edge
         P2IE |= BIT3;                           // P2.3 interrupt enabled
}

void timerInit(){
    // Initialize Timer B1
    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
        TB1CCR0 = 50000;
        TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode

        __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3 w/ interrupt
}


/*
 * INTERRUPT ROUTINES
 */
char state = SLOW_STATE;                           // Initializing first state as SLOW_STATE
// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.

    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.
switch (state)
case SLOW_STATE:
case MEDIUM_STATE:
case FAST_STATE:
}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
    TB1CCR0 += 
}

