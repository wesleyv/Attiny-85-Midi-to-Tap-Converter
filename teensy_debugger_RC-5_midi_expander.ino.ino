// MIDI to Momentary button press using ATTiny85
// NB: Use Sparkfun USB ATTiny85 Programmer
// Set Arduino env to USBTinyISP (Slow)
// Set to 8MHz Internal Clock (required for MIDI baud)

#include <SoftwareSerial.h>

#define MIDIRX 2
#define MIDISTATUS 11
#define MIDICH 9

#define AUXTIP 4
#define AUXRING 3
#define MAINSWITCH 0

#define TRIGPULSE 10000 // Width of the trigger pulse (uS)
SoftwareSerial midiSerial(MIDIRX, -1);

#define AUXTIPCC 88
#define AUXRINGCC 89
#define MAINSWITCHCC 90

uint32_t AUXTIPmicros;
uint32_t AUXRINGmicros;
uint32_t MAINSWITCHmicros;
uint32_t MIDISTATUSmicros;
int byteIgnoreCount = 0;
int byteActionCount = 0;
int SysexIgnoreCount = 0;
bool SysExIgnore = false;
byte dataByte1;
byte dataByte2;

void setup() {
  midiSerial.begin (31250); // MIDI Baud rate
  pinMode (MIDISTATUS, OUTPUT);
  pinMode (AUXTIP, OUTPUT);
  pinMode (AUXRING, OUTPUT);
  pinMode (MAINSWITCH, OUTPUT);
  AUXTIPOff();
  AUXRINGOff();
  MAINSWITCHOff();
  MIDISTATUSOff();
}

void loop() {
  if (SysexIgnoreCount > 10){
    SysexIgnoreCount = 0;
    SysExIgnore = false;
    Serial.println("Sysex Ignore RESET ");
  }
  if (midiSerial.available()) {
    byte incomingByte = midiSerial.read();
    byte StatusBit = incomingByte & 0b10000000;
    byte StatusTypeBits = incomingByte & 0b11110000;
    byte ChannelBits = incomingByte & 0b00001111;
    //    Serial.println("ParseMidi Called");
    Serial.println("incoming Byte = ");
    printBin(incomingByte);
    Serial.println("Sysex Ignore = ");
    Serial.println(SysExIgnore);
    //    Serial.println("");

    if (StatusBit == 0b10000000) {
      //Serial.println("");
      //Serial.println("++++++++++++++++++ ");
      //Serial.println("Status Bit YES statement entered");
      //Serial.println("---------- ");
      //Serial.println("Status Bit = ");
      //printBin(StatusBit);
      //Serial.println("---------- ");
      //Serial.println("StatusTypeBits = ");
      //printBin(StatusTypeBits);
      //Serial.println("");
      //Serial.println("---------- ");
      //Serial.println("ChannelBits = ");
      //printBin(ChannelBits + 1);
      //Serial.println("");
      //Serial.println(ChannelBits);
      //do this if it's a status byte)
      if (incomingByte == 0b11111111) {      //if Reset Message received,
        SysExIgnore = false;
        byteIgnoreCount = 0;
        byteActionCount = 0;
        SysexIgnoreCount = 0;
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Reset Message Received");
      }
      else if (incomingByte == 0b11110000) {       //if Begin SysEx byte received, set SysExIgnore to true
        SysExIgnore = true;
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Sysex ignore Begin");
        SysexIgnoreCount = 0;
      }
      else if (incomingByte == 0b11110111) {      //if End SysEx received,
        SysExIgnore = false;
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Sysex ignore End");
      }
      else if (SysExIgnore == true) {
        Serial.println("----------- ");
        Serial.println("Message ignored by sysex filter 1");
        SysexIgnoreCount++;
      }
      else if (incomingByte == 0b11110001) {      //if Sysex Time Code Quarter Frame received
        byteIgnoreCount++;
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Ignore count + 1 - Sysex time code");
      }
      else if (incomingByte == 0b11110010) {      //if Song position Pointer  received
        byteIgnoreCount = (byteIgnoreCount + 2);
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Ignore count + 2 - Song position pointer");
      }
      else if (incomingByte == 0b11110011) {      //if Song Select received
        byteIgnoreCount++;
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Ignore count + 1 - Song select");
      }
      else if (StatusTypeBits == 0b10000000 || StatusTypeBits == 0b10010000 || StatusTypeBits == 0b10100000 || StatusTypeBits == 0b11100000) {
        //checks if incoming byte is Note off, Note on, Poly Aftertouch, or Pitch Bend message, each of which has 2 data bytes which must be ignored
        byteIgnoreCount = (byteIgnoreCount + 2);
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Ignore count + 2 - Note on, note off, poly aftertouch, pitch bend");
      }
      else if (StatusTypeBits == 0b11000000 || StatusTypeBits == 0b11010000) {
        //checks if incoming byte is Program change or Channel Aftertouch, each of which has 1 data byte which must be ignored
        byteIgnoreCount++;
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Ignore count + 1 - program change or channel aftertouch");
      }
      else if (StatusTypeBits == 0b10110000) { // checks to see if the incoming byte is a Control Change
        if (ChannelBits == (MIDICH - 1)) { //checks to see if the incoming Control Change Byte is on the specified midi channel
          byteActionCount = 2;
          byteIgnoreCount = 0;
          Serial.println("");
          Serial.println("----------- ");
          Serial.println("byte action + 2 - CC recived on desired channel");
        }
        else if (ChannelBits != (MIDICH - 1)) { //Marks the next 2 data bytes to be ignored if the CC message is for the wrong channel
          byteIgnoreCount = (byteIgnoreCount + 2);
          Serial.println("");
          Serial.println("----------- ");
          Serial.println("Ignore count + 2 - CC received on wrong channel");
        }
      }
    }
    else if (StatusBit == 0b00000000 && SysExIgnore == false) {      //do this if it's a data byte and Sysex Ignore is not active,
      // Serial.println("");
      //Serial.println("++++++++++++++++++++++++++ ");
      //Serial.println("Status Bit NO statement entered");
      if (byteIgnoreCount > 0) {
        byteIgnoreCount--; //ignores this byte if a previous status message has indicated subsequent data bites should be ignored
        Serial.println("");
        Serial.println("----------- ");
        Serial.println("Data Byte ignored");
      }
      else if (byteIgnoreCount == 0) {
        if (byteActionCount == 2) {
          // Serial.println("");
          // Serial.println("----------- ");
          // Serial.println("incomingByte assigned to dataByte1:");
          dataByte1 = incomingByte; //if the previous CC status bytes was on the specified channel, this byte will be read as the CC number
          // Serial.println(dataByte1);
          byteActionCount--;
        }
        else if (byteActionCount == 1) {
          //Serial.println("incomingByte assigned to dataByte2:");
          dataByte2 = incomingByte;
          // Serial.println(dataByte2);
          byteActionCount--; //if the previous CC status bytes was on the specified channel, this byte will be read as the CC value
          DoMidiAction();
        }
      }
    }
    // get rif of me probably
    else if (StatusBit == 0b00000000 && SysExIgnore == true) {
      Serial.println("----------- ");
      Serial.println("Message ignored by sysex filter 2");
      SysexIgnoreCount++;
    }
    // ged rid of above till comment
    //    MIDISTATUSOn(); // DELETE ME MOVE ME <------------------------------------------------------------------------------------------------------------------------------------
  }
  AUXTIPOff();
  AUXRINGOff();
  MAINSWITCHOff();
  MIDISTATUSOff();
}

