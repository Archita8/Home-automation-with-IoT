#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
#include<Servo.h>
Servo s;
#include<SoftwareSerial.h>
SoftwareSerial ss(A3, A4);
//SoftwareSerial esp(A5, A6); //A5 is Rx and A6 is Tx.
String pwd = "kt";
//String ssid = "Narzo";
//String pwds = "akshat123";
String usp = "";
int statuslock = 1;
int occupied = 0;
int mode = 1;
int bulb = 0;
int fan = 0;
int count;
int temp;
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  ss.begin(9600);
  //  esp.begin(9600);
  //
  // Serial.print("CWMODE...");
  //  esp.println("AT+CWMODE=3");
  //  while (!esp.find("OK"));
  //  Serial.println("OK");
  //  Serial.print("Connecting with " + ssid + "...");
  //  esp.println("AT+CWJAP=\"" + ssid + "\",\"" + pwds + "\"");
  //  while (!esp.find("OK"));
  //  Serial.println("Connected");
  s.attach(4);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  s.write(0);
  ss.println("Choose the operation");
}


//String reqbOn = "GET /BON HTTP/1.1\r\nHost: akshat08.pythonanywhere.com\r\n\r\n";
//String reqbOff = "GET /BOFF HTTP/1.1\r\nHost: akshat08.pythonanywhere.com\r\n\r\n";
//String reqfOn = "GET /FON HTTP/1.1\r\nHost: akshat08.pythonanywhere.com\r\n\r\n";
//String reqfOff = "GET /FOFF HTTP/1.1\r\nHost: akshat08.pythonanywhere.com\r\n\r\n";
//String reqRead = "GET / HTTP/1.1\r\nHost: akshat08.pythonanywhere.com\r\n\r\n";

