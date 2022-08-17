#include <LiquidCrystal.h>

const byte RS = 11, EN = 12;
const byte DB4 = 4, DB5 = 5, DB6 = 6, DB7 = 7;
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

int flag = 0;
int ore = 0;
int minute = 0;
int secunde = 0;
int milisec = 0;

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Cronometru: ");
  
  DDRD |= (1 << 2) | (1 << 3);

  SREG |= (1 << SREG_I); //activare intreruperi globale
  
  EICRA |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00); //configurare intreruperi externe
  EIMSK |= (1 << INT1) | (1 << INT0);
  EIFR |= (0 << INTF1) | (0 << INTF0);
  PCICR |= (0 << PCIE2) | (0 << PCIE1) | (0 << PCIE0);
  
  TCCR0A = (1 << WGM01) | (0 << WGM00);
  
  OCR0A = 0xFFFF;
  TIMSK0 |= (1 << OCIE0A);
  TCCR0B = (0 << CS02) | (1 << CS01) | (0 << CS00); 
}

void loop() {
 
  lcd.setCursor(0, 1);
  if (ore < 10) 
  {
    lcd.print(0);
    lcd.setCursor(1, 1);
  }
  lcd.print(ore);
  
  lcd.setCursor(2, 1);
  lcd.print(":");
  
  lcd.setCursor(3, 1);
  if (minute < 10) 
  {
      lcd.print(0);
      lcd.setCursor(4, 1);
  }
  lcd.print(minute);
  
  lcd.setCursor(5, 1);
  lcd.print(":");
  
  lcd.setCursor(6, 1);
  if (secunde < 10) 
  {
      lcd.print(0);
      lcd.setCursor(7, 1);
  }
  lcd.print(secunde);
  
  lcd.setCursor(8,1);
  lcd.print(":");
  
  lcd.setCursor(9,1); 
  if (milisec < 1000) 
  {
      lcd.print(0);
      lcd.setCursor(10, 1);
          if (milisec < 100) 
          {
              lcd.print(0);
              lcd.setCursor(11, 1);
          }     
  } 
  lcd.print(milisec);
  
  lcd.setCursor(12, 1);
  lcd.print(" ");
}

inline void incrementOre() 
{
  ++ore;
  if (ore >= 24) 
  {
      ore %= 24;
  }
}

inline void incrementMinute()
{
  ++minute;
  if (minute >= 60) 
  {
      incrementOre();
      minute %= 60;
  }
}

ISR(TIMER0_COMPA_vect) 
{
  if(flag == 1)
  {
      SREG &= ~(1 << SREG_I); //dezactivare intreruperi globale
      ++milisec;
  
  if(milisec == 10000)
  {
      milisec = 0;
      ++secunde;
    
    if (secunde >= 60) 
    {
        incrementMinute();
        secunde %= 60;
    }
  }
  SREG |= (1 << SREG_I);     //activare intreruperi globale
}
}

ISR(INT0_vect) 
{            
    SREG &= ~(1 << SREG_I);
    
    flag = !flag;
    
    SREG |= (1 << SREG_I);
}

ISR(INT1_vect) 
{             
    SREG &= ~(1 << SREG_I);
  
    ore = 0;
    minute = 0;
    secunde = 0;
    milisec = 0;
    flag = 0;
  
    SREG |= (1 << SREG_I);
}
