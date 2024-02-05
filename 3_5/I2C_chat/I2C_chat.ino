#include <Wire.h>

int messFrom = 0;
unsigned char id = 1;
int lastSenderAddress = -1;

struct AddressTable {
  char name;
  int address;
};

AddressTable addressTable[] = {
  {'B', 1},
  {'R', 2},
  {'M', 3}
};

void setup() {
  Serial.begin(9600);
  Wire.begin(id);
  Wire.onReceive(receiveEvent);
}

unsigned long t = millis();

void loop() {
  if (Serial.available()) {
    char inputChar = Serial.read();
    
    if (inputChar == 'a') {
      if (lastSenderAddress != -1) {
        Serial.print("Responding to the last sender\n");
        Serial.print("Message: ");

        Wire.beginTransmission(lastSenderAddress);
        delay(100);

        Wire.write(id);
        while (Serial.available()) {
          char c = Serial.read();
          Serial.write(c);
          Wire.write(c);
        }

        Wire.write('.');

        Wire.endTransmission();
        Serial.write("\nMessage delivered as a response.\n");
      } else {
        Serial.println("No previous sender to respond to.\n");
      }
    } else {
      int recipientAddress = -1;
      for (int i = 0; i < sizeof(addressTable) / sizeof(addressTable[0]); i++) {
        if (addressTable[i].name == inputChar) {
          recipientAddress = addressTable[i].address;
          break;
        }
      }

      if (recipientAddress != -1) {
        Serial.print("Addressee's name: ");
        Serial.write(inputChar);
        Serial.write("\n");
        Serial.print("Message: ");
        
        Wire.beginTransmission(recipientAddress);
        delay(100);
        
        Wire.write(id);
        while (Serial.available()) {
          char c = Serial.read();
          Serial.write(c);
          Wire.write(c);
        }
        
        Wire.write('.');
        
        Wire.endTransmission();
        Serial.write("\nMessage delivered.\n");
      } else {
        Serial.println("Recipient not found in the address table.\n");
      }
    }
  }
}

void receiveEvent(int length) {
  messFrom = Wire.read();
  lastSenderAddress = messFrom;
  Serial.write("Sender's name: ");
  Serial.write(addressToName(messFrom));
  Serial.write("\n");
  Serial.write("Message: ");

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

char addressToName(int addr) {
  for (int i = 0; i < sizeof(addressTable) / sizeof(addressTable[0]); i++) {
    if (addressTable[i].address == addr) {
      return addressTable[i].name;
    }
  }
  return 'X';
}