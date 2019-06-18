#include "Adafruit_Thermal.h"
#include "petitieqrcode.h"
#include "SoftwareSerial.h"
#include "TM1637.h"

#define TX_PIN 22 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 23 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

int inPin = 31;         // the number of the input pin
int outPin = 31;       // the number of the output pin

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

const int buttonPin1 = 28;
const int buttonPin2 = 30;
const int buttonPin3 = 32;
const int buttonPin4 = 34;

const int buttonPin5 = 36;
const int buttonPin6 = 29;
const int buttonPin7 = 31;
const int buttonPin8 = 33;
const int buttonPin9 = 35;
const int buttonPin0 = 37;

int pins[] = {37, 28, 30, 32, 34, 36, 29, 31, 33, 35}; 

const int CLK = 53;
const int DIO = 52;
TM1637 tm1637(CLK,DIO);
TM1637 yearScreen(51,50);

float number = 2;
int mult = 1;

int pressureAnalogPin = 0; //pin where our pressure pad is located.
int pressureReading; //variable for storing our reading

int noPressure = 5; //max value for no pressure on the pad
int lightPressure = 50; //max value for light pressure on the pad
int mediumPressure = 100; //max value for medium pressure on the pad

String lawArticle = "";
String years = "";
bool changedLawArticle = false;
int level = 0;

int answers[] = {242, 311, 246};

void setup() {
 
  Serial.begin(9600);

  pinMode(buttonPin1, INPUT);  //initialize the pushbutton pin as an output
  pinMode(28, OUTPUT); digitalWrite(28, LOW);

  pinMode(buttonPin2, INPUT);  //initialize the pushbutton pin as an output
  pinMode(30, OUTPUT); digitalWrite(30, LOW);


  tm1637.init();
  tm1637.set(7);

  yearScreen.init();
  yearScreen.set(7);

  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
}

