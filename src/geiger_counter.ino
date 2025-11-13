// ------------------- Blynk macros (MUST come first!) -------------------
#define BLYNK_TEMPLATE_NAME "Geiger Counter"
#define BLYNK_TEMPLATE_ID "TMPL3RH6nZVXC"
#define BLYNK_AUTH_TOKEN "YOUR-AUTH-TOKEN"

// ------------------- Libraries -------------------
#include <WiFi.h>                  
#include <BlynkSimpleWifi.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <splash.h>
#include <DFRobot_Geiger.h>

// ------------------- WiFi credentials -------------------
char ssid[] = "Downrange";
char pass[] = "80Whbattery&60%";

// ------------------- Hardware -------------------
#define detect_pin 0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DFRobot_Geiger geiger(detect_pin);

uint16_t cpm = 0;
float usvh = 0.0;

// ------------------- Setup -------------------
void setup() {
  Serial.begin(115200);

  // Start Geiger counter
  geiger.start();

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to WiFi and Blynk");
}

// ------------------- Main loop -------------------
void loop() {
  Blynk.run();

  // Read Geiger data
  cpm = geiger.getCPM();
  usvh = geiger.getuSvh();

  // Print to Serial
  Serial.print("CPM: ");
  Serial.print(cpm);
  Serial.print("  uSvh: ");
  Serial.println(usvh);

  // Update OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16);
  display.print(F("CPM: "));
  display.println(cpm);
  display.setTextSize(2);
  display.print(F("\nuSvh: "));
  display.println(usvh);
  display.display();

  // Send to Blynk IoT
  Blynk.virtualWrite(V0, cpm);   // CPM
  Blynk.virtualWrite(V1, usvh);  // µSv/h

  delay(3000); // Update every 3 seconds
}
