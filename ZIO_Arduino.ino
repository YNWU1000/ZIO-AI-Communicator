#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

/////////////////////////////////////////////////////////////Thermal Printer
#define TX_PIN 2 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 3 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor
int lastcount = LOW;
int currentcount;
int counter;
////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////LCD
// Define the addresses of your six LCDs
#define LCD1_ADDR 0x27
#define LCD2_ADDR 0x26
#define LCD3_ADDR 0x25
#define LCD4_ADDR 0x24
#define LCD5_ADDR 0x23
#define LCD6_ADDR 0x22
#define LCD7_ADDR 0x21
#define LCD8_ADDR 0x20
// Define the number of characters that can fit on one line
#define LCD_COLS 16
// Create an instance of the LiquidCrystal_I2C library for each LCD
LiquidCrystal_I2C lcd1(LCD1_ADDR, LCD_COLS, 2);
LiquidCrystal_I2C lcd2(LCD2_ADDR, LCD_COLS, 2);
LiquidCrystal_I2C lcd3(LCD3_ADDR, LCD_COLS, 2);
LiquidCrystal_I2C lcd4(LCD4_ADDR, LCD_COLS, 2);
LiquidCrystal_I2C lcd5(LCD5_ADDR, LCD_COLS, 2);
LiquidCrystal_I2C lcd6(LCD6_ADDR, LCD_COLS, 2);
LiquidCrystal_I2C lcd7(LCD7_ADDR, LCD_COLS, 2);
LiquidCrystal_I2C lcd8(LCD8_ADDR, LCD_COLS, 2);

byte box[8] = {
0b11111,
0b10001,
0b10001,
0b10001,
0b10001,
0b10001,
0b10001,
0b11111,
};

bool animation = true;
////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(19200);

  mySerial.begin(19200);  
  printer.begin();  

  lcd1.begin();
  lcd2.begin();
  lcd3.begin();
  lcd4.begin();
  lcd5.begin();
  lcd6.begin();
  lcd7.begin();
  lcd8.begin();

lcd1.createChar(0, box);
  lcd2.createChar(0, box);
    lcd3.createChar(0, box);
       lcd4.createChar(0, box);
        lcd5.createChar(0, box);
           lcd6.createChar(0, box);
             lcd7.createChar(0, box);
               lcd8.createChar(0, box);

}

