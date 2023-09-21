#include <LCDWIKI_GUI.h>
#include <LCDWIKI_KBV.h>
#include <math.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define SCREEN_MARGINS 10
#define SMALL_TEXT_FONT 1
#define MEDIUM_TEXT_FONT 2
#define LARGE_TEXT_FONT 3
#define SMALL_TEXT_FONT_HEIGHT 8
#define MEDIUM_TEXT_FONT_HEIGHT 16
#define LARGE_TEXT_FONT_HEIGHT 26
#define BACKGROUND_COLOR 0xFFFF
#define TEXT_COLOR 0x0000
#define TEXT_SELECTED_COLOR 0xFFFFFF
#define TEXT_BACK_COLOR 0xFFFF
#define SELECTED_COLOR 0xFFFF
#define SELECTED_TEXT_COLOR 0xFFFF
#define DRAW_COLOR 0x0000

LCDWIKI_KBV lcd = LCDWIKI_KBV(ILI9488, A3, A2, A1, A0, A4);
float lastTheta = 0;
void initDisplay(){
    lcd.Init_LCD();
    lcd.Set_Rotation(3);
    lcd.Fill_Screen(BACKGROUND_COLOR);
    lcd.Set_Text_Back_colour(TEXT_BACK_COLOR);
    lcd.Set_Draw_color(DRAW_COLOR);
    drawText(25,SCREEN_MARGINS, "Diodo", TEXT_COLOR, LARGE_TEXT_FONT);

    
    for(int i=0; i <= 3 ;i++){
      int16_t x0 = (SCREEN_MARGINS * 1.5) + (MEDIUM_TEXT_FONT_HEIGHT / 2);
      int16_t y0 = SCREEN_MARGINS + LARGE_TEXT_FONT_HEIGHT * 1.5 + (MEDIUM_TEXT_FONT_HEIGHT * i);
      drawText(x0, y0, diodoList[i].getName(), TEXT_COLOR, MEDIUM_TEXT_FONT);
    }
    initIndicator(2.7 * SCREEN_WIDTH / 8, SCREEN_MARGINS , TEXT_COLOR,"Current");
    initIndicator(5.2 * SCREEN_WIDTH / 8, SCREEN_MARGINS , TEXT_COLOR,"Temperature");
}
void drawText(int16_t x0, int16_t y0, String text, uint16_t color, uint8_t font){
  lcd.Set_Text_colour(color);
  lcd.Set_Text_Size(font);
  lcd.Print_String(text,x0,y0);
}
void drawInterface(){
  drawDiodeSelector();
  if(currentState != System::DiodeSelecting){
    drawIndicator(2.7 * SCREEN_WIDTH / 8, SCREEN_MARGINS , TEXT_COLOR,"Current", currentValue, selectedDiode.getMinCurrentValue(), selectedDiode.getMaxCurrentValue());
    drawIndicator(5.2 * SCREEN_WIDTH / 8, SCREEN_MARGINS , TEXT_COLOR,"Temperature", tempValue, selectedDiode.getMinTempValue(), selectedDiode.getMaxTempValue());
  }
}
void drawDiodeSelector(){
  int16_t x0 = (SCREEN_MARGINS * 1.5) + (MEDIUM_TEXT_FONT_HEIGHT / 2);
  String last = selectedDiode.getName();

  for(int i=0; i <= 3 ;i++){
    int16_t y0 = SCREEN_MARGINS + LARGE_TEXT_FONT_HEIGHT * 1.5 + (MEDIUM_TEXT_FONT_HEIGHT * i);

    int16_t xa0 = SCREEN_MARGINS;
    int16_t x1 = xa0 + (MEDIUM_TEXT_FONT_HEIGHT / 2);
    int16_t y1 = y0 + (MEDIUM_TEXT_FONT_HEIGHT / 2);
    int16_t y2 = y0 + MEDIUM_TEXT_FONT_HEIGHT;

    if(diodoList[i].getName() == selectedDiode.getName())
      lcd.Fill_Triangle(xa0,y0,x1,y1,xa0,y2);
    else{
      lcd.Set_Draw_color(0xffff);
      lcd.Fill_Triangle(xa0,y0,x1,y1,xa0,y2);
      lcd.Set_Draw_color(DRAW_COLOR);
    }

  }
}
void initIndicator(int16_t x0, int16_t y0, uint16_t color, String title){
  int16_t r = (3 * SCREEN_WIDTH / 16) - (SCREEN_MARGINS * 2);
  int16_t x1 = x0 + r;
  int16_t y1 = y0 + 2*SCREEN_MARGINS+r;

  drawText(x0, y0, title, TEXT_COLOR, MEDIUM_TEXT_FONT);
  lcd.Fill_Circle(x1, y1, r);
  lcd.Set_Draw_color(0xFFFF);
  lcd.Fill_Circle(x1, y1, r-SCREEN_MARGINS);
  lcd.Set_Draw_color(DRAW_COLOR);

  drawText(x0, y0, title, TEXT_COLOR, MEDIUM_TEXT_FONT);
  if(currentState != System::DiodeSelecting){
    if(title == "Current")
      drawText(x0, y1 + r+2*SCREEN_MARGINS, String(currentValue), TEXT_COLOR, MEDIUM_TEXT_FONT);
    else
      drawText(x0, y1 + r+2*SCREEN_MARGINS, String(tempValue), TEXT_COLOR, MEDIUM_TEXT_FONT);
  }
}
void drawIndicator(int16_t x0, int16_t y0, uint16_t color, String title, int16_t value, float minv,float maxv){
  int16_t r = (3 * SCREEN_WIDTH / 16) - (SCREEN_MARGINS * 2);
  int16_t x1 = x0 + r;
  int16_t y1 = y0 + 2*SCREEN_MARGINS+r;
  float theta = (value-minv)/(maxv-minv)*PI;

  lcd.Set_Draw_color(0xFFFF);
  lcd.Fill_Circle(x1, y1, r-SCREEN_MARGINS);
  lcd.Set_Draw_color(DRAW_COLOR);
  lcd.Draw_Line(x1, y1, x1+r*cos(-theta), y1+r*sin(-theta));
  lastTheta = theta;

  if(currentState != System::DiodeSelecting){
    if(title == "Current")
      drawText(x0, y1 + r+2*SCREEN_MARGINS, String(currentValue), TEXT_COLOR, MEDIUM_TEXT_FONT);
    else
      drawText(x0, y1 + r+2*SCREEN_MARGINS, String(tempValue), TEXT_COLOR, MEDIUM_TEXT_FONT);
  }
}