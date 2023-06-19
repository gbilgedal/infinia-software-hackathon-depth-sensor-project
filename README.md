# infinia-software-hackathon-depth-sensor-project

This project, Cycloduck, was created in a software hackathon organised by INFINIA and Kargakarga, on 17-18th of June, 2023 by Günsu Bilge Dal and Emre Eren.

The name Cycloduck comes from the resemblance of the created product to a duck and a cyclops.

The aim of the project was to implement a wide ranged depth camera using a distance sensor and two servo motors as base structure. Due to it being more than a software project, two aspects will be mentions in this README file.

For the hardware part, the distance sensor and servo were connected to the Arduino, a mini breadboard was also used for necessary purposes.

For the software part, data coming from the sensor were put in a 2D array and visualised with matplotlib. For optimization, gaussian filter and bilinear interpolation was applied to the data gathered.

The rows and columns values were set to 45x45 for demo purposes. Values can be changed for desired sensitivity and output.

Used Hardware:
- VL53l1X Time-of-Flight Distance
- 2 SG90 Mini Servo Motors 
- 2-Axis Pan/Tilt Kit
