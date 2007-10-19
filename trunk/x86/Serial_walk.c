#include <dos.h>
#include <stdio.h>
#include <conio.h>

#define PORT1 0x3F8  /* Defines Serial Port Base Address (COM1 */

#define Servo_Left 0
#define Servo_Right 1
#define Servo_Middle 2

#define Middle_RightUp 65
#define Middle_RightUp_Semi 80
#define Middle_Center 90
#define Middle_LeftUp_Semi 105
#define Middle_LeftUp 120

#define Left_Forward 120
#define Left_Center 100
#define Left_Backward 80
#define Right_Forward 85
#define Right_Center 115
#define Right_Backward 135

#define DELAY 600

void main(void){
    unsigned char c = 0;
    unsigned char chrctr = 0;
	/*int servo[3] = {0,1,2};*/
	/*int exit = 1; */

    outportb(PORT1 + 1, 0); /* Turn off interrupts */

    /* PORT1 Communication Settings */

    outportb(PORT1 + 3, 0x80); /* Set DLAB ON */
    outportb(PORT1 + 0, 0x18); /* Set the baud rate to 4800 */

    outportb(PORT1 + 1, 0x00); /* Set Baud - Divisor latch HIGH */
    outportb(PORT1 + 3, 0x03); /* 8 bits, no parity, 1 stop */
    outportb(PORT1 + 2, 0xC7); /* FIFO Control Register */
    outportb(PORT1 + 4, 0x0B); /* Turn on DTR, RTS, and OUT2) */

    printf("Waiting on transmission from source.\nPress ESC to quit.\n");

	sleep(1);
	outportb(PORT1,Servo_Middle);
	sleep(1);
	c = inportb(PORT1 + 5);
	if (c & 0x01){
        chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
        }
	outportb(PORT1,Middle_Center);
	sleep(1);
	c = inportb(PORT1 + 5);
    if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
        }

	outportb(PORT1,Servo_Left);
	sleep(1);
	c = inportb(PORT1 + 5);
    if (c & 0x01){
        chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}
	outportb(PORT1,Left_Center);
	sleep(1);
	c = inportb(PORT1 + 5);
    if (c & 0x01){
        chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
        }

	outportb(PORT1,Servo_Right);
	sleep(1);
	c = inportb(PORT1 + 5);
    if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
        }
	outportb(PORT1,Right_Center);
	sleep(1);
	c = inportb(PORT1 + 5);
    if (c & 0x01){
        chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
        }

    while(!kbhit()){      /*chrctr != 27){       Execute the loop if ESC has been hit */
    /*c = inportb(PORT1 + 5);
    if (c & 0x01){
        chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}
      printf("Enter Servo:");
      scanf("%d",&servo);
      printf("Enter angle:");
      scanf("%d",&angle); */

	   /*outportb(PORT1,servo);
	   sleep(1);
	   outportb(PORT1,angle);
	   sleep(1);*/

//Middle........
	    outportb(PORT1,Servo_Middle);//Middle
	    delay(100);
	    c = inportb(PORT1 + 5);
	    if (c & 0x01){
		chrctr = inportb(PORT1);
	    printf("ACK= %d\n",chrctr);
		}

	outportb(PORT1,Middle_RightUp_Semi);
	delay(100);
	c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
	printf("ACK= %d\n",chrctr);
	}
	    outportb(PORT1,Servo_Middle);//Middle
	    delay(150);
	    c = inportb(PORT1 + 5);
	    if (c & 0x01){
	chrctr = inportb(PORT1);
	    printf("ACK= %d\n",chrctr);
	}

	outportb(PORT1,Middle_RightUp);
	delay(250);
	c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
	printf("ACK= %d\n",chrctr);
	}
//Right........
	   outportb(PORT1,Servo_Right);//Right Forward
	   delay(250);
	   c = inportb(PORT1 + 5);
       if (c & 0x01){
       chrctr = inportb(PORT1);
	   printf("ACK= %d\n",chrctr);
       }

	   outportb(PORT1,Right_Forward);
	   delay(50);
	   c = inportb(PORT1 + 5);
       if (c & 0x01){
       chrctr = inportb(PORT1);
	   printf("ACK= %d\n",chrctr);
       }

//Left........

	    outportb(PORT1,Servo_Left);//Left Back Thrust
	    delay(50);
		c = inportb(PORT1 + 5);
	    if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

	    outportb(PORT1,Left_Backward);
	    delay(350);
		c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

//Middle........
	outportb(PORT1,Servo_Middle);//Middle
	    delay(100);
	    c = inportb(PORT1 + 5);
	    if (c & 0x01){
		chrctr = inportb(PORT1);
	    printf("ACK= %d\n",chrctr);
		}

	outportb(PORT1,Middle_RightUp_Semi);
	delay(100);
	c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
	printf("ACK= %d\n",chrctr);
	}


	    outportb(PORT1,Servo_Middle);//Middle
	    delay(150);
	    c = inportb(PORT1 + 5);
	    if (c & 0x01){
	chrctr = inportb(PORT1);
	    printf("ACK= %d\n",chrctr);
	}

	    outportb(PORT1,Middle_Center);
	    delay(150);
		c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

    outportb(PORT1,Servo_Middle);
	    delay(100);
		c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

	    outportb(PORT1,Middle_LeftUp_Semi);
	    delay(100);
		c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

	    outportb(PORT1,Servo_Middle);
	    delay(150);
		c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

	    outportb(PORT1,Middle_LeftUp);
	    delay(250);
		c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

		outportb(PORT1,Servo_Left);//Left Forward
	    delay(150);
		c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}

	    outportb(PORT1,Left_Forward);
	    delay(50);
	    c = inportb(PORT1 + 5);
	if (c & 0x01){
	chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
	}


	   outportb(PORT1,Servo_Right);//Right Thrust
	   delay(50);
	   c = inportb(PORT1 + 5);
       if (c & 0x01){
       chrctr = inportb(PORT1);
	   printf("ACK= %d\n",chrctr);
       }

	   outportb(PORT1,Right_Backward);
	   delay(250);
	   c = inportb(PORT1 + 5);
       if (c & 0x01){
       chrctr = inportb(PORT1);
	   printf("ACK= %d\n",chrctr);
       }

		outportb(PORT1,Servo_Middle);
	    delay(100);
		c = inportb(PORT1 + 5);
		if (c & 0x01){
		chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
		}

	    outportb(PORT1,Middle_LeftUp_Semi);
	    delay(100);
		c = inportb(PORT1 + 5);
		if (c & 0x01){
		chrctr = inportb(PORT1);
		printf("ACK= %d\n",chrctr);
		}

	   outportb(PORT1,Servo_Middle);
	   delay(150);
	   c = inportb(PORT1 + 5);
       if (c & 0x01){
       chrctr = inportb(PORT1);
	   printf("ACK= %d\n",chrctr);
       }

	   outportb(PORT1,Middle_Center);
	   delay(150);
	   c = inportb(PORT1 + 5);
       if (c & 0x01){
       chrctr = inportb(PORT1);
	   printf("ACK= %d\n",chrctr);
       }

    }
}









