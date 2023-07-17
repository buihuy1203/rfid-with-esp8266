/*This is the code for Arduino*/

#include<SerialCommand.h>
#include<Wire.h> //This library allows you to communicate with I2C devices
#include<LiquidCrystal_I2C.h>
#include<SPI.h>
#include<MFRC522.h>
SerialCommand sCmd;
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0X27,16,2);
int relay = 8;
String defaultKey = "21 A9 B0 23";
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
  Serial.begin(115200);
  sCmd.addCommand("bat1", unlock);
  sCmd.addCommand("tat1", lock);
  sCmd.addCommand("bat2", del);
  sCmd.addCommand("tat2", swap);
  delay(2000);
}

void loop()
{ 
  if(Serial.available()){
  sCmd.readSerial();
  }
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
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  lcd.setCursor(4,1);
  lcd.print(content);
  delay(1000);
  if (content.substring(1) == defaultKey) //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Success");
    digitalWrite(relay, HIGH);
    delay(3000);
    digitalWrite(relay, LOW);
    printMenu();
    delay(3000);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.print("Access Denied");
    delay(3000);
    printMenu();
  }
}

void printMenu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Move your card ...");
}

void unlock(){
  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Door open");
    digitalWrite(relay, HIGH);
}

void lock(){
  lcd.clear();
  lcd.setCursor(0,0);
      lcd.print("Door close");
      digitalWrite(relay, LOW);
}

void del(){
  defaultKey = "FF FF FF FF";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Key Delete");
}

void swap(){
  while( ! mfrc522.PICC_IsNewCardPresent()){
  continue;
  }
  while( ! mfrc522.PICC_ReadCardSerial()){
  continue;
  }
  String newKey = "";
  for (byte i = 0; i < mfrc522.uid.size; i++){
  newKey.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  newKey.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  newKey.toUpperCase();
  defaultKey = newKey.substring(1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Card Swapped");
}