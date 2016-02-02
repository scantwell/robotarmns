//ARDUINO UNO DATA TYPE SIZES
// byte - 8 bits 1 byte
// int - 16 bits 2 bytes
// long - 32 bits 4 bytes

union robot_int
{
  unsigned int value;
  byte bytes[2];
};


byte input[4];
int count = 0;
int numBytes = 0;
robot_int header;
robot_int len;
robot_int data;
byte bytes[2];

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void printBits(byte myByte){
 for(byte mask = 0x80; mask; mask >>= 1){
   if(mask  & myByte)
       Serial.print('1');
   else
       Serial.print('0');
 }
}
// \x01\n\x00\n
void loop() {

  
  while( Serial.available() == 4)
  {
    header.bytes[0] = Serial.read();
    len.bytes[0] = Serial.read();
    data.bytes[1] = Serial.read();
    data.bytes[0] = Serial.read();
    count =  4;
  }
  if( count == 4)
  {
    Serial.println("START PRINT");
    Serial.println(header.value);
    Serial.println(len.value);
    Serial.print(data.bytes[0], HEX);
    Serial.print(";");
    Serial.print(data.bytes[1], HEX);
    Serial.print(";");
    Serial.println(data.value);
  }
  /*if( count > 0)
  {
     Serial.print("COUNT ");
     Serial.println(count);
     for( int i = 0; i < count; ++i)
    {
      Serial.write(input[count]);
    } 
    count = 0;
  }*/
}
