

#include"cube_mode.h"




int mode=0,mode_changed=0;
uint32_t color_array[]={COLOR(BRIGHTLESS, 0, 0),COLOR(BRIGHTLESS, BRIGHTLESS, 0),COLOR(0, BRIGHTLESS, 0),COLOR(0, BRIGHTLESS,BRIGHTLESS),COLOR(0, 0,BRIGHTLESS),COLOR(BRIGHTLESS, 0,BRIGHTLESS)};
int color_num=0;
long randNumber;
uint32_t random_color[6];


//led mode
void CUBE_MODE::cube_led_switch_RGB()
{
    for(int i=0; i<NUMPIXELS; i++) { 
        setPixelColor(i, COLOR(0, 0, 50));
    }
    show();   // Send the updated pixel colors to the hardware.
    delay_ms(DEFAULT_DELAY_VAL); // Pause before next pass through loop

    for(int i=0; i<NUMPIXELS; i++) { 
        setPixelColor(i, COLOR(0, 50, 0));
    }
    show();   // Send the updated pixel colors to the hardware.
    delay_ms(DEFAULT_DELAY_VAL); // Pause before next pass through loop
    
    for(int i=0; i<NUMPIXELS; i++) { 
        setPixelColor(i, COLOR(50, 0, 0));
    }
    show();   // Send the updated pixel colors to the hardware.
    delay_ms(DEFAULT_DELAY_VAL); // Pause before next pass through loop
}

void CUBE_MODE::cube_breathe_led_all(int wait){

     uint32_t rand_color=Wheel(random(0,255));

    for(int i=0; i<BRIGHTLESS; i++) { // For each pixel in .. 
      for(int n=0;n<NUMPIXELS;n++){
        setPixelColor(n, rand_color);         //  Set pixel's color (in RAM)
      }
      setBrightness(i);
      show();                          //  Update pixels to match
      delay_ms(wait);  
    }  
     for(int i=BRIGHTLESS; i>0; i--) { // For each pixel in ..
       // setPixelColor(n, color);         //  Set pixel's color (in RAM)
       for(int n=0;n<NUMPIXELS;n++){
        setPixelColor(n, rand_color);         //  Set pixel's color (in RAM)
      }
         setBrightness(i);
         show();                          //  Update pixels to match
        delay_ms(wait);  
    }
   // setBrightness(BRIGHTLESS);  
 }
void CUBE_MODE::cube_breathe_led_single(int pixel,uint32_t color,int wait)
{
   for(int i=0; i<BRIGHTLESS; i++) { // For each pixel in .. 
    
      setPixelColor(pixel, color);         //  Set pixel's color (in RAM)
      setBrightness(i);
      show();                          //  Update pixels to match
      delay_ms(wait);  
    }  
   for(int i=BRIGHTLESS; i>0; i--) { // For each pixel in ..
       // setPixelColor(n, color);         //  Set pixel's color (in RAM)
        setPixelColor(pixel, color);         //  Set pixel's color (in RAM)
         setBrightness(i);
         show();                          //  Update pixels to match
        delay_ms(wait);  
    }
  //  setBrightness(BRIGHTLESS);  
}

void CUBE_MODE::cube_rainbow(uint8_t wait) {
  uint16_t i, j;
  setBrightness(BRIGHTLESS);
  for(j=0; j<256; j++) {
    for(i=0; i<NUMPIXELS; i++) {
      setPixelColor(i, Wheel((i+j) & 255));
    }
    show();
    if(!delay_ms(wait))
    {
      return; 
     }
 }
}

