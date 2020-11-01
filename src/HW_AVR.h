void Rtc::begin()
{
    pinMode(A1, OUTPUT);
}

void Rtc::_burstRead()
{
  _sendStart(DS3231_ADDR_W);
  _waitForAck();
  _writeByte(0);
  _waitForAck();
  _sendStart(DS3231_ADDR_R);
  _waitForAck();

  for (int i=0; i<7; i++)
  {
    _burstArray[i] = _readByte();
    if (i<6)
      _sendAck();
    else
      _sendNack();
  }
  _sendStop();
}

uint8_t Rtc::_readRegister(uint8_t reg)
{
   uint8_t readValue=0;
   
   _sendStart(DS3231_ADDR_W);
   _waitForAck();
   _writeByte(reg);
   _waitForAck();
   _sendStart(DS3231_ADDR_R);
   _waitForAck();
   readValue = _readByte();
   _sendNack();
   _sendStop();
   
   return readValue;
}

void Rtc::_writeRegister(uint8_t reg, uint8_t value)
{
  _sendStart(DS3231_ADDR_W);
  _waitForAck();
  _writeByte(reg);
  _waitForAck();
  _writeByte(value);
  _waitForAck();
  _sendStop();
}
