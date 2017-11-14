/*
    FILE          : Assignment3
    PROJECT       : PROG1345 - Assignment #3
    PROGRAMMER    : Rohit Bhardwaj
    FIRST VERSION : 2016-07-06
    DESCRIPTION   : This program uses the MCP9700A temperature sensor and displays the current temperature
                    in the top right corner of the LCD dispaly with one decimal place and also illuminates
                    RGB LED with the specified colour for the given temperature ranges.(-15 C  to 5.0 C)blue
                    (5.1 C to 15 C )yellow ,(15.1 C to 25 C) orange, Above 25.1 C red
*/
# include <LiquidCrystal.h>                                                  //It will add LCD Library in the program
# define RS 19                                                               //RS is Register Select                            
# define ENABLE 18
# define D4 17                                                               //D4 TO D7 are the data lines of the LCD Display
# define D5 16
# define D6 15
# define D7 14
# define SENSOROUTPUTPIN 20                                                  //The output of the sensor is connected at pin20
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);                               //It creates an lcd object

//PWM pins are used to control the brightness of the RGB lED
const int8_t redLedPin = 3;                                                  //red is connected at pin3
const int8_t greenLedPin = 5;                                                //green is connected at pin5
const int8_t blueLedPin = 6;                                                 //blue is connected at pin6

//function prototypes
float averageFunction();
void checkTemperatureRange(float temperature);
void rgbBrightnessControl(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);

void setup()
{
  lcd.begin(16, 2);                                                          //initializes the lcd to 2X16
  //Configures the specified pins to behave as an output.
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
}

void loop()
{
  float averageTemperature = 0;
  averageTemperature = averageFunction ();                                   //calling the averageFunction()
  lcd.printf("Celsius");
  lcd.setCursor(12, 0);                                                      //It places the cursor at top right corner of the LCD dispaly
  lcd.printf("%4.1f", averageTemperature);                                   //It dispalys temperature with one decimal place

  float degreeFahrenheit = (averageTemperature * 1.8) + 32;                  //It converts the temperature in celsius into degree Fahrenheit
  lcd.setCursor(0, 1);                                                       //take the cursor in the second row
  lcd.printf("Fahrenheit");
  lcd.setCursor(12, 1);
  lcd.printf("%4.1f", degreeFahrenheit);                                     //display temperature in degree Fahrenheit

  checkTemperatureRange (averageTemperature);                                //calling the checkTemperatureRange function and passing the parameter in it
}

// FUNCTION        : averageFunction
// DESCRIPTION     : It provides the average temperature value with the help
//                   of sampling and also converts raw temperature value into
//                   voltage and the finally to temperaure in degree celsius
// PARAMETERS      : No parameters are passed
// RETURNS         : It returns the average temperature value
float averageFunction()
{
  //let's take the samples
#define SAMPLESIZE 1000
  float sensorOutputValue = 0.0;
  float voltageValue = 0.0;
  float temperatureValue = 0.0;
  float sampleArray[SAMPLESIZE] = {0};                                         //It creates an array of size 1000 with all elements equal to zero
  float sampleSum = 0;

  //now find the sum of the samples
  for (int16_t x = 0; x < SAMPLESIZE ; x++)
  {
    sensorOutputValue = analogRead(SENSOROUTPUTPIN);                            //get the output from the sensor
    voltageValue = float(sensorOutputValue / 1024) * 3.3;                       //scale it to volts
    temperatureValue = float(voltageValue - 0.5) / 0.01;                        //scale it to degree Celsius
    sampleArray[x] = temperatureValue;                                          //It will update the elements of the array
    sampleSum = sampleSum + sampleArray[x];
  }

  //now get the average
  temperatureValue = sampleSum / SAMPLESIZE;
  return temperatureValue;
}

// FUNCTION        : checkTemperatureRange
// DESCRIPTION     : It will check in which given temperature range the
//                   temperature from the sensor lies
// PARAMETERS      :
// temperature     : It will provide you the current value of temperature
// RETURNS         : Nothing
void checkTemperatureRange(float temperature)
{
  if ( temperature > -15.0 && temperature < 5.0 )
  {
    //turn on the blue light
    rgbBrightnessControl (0, 0, 255);
  }
  else if ( temperature > 5.1 && temperature < 15.0 )
  {
    //turn on the yellow light
    rgbBrightnessControl (255, 255, 0);
  }
  else if (temperature > 15.1 && temperature < 25 )
  {
    //turn on the orange light
    rgbBrightnessControl (230, 40, 0);
  }
  else if (temperature > 25.1 )
  {
    //turn on the red light
    rgbBrightnessControl (255, 0, 0);
  }
}

// FUNCTION        : rgbBrightnessControl
// DESCRIPTION     : It will illuminate your RGB LED with the
//                   specified colour for the given temperature
//                   ranges.
// PARAMETERS      :
// redValue        : intensity/brightness of red light
// greenValue      : intensity/brightness of green light
// blueValue       : intensity/brightness of blue light
// RETURNS         : Nothing
void rgbBrightnessControl(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
  //analogWrite is used to light a RGB LED at varying brightnesses
  analogWrite(redLedPin, redValue);
  analogWrite(greenLedPin, greenValue);
  analogWrite(blueLedPin, blueValue);
}

