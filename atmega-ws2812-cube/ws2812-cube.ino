/*
 * 1、cube不同的姿态led显示不同的模式
 * 2、icm20608判断cube姿态来决定显示哪种模式
 * 
 */
#include <Adafruit_NeoPixel.h>
#include <Adafruit_I2CDevice.h>

//ws2812b
#define PIN        4
#define NUMPIXELS 96
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 1000 
#define BRIGHTLESS 100

#define MODE_FACE_CHANGE_THRESHOLD 15900
#define MODE_POINT_CHANGE_THRESHOLD 8000

int mode=0,mode_changed=0;
uint32_t color_array[]={pixels.Color(BRIGHTLESS, 0, 0),pixels.Color(BRIGHTLESS, BRIGHTLESS, 0),pixels.Color(0, BRIGHTLESS, 0),pixels.Color(0, BRIGHTLESS,BRIGHTLESS),pixels.Color(0, 0,BRIGHTLESS),pixels.Color(BRIGHTLESS, 0,BRIGHTLESS)};
int color_num=0;
long randNumber;
uint32_t random_color[6];

//icm20608
#define I2C_ADDRESS 0x69
Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(I2C_ADDRESS);
int ax,ay,az,gx,gy,gz;
int data_ready=0;

//max4466 mic
const int sampleWindow = 50; 
const int maxScale = 16;
int sample;
unsigned long startMillis;
int peakToPeak = 0; // peak-to-peak level
int signalMax = 0;
int signalMin = 1024;

//eint
int int_count=0;  

void setup() {
  // put your setup code here, to run once:
 
  pixels.begin();
  //
  analogReference(EXTERNAL);
  //gpio
  pinMode(A2,OUTPUT);   //cs set high， iic模式
  digitalWrite(A2,HIGH);  
  pinMode(A3,OUTPUT);   //ad0 set high as i2c addr is 0x69
  digitalWrite(A3,HIGH);
  //serial
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  //init iic
   if (!i2c_dev.begin()) {
    Serial.print("Did not find device at 0x");
    Serial.println(i2c_dev.address(), HEX);
  }
  else{
   // i2c_dev.setSpeed(100000);
    Serial.print("Device found on address 0x");
    Serial.println(i2c_dev.address(), HEX);
  }
  //icm20608
  init_icm20608();
  attachInterrupt(digitalPinToInterrupt(2), icm20608_int_cb, RISING);
  while(1)
  {
     do_without_delay();
     if(ax!=0)
      break;
  }
   
  randomSeed(ax%100);
  Serial.println(ax);
   pixels.clear();
   pixels.show();  
  for(int i=0;i<6;i++)
  {
    random_color[i]=Wheel(random(0,255));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.clear();
  do_without_delay();
  mode_changed=0;
  Serial.println(mode);
  pixels.setBrightness(BRIGHTLESS);
  
  switch(mode)
  {
    case 0:
     // led_switch_RGB();      
     music_mode(2);
      break;
    case 1:
      breathe_led_all(10);
   //   color_num=((color_num++)%(sizeof(color_array)/sizeof(color_array[0])));   //知道错在哪里吗？
   //   color_num%=(sizeof(color_array)/sizeof(color_array[0]));
    //  Serial.println(color_num);
   //   Serial.println(sizeof(color_array)/sizeof(color_array[0]));
      break;
    case 2:
      music_mode(0);
      break;
    case 3:
       //
      rainbowCycle(15);
      break;
    case 4:
      theaterChase(Wheel(random(0,255)),50);
      break;
    case 5:
      one_face_one_color_fixed();
      break;
    case 6:
      one_face_one_color_random();
      break;
    case 7:
       rectangle_random();
      break;
    case 8:
      rectangle_color_flow(15);
      break;
    case 9:
      face_color_flow(30);
      break;
    case 10:          //usb point do all mode
      do_all_mode();
     // music_mode(0);
      break;
    case 11:
      all_color_gradual_change(50);
      break;
    case 12:
      arrow();
      break;
    case 13:
      question_mark();
      break;
    default:
      break;
  }
}
//led mode
void led_switch_RGB()
{
  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(0, 0, 50));
  }
   pixels.show();   // Send the updated pixel colors to the hardware.
   delay_ms(DELAYVAL); // Pause before next pass through loop
    for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(0, 50, 0));

  }
   pixels.show();   // Send the updated pixel colors to the hardware.
    delay_ms(DELAYVAL); // Pause before next pass through loop
    for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(50, 0, 0));
  }
   pixels.show();   // Send the updated pixel colors to the hardware.
   delay_ms(DELAYVAL); // Pause before next pass through loop
}
void breathe_led_all(int wait){

     uint32_t rand_color=Wheel(random(0,255));

    for(int i=0; i<BRIGHTLESS; i++) { // For each pixel in pixels... 
      for(int n=0;n<NUMPIXELS;n++){
        pixels.setPixelColor(n, rand_color);         //  Set pixel's color (in RAM)
      }
      pixels.setBrightness(i);
      pixels.show();                          //  Update pixels to match
      delay_ms(wait);  
    }  
     for(int i=BRIGHTLESS; i>0; i--) { // For each pixel in pixels...
       // pixels.setPixelColor(n, color);         //  Set pixel's color (in RAM)
       for(int n=0;n<NUMPIXELS;n++){
        pixels.setPixelColor(n, rand_color);         //  Set pixel's color (in RAM)
      }
         pixels.setBrightness(i);
         pixels.show();                          //  Update pixels to match
        delay_ms(wait);  
    }
   // pixels.setBrightness(BRIGHTLESS);  
 }
