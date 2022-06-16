/*
 * Simple interface to the Fly Sky IBus RC system.
 */

#include <inttypes.h>

class HardwareSerial;
class Stream;

class FlySkyIBus
{
public:
  void begin(HardwareSerial& serial); /* begin() function is used to retrieve a 
  bidirectional iterator pointing to the first element of the map, 
  an associative container which is of same type as of the map. 
  This function guarantees not to throw any exception and the returned iterator 
  object must not be dereferenced if the map is empty.*/
  void begin(Stream& stream); //the "Stream&"" means reference to Stream class
  void loop(void);
  uint16_t readChannel(uint8_t channelNr);

private:
  enum State
  {
    GET_LENGTH,
    GET_DATA,
    GET_CHKSUML,
    GET_CHKSUMH,
    DISCARD,
  };

  static const uint8_t PROTOCOL_LENGTH = 0x20;
  static const uint8_t PROTOCOL_OVERHEAD = 3; // <len><cmd><data....><chkl><chkh>
  static const uint8_t PROTOCOL_TIMEGAP = 3; // Packets are received very ~7ms so use ~half that for the gap
  static const uint8_t PROTOCOL_CHANNELS = 10;
  static const uint8_t PROTOCOL_COMMAND40 = 0x40; // Command is always 0x40

  uint8_t state;
  Stream* stream; // pointer
  uint32_t last;
  uint8_t buffer[PROTOCOL_LENGTH];
  uint8_t ptr;
  uint8_t len;
  uint16_t channel[PROTOCOL_CHANNELS];
  uint16_t chksum;
  uint8_t lchksum;
};

extern FlySkyIBus IBus; 