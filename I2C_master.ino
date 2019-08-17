//triangular_base
//Tanvesh Bhattad

#include <XBOXRECV.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

int a1 = 4, a2 = 2,b1 = A0, b2 = A1, c1 = 8, c2 = 7, pin_a = 3, pin_b = 5, pin_c = 6;
int pa = 0, pb = 0, pc = 0;
int x = 0, y = 0;
float a = 0, b = 0, c = 0, abs_a = 0, abs_b = 0, abs_c = 0;
boolean command[] = {0, 1, 0, 1, 0, 1};
int max_value = 0;
int pwm = 0;
int max_pwm = 150;
float magnitude = 0;
int rotational = 0, rotational_pwm = 0, max_rotational_pwm = 50 ;

USB Usb;
XBOXRECV Xbox(&Usb);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));


  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(pin_a, OUTPUT);
  pinMode(pin_b, OUTPUT);
  pinMode(pin_c, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {


        if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500 || Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500 || Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500 || Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
          if (Xbox.getAnalogHat(LeftHatX, i) > 7500 ) {
            x = Xbox.getAnalogHat(LeftHatX, i);
            x = map(Xbox.getAnalogHat(LeftHatX, i), 7500, 32768, 0, 100);
          }
          else if ( Xbox.getAnalogHat(LeftHatX, i) < -7500)
          {
            x = Xbox.getAnalogHat(LeftHatX, i);
            x = map(Xbox.getAnalogHat(LeftHatX, i), -7500, -32768, 0, -100);
          }
          else if (Xbox.getAnalogHat(LeftHatX, i) > -7500 && Xbox.getAnalogHat(LeftHatX, i) < 7500)
          {
            x = 0;
          }
          if (Xbox.getAnalogHat(LeftHatY, i) > 7500  ) {
            y = Xbox.getAnalogHat(LeftHatY, i);
            y = map(Xbox.getAnalogHat(LeftHatY, i), 7500, 32768, 0, 100);
          }
          else if (Xbox.getAnalogHat(LeftHatY, i) < -7500)
          {
            y = (Xbox.getAnalogHat(LeftHatY, i));
            y = map(Xbox.getAnalogHat(LeftHatY, i), -7500, -32768, 0, -100);
          }
          else if (Xbox.getAnalogHat(LeftHatY, i) > -7500 && Xbox.getAnalogHat(LeftHatY, i) < 7500)
          {
            y = 0;
          }

        }
        if (Xbox.getAnalogHat(RightHatX, i) > 7500 )
        {
          rotational = (Xbox.getAnalogHat(RightHatX, i));
          rotational_pwm = map(rotational, 7500, 32768, 0, max_rotational_pwm);
        }
        else if (Xbox.getAnalogHat(RightHatX, i) < - 7500 )
        {
          rotational = (Xbox.getAnalogHat(RightHatX, i));
          rotational_pwm = map(rotational, -7500, -32768, 0, -max_rotational_pwm);
        }
        else if (Xbox.getAnalogHat(RightHatX, i) > - 7500 && Xbox.getAnalogHat(RightHatX, i) <  7500 )
        {
          rotational_pwm = 0;
        }
        if (Xbox.getAnalogHat(LeftHatX, i) > -7500 && Xbox.getAnalogHat(LeftHatX, i) < 7500)
          {
            x = 0;
          }
          if (Xbox.getAnalogHat(LeftHatY, i) > -7500 && Xbox.getAnalogHat(LeftHatY, i) < 7500)
          {
            y = 0;
          }
      }
    }
  }
  magnitude = sqrt(sq(x) + sq(y));
  a = (0.67 * x);
  b = ((0.33 * x) - (0.58 * y));
  c = ((0.33 * x) + (0.58 * y));

  if (a >= 0)
  {
    abs_a = a;
  }
  else
  {
    abs_a = -a;
  }
  if (b >= 0)
  {
    abs_b = b;
  }
  else
  {
    abs_b = (-b);
  }
  if (c >= 0)
  {
    abs_c = c;
  }
  else
  {
    abs_c = -c;
  }
  pwm = map(magnitude, 0, 100, 0, max_pwm);
  max_value = max(max(abs_a, abs_b), abs_c);
  pa = map(abs_a, 0, max_value, 0, pwm) + 1;
  pb = map(abs_b, 0, max_value, 0, pwm) + 1;
  pc = map(abs_c, 0, max_value, 0, pwm) + 1;
  if (a >= 0)
  {
    pa = pa;
  }
  else
  {
    pa = -pa;
  }

  if (b >= 0)
  {
    pb = pb;
  }
  else
  {

    pb = -pb;
  }

  if (c >= 0)
  {

    pc = pc;

  }
  else
  {

    pc = -pc;
  }

  pa = pa + rotational_pwm;
  pb = pb - rotational_pwm;
  pc = pc - rotational_pwm;

  if (pa >= 0)
  {
    command[0] = 1;
    command[1] = 0;
  }
  else
  {
    command[0] = 0;
    command[1] = 1;
  }

  if (pb >= 0)
  {
    command[2] = 1;
    command[3] = 0;
  }
  else
  {
    command[2] = 0;
    command[3] = 1;
  }

  if (pc >= 0)
  {
    command[4] = 1;
    command[5] = 0;
    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);

  }
  else
  {
    command[4] = 0;
    command[5] = 1;
    digitalWrite(c1, LOW);
    digitalWrite(c2, HIGH);
  }

  int pins[] = {a1, a2, b1, b2, c1, c2};
  int i = 0;
  for (i = 0; i <= 5; i++)
  {
    digitalWrite(pins[i], command[i]);
  }

  analogWrite(pin_a, abs( pa));
  analogWrite(pin_b, abs( pb));
  analogWrite(pin_c, abs( pc));

  Serial.print(pa);
  Serial.print("         ");
  Serial.print(pb);
  Serial.print("         ");
  Serial.print(pc);
  Serial.print("         ");

  for (int j = 0; j <= 5; j++)
  {
    Serial.print(command[j]);
    Serial.print("  ");
  }
  Serial.println(" ");
}
