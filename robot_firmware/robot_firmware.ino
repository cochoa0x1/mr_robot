
#define echo_pin 8
#define echo_trigger_pin 7

#define speed_l 9
#define fwd_l 11
#define rev_l 10

#define speed_r 6
#define fwd_r 5
#define rev_r 4


unsigned long time;
unsigned long sensor_time;

struct __attribute__ ((packed)) motorPacket {
  bool left_positive;
  uint8_t speed_left;
  bool right_positive;
  uint8_t speed_right;
};





void setup() {

  pinMode(echo_pin, INPUT);
  pinMode(echo_trigger_pin, OUTPUT);

  //left motor
  pinMode(speed_l,OUTPUT);
  pinMode(fwd_l,OUTPUT);
  pinMode(rev_l,OUTPUT);

  //right motor
  pinMode(speed_r,OUTPUT);
  pinMode(fwd_r,OUTPUT);
  pinMode(rev_r,OUTPUT);
  
  Serial.begin(115200);

  time=millis();
  sensor_time =0;
}


double echo_read(double ec_pin, double tr_pin){
  //performs an echo read and returns the results
  
  digitalWrite(tr_pin, LOW);
  delayMicroseconds(2);

  //send a 10 ms pulse
  digitalWrite(tr_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(tr_pin, LOW);
  
  return pulseIn(ec_pin, HIGH)/58.2;
}


void loop() {

  //shutdown the motors if we haven't received a motor packet in the last 500 milliseconds
  if(millis()-time > 200){
      digitalWrite(fwd_l,LOW);
      digitalWrite(rev_l,LOW);
      digitalWrite(fwd_r,LOW);
      digitalWrite(rev_r,LOW);
  }
//
//   //read the ultrasonic

if(millis() - sensor_time > 50){
    double d = echo_read(echo_pin, echo_trigger_pin);
    Serial.println(d);
    sensor_time = millis();
 }


     
  if(Serial.available() >= 5){
    
    char cmd = Serial.read();


    if (cmd == 'M'){

      time = millis();

      //run the motors
      motorPacket  m;
      int len= sizeof(m);
      char buff[len];
      
      for(int i =0; i< len; i++){
        buff[i] = Serial.read();
      }


      memcpy(&m,&buff,len);

      Serial.flush();

      analogWrite(speed_l, m.speed_left);
      analogWrite(speed_r, m.speed_right);

      if( m.left_positive == true ){
        digitalWrite(fwd_l,HIGH);
      }else{
        digitalWrite(rev_l,HIGH);
      }

      if( m.right_positive == true){
        digitalWrite(fwd_r,HIGH);
      }else{
        digitalWrite(rev_r,HIGH);
      }
   }
  }

}
