---
layout: default
title: INFINITY MIRROR
description: EE-Emerge 2021-2022
---

<h1 style="color:darkviolet;">Infinity Mirror Clock Display</h1>

<p align = "center">
<img src="{{site.baseurl}}/assets/images/infinityclockdisplay.png" width="400">
 </p>

The Infinity mirror clock bases its design on the infinity mirror. The Infinity mirror gives the illusion of LED lights going forever into infinity.  It makes use of one real mirror and acrylic with reflective film. The acrylic reflects light on one side while the other side doesn’t. The reflective side of the acrylic is facing towards the real mirror, and LED strips are placed between the two. This allows the viewer to observe the lights reflecting between the two mirrors. 
In order to implement an analog clock within the infinity mirror, the individually addressable LED strips are attached inside the enclosure. There are sixty LEDs to depict sixty seconds. Three different colors are chosen to display the hour, minute and second of the analog clock. 

<h2 style="color:darkviolet;">Enclosure</h2>

<p align = "center">
  <img src="{{site.baseurl}}/assets/images/infinitymirrorenclosure.png" width="400">
  <img src="{{site.baseurl}}/assets/images/infinitymirrorenclosurecutv3.png" width="400">
  </p>

The enclosure for the infinity mirror clock has to be custom made due to the length of sixty LEDs strip. The software used to design the enclosure is “Autodesk Fusion 360”. The accessible 3D printers are not big enough to contain the entirety of enclosure. Therefore, the enclosure was cut into four pieces and then, glue together afterwards.

<h2 style="color:darkviolet;">PCB Design</h2>

<p align = "center">
<img src="{{site.baseurl}}/assets/images/Main PCB.jpg" width="400">
 </p>

The PCB in the infinity clock is custom-designed and acts as the main unit for this project. It is equipped with the RTC to keep track of time and an ultra-low-power microcontroller, MSP430G2553IN20.
