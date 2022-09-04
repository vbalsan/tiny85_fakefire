#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//ordre GRB, plus petit en premier




// Where the WS2811 is attached (PB4)
//#define LEDS PB0
#define LEDS PB3


//#define COM_DFP_RX PB3
#define COM_DFP_TX PB4

#define COM_IR_RX PB1
#define COM_IR_TX PB2

#define nop __asm__("nop\n\t")

void init() 
{
    DDRB |= (1 << COM_IR_TX);
    DDRB |= (1 << COM_DFP_TX);

    PORTB &= ~(1 << LEDS);
    DDRB |= (1 << LEDS);
}


/* timings
T0H 250ns     500ns
T0L 1000ns    2000ns

T1H 875ns     1750ns
T1L 375ns     750ns
*/  

void sendByte(uint8_t theByte)
{
  uint8_t myMask = 0b10000000;
    while(myMask)
    {
      if (theByte & myMask)
      {

        #pragma GCC push_options
        #pragma GCC optimize ("O0")
        // Send 1
        PORTB |= (1 << LEDS);   // Hi (start)
        //PORTB = 0b00001000;     // 1 cycle
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");

        PORTB &= ~(1 << LEDS);
        //PORTB = 0b00000000;     // 1 cycle
        asm("nop");
        //nop;
        #pragma GCC pop_options
      }

      else 
      {
        #pragma GCC push_options
        #pragma GCC optimize ("O0")
        // Send 0
        PORTB |= (1 << LEDS);   // Hi (start)
        //PORTB = 0b00001000;     // 1 cycle
        //asm("nop");


        PORTB &= ~(1 << LEDS);
        //PORTB = 0b00000000;     // 1 cycle
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        #pragma GCC pop_options
      }
      myMask >>= 1;           // 1 cycle
    }

    //PORTB &= ~(1 << LEDS);  // Lo (500ns)
}

// Send to the string
void sendOnePixel(uint8_t green, uint8_t red, uint8_t blue)  
{
    cli();   // Interrupts off
    sendByte(green);
    sendByte(red);
    sendByte(blue);    
    sei();
}


int main()
{
  init();

  //uint8_t b = 255;
  uint8_t idx,g,r,b;
  g = 137;
  r = 255;
  b = 0;

  while(1)
  {        
    for(idx = 0; idx <= 49; idx++, g++, r++, b++)
    {
      sendOnePixel(g,r,b);      
    }     
    _delay_us(50);

    _delay_ms(250); 
  }
}
