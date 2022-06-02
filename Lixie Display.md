---
layout: default
title: LIXIE DISPLAY
discription: EE-Emerge 2021-2022
---

<h1 style="color:darkviolet;">Lixie Clock</h1>

<p align = "center">
  <img src="{{site.baseurl}}/assets/images/3digitdisplay.png" width="400">
  <img src="{{site.baseurl}}/assets/images/LixieCad.png" width="400">
  </p>

The Lixie clock draws heavy inspiration from retro-style Nixie tubes, but the design uses low-voltage LEDs as illumination rather than the quite dangerous, high-voltage filaments used in Nixie tubes. The LEDs light etched arylic panes from the underside according to the current time.

<h2 style="color:darkviolet;">Enclosure</h2>

<p align = "center">
  <img src="{{site.baseurl}}/assets/images/Lixie Enclosure.jpg" width="400">
  <img src="{{site.baseurl}}/assets/images/Lixie Enclosure Lid.jpg" width="400">
  </p>

The enclosure is designed in "Autodesk Fusion 360" and 3D-printed. The design focuses on housing the PCB and creating ten slots to insert the acrylics on top of the enclosure. The vertical small gap is left on the back of the enclosure for cable management and smooth closure for the lid. 

<h2 style="color:darkviolet;">PCB Design</h2>

<p align = "center">
  <img src="{{site.baseurl}}/assets/images/LED PCB.jpg" width="400">
  <img src="{{site.baseurl}}/assets/images/Digit PCB.jpg" width="400">
  </p>

There are two PCBs housed inside each Lixie Display module. One is the LED PCB. The LED PCB is equipped with 50 LEDs and they are layout in the form of 5x10 matrix. The other PCB is equipped with the microcontroller and programmed to send signal to LED PCB. The microcontroller also receives signals from the main unit and update the time for Lixie module. 
