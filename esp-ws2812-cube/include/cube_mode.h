#ifndef CUBE_MODE_H_
#define CUBE_MODE_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define COLOR(r,g,b) (((uint32_t)r << 16) | ((uint32_t)g << 8) | b) //r,g,b 0~255


//ws2812b
#define PIN                 D4
#define NUMPIXELS           96
#define BRIGHTLESS          100
#define DEFAULT_DELAY_VAL   1000   
#define FACE_CHANGE_THRSHLD 15900
#define POINT_CHANGE_THRSHLD 8000
#define ADC_SAMPLE_WINDOW   50

#pragma once

class CUBE_MODE : public Adafruit_NeoPixel{
    public:
        uint8_t mode = 1;
        bool mode_changed = false;
    private:
        void music_mode0(uint16_t peakToPeak);
        void music_mode1(uint16_t peakToPeak);
        void music_mode2(uint16_t peakToPeak);
        void music_mode3(uint16_t peakToPeak);
        void music_mode4(uint16_t peakToPeak);
        bool delay_ms(uint16_t ms);

    public:
        CUBE_MODE(int n,int16_t pin,neoPixelType type):Adafruit_NeoPixel(n,pin,type){}
        CUBE_MODE():Adafruit_NeoPixel(){}

        void led_switch_RGB();
        void breathe_led_all(int wait);
        void breathe_led_single(int pixel,uint32_t color,int wait);
        void rainbow(uint8_t wait);
        void rainbowCycle(uint8_t wait);
        void theaterChase(uint32_t c, uint8_t wait);
        void one_face_one_color_fixed();
        void one_face_one_color_random();
        void rectangle_random();
        void rectangle_color_flow(int wait);
        void face_color_flow(int wait);
        void all_color_gradual_change(int wait);
        void one_by_one_circle(int wait);
        void arrow();
        void question_mark();
        void do_all_mode();
        void music_mode(int mode);  
        uint32_t Wheel(byte WheelPos);
        void cube_get_acc_mode(int16_t ax,int16_t ay,int16_t az);
        void mode_apply();
};
#endif

