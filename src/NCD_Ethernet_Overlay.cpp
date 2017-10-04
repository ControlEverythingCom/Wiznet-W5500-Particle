/* NCD_Ethernet_Overlay library by Travis Elliott
This is a placeholder file required for Particle Library functionality.  It does nothing!!!!!
 */

#include "NCD_Ethernet_Overlay.h"

/**
 * Constructor.
 */
NCD_Ethernet_Overlay::NCD_Ethernet_Overlay()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void NCD_Ethernet_Overlay::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void NCD_Ethernet_Overlay::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void NCD_Ethernet_Overlay::doit()
{
    Serial.println("called doit");
}
