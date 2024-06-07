// -------------------------------------------------------------------------------
void sensor_fn() {
  ////// รับค่า  Sensor DHT
  humi = dht.readHumidity();     //รับค่าความชื้น
  temp = dht.readTemperature();  //รับค่าอุณหภูมิ
  ////// รับค่า Sensor แสง
  LDR_input = analogRead(LDR_Pin);
  ldr = map(LDR_input, 0, 4095, 0, 500);

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  Ultra = microsecondsToCentimeters(duration);
  Serial.print("Distance:");
  Serial.println(Ultra);
  collision_check();

  if (temp >= temp_alarm) {
    alarm_beep();
  }
  if (ldr >= light_alarm) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
}
void collision_check()
{
  if (Ultra <= colision_distance && Ultra != 0)
  {
    processAGVMovement("0");
    delay(50);
    processAGVMovement("2");
    delay(200);
    processAGVMovement("0");
    delay(50);
  }
}


