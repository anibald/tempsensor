// Programa : Sensor de temperatura para multiplos sensores DS18B20
// Autor : Aníbal Deboni Neto (anibaldeboni@gmail.com)

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define ONE_WIRE_BUS 3 // Porta do pino de sinal do DS18B20

OneWire oneWire(ONE_WIRE_BUS); // Define uma instancia do oneWire para comunicacao com o sensor
DallasTemperature sensors(&oneWire); // Cria o objeto para interface com os sensores
DeviceAddress Probes[10]; // Variável para armazenamento dos endereços dos sensores, configurada para até 10 sensores, no entato é só alterar o valor caso necessite de mais
LiquidCrystal lcd(12, 11, 7, 6, 5, 4); // Inicia o LCD

int DevicesFound = 0; // Variável para armazenamento do número total de sensores

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
  DevicesFound = sensors.getDeviceCount();
  // Localiza e mostra enderecos dos sensores
  Serial.print("DallasTemperature version: ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Encontrei ");
  Serial.print(DevicesFound, DEC);
  Serial.println(" sensores.");

  for (int i = 0; i < DevicesFound; i++)
    if (!sensors.getAddress(Probes[i], i))
      Serial.println("Não encontrei a sonda " + i);

  // mostra o endereço encontrado no bus
  for (int i = 0; i < DevicesFound; i++) {
    Serial.print("Sonda " + (String)i + " Endereco: ");
    printAddress(Probes[i]);
    Serial.println();
    sensors.setResolution(Probes[i], 10);
  }
  Serial.println();
  Serial.println();
  lcd.begin(16, 2);

  // Mostra mensagem de inicio no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Procurando...");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print(DevicesFound);
  lcd.print(" encontrados!");
  delay(2000);
}


void printAddress(DeviceAddress deviceAddress) { // Função para mostrar o endereço de um dispositivo
  for (uint8_t i = 0; i < 8; i++) {
    // adiciona um zero, caso seja necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop() {
  // Requisita leitura dos sensores
  sensors.requestTemperatures();
  float temp[10]; // variável de retorno da temperatura
  for (int i = 0; i < DevicesFound; i++) { // loop para ler a temperatura de todos os sesores encontrados
    temp[i] = sensors.getTempC(Probes[i]);
    // manda reposta do sensor para a porta serial
    Serial.print("Sonda ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(temp[i]);
  }
  // Mostra dados no LCD
  for (int i = 0; i < DevicesFound; i++) {
    int Probe = 0;
    Probe = i + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sonda ( )");
    lcd.setCursor(7, 0);
    lcd.print(Probe);

    lcd.setCursor(0, 1);
    lcd.print("Temp:      ");
    // Adiciona o simbolo de grau
    lcd.write(223);
    lcd.print("C");
    lcd.setCursor(6, 1);
    lcd.print(temp[i]);
    delay(2000);
  }
}

