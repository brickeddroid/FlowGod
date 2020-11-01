#include "ipage.hpp"

#include "string.hpp"
#include "../data.hpp"

uint8_t IPage::menulvl = MENU_AUTO;

IPage::IPage(uint8_t dynelems, uint8_t sz_pstr, uint8_t segaddr)
  : lastCursorIndx(dynelems-1),
    currCursorIndx(0),
    prevCursorIndx(lastCursorIndx),
    segptr(&Data::pd_array[segaddr]),
    size_pstr(sz_pstr)
{
	pStrings = new PrintableString[size_pstr];
}

IPage::~IPage()
{
	delete[] pStrings;
	pStrings = NULL;
	DisplayHandler::u8x8.clearDisplay(); 
}

void IPage::unlockValue(){
    pStrings[currCursorIndx].metaFlags &= ~PS_SEL_LOCK;
}

void IPage::lockValue(){
    pStrings[currCursorIndx].metaFlags |= PS_SEL_LOCK;
}

void IPage::update()
{
    if(menulvl == MENU_PENTRY && prevCursorIndx != currCursorIndx){
		pStrings[prevCursorIndx].metaFlags &= ~PS_SELECTED;
		pStrings[currCursorIndx].metaFlags |= PS_SELECTED;
		prevCursorIndx = currCursorIndx;
    }
    this->updateContent();
}

void IPage::reset()
{
	pStrings[currCursorIndx].metaFlags &= ~(PS_SELECTED | PS_SEL_LOCK);
	prevCursorIndx = lastCursorIndx;
	currCursorIndx = 0;
}

bool IPage::isClickOnly()
{
    return (menulvl == MENU_PENTRY && pStrings[currCursorIndx].metaFlags & PS_CLICKONLY);
}
