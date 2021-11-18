#include <Servo.h>
#include <Adafruit_NeoPixel.h>


//set pinnums
//trig : echo 
//4:5 = recycling obj
//6:7 = can_full
//8:9 = bot_full
const unsigned int trigs[] = { 4, 6, 8 };
const unsigned int echos[] = { 5, 7, 9 };
const unsigned int lights = 13;
const unsigned int buzzer = 12;

//set values
unsigned int         ref_dist[] = { 0, 0, 0 };
const unsigned int  numpixels = 16;
const int pet_close = 88;   const int pet_open = 145;
const int can_close = 98;   const int can_open = 50;
const int guard_on = 100;   const int guard_off = 45;
const int gar_dis = 30;   const int obj_dis =30;
Servo srv_pet, srv_can, srv_grd;
Adafruit_NeoPixel pixels(numpixels, lights, NEO_GRB + NEO_KHZ800);
const int notes[] = { 262, 294, 330, 349, 392, 440, 494, 680 };
const char codes[] = "cdefgabc";
const char* songs_no = "2";
const char* songs_yes = "878";
const char* songs_full = "5566553";
const char* songs_start = "7070700707070008070706050607";

//set flags
int flag = 4;
char s_flag = 'N';
int neo_flag =0;


//func tree
/*
-setup()
	|- set_ref_dist()
		|- get_dist()
		|- trig()
		|- get_echoTime()
-loop()
	|- Read_start_ch()
    |- sound_buzzer()
	|- lights_on()
	|- check_garbage_full()
		|- check_distance()
		|- sound_buzzer()
       |- player()
	|- check_obj()
		|- check_distance()
	|- read_obj_msg()
		|- lights_off()
		|- actvate_can()
		|- activate_bot
		|- sound_buzzer()
       |- player()
*/
void set_ref_dist(int id);
unsigned int get_dist(unsigned int index);
void trig(unsigned int pinTrig);
unsigned long get_echoTime(unsigned int pinEcho);
unsigned int Read_start_ch(void);
unsigned int check_garbage_full(void);
unsigned int check_obj(void);
unsigned int read_obj_msg(void);
unsigned int check_distance(int dis,int id);
//void sound_buzzer(int i);
void activate_bot(void);
void actvate_can(void);
void lights_on(void);
void lights_off(void);
void sound_buzzer(char* song);
void player(char ch, int tm);

void setup() {
  //serial communication
    Serial.begin(9600);
    
  //set ultrasonic
    for (int i = 0; i < 3; i++) {
        pinMode(trigs[i], OUTPUT);  pinMode(echos[i], INPUT);
    }
  
  //save reference distance
    for(int i = 0; i < 3; i++){
      set_ref_dist(i);
  }

  
  //set pixel 
  pixels.begin();
  lights_on();
  delay(500);
  lights_off();

  sound_buzzer(songs_start);
  //set servo
    srv_pet.attach(11);  srv_pet.write(pet_close); delay(100);
    srv_can.attach(10);  srv_can.write(can_close); delay(100);
    srv_grd.attach(3);  srv_grd.write(guard_on); delay(100);
  
  
}

void loop() {
  if(flag==4){
    flag = Read_start_ch();

  }
  if(flag==0){
    flag=check_garbage_full();
  }
  
  if(flag==1){
    flag = check_obj();
  }
  
  if(flag==2){
    flag = read_obj_msg();
	
  }
}

//setting reference distance
void set_ref_dist(int id){
    int sum = 0;

    for (int i = 0; i < 100; i++)
        sum = sum + get_dist(id);

        
    ref_dist[id] = sum / 100;

}

//get distance
unsigned int get_dist(unsigned int index){
    trig(trigs[index]);

    unsigned long echo_time = get_echoTime(echos[index]);

    if (echo_time != 0)
        return 17 * echo_time / 100;
    else
        return 0;
}

//trig
void trig(unsigned int pinTrig) {
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrig, LOW);
}

//wait till trig back
unsigned long get_echoTime(unsigned int pinEcho){
    unsigned long echoTime = 0;

    while (digitalRead(pinEcho) == LOW);
    unsigned long startTime = micros();
    while (digitalRead(pinEcho) == HIGH);

    echoTime = micros() - startTime;

    if (echoTime >= 240 && echoTime <= 23000)
        return echoTime;
    else
        return 0;
}

unsigned int Read_start_ch(void) {
    if(Serial.available()>0) {
        Serial.read();
    return 0;
    }
  else return 4;
}

unsigned int check_garbage_full(void){
  int send_gar_msg;
  
  if(s_flag == 'C'){
    delay(500);
    //check can_gar
    send_gar_msg = check_distance(gar_dis,2);
    
  }
  else if(s_flag == 'B'){
    //check bot_gar
    delay(500);
    send_gar_msg = check_distance(gar_dis,1);
  }
  else  send_gar_msg = 0;
    
  if(send_gar_msg==0){
    Serial.print('N');
    lights_on();
    return 1;
   } else {
    Serial.print(s_flag);
	sound_buzzer(songs_full);
    lights_on();
    return 1;
  }
}

unsigned int check_obj(void){
  if(check_distance(obj_dis,0)==1){
    delay(500);
    Serial.print('P');
    
    return 2;
  }
  else return 1;
}

unsigned int read_obj_msg(void){
  if (Serial.available() > 0) {
    char ch = Serial.read();
	
    //run can servo
    if (ch == 'C' ) {
      actvate_can();
	  sound_buzzer(songs_yes);
      s_flag='C';
      lights_off();
      return 0;
    }
    //run bottle servo
    if (ch == 'B') {
      activate_bot();
	  sound_buzzer(songs_yes);
      s_flag='B';
      lights_off();
      return 0;
    }
    //not bottle and can
    if (ch == 'N') {
      sound_buzzer(songs_no);
      delay(5000);
      s_flag='N';
      return 0;
    }
    
    }
  return 2;
}

unsigned int check_distance(int dis,int id){
  unsigned int dist2obj ;
  int sum = 0;

  for (int i = 0; i < 100; i++)
     sum = sum + get_dist(id);
    
  dist2obj = sum / 100;
  
  if(dist2obj > ref_dist[id] + dis || dist2obj < ref_dist[id] - dis)  
    return 1;
  else return 0;
}


void activate_bot(void) {
    srv_grd.write(guard_off); delay(100);
    srv_pet.write(pet_open); delay(2500);
    srv_pet.write(pet_close); delay(100);
    srv_grd.write(guard_on); delay(100);
}

void actvate_can(void) {
    srv_grd.write(guard_off); delay(100);
    srv_can.write(can_open); delay(2500);
    srv_can.write(can_close); delay(100);
    srv_grd.write(guard_on); delay(100);
}

void lights_on(void){
	if(neo_flag==0){
		for(int i=0; i<numpixels; i++) { 
			pixels.setPixelColor(i, pixels.Color(255, 255, 255));
      pixels.show();
		}
		neo_flag=1;
	}else ;
}

void lights_off(void){
	if(neo_flag==1){
		pixels.clear(); // Set all pixel colors to 'off'
		pixels.show();
		neo_flag=0;
	}else ;
}

void sound_buzzer(char* song){

	for (int i=0;i<strlen(song);++i){
		player(song[i],100);
	}	
}

void player(char ch, int tm)
{
	if(ch >= '1' && ch <= '8')  {
		tone (buzzer, notes[(ch-'0')-1]);
		delay(tm);
	}
	else if(ch == '0'){
		delay(tm);
	}
	else;
	
	noTone(buzzer);
}
