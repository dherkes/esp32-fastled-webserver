/*
   ESP32 FastLED WebServer: https://github.com/jasoncoon/esp32-fastled-webserver
   Copyright (C) 2017 Jason Coon

   Built upon the amazing FastLED work of Daniel Garcia and Mark Kriegsman:
   https://github.com/FastLED/FastLED

   ESP32 support provided by the hard work of Sam Guyer:
   https://github.com/samguyer/FastLED

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

String getPower() {
  return String(power);
}

String setPower(String value) {
  power = value.toInt();
  power = power == 0 ? 0 : 1;
  return String(power);
}

String getBrightness() {
  return String(brightness);
}

String setBrightness(String value) {
  brightness = value.toInt();
  //  if (brightness < 0) brightness = 0;
  //  else if (brightness > 255) brightness = 255;
  FastLED.setBrightness(brightness);
  return String(brightness);
}

String getPattern() {
  return String(currentPatternIndex);
}

void setPattern(uint8_t value)
{
  if (value >= patternCount)
    value = patternCount - 1;

  currentPatternIndex = value;

//  if (autoplay == 0) {
//    EEPROM.write(1, currentPatternIndex);
//    EEPROM.commit();
//  }

//  broadcastInt("pattern", currentPatternIndex);
}

String setPattern(String value) {
  setPattern(value.toInt());
  return String(currentPatternIndex);
}

String getPatterns() {
  String json = "";

  for (uint8_t i = 0; i < patternCount; i++) {
    json += "\"" + patterns[i].name + "\"";
    if (i < patternCount - 1)
      json += ",";
  }

  return json;
}

String getPalette() {
  return String(currentPaletteIndex);
}

String setPalette(String value) {
  currentPaletteIndex = value.toInt();
  if (currentPaletteIndex < 0) currentPaletteIndex = 0;
  else if (currentPaletteIndex >= paletteCount) currentPaletteIndex = paletteCount - 1;
  return String(currentPaletteIndex);
}

String getPalettes() {
  String json = "";

  for (uint8_t i = 0; i < paletteCount; i++) {
    json += "\"" + paletteNames[i] + "\"";
    if (i < paletteCount - 1)
      json += ",";
  }

  return json;
}

String getSpeed() {
  return String(speed);
}

String setSpeed(String value) {
  speed = value.toInt();
  return String(speed);
}

String getAutoplay() {
  return String(autoplay);
}

String setAutoplay(String value) {
  autoplay = value.toInt();
  autoplay = autoplay == 0 ? 0 : 1;
  autoPlayTimeout = millis() + (autoplayDuration * 1000);
  return String(autoplay);
}

String getAutoplayDuration() {
  return String(autoplayDuration);
}

String setAutoplayDuration(String value) {
  autoplayDuration = value.toInt();
  if (autoplayDuration < 0) autoplayDuration = 0;
  else if (autoplayDuration > 255) autoplayDuration = 255;
  autoPlayTimeout = millis() + (autoplayDuration * 1000);
  return String(autoplayDuration);
}

String getSolidColor() {
  return String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b);
}

void setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  solidColor = CRGB(r, g, b);

  //  EEPROM.write(2, r);
  //  EEPROM.write(3, g);
  //  EEPROM.write(4, b);
  //  EEPROM.commit();

  setPattern(patternCount - 1);

  //  broadcastString("color", String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b));
}

void setSolidColor(CRGB color)
{
  setSolidColor(color.r, color.g, color.b);
}

String setSolidColor(String value) {
  String r = webServer.arg("r");
  String g = webServer.arg("g");
  String b = webServer.arg("b");
  setSolidColor(r.toInt(), g.toInt(), b.toInt());
  return String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b);
}

FieldList fields = {
  { "power", "Power", BooleanFieldType, 0, 1, getPower, NULL, setPower },
  { "brightness", "Brightness", NumberFieldType, 1, 255, getBrightness, NULL, setBrightness },
  { "pattern", "Pattern", SelectFieldType, 0, patternCount, getPattern, getPatterns, setPattern },
  { "palette", "Palette", SelectFieldType, 0, paletteCount, getPalette, getPalettes, setPalette },
  { "speed", "Speed", NumberFieldType, 1, 255, getSpeed, NULL, setSpeed },
  { "autoplaySection", "Autoplay", SectionFieldType },
  { "autoplay", "Autoplay", BooleanFieldType, 0, 1, getAutoplay, NULL, setAutoplay },
  { "autoplayDuration", "Autoplay Duration", NumberFieldType, 0, 255, getAutoplayDuration, NULL, setAutoplayDuration },
  { "solidColorSection", "Solid Color", SectionFieldType },
  { "solidColor", "Color", ColorFieldType, 0, 255, getSolidColor, NULL, setSolidColor },
};

uint8_t fieldCount = ARRAY_SIZE(fields);
