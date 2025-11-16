#ifndef VNFONT_FULL_VIETNAMESE_H
#define VNFONT_FULL_VIETNAMESE_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

// --- Glyph cơ sở (6x8) ---
struct Glyph {
  const uint8_t bitmap[8];
  uint8_t width;
};

// Base glyphs: chữ thường + chữ hoa + các chữ có thể ghép dấu
// Lưu ý: demo nhỏ, bạn có thể mở rộng đầy đủ 26 chữ + chữ tiếng Việt
const Glyph base_glyphs[] PROGMEM = {
  // Chữ thường
  {{0x00,0x70,0x08,0x78,0x88,0x88,0x78,0x00},6}, // a
  {{0xF0,0x88,0xF0,0x88,0x88,0xF0,0x00,0x00},6}, // b
  {{0x70,0x88,0x80,0x80,0x88,0x70,0x00,0x00},6}, // c
  {{0x70,0x88,0xF8,0x88,0x88,0x88,0xF8,0x00},6}, // i
  {{0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00},6}, // o
  {{0x38,0x44,0x44,0x44,0x44,0x38,0x00,0x00},6}, // u
  {{0x10,0x70,0x88,0xF8,0x88,0x88,0xF8,0x00},6}, // y
  {{0x30,0x10,0x7C,0x82,0x82,0x82,0x7C,0x00},6}, // đ
  {{0x28,0x38,0x10,0x78,0x84,0x84,0x7C,0x00},6}, // â
  {{0x28,0x38,0x10,0x78,0x84,0x84,0x7C,0x00},6}, // ă
  {{0x28,0x38,0x7C,0x80,0xF8,0x84,0x78,0x00},6}, // ê
  {{0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00},6}, // ô
  {{0x10,0x28,0x38,0x44,0x44,0x44,0x38,0x00},6}, // ơ
  {{0x00,0x00,0x84,0x84,0x84,0x8C,0x76,0x00},6}, // ư
  // Chữ hoa
  {{0x00,0x70,0x88,0xF8,0x88,0x88,0xF8,0x00},6}, // A
  {{0xF0,0x88,0xF0,0x88,0x88,0xF0,0x00,0x00},6}, // B
  {{0x70,0x88,0x80,0x80,0x88,0x70,0x00,0x00},6}, // C
  {{0x70,0x88,0xF8,0x88,0x88,0x88,0xF8,0x00},6}, // I
  {{0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00},6}, // O
  {{0x38,0x44,0x44,0x44,0x44,0x38,0x00,0x00},6}, // U
  {{0x10,0x70,0x88,0xF8,0x88,0x88,0xF8,0x00},6}, // Y
  {{0x30,0x10,0x7C,0x82,0x82,0x82,0x7C,0x00},6}, // Đ
  {{0x00,0x10,0x38,0x70,0x88,0xF8,0x88,0x00},6}, // Â
  {{0x00,0x28,0x38,0x70,0x88,0xF8,0x88,0x00},6}, // Ă
  {{0x00,0x28,0x38,0xF8,0x80,0xF8,0x88,0x00},6}, // Ê
  {{0x00,0x38,0x44,0x82,0x82,0xF8,0x44,0x00},6}, // Ô
  {{0x00,0x10,0x28,0x38,0x44,0xF8,0x44,0x00},6}, // Ơ
  {{0x00,0x00,0x84,0x84,0xF8,0x8C,0x76,0x00},6}, // Ư
};

// --- Dấu thanh (sắc, huyền, hỏi, ngã, nặng) ---
struct Accent {
  const uint8_t bitmap[3];
  int8_t x_offset;
  int8_t y_offset;
};

const Accent accents[] PROGMEM = {
  {{0x04,0x02,0x01}, 2, -2}, // sắc
  {{0x01,0x02,0x04}, 2, -2}, // huyền
  {{0x02,0x05,0x02}, 2, -2}, // hỏi
  {{0x05,0x02,0x05}, 2, -2}, // ngã
  {{0x04,0x04,0x04}, 2, -2}  // nặng
};

// --- Map UTF-8 → base + accent ---
struct VNChar {
  const char* utf8;
  uint8_t base_idx;
  int8_t accent_idx; // -1 nếu không có
};

