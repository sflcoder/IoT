void turn_servo(int val, Servo servo){ 
  int last_value = servo.read();
  Serial.println(servo.read()); 
  while (last_value!=val){
      servo.write(last_value);
      delay(15);
      if (last_value>val) {
        last_value-=1;
      }
      else {
        last_value+=1;
      }
  }
}

void servo_ext(String data_from_node){
  if (data_from_node.indexOf("app/ext:")>0) {
      s.write(3);
      Serial.println(data_from_node);
      int angle = data_from_node.substring(23,26).toInt();
      Serial.println(angle);
      turn_servo(angle,servo_extender);
    }
  else{
      s.write(2);
      Serial.println("Error in data");
    }
}

void servo_rot(String data_from_node){
  if (data_from_node.indexOf("app/rot:")>0) {
      s.write(4);
      Serial.println(data_from_node);
      int angle = data_from_node.substring(23,26).toInt();
      Serial.println(angle);
      turn_servo(angle,servo_rotate);
    }
  else{
      s.write(2);
      Serial.println("Error in data");
    }
}

void servo_grb(String data_from_node){
  if (data_from_node.indexOf("app/grb:")>0) {
      s.write(5);
      Serial.println(data_from_node);
      int angle = data_from_node.substring(23,26).toInt();
      Serial.println(angle);
      turn_servo(angle,servo_grabber);
    }
  else{
      s.write(2);
      Serial.println("Error in data");
    }
}


void servo_hgt(String data_from_node){
  if (data_from_node.indexOf("app/hgt:")>0) {
      s.write(6);
      Serial.println(data_from_node);
      int angle = data_from_node.substring(23,26).toInt();
      Serial.println(angle);
      turn_servo(angle,servo_height);
      }
  else{
      s.write(2);
      Serial.println("Error in data");
      }
}
