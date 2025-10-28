//libraries
#include "Enes100.h"

//pin assignments
int TX_PIN = 10;
int RX_PIN = 11;
int AcUro_ID = 97;
void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Room Number, Wifi Module TX Pin, Wifi Module RX Pin
    Enes100.isConnected();
    Enes100.begin("Fear The Rover", MATERIAL, AcUro_ID, 1116, TX_PIN, RX_PIN);
    // At this point we know we are connected.
    Enes100.println("Connected...");
    Enes100.print("Hello World!");
    delay(100000);
}

void loop() {
    float x, y, t; bool v; // Declare variables to hold the data
   
    x = Enes100.getX(); 
    y = Enes100.getY(); 
    t = Enes100.getTheta(); 
    v = Enes100.isVisible(); 

    if (v) // If the ArUco marker is visible
    {
        Enes100.print(x); 
        Enes100.print(",");
        Enes100.print(y);
        Enes100.print(",");
        Enes100.println(t);
    }
    else { 
        Enes100.println("Not visible"); 
    }
    
}