void breathe_led_single(int pixel,uint32_t color,int wait)
{
   for(int i=0; i<BRIGHTLESS; i++) { // For each pixel in pixels... 
    
      pixels.setPixelColor(pixel, color);         //  Set pixel's color (in RAM)
      pixels.setBrightness(i);
      pixels.show();                          //  Update pixels to match
      delay_ms(wait);  
    }  
   for(int i=BRIGHTLESS; i>0; i--) { // For each pixel in pixels...
       // pixels.setPixelColor(n, color);         //  Set pixel's color (in RAM)
        pixels.setPixelColor(pixel, color);         //  Set pixel's color (in RAM)
         pixels.setBrightness(i);
         pixels.show();                          //  Update pixels to match
        delay_ms(wait);  
    }
  //  pixels.setBrightness(BRIGHTLESS);  
}
void rainbow(uint8_t wait) {
  uint16_t i, j;
  pixels.setBrightness(BRIGHTLESS);
  for(j=0; j<256; j++) {
    for(i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    if(!delay_ms(wait))
    {
      return; 
     }
 }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  pixels.setBrightness(BRIGHTLESS);
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    if(!delay_ms(wait))
    {
      return;
      }
  }
}
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<20; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixels.show();

      delay_ms(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void one_face_one_color_fixed()
{
  for(int j=0;j<6;j++){
    for(int i=0; i<16; i++) { 
      pixels.setPixelColor(i+j*16, color_array[j]);
    }
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay_ms(1000);
}

void one_face_one_color_random()
{  
  for(int i=0;i<6;i++)
  {
      random_color[i]=Wheel(random(0,255));
  }
  for(int j=0;j<6;j++){
    for(int i=0; i<16; i++) { 
      pixels.setPixelColor(i+j*16, random_color[j]);
    }
    randNumber++;
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay_ms(1000);
}

void rectangle_random(){
   pixels.clear();
  for(int i=0;i<6;i++)
  {
      random_color[i]=Wheel(random(0,255));
  }
   for(int j=0;j<6;j++){
    for(int i=0;i<5;i++)
    {
      pixels.setPixelColor(i+j*16, random_color[j]);
    }
    pixels.setPixelColor(7+j*16,random_color[j]);
    pixels.setPixelColor(8+j*16,random_color[j]);
    for(int i=11;i<16;i++)
    {
      pixels.setPixelColor(i+j*16, random_color[j]);
    }
    randNumber++;
   }
   pixels.show();   // Send the updated pixel colors to the hardware.
   delay_ms(1000);
}

void rectangle_color_flow(int wait){
  int i;
     pixels.clear();
    for(int j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  
      for(int n=0;n<6;n++){
        for( i=0; i< 5; i++) {
          pixels.setPixelColor(i+n*16, Wheel(((i * 256 / 12) + j) & 255));
        }
        for(int k=11;k<16;k++,i++)
        {
           pixels.setPixelColor(k+n*16, Wheel(((i * 256 / 12) + j) & 255));
        }
        pixels.setPixelColor(8+n*16, Wheel(((i++ * 256 / 12) + j) & 255));
        pixels.setPixelColor(7+n*16, Wheel(((i * 256 / 12) + j) & 255));
      }
      pixels.show();
      if(!delay_ms(wait))
      {
        return;
      }
    }
}

#define COLOR_STEP 8
void face_color_flow(int wait){
  int i;
  
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
  
      for(int n=0;n<6;n++){
        for( i=0; i< 5; i++) {
          pixels.setPixelColor(i+n*16, Wheel((i*COLOR_STEP+ j) & 255));
        }
        for(int k=11;k<16;k++,i++)
        {
           pixels.setPixelColor(k+n*16, Wheel((i*COLOR_STEP + j) & 255));
        }
        for(int k=8;k>4;k--,i++)
        {
           pixels.setPixelColor(k+n*16, Wheel((i*COLOR_STEP + j) & 255));
        }
        pixels.setPixelColor(10+n*16, Wheel((i++*COLOR_STEP + j) & 255));
        pixels.setPixelColor(9+n*16, Wheel((i*COLOR_STEP + j) & 255));
      }
      pixels.show();
      if(!delay_ms(wait))
      {
        return;
      }
    }
}

void all_color_gradual_change(int wait){
    uint16_t i, j;
  pixels.setBrightness(BRIGHTLESS);
  for(j=0; j<256; j++) {
    for(i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, Wheel((j) & 255));
    }
    pixels.show();
    if(!delay_ms(wait))
    {
      return; 
     }
 }
}

void one_by_one_circle(int wait){
        for(int i=0; i< 5; i++) {
           for(int n=0;n<6;n++){
            pixels.setPixelColor(i+n*16,color_array[n]);
           }
            pixels.show();
            if(!delay_ms(wait))
            {
              return; 
             }
        }
        for(int k=11;k<16;k++)
        {
           for(int n=0;n<6;n++){
            pixels.setPixelColor(k+n*16, color_array[n]);
           }
             pixels.show();
          if(!delay_ms(wait))
          {
            return; 
           }
        }
        for(int k=8;k>4;k--)
        {
           for(int n=0;n<6;n++){
            pixels.setPixelColor(k+n*16,color_array[n]);
           }
             pixels.show();
          if(!delay_ms(wait))
          {
            return; 
           }
        }
       for(int n=0;n<6;n++){
         pixels.setPixelColor(10+n*16, color_array[n]);
       }
          pixels.show();
          if(!delay_ms(wait))
          {
            return; 
           }
       for(int n=0;n<6;n++){
          pixels.setPixelColor(9+n*16, color_array[n]);
       }
          pixels.show();
          if(!delay_ms(wait))
          {
            return; 
           }
  }
void arrow(){
   pixels.clear();
  for(int i=0;i<6;i++)
  {
      random_color[i]=Wheel(random(0,255));
  }
   delay_ms(200);
   for(int j=0;j<6;j++){
       pixels.setPixelColor(0+j*16,random_color[j]);
   }
    pixels.show();
   delay_ms(100);
   for(int j=0;j<6;j++){
       pixels.setPixelColor(1+j*16, random_color[j]);
       pixels.setPixelColor(6+j*16, random_color[j]);
       pixels.setPixelColor(7+j*16, random_color[j]);
   }
    pixels.show();
   delay_ms(100);
   for(int j=0;j<6;j++){
       pixels.setPixelColor(1+j*16,0);
       pixels.setPixelColor(7+j*16,0);
      
       pixels.setPixelColor(2+j*16,random_color[j]);
       pixels.setPixelColor(5+j*16, random_color[j]);
       pixels.setPixelColor(8+j*16, random_color[j]);
       pixels.setPixelColor(9+j*16, random_color[j]);
       pixels.setPixelColor(10+j*16, random_color[j]);
   }
    pixels.show();
   delay_ms(100);
   for(int j=0;j<6;j++){
      pixels.setPixelColor(2+j*16, 0);
       pixels.setPixelColor(5+j*16,0);
       pixels.setPixelColor(8+j*16, 0);
       pixels.setPixelColor(9+j*16, 0);
       
       pixels.setPixelColor(3+j*16, random_color[j]);
       pixels.setPixelColor(4+j*16, random_color[j]);
       for(int i=11;i<16;i++)
        pixels.setPixelColor(i+j*16, random_color[j]);
   }
    pixels.show();
   delay_ms(200);
   
  
 }
void question_mark(){
   pixels.clear();
   randNumber = random(0, 6);
   for(int j=0;j<6;j++){
    for(int i=0;i<3;i++)
    {
      pixels.setPixelColor(i+j*16, color_array[randNumber%6]);
    }
    for(int i=7;i<11;i++)
    {
      pixels.setPixelColor(i+j*16, color_array[randNumber%6]);
    }
    pixels.setPixelColor(12+j*16, color_array[(randNumber+random(1,5))%6]);

    randNumber++;
   }
   pixels.show();   // Send the updated pixel colors to the hardware.

   delay_ms(1000);
  
  }
void do_all_mode()
{
    led_switch_RGB();   //1
    if(mode_changed)
      return;
    for(int i=0;i<6;i++){
      breathe_led_all(11);    //2
      if(mode_changed)
        return;
    }
    
    rainbow(20);        //3
    if(mode_changed)
      return;
      
    for(int i=0;i<6;i++){         //4
      theaterChase(color_array[i],50);
      if(mode_changed)
         return;
    }
    
    for(int i=0;i<6;i++){       //5
      one_face_one_color_random();
    if(mode_changed)
      return;
    }
    
    rainbowCycle(15);       //6
   if(mode_changed)
      return;
   
   for(int i=0;i<6;i++){ 
    rectangle_random();   //7
    if(mode_changed)
      return;
   }
     
   

    rectangle_color_flow(10); //8
    if(mode_changed)
      return;
    
   for(int i=0;i<6;i++){ 
      arrow();        //9
    if(mode_changed)
      return;
   }
    
    all_color_gradual_change(20);   //10
     if(mode_changed)
      return;
   for(int i=0;i<6;i++){ 
     question_mark();        //11
   }

    //......
     
    //one_face_one_color();
}
void music_mode(int mode)
{
   startMillis= millis();
   peakToPeak = 0; // peak-to-peak level
   signalMax = 0;
   signalMin = 1024;
    while (millis() - startMillis < sampleWindow)
  {

      sample = analogRead(0);
     Serial.print("sample=");
    Serial.println(sample);
     // sample = analogRead(1);
    if (sample < 1024) // toss out spurious reading
    {
        if (sample > signalMax)        
        {        
          signalMax = sample; // save just the max levels
        }        
        else if (sample < signalMin)        
        {        
           signalMin = sample; // save just the min levels       
        }
    }
  }
  peakToPeak = signalMax - signalMin;
  
   Serial.print("peakToPeak=");
    Serial.println(peakToPeak);
  if(mode==0){
    int displayPeak = map(peakToPeak, 0, 1023, 0, maxScale);
    for(int j=0;j<6;j++){
      for(int i=0; i<displayPeak; i++) { // For each pixel...
    
        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        // Here we're using a moderately bright green color:
        if(i<8)
          pixels.setPixelColor(i+16*j,pixels.Color(0,50,0));
         else if(i<12)
           pixels.setPixelColor(i+16*j,pixels.Color(50,50,0));
         else 
           pixels.setPixelColor(i+16*j,pixels.Color(50,0,0));
    
      }
    }
  }
  else if(mode ==1)
  {
    int c = map(peakToPeak, 10, 1023, 0, 100);
     for(int j=0;j<NUMPIXELS;j++){      
        pixels.setPixelColor(j,pixels.Color(0,c,c));
        
       }      
  }
  else if(mode == 2)
  {
    int c = map(peakToPeak, 10, 1023, 0, 128);
     Serial.print("c=");
    Serial.println(c);
     if(c<66){
       for(int j=0;j<NUMPIXELS;j++){      
        pixels.setPixelColor(j,pixels.Color(0,c,0));
       }
     }
     else if(c<88)
     {
       for(int j=0;j<NUMPIXELS;j++){      
        pixels.setPixelColor(j,pixels.Color(c,c,0));
       }
      }
      else
      {
         for(int j=0;j<NUMPIXELS;j++){      
          pixels.setPixelColor(j,pixels.Color(c,0,0));
         }
      }
    }
    else if(mode == 3)
    {
      int displayPeak = map(peakToPeak, 0, 1023, 0, 9);
      if(displayPeak==1)
      {
          pixels.setPixelColor(5,pixels.Color(0,100,0));
          pixels.setPixelColor(6,pixels.Color(0,100,0));
          pixels.setPixelColor(9,pixels.Color(0,100,0));
          pixels.setPixelColor(10,pixels.Color(0,100,0));
          
      }
     if(displayPeak>1)
     {
        for(int i=0;i<16;i++)
        {
           pixels.setPixelColor(i,pixels.Color(0,100,0));
          }
      }
      if(displayPeak>2)
      {
          int n=displayPeak-2;
          if(n>4) n=4;
         for(int i=16;i<16+4*n;i++)
        {
           pixels.setPixelColor(i,pixels.Color(0,100,0));
         }
         for(int i=63;i>=64-4*n;i--)
        {
           pixels.setPixelColor(i,pixels.Color(0,100,0));
         }
         for(int i=0;i<n;i++)
         {
           pixels.setPixelColor(64+i,pixels.Color(0,100,0));
           pixels.setPixelColor(71-i,pixels.Color(0,100,0));
           pixels.setPixelColor(72+i,pixels.Color(0,100,0));
           pixels.setPixelColor(79-i,pixels.Color(0,100,0));
         }
        for(int i=80;i<80+4*n;i++)
        {
           pixels.setPixelColor(i,pixels.Color(0,100,0));
        }
      }
      if(displayPeak>6)
      {
         for(int i=32;i<37;i++)
         {
           pixels.setPixelColor(i,pixels.Color(100,100,0));
         }
         pixels.setPixelColor(39,pixels.Color(100,100,0));
         pixels.setPixelColor(40,pixels.Color(100,100,0));
         for(int i=43;i<48;i++)
         {
           pixels.setPixelColor(i,pixels.Color(100,100,0));
         }         
      }
      if(displayPeak>7)
      {
        pixels.setPixelColor(37,pixels.Color(100,0,0));
        pixels.setPixelColor(38,pixels.Color(100,0,0));
        pixels.setPixelColor(41,pixels.Color(100,0,0));
        pixels.setPixelColor(42,pixels.Color(100,0,0));
      }      
    }
    else if(4==mode)                                                              
    {
      int c = map(peakToPeak, 10, 1023, 0, 127);
      Serial.print("c=");
      Serial.println(c);
      static int lastc;
     
      if(c>126&&lastc<90){
        for(int i=0;i<6;i++)
        {
            random_color[i]=Wheel(random(0,255));
        }
      }
      for(int j=0;j<6;j++)
        for(int i=0;i<16;i++)
          pixels.setPixelColor(i+j*16,random_color[j]);
      pixels.setBrightness(100);
       lastc=c;
    }
   pixels.show();   // Send the updated pixel colors to the hardware.
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
 //end of led mode
void do_without_delay()
{
    
  if(data_ready)
  {
    get_accel_raw(&ax,&ay,&az);
    get_gyro_raw(&gx,&gy,&gz);
    if(!mode_changed)
    {
      cube_6_face();
      cube_8_point();
    }
    Serial.print("ax=");Serial.print(ax);
    Serial.print(" ay=");Serial.print(ay);
    Serial.print(" az=");Serial.print(az);
  
    Serial.print(" gx=");Serial.print(gx);
    Serial.print(" gy=");Serial.print(gy);
    Serial.print(" gz=");Serial.println(gz);
    data_ready=0;
    //get_fifo_raw();
  }
}
void cube_6_face()
{
      if(ax>MODE_FACE_CHANGE_THRESHOLD){
        
        if(mode!=1)
        {
          mode=1;
          mode_changed=1;
        }
      }
      else if(ay>MODE_FACE_CHANGE_THRESHOLD)
      {
        if(mode!=2)
        {
          mode=2;
          mode_changed=1;
        }
      }
      else if(az>MODE_FACE_CHANGE_THRESHOLD)
      {
        if(mode!=3)
        {
          mode=3;
          mode_changed=1;
        }
      }
       else if(ax<-MODE_FACE_CHANGE_THRESHOLD)
      {
        if(mode!=4)
        {
          mode=4;
          mode_changed=1;
        }
      }
      else if(ay<-MODE_FACE_CHANGE_THRESHOLD)
      {
        if(mode!=5)
        {
          mode=5;
          mode_changed=1;
        }
      }
      else if(az<-MODE_FACE_CHANGE_THRESHOLD)
      {
        if(mode!=0)
        {
          mode=0;
          mode_changed=1;
        }
      }
  }
void cube_8_point()
{
    if(ax>MODE_POINT_CHANGE_THRESHOLD&&ay>MODE_POINT_CHANGE_THRESHOLD&&az>MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=6)
      {
        mode=6;
        mode_changed=1;
      }
    } 
    else if(ax>MODE_POINT_CHANGE_THRESHOLD&&ay>MODE_POINT_CHANGE_THRESHOLD&&az<-MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=7)
      {
        mode=7;
        mode_changed=1;
      }
    } 
     else if(ax>MODE_POINT_CHANGE_THRESHOLD&&ay<-MODE_POINT_CHANGE_THRESHOLD&&az>MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=8)
      {
        mode=8;
        mode_changed=1;
      }
    } 
     else if(ax>MODE_POINT_CHANGE_THRESHOLD&&ay<-MODE_POINT_CHANGE_THRESHOLD&&az<-MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=9)
      {
        mode=9;
        mode_changed=1;
      }
    } 
     else if(ax<-MODE_POINT_CHANGE_THRESHOLD&&ay>MODE_POINT_CHANGE_THRESHOLD&&az>MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=10)
      {
        mode=10;
        mode_changed=1;
      }
    } 
     else if(ax<-MODE_POINT_CHANGE_THRESHOLD&&ay>MODE_POINT_CHANGE_THRESHOLD&&az<-MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=11)
      {
        mode=11;
        mode_changed=1;
      }
    } 
     else if(ax<-MODE_POINT_CHANGE_THRESHOLD&&ay<-MODE_POINT_CHANGE_THRESHOLD&&az>MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=12)
      {
        mode=12;
        mode_changed=1;
      }
    } 
     else if(ax<-MODE_POINT_CHANGE_THRESHOLD&&ay<-MODE_POINT_CHANGE_THRESHOLD&&az<-MODE_POINT_CHANGE_THRESHOLD)
    {
      if(mode!=13)
      {
        mode=13;
        mode_changed=1;
      }
    } 
}
bool delay_ms(int ms)
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
    while(millis()-time1<ms)
    {
      if(mode_changed)
        return false;
      do_without_delay();      
    }
   }
  return true;
}

