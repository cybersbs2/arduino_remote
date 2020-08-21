#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Wire.h>
#include <IRremote.h>

//remote control button name
//button name setting

double ONE = 552634487;
double TWO = 552618167;
double THREE = 552650807;
double FOUR = 552610007;
double FIVE = 552642647;
double SEVEN = 552658967; //OUT1 : pre out1
double EIGHT = 552605927; //OUT2 : pre out2
double NINE = 552638567; //OUT3 : cpu board relay

double ON = 16736925;
double UP = 552616127;
double DOWN = 552648767;
double UP_2 = 552600317;
double DOWN_2 = 552632957;
double PRESS = 4294967295; //if you press any button for a long time, it will receive the data as it.

//pin setting
int VOL_UP = 0;
int VOL_DOWN = 1;
int OUT3 = 3; //cpu board relay
int OUT2 = 5; //pre out2
int RECV_PIN = 6; 
int OUT1 = 7; //pre out1
int IN5 = 8;
int IN4 = 9;
int IN3 = 10;
int IN2 = 11;
int IN1 = 12;
int VOL = 14;

//variable setting
double before_state;
int addr_mode = 0;
int addr_seven = 10;
int addr_eight = 20;
int addr_nine = 30;
int val_mode;
int val_volume;
int flag = 0;
int fflag = 0;
int mode_f = 0;
int k[6] = {0,};
unsigned long time_10s;

bool seven_state = true;
bool eight_state = true;
bool nine_state = true;

byte SpecialChar1[8] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B00000,
  B00000,
  B00000
};

