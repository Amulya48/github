#define BLYNK_TEMPLATE_ID "TMPL3eFtVA8NK"
#define BLYNK_TEMPLATE_NAME "ESP32"
#define BLYNK_AUTH_TOKEN "Dr8DPWe3bZObwKmFKs3k-B1cJhCNqDdE"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char ssid[] = "Vivo y21";
const char password[] = "tourcher18";

int sensor =32; 
int lo1 =4; 
int lo2 =2; 

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus=23;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() { 
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  
  pinMode(sensor,INPUT); 
  pinMode(lo1,INPUT); 
  pinMode(lo2,INPUT); 

  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() { 
  static unsigned long startTime=millis();
  static int peakValue=0; 
  int ecgvalue=0;
  
  // Check for inputs from lo1 and lo2
  if ((digitalRead(lo1)==1)||(digitalRead(lo2)==1)) { 
    Serial.println('!'); 
  } else { 
    // Read analog value from ECG sensor
    int ecgValue = analogRead(sensor);
    
    // Update peak value if current reading is higher
    if (ecgValue > peakValue) {
      peakValue = ecgValue;
    }
    
    // Print the current ECG value
    Serial.println(ecgValue); 
    
    // Send ECG value to Blynk (virtual pin V1)
    Blynk.virtualWrite(V1,ecgValue);
    
    // Log event if ECG value exceeds 3000
    if (ecgValue > 3000) {
      Blynk.logEvent("period_alert", "List of Food Items:Eggs,Oatmeal,Pancakes,Smoothies,Cereal,Sandwiches,Salads,Pasta,Grilled chicken,Rice and curry,Steak,Fish,Stir-fried vegetables,Soup,Pizza, Snacks:Fruits,Chips,Cookies,Yogurt,Nuts,Ice cream,Cake,Pudding,Brownies,Custard");  // Change "ecg_alert" to your desired event name
    }
  }
  
  // Check if one minute has passed
  if (millis() - startTime >= 60000) {
    Serial.print("Peak ECG value in one minute: ");
    Serial.println(peakValue);
    
    // Send peak value to Blynk (virtual pin V2)
    Blynk.virtualWrite(V2, peakValue);
    
    // Reset for next minute
    peakValue =0; 
    startTime =millis(); // Reset the timer
  }
  
  // Run Blynk
  Blynk.run();
  
  // Wait for a bit to keep serial data from saturating
  delay(1); 

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
//  if(temperatureC>35){
//    Blynk.logEvent("temp_alert","Temperature is High");
//  }
  delay(1000);
}