void printBin(byte aByte) {
  for (int8_t aBit = 7; aBit >= 0; aBit--)
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}

void DoMidiAction() {
  //Serial.println("++++++++++++++++++++++++++ ");
  //Serial.println("DoMidiAction Entered");
  //Serial.println("dataByte 1 is");
  //Serial.println(dataByte1);
  //Serial.println("Compare to");
  //Serial.println(AUXTIPCC);
  //Serial.println(AUXRINGCC);
  //Serial.println(MAINSWITCHCC);
  //Serial.println("dataByte 2 is");
  //Serial.println(dataByte2);
  if (dataByte1 == AUXTIPCC && dataByte2 == 0) {
    AUXTIPOn();
    //MIDISTATUSOn();
  }
  // Check for CC number 89 with a value of 0
  else if (dataByte1 == AUXRINGCC && dataByte2 == 0) {
    AUXRINGOn();
    //MIDISTATUSOn();
  }
  else if (dataByte1 == MAINSWITCHCC && dataByte2 == 0) {
    MAINSWITCHOn();
    //MIDISTATUSOn();
  }
}

void AUXTIPOn() {
  digitalWrite (AUXTIP, HIGH);
  AUXTIPmicros = micros() + TRIGPULSE;
  Serial.println("***************************************************************************************");
  Serial.println("Aux Tip Pulsed");
}

void AUXRINGOn() {
  digitalWrite (AUXRING, HIGH);
  AUXRINGmicros = micros() + TRIGPULSE;
  Serial.println("***************************************************************************************");
  Serial.println("Aux Ring Pulsed");
}

void MAINSWITCHOn() {
  digitalWrite (MAINSWITCH, HIGH);
  MAINSWITCHmicros = micros() + TRIGPULSE;
  Serial.println("***************************************************************************************");
  Serial.println("Main Switch Pulsed");
}

void MIDISTATUSOn() {
  digitalWrite (MIDISTATUS, HIGH);
  MIDISTATUSmicros = micros() + TRIGPULSE;
  //     Serial.println("status light on");
}

void MIDISTATUSOff() {
  // Turn off the trigger only after a short delay
  if (MIDISTATUSmicros < micros()) {
    digitalWrite (MIDISTATUS, LOW);
  }
}
void AUXTIPOff() {
  // Turn off the trigger only after a short delay
  if (AUXTIPmicros < micros()) {
    digitalWrite (AUXTIP, LOW);
  }
}

void AUXRINGOff() {
  // Turn off the trigger only after a short delay
  if (AUXRINGmicros < micros()) {
    digitalWrite (AUXRING, LOW);
  }
}

void MAINSWITCHOff() {
  // Turn off the trigger only after a short delay
  if (MAINSWITCHmicros < micros()) {
    digitalWrite (MAINSWITCH, LOW);
  }
}
