/**
 * main.c
 */
#include "tm4c123.h"

void initClock()
{
    // TivaC has an on-board 16MHz crystal.
    // The onboard PLL uses this to produce a 400MHz signal
    // This is first divided by 2 then
    // divided by 1+ the divisor (3) used below.  The net result
    // is a 50MHz operating frequency.
    SYSCTL_RCC &= ~(0x1f << 6);
    SYSCTL_RCC |= (0x15 << 6);
    SYSCTL_RCC &= ~(0x0f<<23); // set up a
    SYSCTL_RCC |= (3 << 23);  // divisor of 3+1 which should give a clock speed of 50MHz
    SYSCTL_RCC &= ~(1 << 13); // clear power down bit
    SYSCTL_RCC &= ~(1 << 0); // enable MOSC
    SYSCTL_RCC &= ~(1 << 11); // turn off bypass bit
}

void initPWM()
{
    SYSCTL_RCGCPWM |= (1 << 1); // turn on PWM1
    SYSCTL_RCGC2 = SYSCTL_RCGC2 | (1 << 5); // turn on GPIOF
    SYSCTL_GPIOHBCTL = SYSCTL_GPIOHBCTL | (1 << 5); // turn on AHB access to GPIOF

    // Will drive the LED's using PWM
    // PF1 -> pin 29 -> Red  -> M1PWM5 Module 1, PWM Gen 2, GPIOPCTL value = 5
    // PF2 -> pin 30 -> Blue -> M1PWM6 Module 1, PWM Gen 3, GPIOPCTL value = 5
    // PF3 -> pin 31 -> Green-> M1PWM7 Module 1, PWM Gen 3, GPIOPCTL value = 5
    GPIOF_AFSEL |= (1<<3) | (1 << 2) | (1 << 1); // select alternative function for GPIOF1,2,3
    GPIOF_DEN = GPIOF_DEN | ( (1 << 3) | (1 << 2 ) | (1 << 1) ); // digital mode bits 1,2,3 of GPIOF
    GPIOF_PCTL = (5 << 12) | (5 << 8) | (5 << 4);

    PWM1_PWMENABLE |= (1 << 7)| (1 << 6) | (1 << 5);
    PWM1_PWM2LOAD = 50000000/25000; // 25kHz assuming system clock is 50MHz
    PWM1_PWM3LOAD = 50000000/25000; // 25kHz assuming system clock is 50MHz
    // PWM counter will count down.  When it reaches 0 the output is set to zero
    // when it counts down to the value in the CMPA or CMPB register the output is
    // driven high.  This means that the duty is proportional to the value in CMPA or CMPB
    PWM1_PWM2GENB = (2 << 0) + (3 << 10); // Drive high on match, low on zero (gen b)
    PWM1_PWM3GENA = (2 << 0) + (3 << 6); // Drive high on match, low on zero (gen a)
    PWM1_PWM3GENB = (2 << 0) + (3 << 10); // Drive high on match, low on zero (gen b)
    PWM1_PWM2CMPB = PWM1_PWM2LOAD/4; // 25% duty for red
    PWM1_PWM3CMPA = (2*PWM1_PWM3LOAD)/4; // 50 % duty for green
    PWM1_PWM3CMPB = (3*PWM1_PWM3LOAD)/4; // 75 % duty for blue
    PWM1_PWM2CTL |= (1 << 0); // enable pwm block
    PWM1_PWM3CTL |= (1 << 0); // enable pwm block
    PWM1_PWMSYNC = 0x0f; // synchronize all counters
}
void main()
{
    initClock();
    initPWM();

    while(1)
    {
    }
}

