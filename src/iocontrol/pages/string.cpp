#include "string.hpp"

#include "ipage.hpp"
#include "../../data/helper.hpp"

U8X8_SSD1306_128X64_NONAME_HW_I2C DisplayHandler::u8x8(/*U8G2_R0,  clock= 8,  data= 7, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

void PrintableString::copyString(char* deststr, const char* srcstr)
{
    const size_t len = strlen(srcstr);
    strcpy(deststr, srcstr);
    deststr[len] = '\0';
}

void PrintableString::copyStringFromPRGMEM(char* deststr, const char* srcstr)
{
    const size_t len = strlen_P(srcstr);
    strcpy_P (deststr, (char*)srcstr);
    deststr[len] = '\0';
}

void PrintableString::setString(PrintableString& pstr, uint8_t posx, uint8_t posy, const char* srcstr, uint8_t metaflags)
{
    pstr.setMeta(posx, posy, metaflags);
    copyString(pstr.str, srcstr);
}

void PrintableString::setStringFromPRGMEM(PrintableString& pstr, uint8_t posx, uint8_t posy, const char* srcstr, uint8_t metaflags)
{
    pstr.setMeta(posx, posy, metaflags);
    copyStringFromPRGMEM(pstr.str, srcstr);
}


void PrintableString::insertNumber(PrintableString& pstr, uint16_t value)
{
    size_t strLen = strlen(pstr.str);
    uint8_t val_tmp = 0;
    while(strLen-- > 0)
    {
	if(pstr.str[strLen] >= 48 && pstr.str[strLen] <= 57){
		val_tmp = value%10;
		pstr.str[strLen] = (val_tmp>9 || val_tmp<0) ? '0' : val_tmp+48;
		value /= 10;
	}
    }
}

void PrintableString::setMeta(uint8_t posX, uint8_t posY, uint8_t metaFlags)
{
    this->posX = posX;
    this->posY = posY;
    this->metaFlags = metaFlags;//(metaFlags > 0) ? metaFlags : this->metaFlags;
}

void PrintableString::insertBinaryAddress(uint8_t address)
{
  uint8_t addrDigit = 5;
  while(addrDigit-- > 0)
  {
    str[addrDigit] = (address&1)+48;
    address >>= 1;
  }
}
// EO String

//DisplayHandler
void DisplayHandler::drawStringFromPRGMEM(const char* srcstr, uint8_t posX, uint8_t posY, bool format_big)
{
    char deststr[8];
    PrintableString::copyStringFromPRGMEM(deststr, srcstr);
    if(format_big){
	u8x8.draw2x2String(posX, posY, deststr);
    } else {
	u8x8.drawString(posX, posY, deststr);
    }
}

void DisplayHandler::draw(IPage* const page)
{    
  if(IPage::menulvl == MENU_AUTO){
    u8x8.drawString(13, 0, "<>>");
  }
  else if(IPage::menulvl == MENU_MANUAL){
    u8x8.drawString(13, 0, "<> ");
  }
  else{
    u8x8.drawString(13, 0, "   ");
  }
  
  PrintableString *strPtr = &(page->pStrings[0]);
  for(uint8_t u = 0; u < page->size_pstr; u++)
  {
    drawPString(strPtr++);
  }
}

void DisplayHandler::drawHLine(uint8_t row, uint8_t colbeg, uint8_t colend, char c){
  for(;colbeg <= colend; colbeg++){
    u8x8.drawGlyph(colbeg, row, c);
  }
}

void DisplayHandler::drawVLine(uint8_t col, uint8_t rowbeg, uint8_t rowend, char c){
  for(;rowbeg <= rowend; rowbeg++){
    u8x8.drawGlyph(col, rowbeg, c);
  }
}

void DisplayHandler::drawPString(PrintableString* const pstr)
{
  uint8_t mflag = pstr->metaFlags;
  
  if((mflag & PS_CLICKONLY || mflag & PS_SEL_LOCK) && mflag & PS_SELECTED){
    static uint8_t cnt = 0;
    if((++cnt) & 0b00000100){
		mflag |= PS_MARKED;
    }
  } else if (XOR(mflag & PS_SELECTED, mflag & PS_INVERTED)){
      mflag |= PS_MARKED;
  } else {
      mflag &= ~PS_MARKED;
  }
    
  if(mflag & PS_MARKED){
    u8x8.setInverseFont(true);
    mflag &= ~PS_MARKED;
  }
  
  if(mflag & PS_BIG){
    u8x8.draw2x2String(pstr->posX, pstr->posY, pstr->str);
  }
  else{
    u8x8.drawString(pstr->posX, pstr->posY, pstr->str);
  }
  u8x8.setInverseFont(false);
  pstr->metaFlags = mflag;
}
