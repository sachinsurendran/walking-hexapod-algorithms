#include <at89x52.h>
#define MAX_SERVOS 3
#define MID1 0xFE//0x1
#define MID2 0x4A//0xB5
#define RT1  0xFF//0x0
#define RT2  0x9F//0x60
#define LFT1 0xFC//0x3
#define LFT2 0xF5//0x0A
#define LOW1 0xDF //0x20
#define LOW2 0x72
#define TRUE 1
#define FALSE 0
//#define SER1 1
//#define SER2 2
//#define SER3 3
#define LFT 0
#define MID 90
#define RT 180
#define XTAL_FREQ_MHZ 4

#define TOTAL_PULSE_WIDTH ( 0xFFFF - (33000 * XTAL_FREQ_MHZ /12) )

#define TURN_OFF MAX_SERVOS // = 3
#define OFF_TIME_PULSE_WIDTH 0x5955 //(OLD value) 0x5BEF
#define CONST_ANG 0xF395 

/*
 * Known Bugs 
 * 1. Value of angles of servos when close, creates a small error in position
 *    NOTE: code present but hashed out due to jitter for angle 0  
 * 2. Dont have full 180` rotation [ FIXED: Sep 19 07]
 * 3. Shit of servo x+1 on recipt of command for x [Reduced by not disabling counting in interrupt TR2=1 ]
 */


char sched[MAX_SERVOS + 1]={0,1,2,TURN_OFF};//Where TURN_OFF is the all signals turnoff time

/* boundary of 0 -145 degree */
unsigned char servo_ang[MAX_SERVOS+1]={1, 50, 70, 0};

/* Timer ticks to be loaded as per scheduled order */
int timer_ticks_per_servo[MAX_SERVOS+1]={0,0,0,0};
int temp_ang =0;

                          //   0,  1, 2, TURN_OFF
// sched[slot] = servo
// servo_ang[servo] = angle

/* UART Variables */
unsigned char uart_data;
unsigned char uart_servo=0;
unsigned char uart_cmd[4];

unsigned char curr_scheduler_slot = 0; //increment with every iteration of scheduler

void sort_schedule();
void set_timer();// Platform dependant
void set_signal_out(char last_servo);
char get_last_servo();
void increment_sched_slot();
char get_next_sched_slot();
void calc_timer_array();

int get_servo_cmd(unsigned char dat);


int get_servo_cmd(unsigned char dat) 
{
	static char indx=0;
	unsigned char servo;
	unsigned char angle;

	uart_cmd[indx++] = dat;
	P0_0=1;
	if (indx == 2) 	{
		P0_1=1;
		indx = 0;
		// message complete
		servo = uart_cmd[0];
		angle = uart_cmd[1];

		if (servo >=0 && servo < MAX_SERVOS) 
		{	
			//Check servo no
		    if( angle >= 0 && angle <= 180) 
			{ //Check angle
				/* All is well */
				servo_ang[servo]=angle;
				P0_2=1;
				return TRUE;
			}
		}
		
	}
	return FALSE;

}




void timer2_interrupt(void) interrupt 5
{
	//TR2 = 0x0; // No more counting
	EA  = 0x0; // Disable interrupts, We should disable counting first.
	TF2 = 0x0; //Clear Interrupt bit


	set_signal_out(get_last_servo());
	set_timer();
	increment_sched_slot();

	EA=1;
	//TR2=1;
	
}

void uart_interrupt(void) interrupt 4
{
    EA = 0x0; //Disable interrupts
    if (RI == 1)
	{ /* if reception occur */
		RI = 0; /* clear reception flag for
               next reception */
        uart_data = SBUF; /* Read receive data */
		if (get_servo_cmd(uart_data) == TRUE) {
	    	SBUF = 0x08; /* Send back YES on uart*/
		}
    }
    else
    {
        TI = 0;
     /* clear transmit flag for
     next transmission*/
    }
    EA = 1; // Enable Interrupts
}

