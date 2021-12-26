#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

//definicja pinów
//definicja pinów dla przyciskó
#define kitchenButton A4
#define bathroomButton 3
#define bedroomButton 4
//definicja pinów dla serwonapedów
#define servoXpin 5
#define servoYpin 6
//definicja pinów dla wyjść ogrzewania
#define kitchenHeating 7
#define bathroomHeating 8
#define bedroomHeating 9
//definicja pinów dla fotorezystorów
#define bottomRightPin A0
#define topRightPin A1
#define topLeftPin A2
#define bottomLeftPin A3
//definicja pinu, na którym znajduje się magistrala
#define readTemp A5
//adres mac urządzenia

byte mac[] = {  
   0x90, 0xA2, 0xDA, 0x00, 0x6C, 0xFE 
};
//adres IP mojego arduino
IPAddress ip(192,168,137,177);

// adres IP serwera NODE.js

IPAddress server(192,168,137,1);
OneWire oneWire(readTemp);
DallasTemperature sensors(&oneWire);

unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long timeDiff = 0;
unsigned long previousTime2 = 0;
unsigned long timeDiff2 = 0;
//inicjuje klienta ethernet
EthernetClient client;

//definicja serw i ustawienie wartości maksymalnych
Servo servoX;
const int servoXtop = 150;
const int servoXbottom = 30;
int servoXcurr=0;
Servo servoY;
const int servoYtop = 130;
const int servoYbottom = 50;
int servoYcurr=0;
float setTemperature[]={20, 25, 20};// ustawione temperatury w pokojach

void setup() {
  //rozpocznij komunikacje Ethernet i serial
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  //delay potrzebny na czas inicjacji

  //informuj o statusie połączenia
  Serial.println("Connecting, please wait.");
  delay(10000);
  if (client.connect(server, 1337)) { // port na którym działa serwer i odbiera dane od Arduino
   Serial.println("Connection successful");
  } 
   else {
    Serial.println("Connection failed");
    } 
  
  //nadaje tryb pracy pinom oraz ustawiam ich początkowe wartości
  pinMode(kitchenHeating, OUTPUT);
  pinMode(bathroomHeating, OUTPUT); 
  pinMode(bedroomHeating, OUTPUT);
  pinMode(kitchenButton, INPUT);
  pinMode(bathroomButton, INPUT); 
  pinMode(bedroomButton, INPUT);
  digitalWrite(kitchenHeating, HIGH);
  digitalWrite(bathroomHeating, HIGH);
  digitalWrite(bedroomHeating, HIGH);
  digitalWrite(kitchenButton, LOW);
  digitalWrite(bathroomButton, LOW);
  digitalWrite(bedroomButton, LOW);
  //ustawienie pinów dla serw i serw w odpowiedniej pozycji wyjsciowej
  servoX.attach(servoXpin);
  servoY.attach(servoYpin);
  servoX.write(85);
  servoY.write(85);
}

        int buttonsRead[]={kitchenButton, bathroomButton, bedroomButton};
        boolean previousStateKitchen=0;
        boolean previousStateBathroom=0;
        boolean previousStateBedroom=0;
        boolean previousState[]={previousStateKitchen, previousStateBathroom, previousStateBedroom};
        
        boolean manualHeatingKitchen=0;
        boolean manualHeatingBathroom=0;
        boolean manualHeatingBedroom=0;
        boolean manualHeating[]={manualHeatingKitchen, manualHeatingBathroom, manualHeatingBedroom };
        
        boolean currentStateKitchen=0;
        boolean currentStateBathroom=0;
        boolean currentStateBedroom=0;
        boolean currentState[]={currentStateKitchen, currentStateBathroom, currentStateBedroom};

        boolean currentManual[]={0,0,0};
        boolean previousManual[]={0,0,0};
//Funkcja debouncing w celu redukcji drgań na przyciskach rzeczywistych 
boolean button(boolean previous, int buttonPin){
  boolean current=digitalRead(buttonPin);
  if(previous != current){
      delay(5);
      current=digitalRead(buttonPin);
    }
    return current;
  }

