#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Room Number, Wifi Module TX Pin, Wifi Module RX Pin
    Enes100.begin("Fear The Rover", MATERIAL, #unsure, 1116, 10, 11);
    // At this point we know we are connected.
    Enes100.println("Connected...");
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
    
    Enes100.mission(WEIGHT, LIGHT);
    // Transmit the MATERIAL_TYPE of the material (Foam or Plastic, in this case FOAM)
    Enes100.mission(MATERIAL_TYPE, FOAM);
    delay(1000);
}