void loop() {
  

  ////////////////WHEN THE CONVERSATION START/GOT RESPONSE
  if (Serial.available()) {
  

    lcd1.clear(); lcd4.clear(); lcd7.clear(); lcd8.clear();

    ////////////////PRINTER
    printer.setFont('A'); //A, B Font type
    printer.justify('R'); //L, C, R Text position
    printer.setSize('L'); //L, M, S Font size
    printer.println(F("////////////ZIO"));
    
    printer.setSize('S'); //L, M, S
    printer.doubleHeightOn();
    printer.println("AI Communicator");
    printer.doubleHeightOff();

    printer.feed(2);

    printer.setFont('A'); //A, B
    printer.justify('L'); //L, C, R
    printer.setSize('L'); //L, M, S
    // ONE LINE:"////////////////"
    printer.println(F("HUMAN:"));

    printer.setFont('A'); //A, B
    printer.justify('L'); //L, C, R
    printer.setSize('M'); //L, M, S

    String question = Serial.readStringUntil('|');
    printer.println(String(question));

    printer.feed(2);

    //////////////////

      String realText = Serial.readStringUntil('\n');

    // lcd1.setCursor(0,0); lcd1.print("//hmmmmmmmmmm...");
    // for (int x = 0; x < 16; x++) {
    //   lcd2.setCursor(x, 0); lcd2.write(byte(0)); lcd2.setCursor(x, 1); lcd2.write(byte(0)); 
    //   lcd3.setCursor(x, 0); lcd3.write(byte(0)); lcd3.setCursor(x, 1); lcd3.write(byte(0)); 
    //   lcd4.setCursor(x, 0); lcd4.write(byte(0)); lcd4.setCursor(x, 1); lcd4.write(byte(0)); 
    //   lcd5.setCursor(x, 0); lcd5.write(byte(0)); lcd5.setCursor(x, 1); lcd5.write(byte(0));
    //   lcd6.setCursor(x, 0); lcd6.write(byte(0)); lcd6.setCursor(x, 1); lcd6.write(byte(0));
    //   lcd7.setCursor(x, 0); lcd7.write(byte(0)); lcd7.setCursor(x, 1); lcd7.write(byte(0));
    //   lcd8.setCursor(x, 0); lcd8.write(byte(0)); lcd8.setCursor(x, 1); lcd8.write(byte(0)); 
    //   delay(100);
    // }

    //   lcd1.clear();lcd2.clear();lcd3.clear();lcd4.clear();
    //   lcd5.clear();lcd6.clear();lcd7.clear();lcd8.clear();






  //String realText = incomingText;

  ///////////////////////////////////////////LCDs

//GROUP1-3
//////First line of LCD1, LCD2, LCD3
  String lcd1A1Text = realText.substring(0, LCD_COLS);
  String lcd2A1Text = realText.substring(LCD_COLS, LCD_COLS*2);
  String lcd3A1Text = realText.substring(LCD_COLS*2, LCD_COLS*3);
 //Second line of LCD1, LCD2, LCD3
  String lcd1A2Text = realText.substring(LCD_COLS*3, LCD_COLS*4);
  String lcd2A2Text = realText.substring(LCD_COLS*4, LCD_COLS*5);
  String lcd3A2Text = realText.substring(LCD_COLS*5, LCD_COLS*6);

//GROUP4-6
//////First line of LCD4, LCD5, LCD6
  String lcd4A1Text = realText.substring(LCD_COLS*6, LCD_COLS*7);
  String lcd5A1Text = realText.substring(LCD_COLS*7, LCD_COLS*8);
  String lcd6A1Text = realText.substring(LCD_COLS*8, LCD_COLS*9);
  //Second line of LCD4, LCD5, LCD6
  String lcd4A2Text = realText.substring(LCD_COLS*9, LCD_COLS*10);
  String lcd5A2Text = realText.substring(LCD_COLS*10, LCD_COLS*11);
  String lcd6A2Text = realText.substring(LCD_COLS*11, LCD_COLS*12);

//GROUP7-8
//////First line of LCD7, LCD8
  String lcd7A1Text = realText.substring(LCD_COLS*12, LCD_COLS*13);
  String lcd8A1Text = realText.substring(LCD_COLS*13, LCD_COLS*14);
  //Second line of LCD7, LCD8
  String lcd7A2Text = realText.substring(LCD_COLS*14, LCD_COLS*15);
  String lcd8A2Text = realText.substring(LCD_COLS*15, LCD_COLS*16);

//PRINT ARRAY

  //Print the first line of LCD1-3
  lcd1.setCursor(0,0); lcd1.print(lcd1A1Text);
  lcd2.setCursor(0,0); lcd2.print(lcd2A1Text);
  lcd3.setCursor(0,0); lcd3.print(lcd3A1Text);
  //Print the second line of LCD1-3
  if (realText.length() > LCD_COLS*3) {
    lcd1.setCursor(0,1); lcd1.print(lcd1A2Text);
    lcd2.setCursor(0,1); lcd2.print(lcd2A2Text);
    lcd3.setCursor(0,1); lcd3.print(lcd3A2Text);

  //Print the first line of LCD4-6

    lcd4.setCursor(0,0); lcd4.print(lcd4A1Text);
    lcd5.setCursor(0,0); lcd5.print(lcd5A1Text);
    lcd6.setCursor(0,0); lcd6.print(lcd6A1Text);
  
  //Print the second line of LCD4-6

    lcd4.setCursor(0,1); lcd4.print(lcd4A2Text);
    lcd5.setCursor(0,1); lcd5.print(lcd5A2Text);
    lcd6.setCursor(0,1); lcd6.print(lcd6A2Text);
  
  
  //Print the first line of LCD7, LCD8
    lcd7.setCursor(0,0); lcd7.print(lcd7A1Text);
    lcd8.setCursor(0,0); lcd8.print(lcd8A1Text);
 
  //Print the second line of LCD7, LCD8

    lcd7.setCursor(0,1); lcd7.print(lcd7A2Text);
    lcd8.setCursor(0,1); lcd8.print(lcd8A2Text);
  }
  


    printer.setFont('A'); //A, B
    printer.justify('R'); //L, C, R
    printer.setSize('L'); //L, M, S
    // ONE LINE:"////////////////"
    printer.println(F("AI:"));

    printer.setFont('A'); //A, B
    printer.justify('R'); //L, C, R
    printer.setSize('M'); //L, M, S
    printer.println("  " + String(realText));

    // delay(3000);

    printer.feed(5);
    printer.sleep();      // Tell printer to sleep   
    delay(500); 
    printer.wake();       // MUST wake() before printing again, even if reset
    printer.setDefault();
    

    }
    else {
      ///////////////WAITING FOR INPUTS
  String DesktopText0 = "Ask me ANYTHING!";
  String DesktopText1 = "ZIO standby. ///";
  lcd1.setCursor(0,0); lcd1.print(DesktopText1);
  lcd1.setCursor(0,1); lcd1.print(DesktopText0);

  String DesktopText4 = "Current-Model///";
  String DesktopText5 = "///////////GPT-4";
  lcd4.setCursor(0,0); lcd4.print(DesktopText4);
  lcd4.setCursor(0,1); lcd4.print(DesktopText5);

  String DesktopText3 = ">>Press>>The>>>>MIDDLE KNOB>>>>&Start Speaking!<<<<<<<<<<<<<<<<<";
  String DeskT3A1Text = DesktopText3.substring(0, LCD_COLS);
  String DeskT3A2Text = DesktopText3.substring(LCD_COLS, LCD_COLS*2);
  String DeskT3A3Text = DesktopText3.substring(LCD_COLS*2, LCD_COLS*3);
  String DeskT3A4Text = DesktopText3.substring(LCD_COLS*3, LCD_COLS*4);
  lcd7.setCursor(0,0); lcd7.print(DeskT3A1Text);
  lcd8.setCursor(0,0); lcd8.print(DeskT3A2Text);
  lcd7.setCursor(0,1); lcd7.print(DeskT3A3Text);
  lcd8.setCursor(0,1); lcd8.print(DeskT3A4Text);

  /////ANIMATIONS
  // if (animation = true) {

  float a1 = random(0, 4);  // amplitude of wave 1
  float f1 = random(1, 5);  // frequency of wave 1
  float p1 = random(0, 180);  // phase of wave 1
  float a2 = random(1, 2);  // amplitude of wave 2
  float f2 = random(1, 6);  // frequency of wave 2
  float p2 = random(0, 360);  // phase of wave 2

for (int x = 0; x < 16; x++) {
 // lcd2.clear();lcd3.clear();lcd5.clear();lcd6.clear();
    float y1 = sin((x+p1)*0.2*f1)*a1+0.5;
    float y2 = sin((x+8+p2)*0.2*f2)*a2+0.5;

  float y3 = sin(x+p2*0.1*f2)*2*a1+0.5;  // Change multiplier value for smaller amplitude
  float y4 = sin((x+8)*0.1*f1)*a2+0.5;

  float y5 = sin(x*0.5+p1)*a2*f2+0.5;  // Change multiplier value for slower frequency
  float y6 = sin((x+8)*0.5+p2)*a1+0.5;

  float y7 = sin(x*0.2+p1)*a1+0.5;  // Add an offset to the sine wave
  float y8 = sin((x+8)*0.2+p2)*a2+0.5;
  lcd2.setCursor(x, int(y1)); lcd2.write(byte(0)); 
  lcd2.setCursor(x, int(y2)); lcd2.write(byte(0));
  lcd3.setCursor(x, int(y3)); lcd3.write(byte(0)); 
  lcd3.setCursor(x, int(y4)); lcd3.write(byte(0));
  lcd5.setCursor(x, int(y5)); lcd5.write(byte(0)); 
  lcd5.setCursor(x, int(y6)); lcd5.write(byte(0));
  lcd6.setCursor(x, int(y7)); lcd6.write(byte(0)); 
  lcd6.setCursor(x, int(y8)); lcd6.write(byte(0));
  delay(random(100,300));}
  delay(random(2000,4000)); // Wait for 5 seconds before clearing all screens and starting the animation again
  lcd2.clear(); lcd3.clear(); lcd5.clear(); lcd6.clear(); // Clear all screens
// }
/////


  // for (int x = 0; x < 16; x += random(16) ) {
  //   for (int y = 0; y < 2; y += random(2) ) {
  // lcd2.setCursor(x, y); lcd2.print("\x00");
  // lcd3.setCursor(x, y); lcd3.print("/");
  // lcd5.setCursor(x, y); lcd5.print("/");
  // lcd6.setCursor(x, y); lcd6.print("/");
  //   }
  //   delay(random(50,200));
  //   lcd2.clear(); lcd3.clear(); lcd5.clear(); lcd6.clear();
  // }
  //
  //String DesktopText2 = ":HELLO human! I am an AI communicator who allows voice conversation with LLM AI like GPT-4 in near real-time. ";
  // String lcdDes1Text = DesktopText2.substring(0, LCD_COLS);
  // String lcdDes2Text = DesktopText2.substring(LCD_COLS, LCD_COLS*2);
  // String lcdDes3Text = DesktopText2.substring(LCD_COLS*2, LCD_COLS*3);
  // String lcdDes4Text = DesktopText2.substring(LCD_COLS*3, LCD_COLS*4);
  // String lcdDes5Text = DesktopText2.substring(LCD_COLS*4, LCD_COLS*5);
  // String lcdDes6Text = DesktopText2.substring(LCD_COLS*5, LCD_COLS*6);
  // String lcdDes7Text = DesktopText2.substring(LCD_COLS*6, LCD_COLS*7);
  // String lcdDes8Text = DesktopText2.substring(LCD_COLS*7, LCD_COLS*8);
  // lcd2.setCursor(0,0); lcd2.print(lcdDes1Text);
  // lcd3.setCursor(0,0); lcd3.print(lcdDes2Text);
  // lcd2.setCursor(0,1); lcd2.print(lcdDes3Text);
  // lcd3.setCursor(0,1); lcd3.print(lcdDes4Text);
  // lcd5.setCursor(0,0); lcd5.print(lcdDes5Text);
  // lcd6.setCursor(0,0); lcd6.print(lcdDes6Text);
  ///

    }




  // Wait for a short period of time before updating the LCDs again
  delay(100);
}