

#include <ModbusIP_ESP8266.h>

//Modbus Registers Offsets
const int SENSOR_IREG = 0;
const int reg_consumo = 1;

//ModbusIP object
ModbusIP mb;

unsigned long lastTime,sampleTime = 100;

//variables
int PinSensor = 36 ;
float caudal = 0;
float volumen = 0;
int sensorValue = 0; //modbus registro
  

void setup() {
    Serial.begin(9600); 
    pinMode(PinSensor, INPUT); 
    WiFi.begin("Casa", "123456");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    mb.server();    //Start Modbus IP
    // Add SENSOR_IREG register - Use addIreg() for analog Inputs
    mb.addIreg(SENSOR_IREG);
    mb.addIreg(reg_consumo);    

    lastTime = millis();
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();

 //Leemos adc
  caudal = analogRead(PinSensor)*0.0073;
  //volumen
    volumen = (caudal/120)+volumen; //paso a litros por segundo


     //Enviamos por el puerto serial
    Serial.print("Caudal: "); 
    Serial.print(caudal); 
    Serial.println(" l/min");
    Serial.print("Consumo: "); 
    Serial.print(volumen); 
    Serial.println(" l"); 
    delay(500);

   //Read each sampleTime
   if (millis()-lastTime > sampleTime) {
       lastTime = millis();
       //Raw value (0-4095)
       sensorValue = caudal;
       mb.Ireg(SENSOR_IREG,(caudal*1000));
       mb.Ireg(reg_consumo,(volumen*10));
   }
   
}
