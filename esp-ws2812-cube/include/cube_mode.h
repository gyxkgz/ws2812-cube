#ifndef CUBE_MODE_H_
#define CUBE_MODE_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define COLOR(r,g,b) (((uint32_t)r << 16) | ((uint32_t)g << 8) | b) //r,g,b 0~255


//ws2812b
#define PIN                 2
#define NUMPIXELS           96
#define BRIGHTLESS          100
#define DEFAULT_DELAY_VAL   1000   
#define FACE_CHANGE_THRSHLD 15900
#define POINT_CHANGE_THRSHLD 8000
#define ADC_SAMPLE_WINDOW   50

#pragma once

class CUBE_MODE : public Adafruit_NeoPixel{
    private:
        void music_mode0(uint16_t peakToPeak);
        void music_mode1(uint16_t peakToPeak);
        void music_mode2(uint16_t peakToPeak);
        void music_mode3(uint16_t peakToPeak);
        void music_mode4(uint16_t peakToPeak);
        uint32_t Wheel(byte WheelPos);
        bool delay_ms(uint16_t ms,bool mode_changed = false);

    public:
        CUBE_MODE(int n,int16_t pin,neoPixelType type):Adafruit_NeoPixel(n,pin,type){}
        CUBE_MODE():Adafruit_NeoPixel(){}

        void cube_led_switch_RGB();
        void cube_breathe_led_all(int wait);
        void cube_breathe_led_single(int pixel,uint32_t color,int wait);
        void cube_rainbow(uint8_t wait);
        void cube_rainbowCycle(uint8_t wait);
        void cube_theaterChase(uint32_t c, uint8_t wait);
        void cube_one_face_one_color_fixed();
        void cube_one_face_one_color_random();
        void cube_rectangle_random();
        void cube_rectangle_color_flow(int wait);
        void cube_face_color_flow(int wait);
        void cube_all_color_gradual_change(int wait);
        void cube_one_by_one_circle(int wait);
        void cube_arrow();
        void cube_question_mark();
        void cube_do_all_mode();
        void cube_music_mode(int mode);    //three diff mode :
};
#endif