void write_reg(uint8_t reg,uint8_t val)
{
    uint8_t buffer[2];
  buffer[0]=reg;
  buffer[1]=val;
  i2c_dev.write(buffer, 2,true);
}

void init_icm20608()
{
  uint8_t buffer[2];
  buffer[0]=0x6b;
  buffer[1]=0x80;
  i2c_dev.write(buffer, 2,true);    //reset
  delay(100);
  buffer[0]=0x6b;
  buffer[1]=0x00;
  i2c_dev.write(buffer, 2,true);    //wake up
  buffer[0]=0x6b;
  i2c_dev.write_then_read(buffer, 1, buffer, 1, true);
  if((buffer[0]&0x40)!=0)
  {
    Serial.println("icm20608 wake up failed! ");
  }
  //sample rate div
  buffer[0]=0x19;
  buffer[1]=0xff;
  i2c_dev.write(buffer, 2,true);
  buffer[0]=0x1a;
  buffer[1]=0x06;
  i2c_dev.write(buffer, 2,true);
  //int
  buffer[0]=0x37;
  buffer[1]=0x00;
  i2c_dev.write(buffer, 2,true);
  buffer[0]=0x38;
  buffer[1]=0x11;
  i2c_dev.write(buffer, 2,true);
  //low power mode
  buffer[0]=0x1e;
  buffer[1]=0x03;
  //i2c_dev.write(buffer, 2,true);
  //fifo enable
  buffer[0]=0x23;
  buffer[1]=0x08;
  i2c_dev.write(buffer, 2,true);
  buffer[0]=0x6a;
  buffer[1]=0x40;
  i2c_dev.write(buffer, 2,true);
  //acc_config
  //wake_on_motion_init();
  //gyro_config
}