byte SpecialChar2[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte SpecialChar3[8] = {
  B01000,
  B01100,
  B01110,
  B01111,
  B01111,
  B01110,
  B01100,
  B01000
};

byte SpecialChar4[8] = {
  B00010,
  B00110,
  B01110,
  B11110,
  B11110,
  B01110,
  B00110,
  B00010
};

byte SpecialChar5[8] = {
  B00100,
  B01110,
  B01110,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
IRrecv irrecv(RECV_PIN);
decode_results results;
LiquidCrystal_I2C lcd(0x27,20,4);

void setup()
{
    //Serial.begin(9600);
    lcd.begin();
    lcd.clear();
    
    lcd.createChar(0, SpecialChar1);
    lcd.createChar(1, SpecialChar2);
    lcd.createChar(2, SpecialChar3);
    lcd.createChar(3, SpecialChar4);
    lcd.createChar(4, SpecialChar5);

    pinMode(VOL_UP, OUTPUT);
    pinMode(VOL_DOWN, OUTPUT);
    pinMode(OUT3, OUTPUT);
    pinMode(OUT2, OUTPUT);
    pinMode(OUT1, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN5, OUTPUT);
    
    irrecv.enableIRIn(); // Start the receiver

    digitalWrite(OUT1, HIGH);
    digitalWrite(OUT2, HIGH);
    digitalWrite(OUT3, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN5, HIGH);
    digitalWrite(VOL_UP, HIGH);
    digitalWrite(VOL_DOWN, HIGH);
    
    val_mode = EEPROM.read(addr_mode);
    for(int i=1; i<=5; i++) {
      if(i == val_mode) {
        digitalWrite(IN1+1-i, LOW);
      } else {
        digitalWrite(IN1+1-i, HIGH);
      }
    }
    
    seven_state = EEPROM.read(addr_seven);
    digitalWrite(OUT1, seven_state);
    if (seven_state == LOW) {
      lcd.setCursor(3,3);
      lcd.print("ON");
    } else {
      lcd.setCursor(3,3);
      lcd.print("OF");
    }
    
    eight_state = EEPROM.read(addr_eight);
    digitalWrite(OUT2, eight_state);
    if (eight_state == LOW) {
      lcd.setCursor(9,3);
      lcd.print("ON");
    } else {
      lcd.setCursor(9,3);
      lcd.print("OF");
    }
    
    nine_state = EEPROM.read(addr_nine);
    digitalWrite(OUT3, nine_state);
    if (nine_state == LOW) {
      lcd.setCursor(15,3);
      lcd.print("ON");
    } else {
      lcd.setCursor(15,3);
      lcd.print("OF");
    }
    
    val_volume = analogRead(VOL)/50;
    val_mode_change();
    
    lcd.setCursor(0,2);
    lcd.print("  Pre1  Pre2  MAIN  ");
    
    lcd.backlight();
    lcd.display();
}

void loop() {
  //if there is new input  
  if (irrecv.decode(&results)) {
    //Serial.println(results.value);
    if(results.value != PRESS) {
      before_state = (double)results.value;
    }
    
    if (before_state == ONE && results.value != PRESS) {
      val_mode = 1;
      val_mode_change();
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      digitalWrite(IN5, HIGH);
      mode_f = 1;
      time_10s = millis();
    } else if (before_state == TWO && results.value != PRESS) {
      val_mode = 2;
      val_mode_change();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      digitalWrite(IN5, HIGH);
      mode_f = 1;
      time_10s = millis();
    } else if (before_state == THREE && results.value != PRESS) {
      val_mode = 3;
      val_mode_change();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      digitalWrite(IN5, HIGH);
      mode_f = 1;
      time_10s = millis();
    } else if (before_state == FOUR && results.value != PRESS) {
      val_mode = 4;
      val_mode_change();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      digitalWrite(IN5, HIGH);
      mode_f = 1;
      time_10s = millis();
    } else if (before_state == FIVE && results.value != PRESS) {
      val_mode = 5;
      val_mode_change();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      digitalWrite(IN5, LOW);
      mode_f = 1;
      time_10s = millis();
    } 

    if (before_state == SEVEN && results.value != PRESS) {
      seven_state = !seven_state;
      digitalWrite(OUT1, seven_state);
      EEPROM.write(addr_seven, seven_state);
      if (seven_state == LOW) {
        lcd.setCursor(1,3);
        lcd.print("  ON");
      } else {
        lcd.setCursor(1,3);
        lcd.print("  OF");
      }
    } else if (before_state == EIGHT && results.value != PRESS) {
      eight_state = !eight_state;
      digitalWrite(OUT2, eight_state);
      EEPROM.write(addr_eight, eight_state);
      if (eight_state == LOW) {
        lcd.setCursor(9,3);
        lcd.print("ON");
      } else {
        lcd.setCursor(9,3);
        lcd.print("OF");
      }
    } else if (before_state == NINE && results.value != PRESS) {
      nine_state = !nine_state;
      digitalWrite(OUT3, nine_state);
      EEPROM.write(addr_nine, nine_state);
      if (nine_state == LOW) {
        lcd.setCursor(15,3);
        lcd.print("ON");
      } else {
        lcd.setCursor(15,3);
        lcd.print("OF");
      }
    }
    
    if (before_state == UP) {
      if(analogRead(VOL)<1005) flag = 1;
      else flag = 0;
    }
    
    if (before_state == DOWN) {
      if(analogRead(VOL)>45) fflag = 1;
      else fflag = 0;
    }

    if (before_state == UP_2) {
      if(analogRead(VOL)<1005) flag = 1;
      else flag = 0;
    }
    
    if (before_state == DOWN_2) {
      if(analogRead(VOL)>45) fflag = 1;
      else fflag = 0;
    }
    
    irrecv.resume(); // Receive the next value
  }

  //if there is no new input
  if(flag == 1) {
    digitalWrite(VOL_UP,LOW);
    delay(100);
  } else {
    digitalWrite(VOL_UP,HIGH);
    val_volume = analogRead(VOL)/50;
    val_volume_change();
  }
  flag = 0;

  if(fflag == 1) {
    digitalWrite(VOL_DOWN,LOW);
    delay(100);
  } else {
    digitalWrite(VOL_DOWN,HIGH);
    val_volume = analogRead(VOL)/50;
    val_volume_change();
  }
  fflag = 0;

  if(mode_f) {
    if(diff(millis(), time_10s, 1000)) {
      mode_f = 0;
      EEPROM.write(addr_mode, val_mode);
    }
  }
}

void val_volume_change() {
  lcd.setCursor(0,0);
  if(val_volume < 10) {
    lcd.print(" ");
  }
  lcd.print(val_volume);
  for(int i=6;i<16;i++) {
    lcd.setCursor(i,0);
    if(i-6<val_volume/2) {
      lcd.write(byte(1));
    } else {
      lcd.write(byte(0));
    }
  }
}

void val_mode_change() {
  for(int i=1; i<7; i++) {
    if(i==val_mode) {
      k_flag(val_mode);
      if(k[val_mode-1]==0) {
        Lcd(val_mode);
      } else if(k[val_mode-1]==1) {
        lcd.setCursor(0,1);
        lcd.print("     IN = AUX ");
        lcd.print(val_mode);
      }         
    }
  }
}

void Lcd(int b) {
  for(int l=0; l<6; l++) {
    for(int m=0; m<l; m++) {
      lcd.setCursor(m,1);
      lcd.print(" ");
    }
    lcd.setCursor(l,1);
    lcd.print("IN = AUX ");
    lcd.print(b);
    lcd.print("     ");
    k[b-1]=1;
    delay(50);
  }
}

void k_flag(int c) {
  for(int i=1; i<7; i++) {
    if(i!=c) {
      k[i-1]=0;
    }
  }
}

boolean diff(unsigned long current, unsigned long previous, unsigned long d) {
  if(current >= previous) return ((current - previous) >= d);
  else return (((0xFFFFFFFF-previous)+current+1) >= d);
}
