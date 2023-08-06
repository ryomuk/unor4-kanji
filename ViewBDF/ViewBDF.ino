#include "Arduino_LED_Matrix.h"

#define MAX_X 12
#define MAX_Y  8

#include "font.h"

//#define CENTER_X (MAX_X/2-1)
//#define CENTER_Y (MAX_Y/2-1)
#define CENTER_X (MAX_X/2)
#define CENTER_Y (MAX_Y/2-1)

#define DELAY_ROTATE     2
#define DELAY_DISPLAY  250

#define STEP_ROTATE   3 /* degree */

ArduinoLEDMatrix matrix;

void  fonttobmp(const byte *font, byte (*bmp)[MAX_X]){
  for(int x = 0; x < MAX_X; x++){
    for(int y = 0; y < MAX_Y; y++){
      bmp[y][x] = (font[x] >> y) & 1;
    }
  }
}

void rotate_z(byte (*dst)[MAX_X], byte (*src)[MAX_X], double d){
  float sind, cosd;
  int rx, ry;
  
  sind = sin(d);
  cosd = cos(d);
  for(int x = 0; x < MAX_X; x++){
    for(int y = 0; y< MAX_Y; y++){
      dst[y][x] = 0;
    }
  }
  for(int x = 0; x < MAX_X; x++){
    for(int y = 0; y < MAX_Y; y++){
      if(src[y][x] == 1){
	rx = cosd*(x-CENTER_X) + sind*(y-CENTER_Y) + CENTER_X;
	ry = -sind*(x-CENTER_X) + cosd*(y-CENTER_Y) + CENTER_Y;
	if( 0<=rx && rx<MAX_X && 0<=ry && ry<MAX_Y){
	  dst[ry][rx] = 1;
	}
      }
    }
  }
}

void rotate_x(byte (*dst)[MAX_X], byte (*src)[MAX_X], double d){
  float cosd;
  int n;
  cosd = cos(d);
  for(int x = 0; x < MAX_X; x++){
    for(int y = 0; y< MAX_Y; y++){
      dst[y][x] = 0;
    }
  }
  for(int x = 0; x < MAX_X; x++){
    for(int y = 0; y < MAX_Y; y++){
      if(src[y][x] == 1){
	n = (int)round((y-CENTER_Y)*cosd + CENTER_Y);
	if(n >=0 && n < MAX_Y){
	  dst[n][x] = 1;
	}
      }
    }
  }
}

void rotate_y(byte (*dst)[MAX_X], byte (*src)[MAX_X], double d){
  int n;
  float cosd;
  cosd = cos(d);
  for(int x = 0; x < MAX_X; x++){
    for(int y = 0; y< MAX_Y; y++){
      dst[y][x] = 0;
    }
  }
  for(int x = 0; x < MAX_X; x++){
    for(int y = 0; y < MAX_Y; y++){
      if(src[y][x] == 1){
	n = (int)round((x-CENTER_X)*cosd + CENTER_X);
	if(n >=0 && n < MAX_X){
	  dst[y][n] = 1;
	}
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  matrix.begin();
}

void loop(){
  byte frame[MAX_Y][MAX_X];
  byte bmp[MAX_Y][MAX_X];
  word unistring[]={
    0x6559,0x5ba4,0x306e,0x7a93,0x304b,0x3089,0x898b,0x308b,0x79cb,0x306f,0x3044,0x3064,0x3082,0x4e0d,0x601d,0x8b70,0x306b,0x5149,0x3063,0x3066,0x305f,0x5317,0x5411,0x304d,0x306e,0x7a93,0x306e,0x3059,0x308a,0x30ac,0x30e9,0x30b9,0x30ae,0x30ea,0x30b7,0x30e3,0x306e,0x6d77,0x3082,0x898b,0x3048,0x305f, 32, 0
  };
  word *c;
  int axis_tbl[]={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0};
  int *axis;

  //#define FONTTEST
#ifdef FONTTEST
  while(Serial.available()){
    String s;
    int c;
    s = Serial.readStringUntil('\n');
    c = s.toInt();
    if(fonttbl[c] != 0){
      fonttobmp(font[fonttbl[c]], frame);
      matrix.renderBitmap(bmp, 8, 12);
      delay(DELAY_DISPLAY);
    }
  }
#endif

  for(c = unistring, axis=axis_tbl; *c != 0; c++, axis++){
    fonttobmp(font[fonttbl[*c]], bmp);
    for(int i = -180; i <= 0; i+=STEP_ROTATE){
      switch(*axis){
      case 0:	rotate_z(frame, bmp, 2.0 * PI * i /360); break;
      case 1:	rotate_y(frame, bmp, 2.0 * PI * i /360); break;
      case 2:	rotate_x(frame, bmp, 2.0 * PI * i /360); break;
      }
      matrix.renderBitmap(frame, 8, 12);
      delay(DELAY_ROTATE);
    }
    delay(DELAY_DISPLAY);
    for(int i = 0; i <= 180; i+=STEP_ROTATE){
      switch(*axis){
      case 0:	rotate_z(frame, bmp, 2.0 * PI * i /360); break;
      case 1:	rotate_y(frame, bmp, 2.0 * PI * i /360); break;
      case 2:	rotate_x(frame, bmp, 2.0 * PI * i /360); break;
      }
      matrix.renderBitmap(frame, 8, 12);
    }
  }
}
