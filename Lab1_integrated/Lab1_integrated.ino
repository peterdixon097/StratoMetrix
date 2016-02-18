#include <SPI.h>
#include <SD.h> //GitTest

float v3p3;

const int chipSelect = 10;

const int v3p3_Pin = A0;
const int xAcc_Pin = A1;
const int yAcc_Pin = A2;
const int zAcc_Pin = A3;
const int temp1_Pin = A4;
const int temp2_Pin = A5;
const int pressure_Pin = A6;
const int humidity_Pin = A7;


void setup()
{
  // initialize the serial communications:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop()
{
  //read pins
  v3p3 = mapADC_mV(analogRead(v3p3_Pin));
  float temp1 = calibrateTemp(analogRead(temp1_Pin));
  float temp2 = calibrateTemp(analogRead(temp2_Pin));
  float xAcc = calibrateAcceleration(analogRead(xAcc_Pin));
  float yAcc = calibrateAcceleration(analogRead(yAcc_Pin));
  float zAcc = calibrateAcceleration(analogRead(zAcc_Pin));
  float humidity = calibrateHumidity(analogRead(humidity_Pin));
  float pressure = calibratePressure(analogRead(pressure_Pin));

  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
      //output everything on one line
      dataFile.print("3.3V input = ");
      dataFile.print(v3p3);
      dataFile.print(" mV, Temp1 = ");
      dataFile.print(temp1);
      dataFile.print(" degC, Temp2 = ");
      dataFile.print(temp2);
      dataFile.print(" degC, X_Acc = ");
      dataFile.print(xAcc);
      dataFile.print(" g, Y_Acc = ");
      dataFile.print(yAcc);
      dataFile.print(" g, Z_Acc = ");
      dataFile.print(zAcc);
      dataFile.print(" g, Humidity = ");
      dataFile.print(humidity);
      dataFile.print(" %, Pressure = ");
      dataFile.print(pressure);
      dataFile.println(" kPa");
  }
    
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  
    

  //output everything on one line
  Serial.print("3.3V input = ");
  Serial.print(v3p3);
  Serial.print(" mV, Temp1 = ");
  Serial.print(temp1);
  Serial.print(" degC, Temp2 = ");
  Serial.print(temp2);
  Serial.print(" degC, X_Acc = ");
  Serial.print(xAcc);
  Serial.print(" g, Y_Acc = ");
  Serial.print(yAcc);
  Serial.print(" g, Z_Acc = ");
  Serial.print(zAcc);
  Serial.print(" g, Humidity = ");
  Serial.print(humidity);
  Serial.print(" %, Pressure = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  delay(100);
}

/*
 * Maps ADC output into millivolts
 */
float mapADC_mV(float inVal)
{
  return inVal*(5000.0/1024.0);
}

/*
 * Maps ADC output into volts
 */
float mapADC_V(float inVal)
{
  return inVal*(5.0/1024.0);    //maybe make "5.0" a variable input
}

/*
 * Calibrates temperature (see TMP3x datasheet). Outputs degrees Celsius.
 */
float calibrateTemp(float inVal)
{
  return mapADC_mV(inVal)/10.0 - 50.0;
}

/*
 * Calibrates pressure (see MPX5100 datasheet). Outputs kPa.
 */
float calibratePressure(float inVal)
{
  return (mapADC_V(inVal)/5.0 + 0.095)/0.009;
}

/*
 * Calibrates acceleration (see ADXL335 datasheet). Outputs in g's.
 */
float calibrateAcceleration(float inVal)
{
//  float zero_G_offset_mV = 1800;
  float zero_G_offset_mV = v3p3/2.0;
  return (mapADC_mV(inVal)-zero_G_offset_mV)/300.0;
}

/*
 * Calibrates humidity (see HIH-4030/31 datasheet). Outputs % humidity.
 */
float calibrateHumidity(float inVal)
{
  return ((mapADC_V(inVal)/5.0)-0.16)/0.0062;
}
