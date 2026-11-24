
#include <Arduino.h>
#include <ESP32Servo.h>

// =====================================
// PIN CONFIGURATION
// =====================================

const uint8_t servoPins[4] = {
    18, 19, 21, 22};

const uint8_t BUTTON_PIN = 23;

// =====================================
// SERVO CONFIGURATION
// =====================================

Servo servos[4];

const uint16_t MIN_PULSE = 900;  // Down
const uint16_t MAX_PULSE = 2100; // Up

// =====================================
// TEST CONFIGURATION
// =====================================

const uint16_t STEP_DELAY = 500;
const uint16_t SERVO_DELAY = 1000;

// =====================================
// BUTTON DEBOUNCE CONFIGURATION
// =====================================

const uint16_t DEBOUNCE_DELAY = 50;

uint8_t lastButtonState = HIGH;
uint8_t stableButtonState = HIGH;

uint32_t lastDebounceTime = 0;

// =====================================
// BUTTON HANDLING
// =====================================

bool isButtonPressed()
{

  uint8_t buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState)
  {
    lastDebounceTime = millis();
    lastButtonState = buttonState;
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_DELAY)
  {

    if (buttonState != stableButtonState)
    {

      stableButtonState = buttonState;

      if (stableButtonState == LOW)
      {
        return true;
      }
    }
  }

  return false;
}

// =====================================
// SERVO MOVEMENT
// =====================================

void moveServo(uint8_t index, uint16_t pulse)
{

  servos[index].writeMicroseconds(pulse);

  Serial.print("Servo ");
  Serial.print(index + 1);
  Serial.print(" -> ");
  Serial.print(pulse);
  Serial.println(" us");

  delay(STEP_DELAY);
}

void initServos()
{

  for (uint8_t i = 0; i < 4; i++)
  {
    servos[i].attach(servoPins[i], MIN_PULSE, MAX_PULSE);
    Serial.print("Servo ");
    Serial.print(i + 1);
    Serial.print(" attached to pin ");
    Serial.println(servoPins[i]);
  }
}

// =====================================
// SINGLE SERVO TEST
// =====================================

void testServo(uint8_t index)
{
  Serial.println();
  Serial.print("Testing Servo ");
  Serial.println(index + 1);

  // Открыть (Up)
  servos[index].writeMicroseconds(2100);
  Serial.println("Up - 2100 us");
  delay(STEP_DELAY);

  // Закрыть (Down)
  servos[index].writeMicroseconds(900);
  Serial.println("Down - 900 us");
  delay(STEP_DELAY);

  servos[index].detach();
  delay(STEP_DELAY);

  Serial.print("Servo ");
  Serial.print(index + 1);
  Serial.println(" test completed");

  delay(SERVO_DELAY);
}

// =====================================
// FULL SERVO TEST
// =====================================

void runServoTest()
{

  initServos();

  Serial.println();
  Serial.println("================================");
  Serial.println("       SERVO TEST STARTED");
  Serial.println("================================");

  for (uint8_t i = 0; i < 4; i++)
  {
    testServo(i);
  }

  Serial.println();
  Serial.println("================================");
  Serial.println("       SERVO TEST FINISHED");
  Serial.println("================================");
}

// =====================================
// SETUP
// =====================================

void setup()
{

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // ESP32 PWM timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  initServos();

  Serial.println();
  Serial.println("================================");
  Serial.println("        ESP32 SERVO TESTER");
  Serial.println("================================");
  Serial.println("Servos initialized");
  Serial.println("Press START button to test");
}

// =====================================
// MAIN LOOP
// =====================================

void loop()
{

  if (isButtonPressed())
  {
    runServoTest();
  }
}