void loop() {
  if (ss.available()) {////////
    char ch = ss.read();
    while (!ss.find("\r\n"));
    delay(10);
    if (ch == 'u') {
      ss.println("Enter password");
      while (!ss.available());
initial:
      String usp = ss.readString();
      usp.trim();
      ss.println(usp);
      if (usp == pwd) {
        Serial.println("Door Opens");
        s.write(90);
        if ((digitalRead(5) > 200) && (digitalRead(6) > 200)) {

          for (int t = 1; t <= 1000; t++)
          { delay(1);
            if (((digitalRead(5) > 200) && (digitalRead(6) <= 200)))
            {
              for (int d = 1; d <= 2000; d++)
              {
                delay(1);
                if (((digitalRead(5) <= 200) && (digitalRead(6) <= 200)))
                {
                  for (int d = 1; d <= 2000; d++)
                  {
                    delay(1);
                    if (((digitalRead(5) <= 200) && (digitalRead(6) > 200)))
                    {
                      for (int d = 1; d <= 2000; d++)
                      {
                        delay(1);
                        if ((digitalRead(5) > 200) && (digitalRead(6) > 200));
                        {
                          count--;
                          Serial.println("Door Closes .");
                          s.write(0);
                          if (count == 0) {
                            Serial.println("Unoccupied");
                            lcd.setCursor(1, 0);
                            lcd.print("Unoccupied");
                            digitalWrite(2, 0);
                            digitalWrite(3, 0);
                          }
                          else
                          { lcd.println(count);
                          }

                        }

                      }
                    }
                  }
                }
              }
            }
            if (((digitalRead(5) <= 200) && (digitalRead(6) > 200)))
            {
              for (int t = 1; t <= 1000; t++)
              { delay(1);
                if (((digitalRead(5) <= 200) && (digitalRead(6) <= 200)))
                {
                  for (int t = 1; t <= 1000; t++)
                  {
                    delay(1);
                    if (((digitalRead(5) > 200) && (digitalRead(6) <= 200)))
                    {
                      for (int t = 1; t <= 1000; t++)
                      {
                        delay(1);
                        if ((digitalRead(5) > 200) && (digitalRead(6) > 200));
                        {
                          Serial.println("Door Closes .");
                          s.write(0);
                          count++;
                          if (count >= 1) {
                            Serial.println("Occupied");
                            lcd.setCursor(1, 0);
                            lcd.print("Occupied");
                          }
                          goto second;
                        }

                      }
                    }
                  }
                }
              }
            }

          }
          s.write(0);


        }
      }
      else
      { ss.println("  Wrong Password");
        goto initial;
      }
    }

second:
    if (ch == 'a')
      delay(10);
    { ss.println("Automatic");
      mode = 1;

    }
    if (ch == 'm') {
      mode = 0;
      ss.println("Manual");
    }
    if (ch == 'b') {
      if (mode == 0)
      {
        bulb = 1 - bulb;
        digitalWrite(3, bulb);
        //          if(bulb == 1)
        //          {
        //            sendRequest(1);
        //          }
        //          else
        //          {
        //            sendRequest(2);
        //          }
      }
    }
    if (ch == 'f') {
      delay(10);
      if (mode == 0) {
        fan = 1 - fan;
        digitalWrite(2, fan);
        //          if (fan == 1)
        //          {
        //            sendRequest(3);
        //          }
        //          else
        //          {
        //            sendRequest(4);
        //          }
      }
      else
      {
        ss.println("not manual");
      }
    }
  }
  if (mode == 1)
  {
    temp = analogRead(A2);
    float tempCel = (temp / 1024.0) * 500;
    ss.println("  Temp in cel=");
    ss.println(tempCel);
    ss.println("*C");
    ss.println("   |    ");
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.setCursor(5, 1);
    lcd.print(tempCel);
    delay(1000);
    if (tempCel >= 25)
    { digitalWrite(2, 1);
    }
    else
    { digitalWrite(2, 0);
    }
    int x = analogRead(A1);
    ss.println("  LDR value=");
    ss.println(x);
    lcd.setCursor(10, 1);
    lcd.print("LDR:");
    lcd.setCursor(14, 1);
    lcd.print(x);
    if (x > 30)
    { digitalWrite(3, 1);
    }
    else
    { digitalWrite(3, 0);
    }
    delay(1000);
  }
  int g = analogRead(A0);
  ss.println(g);
  delay(1000);
  if (g >= 160)
  {
    digitalWrite(13, 1);
  }
  else if (g < 140)
  {
    digitalWrite(13, 0);
  }
}



//void sendRequest(int v) {
//  String req = "";
//  if (v == 1) {
//    req = reqbOn;
//  } else if (v == 2) {
//    req = reqbOff;
//  } else if (v == 3)
//  {
//    req = reqfOn;
//  }else if( v==4)
//  {
//    req = reqfOff;
//  }
//  else
//  {
//    req = reqRead;
//  }
//  Serial.print("Connecting to server...");
//  esp.println("AT+CIPSTART=\"TCP\",\"akshat08.pythonanywhere.com\",80");
//  while (!esp.find("CONNECT"));
//  while (!esp.find("OK"));
//  Serial.println("connected");
//
//  Serial.print("Sending request...");
//  esp.print("AT+CIPSEND=");
//  esp.println(req.length());
//  while (!esp.find("OK"));
//  Serial.print(" OK ");
//  while (!esp.find(">"));
//  Serial.print(" > ");
//  esp.print(req);
//  while (!esp.find("+IPD"));
//  Serial.print("+IPD");
//  while (!esp.find("200 OK"));
//  Serial.print(" 200 OK ");
//  while (!esp.find("\r\n\r\n"));
//  while (!esp.available());
//  if (esp.available()) {
//    String resp = esp.readString();
//    Serial.println(resp);
//    if (resp.indexOf("ON") != -1) {
//      digitalWrite(13, 1);
//    } else if (resp.indexOf("OFF") != -1) {
//      digitalWrite(13, 0);
//    }
//  }
////  esp.println("AT+CIPCLOSE");
////  while (!esp.find("CLOSED"));
////  Serial.println("SUCCESS!!");
//}
