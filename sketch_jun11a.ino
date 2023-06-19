#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<SPI.h>
#include<MFRC522.h>
/*#include<SoftwareSerial.h>
#define Rx 2
#define Tx 3*/
#define SS_PIN 10
#define RST_PIN 9
//SoftwareSerial mySerial(Rx, Tx);
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27,16,2);
int relay = 8;
String inputString = ""; //hold incoming data
bool stringComplete = false; //whether the string is complete
String defaultKey= "21 A9 B0 23";
String openDoor = "openDoor";
String closeDoor = "closeDoor";
void setup()
{
  SPI.begin();     // Initiate  SPI bus
  lcd.init();
  lcd.backlight();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  mfrc522.PCD_Init(); // Initiate MFRC522
  lcd.setCursor(0,0);
  lcd.print("Move your card...");
  inputString.reserve(200);
  Serial.begin(115200);
  /*mySerial.begin(115200); 
  mySerial.print(closeDoor);*/
  Serial.print(closeDoor);
  delay(2000);
}

void loop()
{
  if (stringComplete){
    if(inputString = "bat1"){
      lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Door open");
    Serial.print(openDoor);
    digitalWrite(relay, HIGH);
    inputString = "";
    stringComplete = false;
    }else if(inputString = "tat1"){
      lcd.clear();
  lcd.setCursor(0,0);
      lcd.print("Door close");
      Serial.print(closeDoor);
      digitalWrite(relay, LOW);
      inputString = "";
      stringComplete = false;
    }else if(inputString = "bat2"){
      defaultKey = "FF FF FF FF";
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Key Delete");
    }else if(inputString = "tat2"){
      if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  String newKey ="";
  for (byte i = 0; i < mfrc522.uid.size; i++){
  newKey.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  newKey.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  defaultKey = newKey;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Card Swapped");
  inputString = "";
  stringComplete = false;
    }
  }else{
      // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  lcd.setCursor(0,1);
  lcd.print("UID :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    lcd.setCursor(6,1);
    lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    lcd.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == defaultKey) //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Success");
    digitalWrite(relay, HIGH);
    Serial.print(openDoor);
    delay(3000);
    digitalWrite(relay, LOW);
    Serial.print(closeDoor);
    printMenu();
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    delay(3000);
    printMenu();
  }
  }
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
void printMenu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Move your card ...");
}
