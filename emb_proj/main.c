#define SYSCTL_RCGCGPIO_R  *((volatile unsigned long *) 0x400FE608)
	
// B --> right motor
// A --> left motor
//pin7=enableB, pin6=backwardB, pin5=forwardB, pin4=forwardA, pin3=backwardA, pin2=enableA
#define FORWARD  0xB4 //1011 0100  
#define LEFT  0xAC   //1010 1100
#define RIGHT  0xD4   //1101 0100
#define STOP  0x00   //0000 0000

#define BBB  0x07
#define BBW  0x06 
#define WBB  0x03
#define BWW  0x04
#define WWB  0x01
#define WWW	 0x00

#define PORTB_PIN012 0x07
#define PORTB_CLK_EN 	0x02  
#define PORTA_PIN234567 0xFC
#define PORTA_CLK_EN 0x01;

#define GPIO_PORTA_BASE 0x40004000
#define GPIO_PORTA_DATA_R  *((volatile unsigned long *) (GPIO_PORTA_BASE + 0x3F0))
#define GPIO_PORTA_DIR_R   *((volatile unsigned long *) (GPIO_PORTA_BASE + 0x400))
#define GPIO_PORTA_DEN_R   *((volatile unsigned long *) (GPIO_PORTA_BASE + 0x51C))
	
#define GPIO_PORTB_BASE 0x40005000
#define GPIO_PORTB_DATA_R		(*((volatile unsigned long*) (GPIO_PORTB_BASE +0x3FC)))
#define GPIO_PORTB_DIR_R		(*((volatile unsigned long*) (GPIO_PORTB_BASE +0x400)))
#define GPIO_PORTB_DEN_R		(*((volatile unsigned long*) (GPIO_PORTB_BASE +0x51C)))

/*********************** MOTOR FUNCTIONS *************************/
void moveForward (void){
  GPIO_PORTA_DATA_R = FORWARD;
}
void moveLeft (void){
  GPIO_PORTA_DATA_R = LEFT;
}
void moveRight (void)	{
 GPIO_PORTA_DATA_R = RIGHT;
}
void Stop (void){
 GPIO_PORTA_DATA_R = STOP;
}

/********************** INIT FUNCTIONS **************************************/

void GPIO_PortA_Init(){
  GPIO_PORTA_DEN_R |= PORTA_PIN234567;
  GPIO_PORTA_DIR_R |= PORTA_PIN234567;
}

void GPIO_PortB_Init(){
  GPIO_PORTB_DEN_R |= PORTB_PIN012;
  GPIO_PORTB_DIR_R &= ~PORTB_PIN012;
}

/************************ MAIN ****************************/

int main ()
{
	volatile unsigned delay_clk;
	SYSCTL_RCGCGPIO_R |= 0x03;  // PORTA_CLK_EN + PORTB_CLK_EN
	delay_clk 		    = SYSCTL_RCGCGPIO_R;
	
	GPIO_PortA_Init();
	GPIO_PortB_Init();
	
	while(1)
	{		
		if(GPIO_PORTB_DATA_R == BBB){ 				//forward --> black-black-black
				moveForward();			
		}
		else if(GPIO_PORTB_DATA_R == BBW ){ 	//left  --> black-black-white
				moveLeft();		
		}
		else if(GPIO_PORTB_DATA_R == WBB ){		//right --> white-black-black
				moveRight();		
		}
		else if(GPIO_PORTB_DATA_R == BWW ){ 	//left --> black-white-white
				moveLeft();			
		}
		else if(GPIO_PORTB_DATA_R == WWB ){ 	//right --> white-white-black
				moveRight();
		}
		else if(GPIO_PORTB_DATA_R == WWW ){	  //stop --> white-white-white
				Stop();	
		}
	}
}