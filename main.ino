#include <LiquidCrystal.h>
#include <Servo.h>

const int buzzerPin = A5;
const int echoPin = 9;
const int trigPin = 10;
const int LedR = 2;
const int LedJ = 8;
const int LedV = 13;

Servo servo_aiguillePin; //Le Servo est branché sur la pin 3
long distance=0;
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

void setup() {  
  lcd.begin(16,2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);	
  servo_aiguillePin.attach(3);
  Serial.begin(9600);
}

void loop() {  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Le trigger reçoit un courant 5V pendant 10 microsecondes
  // Ce qui génère une onde ultrason.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Grâce à echoPin et la fonction pulseIn, 
  // On lit la valeur du temps nécessaire à l'onde sonore pour réaliser un aller-retour
  long temps = pulseIn(echoPin, HIGH);
  // On calcule la distance sachant la vitesse du son est d'environ 340 m/s dans l'air
  distance = temps/29/2; //vitesse du son considérée à 347 m/s donc 0,0347 m/ms
  // Notez que la vitesse du son dépend des coniditions atmosphériques
  // Par exemple : la température et l'humidité.
  componentsHandler(distance);
  lcdHandler(distance);
  Serial.print("Distance : ");
  Serial.println(distance);
  delay(500);
}
  
// Gère l'affichage lcd
// Long d représente la distance en cm
void lcdHandler(long d){
  // On efface ce qu'il y a d'écrit sur l'écran avant d'afficher les nouvelles valeurs
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print(d);
  lcd.print("cm");
}

// Gère si les leds, le buzzer doivent être allumés ou éteints
// Gère également l'angle du servo moteur (on simule une aiguille)
// long d représente la distance en cm
void componentsHandler(long d){
  if(d<=5){
    // La distance est inférieur ou égale à 5 cm
    // On allume toutes les LEDs et le buzzer sonne fort
    // On indique l'angle qui représente la première zone de la jauge sur le servo moteur
    digitalWrite(LedR,HIGH);
    digitalWrite(LedJ,HIGH);
    digitalWrite(LedV,HIGH);
    analogWrite(buzzerPin, 255);
    servo_aiguillePin.write(45);
  }else if(d<=15){
    // La distance est inférieur ou égale à 15 cm et supérieur à 5 cm
    // On allume les LEDs jaune et verte 
    // La LED rouge est éteinte et le buzzer ne sonne pas
    // On indique l'angle qui représente la deuxième zone de la jauge sur le servo moteur
    digitalWrite(LedR,LOW);
    digitalWrite(LedJ,HIGH);
    digitalWrite(LedV,HIGH);
    analogWrite(buzzerPin, 0);
    servo_aiguillePin.write(90);
  }else{
    // La distance est supérieur à 15 cm
    // On allume la LED verte 
    // Les LEDs rouge et jaune sont éteintent et le buzzer ne sonne pas
    // On indique l'angle qui représente la troisième zone de la jauge sur le servo moteur
    digitalWrite(LedR,LOW);
    digitalWrite(LedJ,LOW);
    digitalWrite(LedV,HIGH);
    analogWrite(buzzerPin, 0);
    servo_aiguillePin.write(135);
  }
}

  
