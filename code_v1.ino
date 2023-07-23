int potentiometer = 12;
int led = 15;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(potentiometer, INPUT);
ledcSetup(ledChannel, freq, resolution);
ledcAttachPin(led, ledChannel);

}

void loop() {
  // put your main code here, to run repeatedly:
int a = analogRead(potentiometer);
int brightness = map(a, 0, 1023, 0, 255);
ledcWrite(ledChannel, brightness);
Serial.println(brightness);
}