void CUBE_MODE::cube_rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  setBrightness(BRIGHTLESS);
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; i++) {
      setPixelColor(i, Wheel(((i * 256 / numPixels()) + j) & 255));
    }
    show();
    if(!delay_ms(wait))
    {
      return;
      }
  }
}
void CUBE_MODE::cube_theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<20; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < numPixels(); i=i+3) {
        setPixelColor(i+q, c);    //turn every third pixel on
      }
      show();

      delay_ms(wait);

      for (uint16_t i=0; i < numPixels(); i=i+3) {
        setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void CUBE_MODE::cube_one_face_one_color_fixed()
{
  for(int j=0;j<6;j++){
    for(int i=0; i<16; i++) { 
      setPixelColor(i+j*16, color_array[j]);
    }
  }
  show();   // Send the updated pixel colors to the hardware.
  delay_ms(1000);
}

void CUBE_MODE::cube_one_face_one_color_random()
{  
  for(int i=0;i<6;i++)
  {
      random_color[i]=Wheel(random(0,255));
  }
  for(int j=0;j<6;j++){
    for(int i=0; i<16; i++) { 
      setPixelColor(i+j*16, random_color[j]);
    }
    randNumber++;
  }
  show();   // Send the updated pixel colors to the hardware.
  delay_ms(1000);
}

void CUBE_MODE::cube_rectangle_random(){
   clear();
  for(int i=0;i<6;i++)
  {
      random_color[i]=Wheel(random(0,255));
  }
   for(int j=0;j<6;j++){
    for(int i=0;i<5;i++)
    {
      setPixelColor(i+j*16, random_color[j]);
    }
    setPixelColor(7+j*16,random_color[j]);
    setPixelColor(8+j*16,random_color[j]);
    for(int i=11;i<16;i++)
    {
      setPixelColor(i+j*16, random_color[j]);
    }
    randNumber++;
   }
   show();   // Send the updated pixel colors to the hardware.
   delay_ms(1000);
}

void CUBE_MODE::cube_rectangle_color_flow(int wait){
  int i;
     clear();
    for(int j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  
      for(int n=0;n<6;n++){
        for( i=0; i< 5; i++) {
          setPixelColor(i+n*16, Wheel(((i * 256 / 12) + j) & 255));
        }
        for(int k=11;k<16;k++,i++)
        {
           setPixelColor(k+n*16, Wheel(((i * 256 / 12) + j) & 255));
        }
        setPixelColor(8+n*16, Wheel(((i++ * 256 / 12) + j) & 255));
        setPixelColor(7+n*16, Wheel(((i * 256 / 12) + j) & 255));
      }
      show();
      if(!delay_ms(wait))
      {
        return;
      }
    }
}

#define COLOR_STEP 8
void CUBE_MODE::cube_face_color_flow(int wait){
  int i;
  
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
  
      for(int n=0;n<6;n++){
        for( i=0; i< 5; i++) {
          setPixelColor(i+n*16, Wheel((i*COLOR_STEP+ j) & 255));
        }
        for(int k=11;k<16;k++,i++)
        {
           setPixelColor(k+n*16, Wheel((i*COLOR_STEP + j) & 255));
        }
        for(int k=8;k>4;k--,i++)
        {
           setPixelColor(k+n*16, Wheel((i*COLOR_STEP + j) & 255));
        }
        setPixelColor(10+n*16, Wheel((i++*COLOR_STEP + j) & 255));
        setPixelColor(9+n*16, Wheel((i*COLOR_STEP + j) & 255));
      }
      show();
      if(!delay_ms(wait))
      {
        return;
      }
    }
}

void CUBE_MODE::cube_all_color_gradual_change(int wait){
    uint16_t i, j;
  setBrightness(BRIGHTLESS);
  for(j=0; j<256; j++) {
    for(i=0; i<NUMPIXELS; i++) {
      setPixelColor(i, Wheel((j) & 255));
    }
    show();
    if(!delay_ms(wait))
    {
      return; 
     }
 }
}

void CUBE_MODE::cube_one_by_one_circle(int wait){
        for(int i=0; i< 5; i++) {
           for(int n=0;n<6;n++){
            setPixelColor(i+n*16,color_array[n]);
           }
            show();
            if(!delay_ms(wait))
            {
              return; 
             }
        }
        for(int k=11;k<16;k++)
        {
           for(int n=0;n<6;n++){
            setPixelColor(k+n*16, color_array[n]);
           }
             show();
          if(!delay_ms(wait))
          {
            return; 
           }
        }
        for(int k=8;k>4;k--)
        {
           for(int n=0;n<6;n++){
            setPixelColor(k+n*16,color_array[n]);
           }
             show();
          if(!delay_ms(wait))
          {
            return; 
           }
        }
       for(int n=0;n<6;n++){
         setPixelColor(10+n*16, color_array[n]);
       }
          show();
          if(!delay_ms(wait))
          {
            return; 
           }
       for(int n=0;n<6;n++){
          setPixelColor(9+n*16, color_array[n]);
       }
          show();
          if(!delay_ms(wait))
          {
            return; 
           }
  }