void get_accel_raw(int *ax,int *ay,int *az)
{
    uint8_t buffer[6]={0x3b};
   i2c_dev.write_then_read(buffer, 1, buffer, 6, true);
   *ax=(buffer[0]<<8|buffer[1]);
   *ay=(buffer[2]<<8|buffer[3]);
   *az=(buffer[4]<<8|buffer[5]);
 //  Serial.print("get raw");
 //  Serial.println(*ax);
}


void get_gyro_raw(int *gx,int *gy,int *gz)
{
    uint8_t buffer[6]={0x43};
   i2c_dev.write_then_read(buffer, 1, buffer, 6, true);
   *gx=(buffer[0]<<8|buffer[1]);
   *gy=(buffer[2]<<8|buffer[3]);
   *gz=(buffer[4]<<8|buffer[5]);
}

void get_fifo_raw()
{
   uint8_t buffer[6]={0x72};
   i2c_dev.write_then_read(buffer, 1, buffer, 2, true);
   int fifo_cnt=(buffer[0]<<8)&0x0f|buffer[1];
   Serial.print("fifo");
   Serial.println(fifo_cnt);
//   uint8_t buffer[6]={0x74};
//   i2c_dev.write_then_read(buffer, 1, buffer, 6, true);
}
void wake_on_motion_init()
{
    write_reg(0x6b,0);      //wake up
    write_reg(0x6c,0x07);   //enable acc x y z
    write_reg(0x1d,0x1);    //
    write_reg(0x38,0xe0);   //enable wom
    write_reg(0x1f,64);   //threshold
    write_reg(0x69,0xc0); //enable acc intel
    write_reg(0x1e,0x09); //freq wake up set
    write_reg(0x6b,0x20);  //enable cycle mode
}

void icm20608_int_cb(){
    int_count++;
   Serial.println(int_count);  
    data_ready=1;
}
