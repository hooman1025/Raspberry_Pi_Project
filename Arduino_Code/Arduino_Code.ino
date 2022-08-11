#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

//wifi setup
#define SSID        "kcci01"
#define PASSWORD    "kccikcci"
#define HOST_NAME   "192.168.0.12"
#define HOST_PORT   23

SoftwareSerial mySerial(10, 9); /* RX:D10, TX:D9 */
ESP8266 wifi(mySerial);
uint8_t buffer[64] = {0};
 
//BTN,Value setup
#define option_up 4
#define option_down 5
#define select 2
#define next_btn 3

int flag = 0;
String menu[6] = { "Americano", "Espresso", "Caffelatte", "Icetea", "Herbtea", "Chocolatte" };
String water_temp[2] = { "Ice", "Hot" };
String drink_size[3] = {"Tall", "Grande", "Venti"};
String syrup[3] {"Noshot", "Oneshot", "Twoshot"};
int menu_num = 0, water_temp_num = 0, drink_size_num = 0, syrup_num = 0;

void options(String option_arr, int option_num);
void lcd_print();

String Info;

int lcd_title = 1;

//LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Interrupt setup
boolean result_select = false;
boolean result_flag = false;

void Parsing() {
  if (result_select == true) {
    result_select = false;
    return;
  }
  delay(10);
  result_select = true;
}

void nextInfo() {
  if (result_flag == true) {
    result_flag = false;
    return;
  }
  delay(10);
  result_flag = true;
}

void setup() {

  //wifi setup
  Serial.begin(9600);
  Serial.print("setup begin\r\n");

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  while (1) {
    if (wifi.setOprToStationSoftAP()) {
      Serial.print("to station + softap ok\r\n");
      break;
    } else {
      Serial.print("to station + softap err\r\n");
    }
  }

  while (1) {
    if (wifi.joinAP(SSID, PASSWORD)) {
      Serial.print("Join AP success\r\n");
      Serial.print("IP:");
      Serial.println( wifi.getLocalIP().c_str());
      break;
    } else {
      Serial.print("Join AP failure\r\n");
    }
  }

  while (1) {
    if (wifi.disableMUX()) {
      Serial.print("single ok\r\n");
      break;
    } else {
      Serial.print("single err\r\n");
    }
  }
  Serial.print("setup end\r\n");


  while (1) {
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
      Serial.print("create tcp ok\r\n");
      break;
    } else {
      Serial.print("create tcp err\r\n");
    }
  }

  //pin setup
  pinMode(option_up, INPUT);
  pinMode(option_down, INPUT);

  pinMode(select, INPUT);
  pinMode(next_btn, INPUT);

  attachInterrupt(digitalPinToInterrupt(select), Parsing, RISING);
  attachInterrupt(digitalPinToInterrupt(next_btn), nextInfo, RISING);

  lcd.init();
  lcd.backlight();
}

void loop() {


  //menu select
  switch (flag) {
    case 0 :
      menu_num = options(menu, menu_num, 6);

      if (lcd_title == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print("Menu Select");
        lcd_print(menu, menu_num);
        lcd_title = 0;
      }


      break;
    case 1 :
      water_temp_num = options(water_temp, water_temp_num, 2);

      if (lcd_title == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print("Temp Select");
        lcd_print(water_temp, water_temp_num);
        lcd_title = 0;
      }

      break;
    case 2 :
      drink_size_num = options(drink_size, drink_size_num, 3);

      if (lcd_title == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print("Size Select");
        lcd_print(drink_size, drink_size_num);
        lcd_title = 0;
      }
      break;
    case 3 :
      syrup_num = options(syrup, syrup_num, 3);

      if (lcd_title == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print("Syrup Select");
        lcd_print(syrup, syrup_num);
        lcd_title = 0;

      }

      break;
    case 4 :
      if (lcd_title == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print(menu[menu_num]);
        lcd.setCursor(13, 0);
        lcd.print(water_temp[water_temp_num]);
        lcd.setCursor(0, 1);
        lcd.print(drink_size[drink_size_num]);
        lcd.setCursor(9, 1);
        lcd.print(syrup[syrup_num]);
        lcd_title = 0;
      }
  }

  if (result_select == true) {
    delay(100);
    Info = menu[menu_num] + "#" + water_temp[water_temp_num] + "#" + drink_size[drink_size_num] + "#" + syrup[syrup_num];
    Serial.println(Info);
    strcpy(buffer, Info.c_str());
    wifi.send(buffer, strlen(buffer));
    result_select = false;
  }
  if (result_flag == true) {
    delay(200);
    flag++;
    lcd_title = 1;
    lcd.clear();
    if (flag > 4) flag = 0;

    result_flag = false;
  }
  delay(10);
}

//option btn function
int options(String option[], int option_num, int strsize)
{

  strsize--;
  if (digitalRead(option_up) == HIGH) {
    option_num--;
    if (option_num < 0)
      option_num = strsize;
    lcd_print(option, option_num);

  }
  else if (digitalRead(option_down) == HIGH) {
    option_num++;
    if (option_num > strsize)
      option_num = 0;
    lcd_print(option, option_num);
  }

  delay(100);
  return option_num;
}

//lcd print function
void lcd_print(String option[], int option_num) {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  //lcd_title = 1;
  lcd.setCursor(0, 1);
  lcd.print(option[option_num]);
}