void CUBE_MODE::cube_arrow(){
   clear();
  for(int i=0;i<6;i++)
  {
      random_color[i]=Wheel(random(0,255));
  }
   delay_ms(200);
   for(int j=0;j<6;j++){
       setPixelColor(0+j*16,random_color[j]);
   }
    show();
   delay_ms(100);
   for(int j=0;j<6;j++){
       setPixelColor(1+j*16, random_color[j]);
       setPixelColor(6+j*16, random_color[j]);
       setPixelColor(7+j*16, random_color[j]);
   }
    show();
   delay_ms(100);
   for(int j=0;j<6;j++){
       setPixelColor(1+j*16,0);
       setPixelColor(7+j*16,0);
      
       setPixelColor(2+j*16,random_color[j]);
       setPixelColor(5+j*16, random_color[j]);
       setPixelColor(8+j*16, random_color[j]);
       setPixelColor(9+j*16, random_color[j]);
       setPixelColor(10+j*16, random_color[j]);
   }
    show();
   delay_ms(100);
   for(int j=0;j<6;j++){
      setPixelColor(2+j*16, 0);
       setPixelColor(5+j*16,0);
       setPixelColor(8+j*16, 0);
       setPixelColor(9+j*16, 0);
       
       setPixelColor(3+j*16, random_color[j]);
       setPixelColor(4+j*16, random_color[j]);
       for(int i=11;i<16;i++)
        setPixelColor(i+j*16, random_color[j]);
   }
    show();
   delay_ms(200);
   
  
 }
void CUBE_MODE::cube_question_mark(){
   clear();
   randNumber = random(0, 6);
   for(int j=0;j<6;j++){
    for(int i=0;i<3;i++)
    {
      setPixelColor(i+j*16, color_array[randNumber%6]);
    }
    for(int i=7;i<11;i++)
    {
      setPixelColor(i+j*16, color_array[randNumber%6]);
    }
    setPixelColor(12+j*16, color_array[(randNumber+random(1,5))%6]);

    randNumber++;
   }
   show();   // Send the updated pixel colors to the hardware.

   delay_ms(1000);
  
  }
void CUBE_MODE::cube_do_all_mode()
{
    cube_led_switch_RGB();   //1
    if(mode_changed)
      return;
    for(int i=0;i<6;i++){
      cube_breathe_led_all(11);    //2
      if(mode_changed)
        return;
    }
    
    cube_rainbow(20);        //3
    if(mode_changed)
      return;
      
    for(int i=0;i<6;i++){         //4
      cube_theaterChase(color_array[i],50);
      if(mode_changed)
         return;
    }
    
    for(int i=0;i<6;i++){       //5
     cube_one_face_one_color_random();
    if(mode_changed)
      return;
    }
    
    cube_rainbowCycle(15);       //6
   if(mode_changed)
      return;
   
   for(int i=0;i<6;i++){ 
    cube_rectangle_random();   //7
    if(mode_changed)
      return;
   }
     
   

    cube_rectangle_color_flow(10); //8
    if(mode_changed)
      return;
    
   for(int i=0;i<6;i++){ 
      cube_arrow();        //9
    if(mode_changed)
      return;
   }
    
    cube_all_color_gradual_change(20);   //10
     if(mode_changed)
      return;
   for(int i=0;i<6;i++){ 
     cube_question_mark();        //11
   }

    //......
     
    //one_face_one_color();
}


