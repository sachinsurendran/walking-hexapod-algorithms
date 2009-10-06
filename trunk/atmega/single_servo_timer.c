#include <avr/io.h>
#include <avr/interrupt.h>

#define  sbi(port,bit)  (port |= (1<<bit))   //set bit in port
#define  cbi(port,bit)  (port &= ~(1<<bit))  //clear bit in port 
/*
 * Do a servo control program to get servo to turn to 90 degree 
 */
#define STATES 2 //ON and OFF
#define ON  1
#define OFF 0
unsigned int timer_ticks[STATES]={ 0x5A55, 934};// Absolute 0 = 1000
//unsigned int timer_ticks[STATES]={ 0x5A55, 1319};//0
//unsigned int timer_ticks[STATES]={ 0x5A55, 1528};//40
//unsigned int timer_ticks[STATES]={ 0x5A55, 1666};//60
//unsigned int timer_ticks[STATES]={ 0x5A55, 1805};//80
//unsigned int timer_ticks[STATES]={ 0x5A55, 1944};//100
//unsigned int timer_ticks[STATES]={ 0x5A55, 2083};//120
//unsigned int timer_ticks[STATES]={ 0x5A55, 2500};//180
//unsigned int timer_ticks[STATES]={ 0x5A55, 2795};//270
//unsigned int timer_ticks[STATES]={ 0x5A55, 2795};//Experimental
unsigned char state = ON;// ON first


ISR(TIMER1_COMPA_vect)
{
#ifdef DEBUG_LED
    static unsigned char i=0;
    if (i==20){
        if(PORTD) {
            PORTD = 0x00;
        }else {
            PORTD = 0xff;
        }
        i = 0;
    }
    i++;
#endif

    OCR1A = timer_ticks[state];
    /* Keep turning state ON & OFF */
    if(state){
        /* Any bit on PortA can be connected to servo */
        PORTA = 0xff;
    }else {
        PORTA = 0x00;
    }

    state = !state;
//    TCNT1 = 1;// Avoid any race condition

    return ;
}

int main(void)
{
    /* Timer control registers TCCR1B & TCCR1A */
    TCCR1B = 0x0A;
              /* 000 01  010 */
              /* DDD CTC 1/8clk */
    TCCR1A = 0x00;//Nothing of interest

    /* 
     * Output compare registers 
     * 90 deg translates to 1.5ms ON and 18.5ms OFF
     */ 

     //ON first
     OCR1A = timer_ticks[OFF];

    /* Interrupt settings in Interrupt MAsk register */
    TIMSK  = 0x00;//Clear all and set just one of interest 
    sbi(TIMSK,OCIE1A);     //OCIE1A = 1;/*Raise interrupt on match OCR1A */

    /* set counter to 1 */
    TCNT1 = 1;
    /* Io ports enable */
    DDRD=0xff;//Enable portD
        
    DDRA=0xff;//For debugging

    PORTD = 0x01;
    sei();// Enables interrupts

    while(1) {
        ;
    }
}