void main()
{
	/* Test counter : Remove */
	int test_counter = 0;

	T2MOD=0x0;//Down Count = 1 , else up count
	T2CON=0;//Null it
	C_T2=0;// C_T2 = 0 means timer	
	
	EXEN2=0x0;//disable external pin connectivity
	CP_RL2=0x0;// In Auto reload mode

	IE=0;//Disable all interrupts

	/* UART initialization */
    SCON = 0x50; // uart in mode 1 (8 bit), REN=1 
    TMOD = TMOD | 0x20 ; // Timer 1 in mode 2 
    TH1 = 0xF7; // 4800 Bds at 16.0 MHz 
    TL1 = 0xF7; // 4800 Bds at 16.0 MHz 
    ES = 0x1; // Enable serial interrupt
    IE = 0x91; //enable interrupts, and external interrupt 0
    TR1 = 1; // Timer 1 run */
    /* UART init done */
	
	ET2=0x1; //Enable Timer 2
	

	//sort_schedule();
	P1_0 = 0;
	P1_1 = 0;
	P1_2 = 0;

	RCAP2H = LOW1; // Turn off time for all servos
	RCAP2L = LOW2; // Turn off time for all servos
	curr_scheduler_slot = TURN_OFF;//Since we are loading for TURN_OFF
	
	//calc_timer_array();

	TR2=0x1; //Enable connect to counter :)

	EA=0x1;//Enable Interupts

	/*Test CODE*/
	P0_0=P0_1=P0_2=0;
	
	while(1) {
		calc_timer_array();
	}

}


char
get_next_sched_slot() {
	if (curr_scheduler_slot == TURN_OFF) {
		return 0;
	} else {
		return curr_scheduler_slot + 1;
	}
}

char 
get_last_servo() {
	if (curr_scheduler_slot == 0)
	{
		return TURN_OFF;
	} else {
		return curr_scheduler_slot - 1;
	}
}

void 
set_signal_out(char last_servo) {

	switch (last_servo)
	{
	case 0:
		P1_0 = 0;
		P1_1 = 1;
		P1_2 = 0;
		break;
	
	case 1:
		P1_0 = 0;
		P1_1 = 0;
		P1_2 = 1;
		break;
 
	case 2:
		P1_0 = 0;
	    P1_1 = 0;
		P1_2 = 0;
	    break;

	case TURN_OFF:
		P1_0 = 1;
	    P1_1 = 0;
		P1_2 = 0;
		break;
	}
}


void 
set_timer() {

	/* Load values from Precalculated Array */
	RCAP2H = timer_ticks_per_servo[curr_scheduler_slot] >> 8;
	RCAP2L = timer_ticks_per_servo[curr_scheduler_slot] & 0xff;

}

void
calc_timer_array()
{
	int servo_slot;
	int counter;
	
	/*
	 * Algo needs counter to have value of first servo to proceed
	 * ,Do that calc beforehand 
	 */
	counter = (15 * servo_ang[0]) + CONST_ANG; 
	timer_ticks_per_servo[TURN_OFF] = counter;

	for(servo_slot = 0; servo_slot < MAX_SERVOS; servo_slot++) {

		if (servo_slot == (TURN_OFF - 1)) { // Load OFF time for Servo
			timer_ticks_per_servo[TURN_OFF - 1] = OFF_TIME_PULSE_WIDTH;//0xABE0 + counter_prev - 0xFFFF;
			break;
		}
		//For a sched slot we load for the next slot,hence 'sched_slot + 1'
		counter =  ( 15 * servo_ang[servo_slot + 1]) + CONST_ANG; 
		timer_ticks_per_servo[servo_slot] = counter;
	}

}

void
increment_sched_slot()
{
	if (curr_scheduler_slot == TURN_OFF)	{
	curr_scheduler_slot = 0;
	} else {
	curr_scheduler_slot++;
	}
}

