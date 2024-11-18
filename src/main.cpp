#include <Arduino.h>
#include <ThingerESP32.h>

#define USERNAME "rm99565"
#define DEVICE_ID "Ampz_ESP32_01"
#define DEVICE_CREDENTIAL "_edWIoRnAY9@6J#n"
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#define SSID "Wokwi-GUEST" 
#define SSID_PASSWORD ""

#define LED_PORT 23
#define PHOTORESISTOR_PORT 12
#define PIR_PORT 4
#define POTENTIOMETER_PORT 34

// Define a tensão de alimentação do circuito
#define VOLTAGE_LEVEL 110

int luzLigada;
int presencaDetectada;
int valorCorrente, valorPotenciometro;

void setup() {
  Serial.begin(115200);
  thing.add_wifi(SSID, SSID_PASSWORD);
  delay(500);

  pinMode(LED_PORT, OUTPUT);
  pinMode(PHOTORESISTOR_PORT, INPUT);
  pinMode(PIR_PORT, INPUT);
  pinMode(POTENTIOMETER_PORT, INPUT);

  thing["iluminacao"] >> outputValue(luzLigada);
  thing["presenca"] >> outputValue(presencaDetectada);
  thing["corrente"] >> outputValue(valorCorrente);
  thing["tensao"] >> outputValue(VOLTAGE_LEVEL);
}

void leituraCorrente() {
  valorPotenciometro = analogRead(POTENTIOMETER_PORT);
  int amperagemMaxima = VOLTAGE_LEVEL == 110 ? 10 : 20;

  valorCorrente = map(valorPotenciometro, 0, 4095, 0, amperagemMaxima);
  Serial.print("Corrente elétrica [");
  Serial.print(VOLTAGE_LEVEL);
  Serial.print("V]: ");
  Serial.print(valorCorrente);
  Serial.println(" A");
}

void leituraLuminosidade() {
  luzLigada = !digitalRead(PHOTORESISTOR_PORT);
  Serial.print("Luz está ligada: ");
  
  if(luzLigada == HIGH) {
    Serial.println("Sim");
  } else {
    Serial.println("Não");
  }
}

void leituraPresenca() {
  presencaDetectada = digitalRead(PIR_PORT);
  Serial.print("Presença detectada: ");
  
  if(presencaDetectada == HIGH) {
    Serial.println("Sim");
  } else {
    Serial.println("Não");
  }
}

void loop() {
  leituraPresenca();
  leituraLuminosidade();
  leituraCorrente();
  
  bool desperdicioEnergia = presencaDetectada == LOW && (luzLigada == HIGH || valorCorrente > 0);
  digitalWrite(LED_PORT, desperdicioEnergia ? HIGH : LOW);

  thing.handle();
  
  delay(1000); 
}
