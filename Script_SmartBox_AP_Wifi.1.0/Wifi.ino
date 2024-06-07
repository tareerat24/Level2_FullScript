void initWiFi()
{
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);
  Serial.println(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  digitalWrite(2, HIGH);
}
