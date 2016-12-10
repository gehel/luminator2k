//
// Created by gehel on 10.12.16.
//

#include "Panel.h"

Panel::Panel(Adafruit_NeoPixel& _neoPixel): neoPixel(_neoPixel) {
    selectedEffect = EFFECT_BLINK_WHITE;
    currentStep = 0;
    maxSteps = 1;
    lastStepTime = millis();
    stepDuration = 10;
    active = false;
}

void Panel::begin() {
    neoPixel.begin();
    neoPixel.show();
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
            }
        } else {
            stepBlack(currentStep);
        }
        neoPixel.show();
    }
}

void Panel::off() { active = false; }

void Panel::on() { active = true; }

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
    }
}

void Panel::temporaryEffect(uint8_t effect) {
    Serial.println("temporaryEffect");
    if (!isTemporaryEffectActive()) {
        Serial.println("temporaryEffectReal");
        tempSavedEffect = selectedEffect;
        setEffect(effect);
    }
}

void Panel::resetTemporaryEffect() {
    Serial.println("resetTemporaryEffect");
    if (isTemporaryEffectActive()) {
        Serial.println("resetTemporaryReal");
        setEffect(tempSavedEffect);
        tempSavedEffect = -1;
    }
}

boolean Panel::isTemporaryEffectActive() {
    return tempSavedEffect != -1;
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

void Panel::colorWipe(uint32_t c) {
    for(uint16_t i = 0; i<neoPixel.numPixels(); i++) {
        neoPixel.setPixelColor(i, c);
    }
}

void Panel::rainbow(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256; j++) {
        for(i=0; i<neoPixel.numPixels(); i++) {
            neoPixel.setPixelColor(i, Wheel((i+j) & 255));
        }
        neoPixel.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void Panel::rainbowCycle(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< neoPixel.numPixels(); i++) {
            neoPixel.setPixelColor(i, Wheel(((i * 256 / neoPixel.numPixels()) + j) & 255));
        }
        neoPixel.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void Panel::theaterChase(uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
                neoPixel.setPixelColor(i+q, c);    //turn every third pixel on
            }
            neoPixel.show();

            delay(wait);

            for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
                neoPixel.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void Panel::theaterChaseRainbow(uint8_t wait) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
                neoPixel.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
            }
            neoPixel.show();

            delay(wait);

            for (uint16_t i=0; i < neoPixel.numPixels(); i=i+3) {
                neoPixel.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
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
