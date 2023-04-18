// ESP32 MENU BOX

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Include the libraries we need for temp sensor

#include <OneWire.h>
#include <DallasTemperature.h>

float temperatureC_chanA;
float temperatureC_chanB;

int tempCheckCounter;


// GPIO where the DS18B20 is connected to
const int oneWireBus = 13;
const int oneWireBus2 = 15;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
OneWire oneWire2(oneWireBus2);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&oneWire2);
//


// debounce variables //

//Top Level Variables:
int DEBUG = 0;  //Set to 1 to enable serial monitor debugging info

//Switch Variables:

// switch 1
int state_s1 = 0;
int state_prev_s1 = 0;
int pin_s1 = 23;
int val_s1 = 0;
unsigned long t_s1 = 0;
unsigned long t_0_s1 = 0;
unsigned long bounce_delay_s1 = 10;
unsigned long hold_delay_s1 = 1000;

// switch 2
int state_s2 = 0;
int state_prev_s2 = 0;
int pin_s2 = 25;
int val_s2 = 0;
unsigned long t_s2 = 0;
unsigned long t_0_s2 = 0;
unsigned long bounce_delay_s2 = 10;
unsigned long hold_delay_s2 = 1000;

// switch 3
int state_s3 = 0;
int state_prev_s3 = 0;
int pin_s3 = 26;
int val_s3 = 0;
unsigned long t_s3 = 0;
unsigned long t_0_s3 = 0;
unsigned long bounce_delay_s3 = 10;
unsigned long hold_delay_s3 = 1000;

// switch 4
int state_s4 = 0;
int state_prev_s4 = 0;
int pin_s4 = 27;
int val_s4 = 0;
unsigned long t_s4 = 0;
unsigned long t_0_s4 = 0;
unsigned long bounce_delay_s4 = 10;
unsigned long hold_delay_s4 = 1000;


// MENU variables
// there are 4 main menus 0 - 3 , 0 default

// 0 = current case temps A and B sensor
// 1 = set enclosure case temp , press enter to set ( up down adjust ) , back done
// 2 = set printer emergency shut down temp , press enter to set ( up down adjust ) , back done
// 3 = display all , current temps , enclosure temp target, emergency shutdown temp
 
int main_menu_state = 0;
int sub_menu_active = 0;
int temp_enclosure_target = 21; // default , use memeory later to load last used.
int temp_emergency_shutdown = 50; // default , use memeory later to load last used.

int temp_current = 0;

// OLED This line defines the controller, resolution and interface type to use from the library
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


//===============================================================================
//  Initialization
//===============================================================================

void setup(void) {

 Serial.begin(115200);
 
/// ################  

 // power for temp sensors A & B
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  
sensors.begin();
sensors2.begin();

/// ################

  u8g2.begin();


 // initialize digital pins for buttons

  pinMode(23,INPUT); //INPUT_PULLUP will use the Arduino's internal pullup resistor
  pinMode(25,INPUT);
  pinMode(26,INPUT);
  pinMode(27,INPUT);
  

  //if DEBUG is turned on, intiialize serial connection
  if(DEBUG) {Serial.begin(115200);Serial.println("Debugging is ON");}

  
}  // END setup


 


void loop() {


  ///  ########

  // power for temp sensors chan A and B
digitalWrite(12, HIGH);
digitalWrite(14, HIGH);
tempCheckCounter++;

if (tempCheckCounter >= 100 ) {
sensors.requestTemperatures(); 
sensors2.requestTemperatures(); 
temperatureC_chanA = sensors.getTempCByIndex(0);
temperatureC_chanB = sensors2.getTempCByIndex(0);

// Serial.print(temperatureC_chanA);
// Serial.println("ºC");
// Serial.print(temperatureC_chanB);
// Serial.println("CahnB ºC");

tempCheckCounter = 0;
}

  ///  ########


  // debounce menu state machines
  SM_s1();
  SM_s2();
  SM_s3();
  SM_s4();


// MAIN MENU DISPLAY PAGE

if (sub_menu_active == 0) {

if  (main_menu_state == 0 ) {
    u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_10x20_tr);  // Set the font
    
    u8g2.setCursor(70,20);
    u8g2.print(temperatureC_chanA);
    u8g2.drawStr(8,18,"tempA");   // Write
    
    u8g2.setCursor(70,40);
    u8g2.print(temperatureC_chanB);
    u8g2.drawStr(8,40,"tempB");   // Write
    
    u8g2.print(temp_current); 
    u8g2.drawRFrame(0,0,127,63,7);       // Draw a rectangle around it
  } 
  while (u8g2.nextPage() );
} // END MAIN MENU DISPLAY PAGE


