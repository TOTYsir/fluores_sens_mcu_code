#ifndef __BLE_H
#define __BLE_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "5d9f680d-aa9a-43d7-9d88-34049e7663e0"
#define CHARACTERISTIC_UUID_RX "b1c2e973-65d0-4349-8dbe-fa4093747d65"
#define CHARACTERISTIC_UUID_TX "bcf0e6da-9d88-4497-bad3-2fc93acb2f78"

void ble_init();
void ble_send(float volts0, float volts1);

#endif

