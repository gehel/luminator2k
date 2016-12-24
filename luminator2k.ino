#include <Adafruit_NeoPixel.h>
#include <Panel.h>
#include <Rotary.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define WHEEL_PUSH_PIN A0

#define PANEL_PIN_1 13
#define PANEL_PIN_2 4
#define NB_PIXELS_IN_PANEL 16

const uint8_t switch_pins[] = { 9, 10, 11, 12 };
const uint8_t button_pins[] = { 5, 6, 7, 8 };

int switch_states[4] = { 2, 2, 2, 2 };
int button_states[4] = { 2, 2, 2, 2 };
int switch_states_prev[4] = { 3, 3, 3, 3 };
int button_states_prev[4] = { 3, 3, 3, 3 };

int wheel_push_state = HIGH;
int wheel_push_state_prev = LOW;

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

#define NUM_PANELS 2
Panel panels[NUM_PANELS] = {
    Panel(strip1),
    Panel(strip2)
};

uint8_t selected_panel = 0;

Rotary encoder = Rotary(2, 3);

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

#ifdef DEBUG
    Serial.begin(57600);
#endif

    for (uint8_t i = 0; i < 4; i++) {
        pinMode(button_pins[i], INPUT);
        pinMode(switch_pins[i], INPUT);
    }

    pinMode(WHEEL_PUSH_PIN, INPUT_PULLUP);

    for (uint8_t i = 0; i < NUM_PANELS; i++) {
        panels[i].begin();
        panels[i].setEffect(EFFECT_HORIZONTAL);
    }

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
    sei();
}

ISR(PCINT2_vect) {
    unsigned char result = encoder.process();
    if (result == DIR_NONE) {
        // do nothing
    } else if (result == DIR_CW) {
        panels[selected_panel].incEffect();
    } else if (result == DIR_CCW) {
        panels[selected_panel].decEffect();
    }
}

void loop() {
    readState();

    processButtons();
    processSwitches();
    processWheel();

    for (uint8_t i = 0; i < NUM_PANELS; i++) {
        panels[i].pump();
    }
}

void processButtons() {
    if (button_states[0] != button_states_prev[0]) {
        if (button_states[0] == LOW) panels[0].temporaryEffect(EFFECT_BLINK_BLUE);
        else panels[0].resetTemporaryEffect();
    }
    if (button_states[1] != button_states_prev[1]) {
        if (button_states[1] == LOW) panels[0].temporaryEffect(EFFECT_BLINK_GREEN);
        else panels[0].resetTemporaryEffect();
    }
    if (button_states[2] != button_states_prev[2]) {
        if (button_states[2] == LOW) panels[1].temporaryEffect(EFFECT_BLINK_YELLOW);
        else panels[1].resetTemporaryEffect();
    }
    if (button_states[3] != button_states_prev[3]) {
        if (button_states[3] == LOW) panels[1].temporaryEffect(EFFECT_BLINK_RED);
        else panels[1].resetTemporaryEffect();
    }
}

void processSwitches() {
    if (switch_states[0] != switch_states_prev[0]) {
        if (switch_states[0] == LOW) panels[0].on();
        else if (switch_states[1] == HIGH) panels[0].off();
    }
    if (switch_states[1] != switch_states_prev[1]) {
        if (switch_states[1] == LOW) panels[0].on();
        else if (switch_states[0] == HIGH) panels[0].off();
    }

    if (switch_states[2] != switch_states_prev[2]) {
        if (switch_states[2] == LOW) panels[1].on();
        else if (switch_states[3] == HIGH) panels[1].off();
    }
    if (switch_states[3] != switch_states_prev[3]) {
        if (switch_states[3] == LOW) panels[1].on();
        else if (switch_states[2] == HIGH) panels[1].off();
    }
}

void processWheel() {
    if (wheel_push_state != wheel_push_state_prev) {
        if (wheel_push_state == LOW) {
            if (selected_panel == 0) selected_panel = 1;
            else selected_panel = 0;
        } else {
            panels[selected_panel].colorWipe(panels[selected_panel].color(255, 255, 255));
            panels[selected_panel].show();
            delay(200);
            panels[selected_panel].colorWipe(panels[selected_panel].color(0, 0, 0));
            panels[selected_panel].show();
        }
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

    wheel_push_state_prev = wheel_push_state;
    wheel_push_state = digitalRead(WHEEL_PUSH_PIN);
}