void CUBE_MODE::music_mode0(uint16_t peakToPeak)
{
    int displayPeak = map(peakToPeak, 0, 1023, 0, 16);
    for(int j=0;j<6;j++){
        for(int i=0; i<displayPeak; i++) { // For each pixel...
            if(i<8)
                setPixelColor(i+16*j,Color(0,50,0));
            else if(i<12)
                setPixelColor(i+16*j,Color(50,50,0));
            else 
                setPixelColor(i+16*j,Color(50,0,0));
        
      }
    }
}

 void CUBE_MODE::music_mode1(uint16_t peakToPeak)
{
    int c = map(peakToPeak, 10, 1023, 0, 100);
     for(int j=0;j<NUMPIXELS;j++){      
        setPixelColor(j,Color(0,c,c));
        
    }      

}
 void CUBE_MODE::music_mode2(uint16_t peakToPeak){
    int c = map(peakToPeak, 10, 1023, 0, 128);
    // Serial.print("c=");
    // Serial.println(c);
    if(c<66){
        for(int j=0;j<NUMPIXELS;j++){      
            setPixelColor(j,Color(0,c,0));
        }
    }
    else if(c<88)
    {
        for(int j=0;j<NUMPIXELS;j++){      
            setPixelColor(j,Color(c,c,0));
        }
    }
    else
    {
        for(int j=0;j<NUMPIXELS;j++){      
            setPixelColor(j,Color(c,0,0));
        }
    }
}
 void CUBE_MODE::music_mode3(uint16_t peakToPeak)
{  
    int displayPeak = map(peakToPeak, 0, 1023, 0, 9);
    if(displayPeak==1)
    {
        setPixelColor(5,Color(0,100,0));
        setPixelColor(6,Color(0,100,0));
        setPixelColor(9,Color(0,100,0));
        setPixelColor(10,Color(0,100,0));
    }
    if(displayPeak>1)
    {
        for(int i=0;i<16;i++)
        {
            setPixelColor(i,Color(0,100,0));
        }
    }
    if(displayPeak>2)
    {
        int n=displayPeak-2;
        if(n>4) n=4;
        for(int i=16;i<16+4*n;i++)
        {
            setPixelColor(i,Color(0,100,0));
        }
        for(int i=63;i>=64-4*n;i--)
        {
            setPixelColor(i,Color(0,100,0));
        }
        for(int i=0;i<n;i++)
        {
            setPixelColor(64+i,Color(0,100,0));
            setPixelColor(71-i,Color(0,100,0));
            setPixelColor(72+i,Color(0,100,0));
            setPixelColor(79-i,Color(0,100,0));
        }
        for(int i=80;i<80+4*n;i++)
        {
            setPixelColor(i,Color(0,100,0));
        }
    }
    if(displayPeak>6)
    {
        for(int i=32;i<37;i++)
        {
            setPixelColor(i,Color(100,100,0));
        }
        setPixelColor(39,Color(100,100,0));
        setPixelColor(40,Color(100,100,0));
        for(int i=43;i<48;i++)
        {
            setPixelColor(i,Color(100,100,0));
        }         
    }
    if(displayPeak>7)
    {
        setPixelColor(37,Color(100,0,0));
        setPixelColor(38,Color(100,0,0));
        setPixelColor(41,Color(100,0,0));
        setPixelColor(42,Color(100,0,0));
    }      
}
 void CUBE_MODE::music_mode4(uint16_t peakToPeak)
{
    int c = map(peakToPeak, 10, 1023, 0, 127);
    //   Serial.print("c=");
    //   Serial.println(c);
      static int lastc;
     
    if(c>126&&lastc<90){
        for(int i=0;i<6;i++)
        {
            random_color[i]=Wheel(random(0,255));
        }
    }
      for(int j=0;j<6;j++)
        for(int i=0;i<16;i++)
          setPixelColor(i+j*16,random_color[j]);
      setBrightness(100);
       lastc=c;}

void CUBE_MODE::cube_music_mode(int mode)
{
    unsigned long startMillis= millis();
    uint16_t peakToPeak = 0; // peak-to-peak level
    uint16_t signalMax = 0;
    uint16_t signalMin = 1024;
    uint16_t sample;

    while (millis() - startMillis < ADC_SAMPLE_WINDOW)
    {

        sample = analogRead(0);
        // Serial.print("sample=");
        // Serial.println(sample);
        if (sample < 1024) // toss out spurious reading
        {
            sample > signalMax ? signalMax = sample:signalMin = sample;
            // if (sample > signalMax)        
            // {        
            //     signalMax = sample; // save just the max levels
            // }        
            // else if (sample < signalMin)        
            // {        
            // signalMin = sample; // save just the min levels       
            // }
        }
    }      
    peakToPeak = signalMax - signalMin;
    // Serial.print("peakToPeak=");
    // Serial.println(peakToPeak);

    switch(mode)
    {
        case 0:
            music_mode0(peakToPeak);break;
        case 1:
            music_mode1(peakToPeak);break;
        case 2:
            music_mode2(peakToPeak);break;
        case 3:
             music_mode3(peakToPeak);break;
        case 4:
             music_mode4(peakToPeak);break;
        default:
            break;
    }
   show();   // Send the updated pixel colors to the hardware.
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
//色轮
uint32_t CUBE_MODE::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
 //end of led mode
 /*
* fake nonblocking delay,make mode change as soon as possible
*
*/
extern void do_without_delay();

bool CUBE_MODE::delay_ms(uint16_t ms,bool mode_changed) 
{
    long time1;
    if(mode_changed)
        return false;   //如果要切换模式，就不延时了。
    if(ms<10)
    {
        delay(ms);
    }
    else
    {
        time1=millis();
        while((uint16_t)(millis()-time1)< ms)
        {
            if(mode_changed)
                return false;
            do_without_delay();      //do something fast tasks without delay
            yield();
        }
    }
    return true;
}

