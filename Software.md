---
layout: default
title: SOFTWARE
description: EE-Emerge 2021-2022
---

<h1> Software </h1>

The Energia integrated development environment (Version 1.8.7E21) produced by Texas Instruments was used to write C/C++ code to program several MSP430 microcontrollers used to drive the circuits of both the Infinity Mirror Clock and the Digit Clock composed of six individual digit modules reminiscent of Nixie tubes.

# Infinity Mirror Code

The PCF8523 Real Time Clock library was used to interface with the physical module incorporated into the circuitry of the Infinity Mirror Clock. Additionally, a library originating from a past EE-Emerge project was used to control each of the individually-addressable ws2812 neopixel LEDs that make up the strips of LEDs running along the inside of the Infinity Mirror Clock. The PCF8523 library is used to retrieve the current time from the Real Time Clock module, from which the hour, minute, and seconds were extracted. Logic is implemented to light up the appropriate LED located at the position within the analog Infinity Mirror Clock corresponding to these extracted values.

<p align = "center">
  <img src="{{site.baseurl}}/assets/images/infinitymirrorclockcode.png" width="600">
  </p>
  
# Digit Board Code

The code uploaded to the microcontroller of each of the six individual modules that make up the Digital Clock display is relatively similar, with changes accounting for the tens and ones digits of each time quantity (seconds, minutes, hours) implemented. This code also makes use of the PCF8523 and ws2812 software libraries to light up the row of LEDs under the acrylic panel engraved with the correct digit to light up based on the current time.

<p align = "center">
  <img src="{{site.baseurl}}/assets/images/digitclockcode.png" width="600">
  </p>


[back](./)
