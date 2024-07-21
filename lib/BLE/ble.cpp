#include <Arduino.h>
#include "ble.h"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;


// Custom server callback class to handle device connection and disconnection events (overwrite)
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
    }
};

/* 
    Custom characteristic callback class to handle data
    received from the client and print it to the serial monitor (currently NOT USED) 
*/
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue(); // Get the value written to the characteristic
        if (rxValue.length() > 0) {
            Serial.print("RX: ");
            for (int i = 0; i < rxValue.length(); i++)
                Serial.print(rxValue[i]);
            Serial.println();
        }
    }
};

void ble_init() {
    BLEDevice::init("ESP32S3_BLE"); // initialise the BLE device and name it
    pServer = BLEDevice::createServer(); // create the server
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );

    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();
    pServer->getAdvertising()->start(); // start advertising

    Serial.println("Waiting for a client connection...");
    Serial.println("START");

    pTxCharacteristic->setValue("HELLO WORLD");
}


// Send data to the connected BLE client
void ble_send(float volts0, float volts1, float norm_val) {
    if (deviceConnected) {
        char txString[64];
        // snprintf - write formatted output to sized buffer
        snprintf(txString, sizeof(txString), "PDy: %.2f V, PDo: %.2f V, norm: %.2f\n", volts0, volts1, norm_val);
        pTxCharacteristic->setValue(txString); // Set the value for the TX characteristic
        pTxCharacteristic->notify();
    }
    // If the device disconnected and was previously connected
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);
        pServer->startAdvertising();
        // Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // If the device just connected
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}
