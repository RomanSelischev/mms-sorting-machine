#include <AccelStepper.h>
#define ROUT 5
#define GOUT 6
#define BOUT 7
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
// To-do:
// MODE to work in different modes

AccelStepper stepper(8, IN1, IN3, IN2, IN4);


void setup(){
  Serial.begin(9600);

  // set speed for calibration
  stepper.setMaxSpeed(300.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(300);

  long minDiff = 1025;
  long index = 0;
  const int ROUNDS = 18;

  // find the hole
  for (int i = 0; i < ROUNDS; i++) {
    int light = getLightness(255, 255, 255, 300);
    int dark = getLightness(0, 0, 0, 300);
    int diff = dark - light;
    if (diff <= minDiff) {
      minDiff = diff;
      index = i;
    }
    Serial.println(diff);
    Serial.println(index);
    stepper.move(32);
    stepper.runToPosition();
  }

  // go back to hole
  long wayback = 32 * (index - ROUNDS - 1);
  stepper.move(wayback);

  Serial.println(wayback);

  // set speed for work
  stepper.runToPosition();
  stepper.setMaxSpeed(1024.0);
  stepper.setAcceleration(1024.0);
  delay(2000);
}

void loop()
{
  String color = classify();
  Serial.println(color);

  if (color != "None") {
    setRotator(color);
  }

  stepper.move(522);
  stepper.runToPosition();
  stepper.move(90);
  stepper.runToPosition();
  stepper.move(-100);
  stepper.runToPosition();
}

String classify()
{
  int red = getLightness(255, 0, 0, 500);
  int green = getLightness(0, 255, 0, 500);
  int blue = getLightness(0, 0, 255, 500);
  // blue is not in use btw

  // output to IDE's `Serial Monitor`
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(blue);

  // green or purple
  if (550 < red && red <= 620)
  {
    // green
    if (450 < green && green <= 525)
    {
      return "Green";
    }
    // purple
    if (525 < green && green <= 575)
    {
      return "Purple";
    }
    return "None";
  }

  // red
  if (475 < red && red <= 550)
  {
    if (525 < green && green <= 575)
    {
      return "Red";
    }
    return "None";
  }

  // yellow or orange
  if (419 < red && red <= 475)
  {
    // orange
    if (500 < green && green <= 560)
    {
      return "Orange";
    }
    // yellow
    if (430 < green && green <= 500)
    {
      return "Yellow";
    }
    return "None";
  }
  return "None";

}

void setRotator(String color)
{
  // заглушка  поворота мотора на будущее
  return;
}

void setColor(int r, int g, int b)
{
  // устанавливает цвет светодиода
  analogWrite(ROUT, r);
  analogWrite(GOUT, g);
  analogWrite(BOUT, b);
}

int getLightness()
{
  // измеряет текущую освещенность
  return analogRead(A3);
}

int getLightness(int r, int g, int b, int measureDelay)
{
  // измеряет освещенность цветом после задержки
  setColor(r, g, b);
  delay(measureDelay);
  int lightness = getLightness();
  setColor(0, 0, 0);
  return lightness;
}