// SET ENCLUSURE TEMP DISPLAY PAGE
if  (main_menu_state == 1 ) {
    u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_10x20_tr);  // Set the font
    u8g2.drawStr(16,16,"SET ENC TEMP");   // Write 
    u8g2.drawRFrame(0,0,127,63,7);       // Draw a rectangle around it
  } 
  while (u8g2.nextPage() );
} // END SET ENCLUSURE TEMP DISPLAY PAGE



// SET EMERGENCY SHUTDOWN TEMP DISPLAY PAGE
if  (main_menu_state == 2 ) {
    u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_10x20_tr);  // Set the font
    u8g2.drawStr(16,16,"SHUTDOWN");   // Write 
    u8g2.drawRFrame(0,0,127,63,7);       // Draw a rectangle around it
  } 
  while (u8g2.nextPage() );
} // END SET EMERGENCY SHUTDOWN TEMP DISPLAY PAGE

// DISPLAY ALL DISPLAY PAGE
if  (main_menu_state == 3 ) {

// temp_enclosure_target 
// temp_emergency_shutdown
  
u8g2.clearBuffer(); // clear the internal memory
u8g2.setFont(u8g2_font_10x20_tn); // set fonts

u8g2.setFont(u8g2_font_10x20_tr);
u8g2.drawStr(0, 20, "Enc Tmp=");

u8g2.setFont(u8g2_font_10x20_tn); // set fonts
u8g2.setCursor(80,20);
u8g2.print(temp_enclosure_target);   // Write
 
u8g2.setFont(u8g2_font_10x20_tr);
u8g2.drawStr(0, 40, "Shutdown=");

u8g2.setFont(u8g2_font_10x20_tn); // set fonts
u8g2.setCursor(90,40); 
u8g2.print(temp_emergency_shutdown);   // Write 
u8g2.drawRFrame(0,0,127,63,7);       // Draw a rectangle around it
u8g2.sendBuffer(); // transfer internal memory to the display


  
  
} // DISPLAY ALL DISPLAY PAGE


// loop the MAIN MENU pages
if (main_menu_state >= 4) { main_menu_state = 0; }
if (main_menu_state <= -1) { main_menu_state = 3; }



// Serial.print("main_menu_state=");
// Serial.println(main_menu_state);

// Serial.print("sub_menu_active=");
// Serial.println(sub_menu_active);

} // END MAIN MENU LOOP



if (sub_menu_active == 1) {

    u8g2.clearBuffer(); // clear the internal memory
    
    u8g2.setFont(u8g2_font_10x20_tr);  // Set the font
    u8g2.drawStr(0,20,"SET +/- C=");   // Write 
    u8g2.setFont(u8g2_font_10x20_tn); // set fonts
   u8g2.setCursor(90,20); 
   u8g2.print(temp_enclosure_target);   // Write 
    u8g2.drawRFrame(0,0,127,63,7);       // Draw a rectangle around it
    u8g2.sendBuffer(); // transfer internal memory to the display
  }

  
      }; // END loop


// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}




void SM_s1() {
  
  val_s1 = digitalRead(pin_s1);
  state_prev_s1 = state_s1;

  //Button 1 State Machine Section
  switch (state_s1) {
    case 0: //RESET!
      //Catch all "home base" for the State MAchine
      state_s1 = 1;
    break;

    case 1: //WAIT
      //Wait for the switch to go low
      if (val_s1 == LOW) {state_s1 = 2;}
    break;

    case 2: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s1 = millis();
      state_s1 = 3;
    break;

    case 3: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s1 = millis();
      if (t_s1 - t_0_s1 > bounce_delay_s1) {state_s1 = 4;}
      if (val_s1 == HIGH) {state_s1 = 0;}
    break;

    case 4: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s1 = millis();
      if (val_s1 == HIGH) {state_s1 = 5;}
      if (t_s1 - t_0_s1 > hold_delay_s1) {state_s1 = 6;}
    break;

    case 5: //TRIGGERED! /////////////////////////////////////////////////////////////////

   // previous main menu
   if (sub_menu_active == 0) { main_menu_state--; }
     
   // set enclosure temp set sub menu
   if (sub_menu_active == 1 ){ temp_enclosure_target--;}
   
   // set emergency shutdown temp sub menu  
   if (sub_menu_active == 2 ){ temp_emergency_shutdown--;}
      
      //reset the State Machine
      state_s1 = 0;
    break;

//////////////////////////////////////////////////////////////////////////////////////////


    case 6: //HOLD!
      //proceed to LOW WAIT
      state_s1 = 7;
    break;

    case 7: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {state_s1 = 0;}
    break;
  }
} // END switch (state_s1)




 // Button 2 State Machine Section
