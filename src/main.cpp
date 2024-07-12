#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// OLED spec
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// SSD1306 I2C display
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ADC ADS1115
Adafruit_ADS1115 ads;

// BLE UUIDs
#define SERVICE_UUID "5d9f680d-aa9a-43d7-9d88-34049e7663e0"
#define CHARACTERISTIC_UUID_RX "b1c2e973-65d0-4349-8dbe-fa4093747d65"
#define CHARACTERISTIC_UUID_TX "bcf0e6da-9d88-4497-bad3-2fc93acb2f78"


// ESP32 GPIO for LED
const int LEDpin = A0;


// ADC readings 
#define MAX_SIZE 50
float photodiode_readings[3][MAX_SIZE];
int data_index = 0;


uint8_t txValue = 0;
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic = NULL;


bool deviceConnected = false;
bool oldDeviceConnected = false;

// LED cycle control
const int samplesPerOnCycle = 150;
const int samplesPerOffCycle = 60;

// Counter initialisation
int sampleCounter = 0; // Sample numbers in every cycle
int totalSampleCounter = 0; // Total samples
int cycleCounter = 0; // Cycle numbers
bool LEDState = HIGH; // LED initialised to ON


const int PWM_CHANNEL = 0;    // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 1000;
const int PWM_RESOLUTION = 10; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 

// The max duty cycle value based on PWM resolution (will be 255 if resolution is 8 bits)
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 


// BLE server callback
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
    }
};

// BLE characteristic
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0) {
            Serial.print("RX: ");
            for (int i = 0; i < rxValue.length(); i++)
                Serial.print(rxValue[i]);
            Serial.println();
        }
    }
};



void readAndSendData() {
    int16_t adc0, adc1;
    float volts0, volts1;

    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);

    volts0 = ads.computeVolts(adc0);
    volts1 = ads.computeVolts(adc1);

    // elapsed time during the experiment
    unsigned long elapsedTime = millis();

    // Serial monitor
    Serial.printf("Cycle: %d, Total Sample: %d, Cycle Sample: %d, Time: %lu ms, PDy Voltage: %.3f V, PDo Voltage: %.3f V\n", cycleCounter, totalSampleCounter + 1, sampleCounter + 1, elapsedTime, volts0, volts1);

    // buffer
    if (data_index < MAX_SIZE) {
        photodiode_readings[0][data_index] = volts0;
        photodiode_readings[1][data_index] = volts1;
        data_index++;
    } else {
        for (int i = 0; i < MAX_SIZE - 1; i++) {
            photodiode_readings[0][i] = photodiode_readings[0][i + 1];
            photodiode_readings[1][i] = photodiode_readings[1][i + 1];
        }
        
        photodiode_readings[0][MAX_SIZE - 1] = volts0;
        photodiode_readings[1][MAX_SIZE - 1] = volts1;
    }


    // OLED display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Photodiode Readings");
    display.setCursor(0, 20);
    display.printf("PDy Voltage: %.3f V", volts0);
    display.setCursor(0, 40);
    display.printf("PDo Voltage: %.3f V", volts1);

    display.display();

    // BLE notification
    if (deviceConnected) {
        char txString[64];
        snprintf(txString, sizeof(txString), "PDy: %.3f V\nPDo: %.3f V\n", volts0, volts1);

        pTxCharacteristic->setValue(txString);
        pTxCharacteristic->notify();
    }

    // Check the device connection
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);
        pServer->startAdvertising(); // Advertise
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }

    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}



void setup() {
    Serial.begin(115200);

    // // GPIO output
    // pinMode(LEDpin, OUTPUT);
    // analogWrite(LEDpin, 0);

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(LEDpin, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, 0);

    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS.");
        while (1); // keep waiting
    }

    // ADC data rate
    ads.setDataRate(RATE_ADS1115_250SPS);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1); // keep waiting
    }

    // Wait for signal sync for data collection
    display.display();
    delay(5000);
    display.clearDisplay();

    BLEDevice::init("ESP32S3_BLE");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxCharacteristic->addDescriptor(new BLE2902());

    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();
    pServer->getAdvertising()->start();

    Serial.println("Waiting for a client connection...");
    // Sync signal between data acquistion side based on Python and serial communicaiton
    Serial.println("START");
}



void loop() {
    readAndSendData();
    
    // Sample counter
    sampleCounter++;
    totalSampleCounter++;

    // Different sample points for ON/OFF
    int samplesPerCycle = (LEDState == HIGH) ? samplesPerOnCycle : samplesPerOffCycle;

    if (sampleCounter >= samplesPerCycle) {
        // ON <-> OFF
        LEDState = !LEDState;

        if (LEDState) {
            ledcWrite(PWM_CHANNEL, (int)(MAX_DUTY_CYCLE * 0.70)); // TARGET - 0.6mW
            // analogWrite(LEDpin, 120);
        } else {
            ledcWrite(PWM_CHANNEL, 0);
            // analogWrite(LEDpin, 120);
        }

        // Reset the counter for recording the sample numbers in the current cycle
        sampleCounter = 0;

        // Increase the counter for recording the total running cycles
        if (LEDState == HIGH) {
            cycleCounter++;
        }
    }
}