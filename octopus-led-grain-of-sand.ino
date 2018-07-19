/*
 * octopus-led-grain-of-sand
 *
 * Copyright (C) 2018 Wolfgang Klenk <wolfgang.klenk@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Adafruit_NeoPixel.h> 
#include <Adafruit_Sensor.h>

#include <Adafruit_BNO055.h>

#define NUMPIXELS      32
#define PIN_PIXELS     15 // Use pin 15 for NeoPixels

// Tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno055 = Adafruit_BNO055(55);

int x = 0;
int y = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  while (!Serial) {
    // Wait for serial port to connect. Needed for native USB
    delay(100);
  }

  Serial.print("Initializing BNO055: ");
  if (bno055.begin()) {
    Serial.println("OK");
  } else {
    Serial.println("Not found");
  }

  // Initialize NeoPixel library
  pixels.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  // Read acceleration values for x, y and z
  imu::Vector<3> bnoAccel = bno055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  Serial.print("Acceleration (m/s^2) x=");
  Serial.print(bnoAccel.x());
  Serial.print(", y=");
  Serial.print(bnoAccel.y());
  Serial.print(", z=");
  Serial.println(bnoAccel.z());

  if (bnoAccel.x() < 0) {
    x = x + 1;
    if (x >= 4) {
      x = 3;
    }
  } else {
    x = x - 1;
    if (x < 0) {
      x = 0;
    }
  }

  if (bnoAccel.y() > 0) {
    y = y + 1;
    if (y >= 8) {
      y = 7;
    }
  } else {
    y = y - 1;
    if (y < 0) {
      y = 0;
    }
  }

  Serial.print("x=");
  Serial.print(x);
  Serial.print(" y=");
  Serial.print(y);
  Serial.print(" pixelPosition=");
  Serial.println(toNeoPixelPosition(x,y));

  pixels.clear();
  pixels.setPixelColor(toNeoPixelPosition(x,y), pixels.Color(150,0,0));
  pixels.show();

  delay(100);
}

uint16_t toNeoPixelPosition(int x, int y) {

  return x * 8 + y;
}

