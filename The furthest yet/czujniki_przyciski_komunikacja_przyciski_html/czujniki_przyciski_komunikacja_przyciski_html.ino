#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
//definicja pinów

#define kitchenButton 2
#define bathroomButton 3
#define bedroomButton 4

#define kitchenHeating 7
#define bathroomHeating 8
#define bedroomHeating 9
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
        float setTemperature[]={20, 25, 20};// ustawione temperatury w pokojach
        int piec;//on or off
        int petla;
        byte addr[8];
        int tablica[8];
        
        
       if(timeDiff>=10000UL){
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
      }
        previousTime=currentTime;
        }



 
          
        //mechaniczne uruchomienie ogrzewania

        //sprawdzenie zgodności temperatury z ustawioną lub stanu przycisków i ustawienie wyjść cyfrowych
        for (int i=0; i<m; i=i+1)
        {       
          currentState[i]=button(previousState[i], buttonsRead[i]);
          if((currentState[i]==1 && previousState[i]==0) || (currentManual[i]==1 && previousManual[i]==0))
          {
            manualHeating[i]= !manualHeating[i];

            
          }
          previousState[i]=currentState[i];
          previousManual[i]=currentManual[i];
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
        

// komunikacja z serwerem aka odbieranie, wysyłanie nie może byc ciągłe bo są fest lagi
char c;
int pinVal[3];
char paczkaClient[200];
String DoSo;
  //if you are connected and data is available
  if (client.available()>0) {
       String whatToDo;
       for (int i=0; i<=client.available()+2; i=i+1)
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
          if (whatToDo == "ON1") {
              currentManual[0]=1; 
              client.println("Heating mode changed for station 1"); 
          }else{
              currentManual[0]=0;
            }

            if (whatToDo == "ON2") {
              currentManual[1]=1; 
              client.println("Heating mode changed for station 2"); 
          }else{
              currentManual[1]=0;
            }
            
            if (whatToDo == "ON3") {
              currentManual[2]=1; 
              client.println("Heating mode changed for station 3");  
          }else{
              currentManual[2]=0;
            }
       
        whatToDo = " ";
  }
}
  
