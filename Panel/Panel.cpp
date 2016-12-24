//
// Created by gehel on 10.12.16.
//

#include "Panel.h"

Panel::Panel(Adafruit_NeoPixel& _neoPixel): neoPixel(_neoPixel) {
    selectedEffect = EFFECT_BLINK_WHITE;
    tempSavedEffect = EFFECT_NONE;
    currentStep = 0;
    maxSteps = 1;
    lastStepTime = millis();
    stepDuration = 10;
    active = false;
    neoPixel.setBrightness(64);
}

void Panel::begin() {
    neoPixel.begin();
    neoPixel.show();
}

void Panel::incEffect() {
    if (selectedEffect == EFFECT_MAX) setEffect(0);
    else setEffect(selectedEffect + 1);
}

void Panel::decEffect() {
    if (selectedEffect == 0) setEffect(EFFECT_MAX);
    else setEffect(selectedEffect - 1);
}

void Panel::pump() {
    unsigned long now = millis();
    if (lastStepTime + stepDuration < now) {
        lastStepTime = now;
        currentStep++;
        if (currentStep >= maxSteps) currentStep = 0;
        if (active) {
            switch (selectedEffect) {
                case EFFECT_BLACK:
                    stepBlack(currentStep);
                    break;
                case EFFECT_BLINK_WHITE:
                    stepBlinkWhite(currentStep);
                    break;
                case EFFECT_BLINK_RED:
                    stepBlinkRed(currentStep);
                    break;
                case EFFECT_BLINK_GREEN:
                    stepBlinkGreen(currentStep);
                    break;
                case EFFECT_BLINK_BLUE:
                    stepBlinkBlue(currentStep);
                    break;
                case EFFECT_BLINK_YELLOW:
                    stepBlinkYellow(currentStep);
                    break;
                case EFFECT_HORIZONTAL:
                    stepHorizontal(currentStep);
                    break;
                case EFFECT_SNAKE:
                    stepSnake(currentStep);
                    break;
                case EFFECT_RAINBOW:
                    stepRainbow(currentStep);
                    break;
                case EFFECT_RAINBOW_CYCLE:
                    stepRainbowCycle(currentStep);
                    break;
                case EFFECT_THEATER_CHASE:
                    stepTheaterChase(currentStep);
                    break;
                case EFFECT_THEATER_CHASE_RAINBOW:
                    stepTheaterChaseRainbow(currentStep);
                    break;
            }
        } else {
            stepBlack(currentStep);
        }
        neoPixel.show();
    }
}

void Panel::off() { active = false; }

void Panel::on() { active = true; }

uint32_t Panel::color(uint8_t r, uint8_t g, uint8_t b) {
    return neoPixel.Color(r, g, b);
}

void Panel::show() {
    neoPixel.show();
}

void Panel::setEffect(uint8_t effect) {
    selectedEffect = effect;
    currentStep = 0;
    switch (effect) {
        case EFFECT_BLACK:
            maxSteps = 1;
            stepDuration = 10000;
        case EFFECT_BLINK_WHITE:
        case EFFECT_BLINK_RED:
        case EFFECT_BLINK_GREEN:
        case EFFECT_BLINK_BLUE:
        case EFFECT_BLINK_YELLOW:
            maxSteps = 12;
            stepDuration = 4;
            break;
        case EFFECT_HORIZONTAL:
            maxSteps = 5;
            stepDuration = 200;
            break;
        case EFFECT_SNAKE:
            maxSteps = neoPixel.numPixels();
            stepDuration = 100;
            break;
        case EFFECT_RAINBOW:
        case EFFECT_RAINBOW_CYCLE:
            maxSteps = 256;
            stepDuration = 10;
            break;
        case EFFECT_THEATER_CHASE:
            maxSteps = 3;
            stepDuration = 100;
            break;
        case EFFECT_THEATER_CHASE_RAINBOW:
            maxSteps = 3 * 256;
            stepDuration = 80;
            break;
    }
}

void Panel::temporaryEffect(uint8_t effect) {
    if (tempSavedEffect == EFFECT_NONE) {
        tempSavedEffect = selectedEffect;
    }
    setEffect(effect);
}

void Panel::resetTemporaryEffect() {
    if (tempSavedEffect != EFFECT_NONE) setEffect(tempSavedEffect);
    tempSavedEffect = EFFECT_NONE;
}

void Panel::stepBlack(uint16_t step) {
    colorWipe(neoPixel.Color(0, 0, 0));
}

void Panel::stepBlink(uint16_t step, uint32_t color) {
    if (step == 0) colorWipe(color);
    else colorWipe(neoPixel.Color(0, 0, 0));
}

void Panel::stepBlinkWhite(uint16_t step) {
    stepBlink(step, neoPixel.Color(255, 255, 255));
}

void Panel::stepBlinkRed(uint16_t step) {
    stepBlink(step, neoPixel.Color(255, 0, 0));
}

void Panel::stepBlinkGreen(uint16_t step) {
    stepBlink(step, neoPixel.Color(0, 255, 0));
}

void Panel::stepBlinkBlue(uint16_t step) {
    stepBlink(step, neoPixel.Color(0, 0, 255));
}

void Panel::stepBlinkYellow(uint16_t step) {
    stepBlink(step, neoPixel.Color(255, 255, 0));
}

void Panel::stepHorizontal(uint16_t step) {
    for(uint16_t i = 0; i<neoPixel.numPixels(); i++) {
        if ((i / 4) == step) neoPixel.setPixelColor(i, neoPixel.Color(255, 255, 255));
        else neoPixel.setPixelColor(i, neoPixel.Color(0, 0, 0));
    }
}

void Panel::stepSnake(uint16_t step) {
    for(uint16_t i = 0; i<neoPixel.numPixels(); i++) {
        if (i == step) neoPixel.setPixelColor(i, neoPixel.Color(255, 255, 255));
        else neoPixel.setPixelColor(i, neoPixel.Color(0, 0, 0));
    }
}

void Panel::colorWipe(uint32_t c) {
    for(uint16_t i = 0; i<neoPixel.numPixels(); i++) {
        neoPixel.setPixelColor(i, c);
    }
}

void Panel::stepRainbow(uint16_t step) {
    for(uint16_t i=0; i<neoPixel.numPixels(); i++) {
        neoPixel.setPixelColor(i, Wheel((i+step) & 255));
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void Panel::stepRainbowCycle(uint16_t step) {
    for(uint16_t i=0; i< neoPixel.numPixels(); i++) {
        neoPixel.setPixelColor(i, Wheel(((i * 256 / neoPixel.numPixels()) + step) & 255));
    }
}

//Theatre-style crawling lights.
void Panel::stepTheaterChase(uint16_t step) {
        for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
            neoPixel.setPixelColor(i+step, neoPixel.Color(255, 255, 255));    //turn every third pixel on
        }
        for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
            neoPixel.setPixelColor(i+((step+1)%3), 0);        //turn every third pixel off
        }
}

//Theatre-style crawling lights with rainbow effect
void Panel::stepTheaterChaseRainbow(uint16_t step) {
    int j = step % 256;
    int q = step % 3;
    int r = (step + 1) % 3;
    for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
        neoPixel.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
    }
    for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
        neoPixel.setPixelColor(i+r, 0);        //turn every third pixel off
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Panel::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return neoPixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return neoPixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return neoPixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
