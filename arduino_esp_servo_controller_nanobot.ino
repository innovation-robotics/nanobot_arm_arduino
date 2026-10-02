#include <Servo.h>
// #include <SoftwareSerial.h>

Servo servos[4]; // 0-4 Arm, 5 Gripper
const int servo_pins[4] = {3, 5, 6, 9};
// SoftwareSerial(RX_Pin, TX_Pin)
// SoftwareSerial EspSerial(12, 13); // RX = Pin 12, TX = Pin 13
//gripper 90 open
//gripper 177 close
const int offset[4] = {90, 150, 150, 177}; 

void setup() 
{
  Serial.begin(115200);
  // Serial.println("Arduino Uno Ready!");

  // ESP32 Communication
  // EspSerial.begin(9600);
  
  for (int i = 0; i < 4; i++) {
    servos[i].attach(servo_pins[i]);
  }
  delay(1000);

  for (int i = 0; i < 4; i++)
  {
    servos[i].write(offset[i]); // Gripper Servo
  }
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Serial.print("Received from ESP32: ");
    // Serial.println(input);
    
    // Parse Arm Command: <deg1,deg2,deg3,deg4,deg5>
    if (input.startsWith("<") && input.endsWith(">")) {
      input = input.substring(1, input.length() - 1); // Strip < and >
      
      int start_idx = 0;
      int joint_idx = 0;
      for (int i = 0; i <= input.length(); i++) {
        if (i == input.length() || input.charAt(i) == ',') {
          String val_str = input.substring(start_idx, i);
                    
          // Serial.println(start_idx);
          // Serial.println(i);
          // Serial.println(val_str);

          int angle = offset[joint_idx] + val_str.toInt();
          if(joint_idx<3)
          {
            servos[joint_idx].write(angle);
            joint_idx++;
          }
          start_idx = i + 1;
        }
      }
      // Serial.println("--------------");
    }

    // Parse Gripper Command: >deg>
    else if (input.startsWith(">") && input.endsWith(">")) {
      input = input.substring(1, input.length() - 1); // Strip >
      int angle = input.toInt();
      angle = offset[3] + angle;
      servos[3].write(angle); // Gripper Servo
    }
  }
}