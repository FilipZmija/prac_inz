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
#define bedroomHeating 7
#define kitchenHeating 8
#define bathroomHeating 9

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

//zmienne przechowujące czas
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
boolean trackerMode=0;

void setup() {
  //rozpocznij komunikacje Ethernet i serial
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  
  //delay potrzebny na czas inicjacji
  //informuj o statusie połączenia
  Serial.println("Connecting, please wait.");
  delay(10000);
  if (client.connect(server, 1337)){ // port na którym działa serwer i odbiera dane od Arduino
    Serial.println("Connection successful");
  } 
  else{
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

//definicja zmiennych obsługujących przyciski i elementy grzewcze
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
  //wpisanie czasu od restartu urządzenia do zmiennej
        currentTime=millis();
        timeDiff=currentTime-previousTime;
        timeDiff2=currentTime-previousTime2;
        //definicja zmiennych pomocniczych
        int n=5;
        int m=3;
        //definicja zmiennych przechowujących temperature
        float temperatureBathroom;
        float temperatureKitchen;
        float temperatureBedroom;
        float temperatureI;
        float temperature[n]; // tablica przechowujace obecna temperature
        
        
        //wpisanie numerów pinów do tablicy zmiennych
        int heatingDevices[3]={kitchenHeating, bathroomHeating, bedroomHeating};
        int buttons[3]={kitchenButton,bathroomButton, bedroomButton};
        float heating[n]; //tablica pomocnicza do sterowania wyjsciami dla elementów grzewczych
        char *CurrentTemp[]={"Current temperature in Bathroom: ", "Current temperature in Kitchen: ", "Current temperature in Bedroom: ", };
        //zmienne dodatkowe używane w procesie tworzenia porgramu
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

        int aTop = (topLeft + topRight) / 2;   // średnia arytm. z pomiarów górnych fotorezystorów
        int aBot = (botLeft + botRight) / 2;   // średnia arytm. z pomiarów dolnych fotorezystorów
        int aLeft = (topLeft + botLeft) / 2;  // średnia arytm. z pomiarów lewych fotorezystorów
        int aRight = (topRight + botRight) / 2; // średnia arytm. z pomiarów prawych fotorezystorów

        //funkcja wywoływana co ~30s, odczytuje wartości temperatury z czujników
       if(timeDiff>=30000UL){
        sensors.requestTemperatures();
        for(int i=0; i<m; i=i+1){
          
          
          temperature[i] = sensors.getTempCByIndex(i);
          if (temperature[i]==-127)
          {
              m=i;
              i=0;
          }
          Serial.print(CurrentTemp[i]);
          Serial.print("  " );
          Serial.println(temperature[i]);      
          //wysłanie temperatury do serwera
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
   if(trackerMode=0){
       if(abs(aTop - aBot) > 20) {
         if (aTop > aBot) {
           servoY.write(servoYcurr+1);
           if (servoYcurr > servoYtop) { 
             servoYcurr = servoYtop;
             servoY.write(servoYcurr);
           }
         }
         else if (aTop < aBot) {
          servoY.write(servoYcurr-1);
          if (servoYcurr < servoYbottom) {
            servoYcurr = servoYbottom;
            servoY.write(servoYcurr);
          }
         }
         else{
          servoYcurr=servoYcurr;
          servoY.write(servoYcurr);
         }
         delay(50);
        }

    if(abs(aLeft - aRight) > 20) {
      if (aRight > aLeft) {
        servoX.write(servoXcurr+1);
        if (servoXcurr > servoXtop) {
          servoXcurr = servoXtop;
          servoX.write(servoXcurr);
        }
      }
      else if (aRight < aLeft) {
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
  }
//obdiór poleceń z serwera i ich przetworzenie
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
        

       
         //włączenie ogrzewania na podstawie informacji z serwera
          if (whatToDo == "ON001") {
              currentManual[0]=1; 
              
          }else{
              currentManual[0]=0;
            }

            if (whatToDo == "ON002") {
              currentManual[1]=1; 
              
          }else{
              currentManual[1]=0;
            }
            
            if (whatToDo == "ON003") {
              currentManual[2]=1; 
             
          }else{
              currentManual[2]=0;
            }
        //zmiana oczekiwanej temperatury dla każdego z pomieszczeń w zależności od informacji z serwera
        String whatToDo2 = String(whatToDo.charAt(0));
        Serial.print(whatToDo2);
        char setTemp[]={paczkaClient[1], paczkaClient[2], paczkaClient[3], paczkaClient[4]};
        if (whatToDo2 == "K") {
            setTemperature[1]=atof(setTemp);
        }

        if (whatToDo2 == "A") {

            setTemperature[0]=atof(setTemp);
        }

        if (whatToDo2 == "E") {
            setTemperature[2]=atof(setTemp);
        }
        
        if (whatToDo == "MODE0") {
          trackerMode=0;
        }else if(whatToDo == "MODE1"){
          trackerMode=1;
        }
        char setAngle[]={paczkaClient[1], paczkaClient[2], paczkaClient[3]};
       if(trackerMode=1){          
        //poruszenie serwem
        
          if (whatToDo2 == "H") {
           servoX.write(atof(setAngle));
          }

          if (whatToDo2 == "V") {
           servoX.write(atof(setAngle));
          }
       }
       whatToDo = " ";
  }
}