void loop() {
//  Serial.print("Pressure Pad Reading = ");
//  Serial.println(pressureReading);
//  delay(200);

  // Enter numbers
  for(int num = 0; num < 10; num++) {
    if (digitalRead(pins[num]) == HIGH) {

      if(lawArticle.length() == 3) {
        years += num;
      }else {
        lawArticle += num;
      }
      
       changedLawArticle = true;
    }
  }

  if(years.length() > 2) {
    lawArticle = "";
    years = "";
  }

  // display entered number
  if (changedLawArticle) {
    changedLawArticle = false;
    tm1637.displayNum(lawArticle.toInt(),false,lawArticle.length());
    yearScreen.displayNum(years.toInt(),false,years.length());
    delay(200);
  }

  // check
  pressureReading = analogRead(pressureAnalogPin);
  if (pressureReading > mediumPressure) {

    if (lawArticle.length() == 3 && years.length() > 0) {
      if (lawArticle.toInt() == answers[level]) {
        next();
      }else{
        failed();
      }
    }else {
      wrongInput();
    }
    
  }

}


 void next() {
  // Feedback
    mySerial.begin(19200);
    printer.begin();
    
   
    switch(level) {
      case 0: 
        printer.justify('C');
        printer.setSize('L');  
        printer.println(F("CORRECT!"));
        
        printer.feed(1);
        printer.setSize('S');  
        printer.setLineHeight(); // Reset to default
        printer.println(F(" Artikel 242 is inderdaad het \njuiste artikel om deze misdaad \nmee te bestraffen."));
        printer.feed(1);
        printer.println(F(" Laten we hopen dat deze man \nniet weer de fout in zal gaan. \nOp naar de tweede misdaad!"));   
        printer.feed(3);
//        printer.println(("years: " + years));
        break;

            

      case 1:
        printer.justify('C');
        printer.setSize('L');  
        printer.println(F("CORRECT!"));
        
        printer.feed(1);
        printer.setSize('S');  
        printer.setLineHeight(); // Reset to default
        printer.println(F(" Artikel 311 is inderdaad het \njuiste artikel om deze misdaad \nmee te bestraffen."));
        printer.feed(1);
        printer.println(F(" Dit is natuurlijk een erg \ncomplexe zaak, deze man wil \nalleen zijn vader helpen, maar \ndat moet natuurlijk niet \nten koste van een ander."));
        printer.setLineHeight(); // Reset to default
        printer.feed(3);
//        printer.println(("years: " + years));
        break;

             

      case 2:
        printer.justify('C');
        printer.setSize('L');  
        printer.println(F("CORRECT!"));
        
        printer.feed(1);
        printer.setSize('S');  
        printer.setLineHeight(); // Reset to default
        printer.println(F(" Artikel 246 is inderdaad het \njuiste artikel om deze misdaad \nmee te bestraffen."));
        printer.feed(1);
        printer.println(F(" Wat knap dat jij dat weet. \nVrouwen kunnen namelijk niet \nworden bestraft met verkrachting\nvolgens artikel 242, omdat \ndit wetsartikel het heeft \nover het seksueel binnendringen \nvan een ander. Aangezien \nvrouwen geen uitwending \ngeslachtsorgaan hebben is het \nvoor hen wettelijk niet mogelijk\nom iemand te verkrachten!"));
        printer.feed(1);
        printer.println(F(" Het is tijd om artikel 242 te \nverruimen zodat ook vrouwen \nveroordeeld kunnen worden \nvoor verkrachting!"));
        printer.feed(1);
        printer.println(F(" Steun de petitie door hem te \ntekenen, dit kan door de QR-code\nte scannen of naar de \nonderstaande link te gaan"));
        printer.feed(2);
        printer.printBitmap(petitieqrcode_width, petitieqrcode_height, petitieqrcode_data);
        printer.feed(1);
        printer.println(F("https://bit.ly/2HTQBHD"));
        printer.feed(3);
//        printer.println(("years: " + years));
        level = -1;
        break;        
    }
    
//     printer.feed(3);

    //Reset
    lawArticle = "";
    years = "";
    level++;

 }

 void failed() {
    //Feedback
    mySerial.begin(19200);
    printer.begin(); 
    
   
    switch(level) {
      case 0: 
        printer.justify('C');
        printer.setSize('L');  
        printer.println(F("INCORRECT!"));

        printer.feed(1);
        printer.setSize('S');  
        printer.setLineHeight(); // Reset to default
        printer.println(F(" Helaas, dat is niet het goede \nantwoord. Je hoort deze misdaad \nte bestraffen met artikel 242, \nje mag hierbij maximaal 12 jaar \naan de misdadiger geven."));
        printer.feed(1);
        printer.println(F("  Laten we hopen dat je \nde volgende vraag wel \ngoed weet te beantwoorden!"));
        printer.setLineHeight(); // Reset to default
        printer.feed(3);
//        printer.println(("years: " + years));
        break;          

      case 1:
        printer.justify('C');
        printer.setSize('L');  
        printer.println(F("INCORRECT!"));

        printer.feed(1);
        printer.setSize('S');  
        printer.setLineHeight(); // Reset to default
        printer.println(F("Helaas, dat is niet het goede \nantwoord. Je hoort deze misdaad \nte bestraffen met artikel 311, \nje mag hierbij maximaal 6 jaar \naan de misdadiger geven."));
        printer.feed(1);
        printer.println(F("  Laten we hopen dat je \nde laatste vraag wel \ngoed weet te beantwoorden!"));
        printer.setLineHeight(); // Reset to default
        printer.feed(3);
//        printer.println(("years: " + years));
      break;

      case 2:
        printer.justify('C');
        printer.setSize('L');  
        printer.println(F("INCORRECT!"));

        printer.feed(1);
        printer.setSize('S');  
        printer.setLineHeight(); // Reset to default
        printer.println(F("Helaas, dat is niet het goede \nantwoord. Je hoort deze misdaad \nte bestraffen met artikel 246, \nje mag hierbij maximaal 8 jaar \naan de misdadiger geven."));
        printer.feed(1);
        printer.println(F("Vrouwen kunnen niet worden be-\nstraft met verkrachting volgens\nartikel 242, omdat dit \nwetsartikel het heeft over\nhet seksueel binnendringen\nvan een ander. Aangezien vrouwen\ngeen uitwending geslachtsorgaan\nhebben is het voor hen wettelijk\nniet mogelijk om iemand\nte verkrachten!"));
        printer.feed(1);
        printer.println(F(" Het is tijd om artikel 242 te \nverruimen zodat ook vrouwen \nveroordeeld kunnen worden \nvoor verkrachting!"));
        printer.feed(1);
        printer.println(F(" Steun de petitie door hem te \ntekenen, dit kan door de QR-code\nte scannen of naar de \nonderstaande link te gaan"));
        printer.feed(2);
        printer.printBitmap(petitieqrcode_width, petitieqrcode_height, petitieqrcode_data);
        printer.feed(1);
        printer.println(F("https://bit.ly/2HTQBHD"));
        printer.feed(3);
//        printer.println(("years: " + years));
        level = -1;
      break;
    }


//    printer.feed(3);

    // Reset
    lawArticle = "";
    years = "";
    level++;

 }

  void wrongInput() {
    mySerial.begin(19200);
    printer.begin(); 
    printer.justify('C');
    printer.setSize('S');  
    printer.println(("Voer minimaal 4 getallen in.\n 3 getallen voor het artikel en\nminimaal 1 getal voor de straf"));
    printer.feed(3);

    lawArticle = "";
    years = "";

 }
 
  
