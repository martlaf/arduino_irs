
//Ce fichier sert à enregistrer dans un fichier CSV sur une carte SD à partir d'un Arduino les valeurs d'accéléromètre et de gyroscope dans le temps, afin de recomposer la position dans le temps du module a posteriori.
//Par : Martin Lafrance
//Date : Mai 2014

#include "I2Cdev.h"
#include "MPU6050.h"  //Jeff Rowberg's
#include "Math.h"
#include "Wire.h"
#include "SD.h"
#include "SPI.h"

MPU6050 accelgyro;

int16_t ax, ay, az;

int sensorValue[3]  = { 0, 0, 0};
int sensorZero[3]  = { 0, 0, 0};

File dataFile;

void setup() {

    SD.begin(5); //memoire sur carte SD
    Wire.begin(); //connection avec le MPU
    
    Serial.begin(9600); //connection serie pour debuggage/monitoring

    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // create a new file
    char name[13];
    strcpy_P(name, PSTR("FAST00.CSV"));
    for (uint8_t n = 0; n < 100; n++) {
      name[4] = '0' + n / 10;
      name[5] = '0' + n % 10;
      if (!SD.exists(name)) {
        dataFile = SD.open(name, FILE_WRITE);
        if (dataFile) break;
      }
    }
    
    Serial.print("File name is : ");
    Serial.println(name);

    calibrateSensors();
}

void loop() {

    dataFile.print(millis(), DEC); dataFile.print(",");
    accelgyro.getAcceleration(&ax, &ay, &az);


    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.println("\t");
    
    dataFile.print(ax, DEC); dataFile.print(",");
    dataFile.print(ay); dataFile.print(",");
    dataFile.print(az); dataFile.print(",\n");
    dataFile.flush();
    
}
