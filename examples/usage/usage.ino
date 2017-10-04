// Example usage for NCD_Ethernet_Overlay library by Travis Elliott.

#include "NCD_Ethernet_Overlay.h"

// Initialize objects from the lib
NCD_Ethernet_Overlay nCD_Ethernet_Overlay;

void setup() {
    // Call functions on initialized library objects that require hardware
    nCD_Ethernet_Overlay.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    nCD_Ethernet_Overlay.process();
}
