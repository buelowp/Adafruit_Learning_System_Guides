#if 1 // Change to 0 to disable this code (must enable ONE user*.cpp only!)

#include "globals.h"

static int xIn;
static int yIn;

// This file provides a crude way to "drop in" user code to the eyes,
// allowing concurrent operations without having to maintain a bunch of
// special derivatives of the eye code (which is still undergoing a lot
// of development). Just replace the source code contents of THIS TAB ONLY,
// compile and upload to board. Shouldn't need to modify other eye code.

// User globals can go here, recommend declaring as static, e.g.:
// static int foo = 42;

// Called once near the end of the setup() function. If your code requires
// a lot of time to initialize, make periodic calls to yield() to keep the
// USB mass storage filesystem alive.
void user_setup(void) 
{
    showSplashScreen = false;
    moveEyesRandomly = false;
    pinMode(13, OUTPUT);
}

// Called periodically during eye animation. This is invoked in the
// interval before starting drawing on the last eye (left eye on MONSTER
// M4SK, sole eye on HalloWing M0) so it won't exacerbate visible tearing
// in eye rendering. This is also SPI "quiet time" on the MONSTER M4SK so
// it's OK to do I2C or other communication across the bridge.
// This function BLOCKS, it does NOT multitask with the eye animation code,
// and performance here will have a direct impact on overall refresh rates,
// so keep it simple. Avoid loops (e.g. if animating something like a servo
// or NeoPixels in response to some trigger) and instead rely on state
// machines or similar. Additionally, calls to this function are NOT time-
// constant -- eye rendering time can vary frame to frame, so animation or
// other over-time operations won't look very good using simple +/-
// increments, it's better to use millis() or micros() and work
// algebraically with elapsed times instead.
void user_loop(void) 
{
    char coords[2][5];
    int index = 0;
    int i = 0;

    // Clean up so we don't use old data
    memset(coords[0], '\0', 5);
    memset(coords[1], '\0', 5);

    // There isn't a big transfer, so just grab it all, and tell the eyes where to go
    while (Serial.available() > 0) {
        char c = Serial.read();

        switch (c) {
            case '\n':
                eyeTargetX = std::stof(coords[0]);
                eyeTargetY = std::stof(coords[1]);
                Serial.print(coords[0]);
                Serial.print(",");
                Serial.println(coords[1]);
                break;
            case ',':
                index++;
                i = 0;
                break;
            default:
                coords[index][i++] = c;
                break;
        }

        // Gonna need better handling of errors here should we never
        // get a comma or newline
        if (i == 5) {
            return;
        }
    }
}

#endif // 0
