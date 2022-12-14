
const int buttons[3] ={22,23,24}; 
int numMap =0;
  int num;
void setup() {

  Serial.begin(9600);
  
  
//intialize GPIO button inputs
   int i;
   for(i=22; i<25; i++){
      pinMode(i, INPUT_PULLUP);
   }

}

void loop() {
  // put your main code here, to run repeatedly:
  num = button_slot(buttons);  //number pressed in buttons
  if(num!= numMap){// if the slot value changed
    numMap = num; //puts tile nuber value in number map
    Serial.println(num);
  delay(100);
  }
}
int button_slot(int slot[]){
  int val[3],i;
for(i=0;i<3;i++){
  if(digitalRead(slot[i])==LOW){
  val[i]=1;
  }
  else{
  val[i]=0;
  }
} 
  //Serial.print(val[2]);
  //Serial.print(val[1]);
  //Serial.print(val[0]);
  //Serial.println("...");
  return (val[0]+val[1]*2+val[2]*4);
} 
