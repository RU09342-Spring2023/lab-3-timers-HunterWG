/*
 * Part3.c
 *
 *  Created on: April 23, 2023
 *      Author: Hunter Geitz
 *
 *      YOU NEED TO FILL IN THIS AUTHOR BLOCK
 */
#include <msp430.h>
#include <msp430fr2355.h>

unsigned int Press_Time = 0;
unsigned int Blink= 50000;
unsigned const int Blink_Initial = 50000;

void gpioInit();
void timerInit();

void main()
{
    WDTCTL = WDTPW + WDTHOLD;                       // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    gpioInit();
    timerInit();
    __enable_interrupt();                           // Enable global interrupts
    __bis_SR_register(LPM3_bits | GIE);             // Enter LPM with interrupts enabled
}
void gpioInit()
{
    //Config 1.0(Red LED)
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;
    //Config 2.3(Button)
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES |= BIT3;                          // P4.1 High to low edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled
    //Config 4.1(Reset Button)
    P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES |= BIT1;                          // P4.1 High to low edge
    P4IE |= BIT1;                           // P4.1 interrupt enabled
}
void timerInit(){
    //Cofig TB0CCR0 (Timer B0)
    TB0CCR0 = 100;
    TB0CTL |= TBSSEL_1 + MC_0 + ID_3 + TBCLR;       // Set ACLK and clear it
    TB0CCTL0 |= CAP + CM_3;                         // Set Timer B0 to capture mode 
    //Config TB1CCR0 (Timer B1)
    TB1CTL = TBSSEL_1 | MC_1 | ID_3;                // Using ACLK in up mode
    TB1CCR0 = Blink;                                // Set the max count for Timer B1
    TB1CCTL0 = CCIE;                                // TBCCR0 interrupt enabled
}
// Button 2.3 interrupt service routine - Input button
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    if (P2IES & BIT3)                     // Checks if button has a rising edge
    {
        P2IES &= ~BIT3;                   
        TB0CTL |= MC_2;                  
    }
    else if (P2IES != BIT3)               // Checks if button has a falling edge
    {
        TB0CTL &= ~(MC0 + MC1);           // Stop timer B0
        Press_Time = TB0R;                
        TB1CCR0 = Press_Time;             // Set B1 max 

        P2IES |= BIT3;                    // Change edge sensitivity for rising edge next
        TB0CTL |= TBCLR;                  // Clearing B0 for next cycle
    }
}
// Button 4.1 interrupt service routine - Reset button
#pragma vector = PORT4_VECTOR
__interrupt void Port4_ISR(void)
{
    Blink = Blink_Initial;         // Set Blink to initial
    TB1CCR0 = Blink;               // Set CCR0 to Blink
    TB0CTL |= TBCLR;               // Clear Timer B0
    P4IFG &= ~BIT1;                // Clear interrupt flag for P4.1
    TB0CTL &= ~(MC0 + MC1);        // Stop Timer B0 - just in case
}
// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    P1OUT ^= BIT0;          // Toggle Red LED
}





