/*
 * Kollane tuli põleb siis kui oodatakse koputamis rütmi.
 * Punane tuli annab teada, et on toimunud viga. 4 vilgatust siis kui rütm ei ole sobiv,
 * nt on liialt lühike. 20 kiiret vilgatust kui rütmid ei klapi.
 * Esimene roheline tuli annab märku, et salarütm on salvestatud. Teine roheline tuli 
 * annab märku kui on lõpetatud sissesaamis koputuse katse.
 * Relee tuli läheb põlema siis kui koputuste rütmid kattuvad.
 * 
 */


#define kiirendus A0
#define ledk 10
#define ledp 9
#define ledr1 7
#define ledr2 6
#define relee 2

#define snivoo 15 //50 kui otse andurit koputada, 15 kui lauapeale koputada
#define ooteaeg 150000
#define salvestusaeg 10000000
#define mnivoo 0.1 //Sobivus nivoo

long starttime;
long mtime;

int baseline;
int meassured;
int lm_meassured = 0;

long knockcode[10];
float kc_ratios[9];
long knocking[10];
float k_ratios[9];
float kc_k_dif[9];
short index = 0;

void seeker(int level, long pause, long rtime, long list[]);
void find_ratios(long list[], float ratios);
boolean check(long data[]);
void diffs(float l1[], float l2[]);
boolean approve(float errors[], float aerror);

void setup() {
  Serial.begin(9600);
  pinMode(relee, OUTPUT);
  pinMode(ledk, OUTPUT);
  pinMode(ledp, OUTPUT);
  pinMode(ledr1, OUTPUT);
  pinMode(ledr2, OUTPUT);
  pinMode(kiirendus, INPUT);
  for(int i = 0; i < 10; i++){
    baseline += analogRead(kiirendus);
  }
  baseline = baseline/10;

  Serial.println("Salvesta kood");
  seeker(snivoo, ooteaeg, salvestusaeg, knockcode);
  digitalWrite(ledr1,HIGH);
}

void find_ratios(long list[], float ratios[]){
  Serial.println("Suhete leidmine");
  for (int i = 0; i < 8; i++){
    if (list[i+1] != 0){
      ratios[i] = (double)list[i]/(double)list[i+1];
      Serial.println(String(ratios[i]) + " - " + String(i));
    }
    else{
      Serial.println("Appi");
      ratios[i] = 0;
    }
  }
  for (int i = 0; i < 9; i++){
    Serial.println(list[i]);
  }
}

void diffs(float l1[], float l2[], float dif[]){
  Serial.println("Kahe rütmi erinevused");
  for (int i = 0; i < 9; i++){
    dif[i] = abs(l1[i] - l2[i]); 
  }
  for (int i = 0; i < 9; i++){
    Serial.println(dif[i]);
  }
}

boolean approve(float errors[], float aerror){
  for (int i = 0; i < 9; i++){
    Serial.println(errors[i] > aerror);
     if (errors[i] > aerror){
      return false;
     }
  }
  return true;
}



boolean check(long data[]){
  if (data[1] == 0){
    for (int i = 0; i < 4; i++){
      digitalWrite(ledp,HIGH);
      delay(100);
      digitalWrite(ledp,LOW);
      delay(100);
    }
    
    return false;
  }
  return true;
}
void seeker(int level, long pause, long rtime, long list[]){
  digitalWrite(ledk, HIGH);
  while(true){
    if (abs(analogRead(kiirendus) - baseline) > level){
      if (index != 0){
        mtime = micros();
        if (mtime - list[(index - 1)] > pause){
          Serial.println(String(abs(analogRead(kiirendus) - baseline)) + " - " + String(index));
          list[index] = mtime;
          index ++;
        }
      }
      else{
        starttime = micros();
        list[index] = starttime;
        index ++;
      }
    }
    if (index > 9){
      index = 0;
      if (check(list)){
        break;
      }
    }
    if (index != 0){
      if ((micros() - starttime) > rtime){
        //Serial.println("täidan alates - " + String(index) );
        for (int i = index; i < 10; i++){
          list[i] = 0;
        }
        index = 0;
        if (check(list)){
          break;
        }
      }
    }
  }
  digitalWrite(ledk, LOW);
  for (int i = 0; i < 10; i++){
    Serial.println(list[i]);
  }
}


void loop() {
  Serial.println("Arva kood");
  digitalWrite(ledr2,LOW);
  seeker(snivoo, ooteaeg, salvestusaeg, knocking);
  digitalWrite(ledr2,HIGH);
  Serial.println("Analüüsi algus");
  find_ratios(knockcode, kc_ratios);
  find_ratios(knocking, k_ratios);
  diffs(kc_ratios, k_ratios, kc_k_dif);

  if (approve(kc_k_dif, mnivoo)){
    digitalWrite(relee, HIGH);
    delay(5000);
    digitalWrite(relee, LOW);
  }
  else{
    for (int i = 0; i < 20; i++){
      digitalWrite(ledp,HIGH);
      delay(50);
      digitalWrite(ledp,LOW);
      delay(50);
    }
  }
}
