#define kiirendus A0
#define relee 2

int baseline;
int meassured;


void setup() {
  Serial.begin(9600);
  pinMode(relee, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(relee, LOW);
  pinMode (kiirendus, INPUT);
  
  for(int i = 0; i < 10; i++){
    baseline += analogRead(kiirendus);
  }
  baseline = baseline/10;
  Serial.println(baseline);
  digitalWrite(10, HIGH);
}

void loop() {
  meassured = abs(analogRead(kiirendus) - baseline);
  Serial.println(meassured);
  if (meassured > 20){
    digitalWrite(relee, HIGH);
    delay(100);
    digitalWrite(relee, LOW);
  }
  

}
