#include <Adafruit_VL53L1X.h>
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <vl53l1x_class.h>
#include <vl53l1x_error_codes.h>

#include "Adafruit_VL53L1X.h"
#include <Servo.h>

#define IRQ_PIN 2
#define XSHUT_PIN 3
#define ROWS 45
#define COLS 45
#define FOV 90
#define STEP FOV / ROWS
#define START_ANGLE 30

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

Servo servo_top;
Servo servo_bot;

void setup() {

  //CODE FOR TOF
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("Adafruit VL53L1X sensor demo"));

  Wire.begin();
  if (!vl53.begin(0x29, & Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (!vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }
  Serial.println(F("Ranging started"));

  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());

  /*
  vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L1X_SetInterruptPolarity(0);
  */
  //END OF CODE FOR TOF

  //CODE FOR SERVO
  servo_top.attach(9);
  servo_bot.attach(8);
  //END OF CODE FOR SERVO

}

void loop() {
  servo_bot.write(0);
  servo_top.write(START_ANGLE);
  delay(300);

  int16_t distance;
  int array[ROWS][COLS];
  int i, j;

  // Fill the array with values
  for (int i = 0; i < ROWS; i++) {
    if (i % 2 == 0) {
      for (int j = 0; j < COLS; j++) {
        while (!vl53.dataReady()) {
        }
        distance = vl53.distance();
        array[i][j] = distance;
        if (distance == -1) {
          distance = 4000;
        }

        servo_bot.write(STEP * j);
        servo_top.write(START_ANGLE + STEP * i);

        Serial.print(F("Distance: "));
        Serial.print(distance);

        Serial.print(F(" i: "));
        Serial.print(i);

        Serial.print(F(" j: "));
        Serial.println(j);

        delay(120);
        // data is read out, time for another reading!
        vl53.clearInterrupt(); 
        Serial.println();
      }
    } else {
      for (int j = COLS - 1; j >= 0; j--) {
        while (!vl53.dataReady()) {
        }
        distance = vl53.distance();
        array[i][j] = distance;
        if (distance == -1) {
          distance = 4000;
        }

        servo_bot.write(STEP * j);
        servo_top.write(START_ANGLE + STEP * i);

        Serial.print(F("Distance: "));
        Serial.print(distance);

        Serial.print(F(" i: "));
        Serial.print(i);

        Serial.print(F(" j: "));
        Serial.println(j);

        delay(120);
        // data is read out, time for another reading!
        vl53.clearInterrupt(); 
        Serial.println();
      }
    }
  }
  Serial.println("END - ARRAY FILLED");
}