void loop() {
        currentTime=millis();
        timeDiff=currentTime-previousTime;
        timeDiff2=currentTime-previousTime2;
        float temperatureBathroom;
        float temperatureKitchen;
        float temperatureBedroom;
        int n=5;
        int m=3;
        int heatingDevices[3]={kitchenHeating, bathroomHeating, bedroomHeating};
        int buttons[3]={kitchenButton,bathroomButton, bedroomButton};
        float temperatureI;
        float temperature[n]; // tablica przechowujace obecna temperature
        float heating[n]; //tablica pomocnicza do sterowania wyjsciami dla elementów grzewczych
        char *CurrentTemp[]={ "Current temperature in Kitchen: ", "Current temperature in Bathroom: ", "Current temperature in Bedroom: "};
        
        int piec;//on or off
        int petla;
        byte addr[8];
        int tablica[8];

         //odczytanie kątów z enkodera serwonapedów
        servoXcurr = servoX.read();        
        servoYcurr = servoY.read();
  
        //odczytanie wartości z fotorezystorów, dokonuje mnożenia ponieważ posidam 2 inne fotorzystory o innej rezystancji
        int topLeft = analogRead(topLeftPin);
        int botLeft = analogRead(bottomLeftPin);
        int topRight = 2.3*analogRead(topRightPin);
        int botRight = 2.3*analogRead(bottomRightPin);

        int avgtop = (topLeft + topRight) / 2;   // średnia arytm. z pomiarów górnych fotorezystorów
        int avgbot = (botLeft + botRight) / 2;   // średnia arytm. z pomiarów dolnych fotorezystorów
        int avgleft = (topLeft + botLeft) / 2;  // średnia arytm. z pomiarów lewych fotorezystorów
        int avgright = (topRight + botRight) / 2; // średnia arytm. z pomiarów prawych fotorezystorów

        
       if(timeDiff>=30000UL){
        sensors.requestTemperatures();
        for(int i=0; i<m; i=i+1)
        {
          
          
          temperature[i] = sensors.getTempCByIndex(i);
          if (temperature[i]==-127)
          {
              m=i;
              i=0;
          }
          Serial.print(CurrentTemp[i]);
          Serial.print("  " );
          Serial.println(temperature[i]);      
          client.print(String(round(temperature[i])));          
      } 
        previousTime=currentTime;
       
        }

         
          
        //mechaniczne uruchomienie ogrzewania

        //sprawdzenie zgodności temperatury z ustawioną lub stanu przycisków i ustawienie wyjść cyfrowych
        for (int i=0; i<m; i=i+1)
        {       
          currentState[i]=button(previousState[i], buttonsRead[i]);
          if((currentState[i]==1 && previousState[i]==0) || (currentManual[i]==1))
          {
            manualHeating[i]= !manualHeating[i];           
          }
          previousState[i]=currentState[i];
          //previousManual[i]=currentManual[i];
          currentManual[i]=0;
          if(manualHeating[i]==1)
          {
            digitalWrite(heatingDevices[i], LOW);
            
          }else if(manualHeating[i]==0){
            
            digitalWrite(heatingDevices[i], HIGH);
            
            if(setTemperature[i]>temperature[i])
            {
              digitalWrite(heatingDevices[i], LOW);
              heating[i]=digitalRead(heatingDevices[i]);
        
            }else if (setTemperature[i]<=temperature[i]){
              digitalWrite(heatingDevices[i], HIGH);
              heating[i]=digitalRead(heatingDevices[i]);      
            }
          }
          
        }
        //obsłouga serw 
        if(abs(avgtop - avgbot) > 20) {

    if (avgtop > avgbot) {
      servoY.write(servoYcurr+1);
      if (servoYcurr > servoYtop) { 
        servoYcurr = servoYtop;
        servoY.write(servoYcurr);
      }
    }
    else if (avgtop < avgbot) {
      servoY.write(servoYcurr-1);
      if (servoYcurr < servoYbottom) {
        servoYcurr = servoYbottom;
        servoY.write(servoYcurr);
      }
    }
    else {
      servoYcurr=servoYcurr;
      servoY.write(servoYcurr);
    }
    delay(50);
  }

  if(abs(avgleft - avgright) > 20) {
    if (avgright > avgleft) {
      servoX.write(servoXcurr+1);
      if (servoXcurr > servoXtop) {
        servoXcurr = servoXtop;
        servoX.write(servoXcurr);
      }
    }
    else if (avgright < avgleft) {
      servoX.write(servoXcurr-1);
      if (servoXcurr < servoXbottom) {
        servoXcurr = servoXbottom;
        servoX.write(servoXcurr);
      }
    }
    else {
      servoXcurr=servoXcurr;
      servoX.write(servoXcurr);
    }
    delay(50);
  }

//obdiór poleceń z serwera/apki webowej
char c;
char paczkaClient[200];
String DoSo;
  //if you are connected and data is available
  if (client.available()>0) {
    
       String whatToDo;
       for (int i=0; i<5; i=i+1)
       {   
        if(i==0)
          {
            memset(paczkaClient, 0, sizeof(paczkaClient));
          }
        c = client.read();
        paczkaClient[i]=c;
       }
        whatToDo=String(paczkaClient);
        Serial.print(whatToDo);
        

       
         // turning heating on via webpage 
          if (whatToDo == "ON001") {
              currentManual[0]=1; 
              
          }else{
              currentManual[0]=0;
            }

            if (whatToDo == "ON003") {
              currentManual[1]=1; 
              
          }else{
              currentManual[1]=0;
            }
            
            if (whatToDo == "ON002") {
              currentManual[2]=1; 
             
          }else{
              currentManual[2]=0;
            }

        String whatToDo2 = String(whatToDo.charAt(0));
        Serial.print(whatToDo2);
        char setTemp[]={paczkaClient[1], paczkaClient[2], paczkaClient[3], paczkaClient[4]};
        if (whatToDo2 == "K") {
            setTemperature[0]=atof(setTemp);
        }

        if (whatToDo2 == "E") {

            setTemperature[1]=atof(setTemp);
        }

        if (whatToDo2 == "A") {
            setTemperature[2]=atof(setTemp);
        }
            
        whatToDo = " ";
  }
}