void SM_s2() {

val_s2 = digitalRead(pin_s2);
  state_prev_s2 = state_s2;

  switch (state_s2) {
    case 0: //RESET!
      //Catch all "home base" for the State MAchine
      state_s2 = 1;
    break;

    case 1: //WAIT
      //Wait for the switch to go low
      if (val_s2 == LOW) {state_s2 = 2;}
    break;

    case 2: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s2 = millis();
      state_s2 = 3;
    break;

    case 3: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s2 = millis();
      if (t_s2 - t_0_s2 > bounce_delay_s2) {state_s2 = 4;}
      if (val_s2 == HIGH) {state_s2 = 0;}
    break;

    case 4: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s2 = millis();
      if (val_s2 == HIGH) {state_s2 = 5;}
      if (t_s2 - t_0_s2 > hold_delay_s2) {state_s2 = 6;}
    break;

//////////////////////////////////////////////////////////////////////////////////////////

    case 5: //TRIGGERED!
    
    // next main menu
 if (sub_menu_active == 0) { main_menu_state++; }

     // set enclosure temp sub menu
   if (sub_menu_active == 1 ){ temp_enclosure_target++;}

   // set emergency shutdown temp sub menu  
   if (sub_menu_active == 2 ){ temp_emergency_shutdown++;}
    
      //reset the State Machine
      state_s2 = 0;
    break;

////////////////////////////////////////////////////////////////////////////////////////////

    case 6: //HOLD!
      //proceed to LOW WAIT
      state_s2 = 7;
    break;

    case 7: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s2 == HIGH) {state_s2 = 0;}
    break;
    
  }
} // END switch (state_s2)



void SM_s3() {

  val_s3 = digitalRead(pin_s3);
  state_prev_s3 = state_s3;

 // button 3 State Machine Section
 
  switch (state_s3) {
    
    case 0: //RESET!
      //Catch all "home base" for the State MAchine
      state_s3 = 1;
    break;

    case 1: //WAIT
      //Wait for the switch to go low
      if (val_s3 == LOW) {state_s3 = 2;}
    break;

    case 2: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s3 = millis();
      state_s3 = 3;
    break;

    case 3: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s3 = millis();
      if (t_s3 - t_0_s3 > bounce_delay_s3) {state_s3 = 4;}
      if (val_s3 == HIGH) {state_s3 = 0;}
    break;

    case 4: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s3 = millis();
      if (val_s3 == HIGH) {state_s3 = 5;}
      if (t_s3 - t_0_s3 > hold_delay_s3) {state_s3 = 6;}
    break;

    case 5: //TRIGGERED!

 // EXIT Set Enclosure Temp sub menu
 if (sub_menu_active == 1 ) { 
  main_menu_state =0; 
  sub_menu_active =0;
  }

// EXIT Set Enclosure Temp sub menu
 if (sub_menu_active == 2 ) { 
  main_menu_state =0; 
 sub_menu_active =0;
 }

 // EXIT Set Enclosure Temp sub menu
 if (sub_menu_active == 3 ) { 
  main_menu_state =0; 
  sub_menu_active =0;
  }
  
      //reset the State Machine
      state_s3 = 0;
    break;

    case 6: //HOLD!
      //proceed to LOW WAIT
      state_s3 = 7;
    break;

    case 7: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s3 == HIGH) {state_s3 = 0;}
    break;
  }
} // END switch (state_s3)


void SM_s4() {

  val_s4 = digitalRead(pin_s4);
  state_prev_s4 = state_s4;

 //Button 4  State Machine Section
  switch (state_s4) {

    case 0: //RESET!
      //Catch all "home base" for the State MAchine
      state_s4 = 1;
    break;

    case 1: //WAIT
      //Wait for the switch to go low
      if (val_s4 == LOW) {state_s4 = 2;}
    break;

    case 2: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s4 = millis();
      state_s4 = 3;
    break;

    case 3: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s4 = millis();
      if (t_s4 - t_0_s4 > bounce_delay_s4) {state_s4 = 4;}
      if (val_s4 == HIGH) {state_s4 = 0;}
    break;

    case 4: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s4 = millis();
      if (val_s4 == HIGH) {state_s4 = 5;}
      if (t_s4 - t_0_s4 > hold_delay_s4) {state_s4 = 6;}
    break;

    case 5: //TRIGGERED!

 // Enter Set Enclosure Temp sub menu
 if (main_menu_state == 1 ) { sub_menu_active=1; }

  // Enter Set Enclosure Temp sub menu
 if (main_menu_state == 2 ) { sub_menu_active=2; }

 
    
      //reset the State Machine
      state_s4 = 0;
    break;

    case 6: //HOLD!
      //proceed to LOW WAIT
      state_s4 = 7;
    break;

    case 7: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s4 == HIGH) {state_s4 = 0;}
    break;
  }
} // END switch (state_s4)
