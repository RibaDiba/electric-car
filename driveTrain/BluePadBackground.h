#pragma once
#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void bluepadStartup(){
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
      const uint8_t* addr = BP32.localBdAddress();
      Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

      // Setup the Bluepad32 callbacks
      BP32.setup(&onConnectedController, &onDisconnectedController);

      // "forgetBluetoothKeys()" should be called when the user performs
      // a "device factory reset", or similar.
      // Calling "forgetBluetoothKeys" in setup() just as an example.
      // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
      // But it might also fix some connection / re-connection issues.
      //BP32.forgetBluetoothKeys();

      // Enables mouse / touchpad support for gamepads that support them.
      // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
      // - First one: the gamepad
      // - Second one, which is a "virtual device", is a mouse.
      // By default, it is disabled.
      BP32.enableVirtualDevice(false);
  }