VNChar vnchars[] = {
  // Chữ thường
  {"a",0,-1},{"á",0,0},{"à",0,1},{"ả",0,2},{"ã",0,3},{"ạ",0,4},
  {"i",3,-1},{"í",3,0},{"ì",3,1},{"ỉ",3,2},{"ĩ",3,3},{"ị",3,4},
  {"o",4,-1},{"ó",4,0},{"ò",4,1},{"ỏ",4,2},{"õ",4,3},{"ọ",4,4},
  {"u",5,-1},{"ú",5,0},{"ù",5,1},{"ủ",5,2},{"ũ",5,3},{"ụ",5,4},
  {"y",6,-1},{"ý",6,0},{"ỳ",6,1},{"ỷ",6,2},{"ỹ",6,3},{"ỵ",6,4},
  {"â",8,-1},{"ấ",8,0},{"ầ",8,1},{"ẩ",8,2},{"ẫ",8,3},{"ậ",8,4},
  {"ă",9,-1},{"ắ",9,0},{"ằ",9,1},{"ẳ",9,2},{"ẵ",9,3},{"ặ",9,4},
  {"ê",10,-1},{"ế",10,0},{"ề",10,1},{"ể",10,2},{"ễ",10,3},{"ệ",10,4},
  {"ô",11,-1},{"ố",11,0},{"ồ",11,1},{"ổ",11,2},{"ỗ",11,3},{"ộ",11,4},
  {"ơ",12,-1},{"ớ",12,0},{"ờ",12,1},{"ở",12,2},{"ỡ",12,3},{"ợ",12,4},
  {"ư",13,-1},{"ứ",13,0},{"ừ",13,1},{"ử",13,2},{"ữ",13,3},{"ự",13,4},
  {"đ",7,-1},
  // Chữ hoa
  {"A",14,-1},{"Á",14,0},{"À",14,1},{"Ả",14,2},{"Ã",14,3},{"Ạ",14,4},
  {"I",17,-1},{"Í",17,0},{"Ì",17,1},{"Ỉ",17,2},{"Ĩ",17,3},{"Ị",17,4},
  {"O",18,-1},{"Ó",18,0},{"Ò",18,1},{"Ỏ",18,2},{"Õ",18,3},{"Ọ",18,4},
  {"U",19,-1},{"Ú",19,0},{"Ù",19,1},{"Ủ",19,2},{"Ũ",19,3},{"Ụ",19,4},
  {"Y",20,-1},{"Ý",20,0},{"Ỳ",20,1},{"Ỷ",20,2},{"Ỹ",20,3},{"Ỵ",20,4},
  {"Â",21,-1},{"Ấ",21,0},{"Ầ",21,1},{"Ẩ",21,2},{"Ẫ",21,3},{"Ậ",21,4},
  {"Ă",22,-1},{"Ắ",22,0},{"Ằ",22,1},{"Ẳ",22,2},{"Ẵ",22,3},{"Ặ",22,4},
  {"Ê",23,-1},{"Ế",23,0},{"Ề",23,1},{"Ể",23,2},{"Ễ",23,3},{"Ệ",23,4},
  {"Ô",24,-1},{"Ố",24,0},{"Ồ",24,1},{"Ổ",24,2},{"Ỗ",24,3},{"Ộ",24,4},
  {"Ơ",25,-1},{"Ớ",25,0},{"Ờ",25,1},{"Ở",25,2},{"Ỡ",25,3},{"Ợ",25,4},
  {"Ư",26,-1},{"Ứ",26,0},{"Ừ",26,1},{"Ử",26,2},{"Ữ",26,3},{"Ự",26,4},
  {"Đ",16,-1}
};

int vn_count = sizeof(vnchars)/sizeof(VNChar);

// --- Lấy VNChar từ UTF-8 ---
const VNChar* getVNChar(const char* utf8){
  for(int i=0;i<vn_count;i++){
    if(strcmp(vnchars[i].utf8,utf8)==0) return &vnchars[i];
  }
  return nullptr;
}

// --- Vẽ glyph cơ sở ---
void drawGlyph(Adafruit_GFX *tft,int x,int y,const uint8_t* bmp,uint8_t w,uint16_t color,uint8_t scale){
  for(int r=0;r<8;r++)
    for(int c=0;c<w;c++)
      if(bmp[r] & (0x80>>c))
        tft->fillRect(x+c*scale,y+r*scale,scale,scale,color);
}

// --- Vẽ accent ---
void drawAccent(Adafruit_GFX *tft,int x,int y,const Accent* ac,uint16_t color,uint8_t scale){
  for(int r=0;r<3;r++){
    for(int c=0;c<3;c++){
      if(ac->bitmap[r] & (0x80>>c)){
        tft->fillRect(x + (c+ac->x_offset)*scale, y + (r+ac->y_offset)*scale, scale, scale, color);
      }
    }
  }
}

// --- In chuỗi UTF-8 ---
void printVN(Adafruit_GFX *tft,const char* s,uint16_t color,int x,int y,uint8_t scale=1){
  while(*s){
    char utf8[4]={0};
    utf8[0]=*s++;
    if((unsigned char)utf8[0]>=0xC2){utf8[1]=*s++; if((unsigned char)utf8[0]>=0xE0) utf8[2]=*s++;}

    const VNChar* vn = getVNChar(utf8);
    if(vn){
      drawGlyph(tft,x,y,base_glyphs[vn->base_idx].bitmap,base_glyphs[vn->base_idx].width,color,scale);
      if(vn->accent_idx>=0) drawAccent(tft,x,y,&accents[vn->accent_idx],color,scale);
    }else{
      tft->setCursor(x,y);
      tft->setTextColor(color);
      tft->setTextSize(scale);
      tft->print((char)utf8[0]);
    }
    x += 8*scale;
  }
}

#endif