#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial s(3, 2); //rx tx
Servo servo_rotate;
Servo servo_grabber;
Servo servo_height;
Servo servo_extender;

char temp_char;

void setup() {
  Serial.begin(115200);
  s.begin(115200);
  servo_extender.attach(8);servo_extender.write(90);
  servo_rotate.attach(9);  servo_rotate.write(90);
  servo_grabber.attach(10);  servo_grabber.write(90);
  servo_height.attach(11);  servo_height.write(90);

}

void loop() {
  String data_from_node = "";
  while (s.available() > 0){
    temp_char = s.read();
    data_from_node = data_from_node + temp_char;
  }
  if (data_from_node!=""){
    String substr = data_from_node.substring(19,22);
    if (substr == "ext"){
      servo_ext(data_from_node);
    }
    else if (substr == "rot"){
      delay(100);
      servo_rot(data_from_node);
    }
    else if (substr == "grb"){
      servo_grb(data_from_node);
    }
    else if (substr == "hgt"){
      servo_hgt(data_from_node);
    }
    else{
      s.write(2);
      Serial.println("Error in data");
    }
  }
}
