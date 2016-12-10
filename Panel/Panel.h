//
// Created by gehel on 10.12.16.
//

#ifndef LUMINATOR2K_PANEL_H
#define LUMINATOR2K_PANEL_H

#include <Adafruit_NeoPixel.h>

#define EFFECT_BLACK 0
#define EFFECT_BLINK_WHITE 1
#define EFFECT_BLINK_RED 2
#define EFFECT_BLINK_GREEN 3
#define EFFECT_BLINK_BLUE 4
#define EFFECT_BLINK_YELLOW 5
#define EFFECT_HORIZONTAL 6

class Panel {

public:
    Panel(Adafruit_NeoPixel& neoPixel);

    void begin();
    void pump();
    void off();
    void on();
    void setEffect(uint8_t effect);
    void temporaryEffect(uint8_t effect);
    void resetTemporaryEffect();
    boolean isTemporaryEffectActive();

private:
    void stepBlack(uint16_t step);
    void stepBlink(uint16_t step, uint32_t color);
    void stepBlinkWhite(uint16_t step);
    void stepBlinkRed(uint16_t step);
    void stepBlinkGreen(uint16_t step);
    void stepBlinkBlue(uint16_t step);
    void stepBlinkYellow(uint16_t step);
    void stepHorizontal(uint16_t step);
    void colorWipe(uint32_t c);
    void rainbow(uint8_t wait);
    void rainbowCycle(uint8_t wait);
    void theaterChase(uint32_t c, uint8_t wait);
    void theaterChaseRainbow(uint8_t wait);
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
