#include <Adafruit_NeoPixel.h>
#include <Panel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

#define PANEL_PIN_1 13
#define PANEL_PIN_2 4
#define NB_PIXELS_IN_PANEL 16

const uint8_t switch_pins[] = { 9, 10, 11, 12 };
const uint8_t button_pins[] = { 5, 6, 7, 8 };

int switch_states[4] = { 2, 2, 2, 2 };
int button_states[4] = { 2, 2, 2, 2 };
int switch_states_prev[4] = { 3, 3, 3, 3 };
int button_states_prev[4] = { 3, 3, 3, 3 };

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NB_PIXELS_IN_PANEL, PANEL_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NB_PIXELS_IN_PANEL, PANEL_PIN_2, NEO_GRB + NEO_KHZ800);

Panel panel1 = Panel(strip1);
Panel panel2 = Panel(strip2);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    Serial.begin(57600);

    for (uint8_t i = 0; i < 4; i++) {
        pinMode(button_pins[i], INPUT);
        pinMode(switch_pins[i], INPUT);
    }

    panel1.begin();
    panel1.setEffect(EFFECT_HORIZONTAL);
    panel2.begin();
    panel2.setEffect(EFFECT_HORIZONTAL);
}

void loop() {
    readState();

    processButtons();
    processSwitches();

    panel1.pump();
    panel2.pump();
}

void processButtons() {
    if (button_states[0] != button_states_prev[0]) {
        if (button_states[0] == LOW) panel1.setEffect(EFFECT_BLINK_BLUE);
        else panel1.setEffect(EFFECT_HORIZONTAL);
    }
    if (button_states[1] != button_states_prev[1]) {
        if (button_states[1] == LOW) panel1.setEffect(EFFECT_BLINK_GREEN);
        else panel1.setEffect(EFFECT_HORIZONTAL);
    }
    if (button_states[2] != button_states_prev[2]) {
        if (button_states[2] == LOW) panel2.setEffect(EFFECT_BLINK_YELLOW);
        else panel2.setEffect(EFFECT_HORIZONTAL);
    }
    if (button_states[3] != button_states_prev[3]) {
        if (button_states[3] == LOW) panel2.setEffect(EFFECT_BLINK_RED);
        else panel2.setEffect(EFFECT_HORIZONTAL);
    }
}

void processSwitches() {
    if (switch_states[0] != switch_states_prev[0]) {
        if (switch_states[0] == LOW) panel1.on();
        else if (switch_states[1] == HIGH) panel1.off();
    }
    if (switch_states[1] != switch_states_prev[1]) {
        if (switch_states[1] == LOW) panel1.on();
        else if (switch_states[0] == HIGH) panel1.off();
    }

    if (switch_states[2] != switch_states_prev[2]) {
        if (switch_states[2] == LOW) panel2.on();
        else if (switch_states[3] == HIGH) panel2.off();
    }
    if (switch_states[3] != switch_states_prev[3]) {
        if (switch_states[3] == LOW) panel2.on();
        else if (switch_states[2] == HIGH) panel2.off();
    }
}

void readState() {
    uint8_t i;
    for (i = 0; i < 4; i++) {
        button_states_prev[i] = button_states[i];
        button_states[i] = digitalRead(button_pins[i]);
        switch_states_prev[i] = switch_states[i];
        switch_states[i] = digitalRead(switch_pins[i]);
    }
}
