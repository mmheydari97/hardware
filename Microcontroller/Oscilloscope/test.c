/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 7/6/2018
Author  : 
Company : 
Comments: 


Chip type               : ATmega32A
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32a.h>

#include <delay.h>

#include <stdbool.h>
// Declare your global variables here

char digits[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,
                 0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
                 
unsigned char Fs_LSB,Fs_MSB;
unsigned char Length_LSB,Length_MSB;
unsigned int sum=0;
unsigned char checksum;                 
                        
unsigned char temp[1000];
unsigned int prescaler[5] = {1, 8, 64, 256, 1024};
bool check[5]={false, false, false, false, false}; 
unsigned int i, j, k;
unsigned int length,Fs;


float Tosc =  1.25e-7;
float Ts, N, r;
bool flag, flag2, flag3;
//unsigned char r;


// Standard Input/Output functions
#include <stdio.h>
#include <math.h>

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (1<<ADLAR))

// Read the 8 most significant bits
// of the AD conversion result
unsigned char read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCH;
}

void calc_timerprescaler(void)
{
    for (i = 0; i < 5; i++)
    {
        N = Ts/(prescaler[i]*Tosc);
        r = N - floor(N);
        if(N <= 0xFFFF)
            if(N-r == 0)
                j = i;
            else
            {
                flag = true;
                check[i] = true;
            }
                    
    }
    
    if(flag)
        for (i = 0; i < 5; i++)
            if(check[i])
                j = i;
                
    
    switch(j)
    {
        case 1:
            TCCR1B |= 0x0A;
            //OCR1A = (unsigned int)(Ts/(prescaler[1]*Tosc));
            OCR1AH = ((int)(Ts/(prescaler[1]*Tosc)))/256;
            OCR1AL = ((int)(Ts/(prescaler[1]*Tosc)))%256;
            break;
        case 2:
            TCCR1B |= 0x0B;
            OCR1AH = ((int)(Ts/(prescaler[2]*Tosc)))/256;
            OCR1AL = ((int)(Ts/(prescaler[2]*Tosc)))%256;
            break;
        case 3:
            TCCR1B |= 0x0C;
            OCR1AH = ((int)(Ts/(prescaler[3]*Tosc)))/256;
            OCR1AL = ((int)(Ts/(prescaler[3]*Tosc)))%256;
            break;
        case 4:
            TCCR1B |= 0x0D;
            OCR1AH = ((int)(Ts/(prescaler[4]*Tosc)))/256;
            OCR1AL = ((int)(Ts/(prescaler[4]*Tosc)))%256;
            break;
        default:
            TCCR1B |= 0x09;
            OCR1AH = ((int)(Ts/(prescaler[0]*Tosc)))/256;
            OCR1AL = ((int)(Ts/(prescaler[0]*Tosc)))%256;
    }   
}


void my_init(void);

void main(void)
{
// Declare your local variables here

my_init();

PORTD.6 = 0;  // turn off right 7-seg
PORTD.7 = 1;  //turn on left 7-seg  and show 'A'
PORTC = digits[10];

while (1)
      {
      // Place your code here
            TCCR1B &= 0xF8;       // Timer/Counter stopped

            if(getchar()=='S')
            {   
                
                putchar('S');
                
				
				// getting Fs
                Fs_MSB = getchar();
                putchar('M');
                Fs_LSB = getchar();
                putchar('L');
                Fs = Fs_MSB*256 + Fs_LSB;
                Ts =(float) (1.0 / Fs);
                
				// getting length
                Length_MSB = getchar();
                putchar('A');
                Length_LSB = getchar();
                putchar('B');
                length = Length_MSB*256 + Length_LSB;
                
				// Set prescaler and Output compare register A(OCR1A) 
                calc_timerprescaler();
                
                TCNT1=0;  
                flag2 = false;
                flag3 = false;
                i=0;
                k=0;
                sum=0;   
                while(i<length)
                {
                    //want to send
                    if((UCSRA & 0x80) ==0x80)
                    {
                        if(UDR=='N')
                        {   
                            flag2 = true;
                        } 
                    }
                    
                    //want to receive
                    if((TIFR & (1 << OCF1A))==0x10)
                    {   
                        
                        TIFR = (1 << OCF1A);
                        temp[i] = read_adc(0);
                        sum += temp[i];
                        flag3 = true;
                        i++;
                    }
                    //k is number of data sent
                    if(flag2 && flag3)
                    {
                        putchar(temp[k]);
                        k++;
                        flag2 = false;
                        flag3 = false;      
                    }
                    
                }
                        

				// continue sending data
                for( i=k ; i < length ; i++)
                {
                    if(getchar()=='N')
                        putchar(temp[i]);    
                }

					
                // sending checksum
				checksum = sum % 256;				
                if(getchar()=='E')
                    putchar(checksum);
            }
			
			// if app won't work , show 'E' on 7-seg and send 'T'
            else
            {   
                PORTC = digits[14];
                putchar('T');         
            }
			
            for( i=0 ; i < length ; i++)
                temp[i]=0;

      }
}



void my_init(void)
{

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=Out Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(1<<DDA7) | (1<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=0 Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRC=(1<<DDC7) | (1<<DDC6) | (1<<DDC5) | (1<<DDC4) | (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=Out Bit6=Out Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(1<<DDD7) | (1<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=0 Bit6=0 Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 8000.000 kHz
// Mode: CTC top=OCR1A
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.125 us
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1A = 0x00FF;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 115200
//UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
//UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
//UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
//UBRRH=0x00;
//UBRRL=0x03;
UCSRA=0x02;
UCSRB=0x18;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x08;

// Analog Comparator initialization
// Analog Comparator: On
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
// Analog Comparator Input Capture by Timer/Counter 1: On
ACSR=(0<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (1<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 62.500 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: Free Running
// Only the 8 most significant bits of
// the AD conversion result are used
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (1<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);


// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

}


 /*
                i=(int)(Ts) & 0xF000;
                i = i>>12;
                PORTC=digits[i];
                delay_ms(1000);
                i=(int)(Ts) & 0x0F00;
                i= i>>8;
                PORTC=digits[i];
                delay_ms(1000);
                i=(int)(Ts) & 0x00F0;
                i = i>>4;
                PORTC=digits[i];
                delay_ms(1000);
                i=(int)(Ts) & 0x000f;
                PORTC=digits[i];
                delay_ms(1000);
                */