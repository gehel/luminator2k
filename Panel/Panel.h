//
// Created by gehel on 10.12.16.
//

#ifndef LUMINATOR2K_PANEL_H
#define LUMINATOR2K_PANEL_H

#include <Adafruit_NeoPixel.h>

#define DEBUG

#ifdef DEBUG
  #define PRINT(x) Serial.print(x)
  #define PRINTLN(x) Serial.println(x)
#else
  #define PRINT(x)
  #define PRINTLN(x)
#endif

#define EFFECT_BLACK 0
#define EFFECT_HORIZONTAL 1
#define EFFECT_SNAKE 2
#define EFFECT_RAINBOW 3
#define EFFECT_RAINBOW_CYCLE 4
#define EFFECT_THEATER_CHASE 5
#define EFFECT_THEATER_CHASE_RAINBOW 6
#define EFFECT_MAX 6

#define EFFECT_BLINK_WHITE 100
#define EFFECT_BLINK_RED 101
#define EFFECT_BLINK_GREEN 102
#define EFFECT_BLINK_BLUE 103
#define EFFECT_BLINK_YELLOW 104

#define EFFECT_NONE 255

class Panel {

public:
    Panel(Adafruit_NeoPixel& neoPixel);

    void begin();
    void pump();
    void off();
    void on();
    uint32_t color(uint8_t r, uint8_t g, uint8_t b);
    void show();
    void colorWipe(uint32_t c);
    void incEffect();
    void decEffect();
    void setEffect(uint8_t effect);
    void temporaryEffect(uint8_t effect);
    void resetTemporaryEffect();

private:
    void stepBlack(uint16_t step);
    void stepBlink(uint16_t step, uint32_t color);
    void stepBlinkWhite(uint16_t step);
    void stepBlinkRed(uint16_t step);
    void stepBlinkGreen(uint16_t step);
    void stepBlinkBlue(uint16_t step);
    void stepBlinkYellow(uint16_t step);
    void stepHorizontal(uint16_t step);
    void stepSnake(uint16_t step);
    void stepRainbow(uint16_t step);
    void stepRainbowCycle(uint16_t step);
    void stepTheaterChase(uint16_t step);
    void stepTheaterChaseRainbow(uint16_t step);
    uint32_t Wheel(byte WheelPos);

    Adafruit_NeoPixel& neoPixel;

    uint8_t selectedEffect;
    uint8_t tempSavedEffect;
    uint16_t currentStep;
    uint16_t maxSteps;
    unsigned long lastStepTime;
    unsigned long stepDuration;
    boolean active;
};


#endif //LUMINATOR2K_PANEL_H
