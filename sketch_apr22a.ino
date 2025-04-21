#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Bonezegei_DHT11.h>

// Definisikan pin sensor DHT
#define DHTPIN 2          // Pin data DHT22
#define DHTTYPE DHT22     // Tipe sensor DHT22

Bonezegei_DHT11 dht(2);  // Inisialisasi sensor DHT22

// Inisialisasi LCD I2C (alamat 0x27 adalah alamat umum untuk LCD I2C)
LiquidCrystal_I2C lcd(0x27,20,4);
// Definisikan pin untuk relay
#define RELAY_PIN 7

void setup() {
  lcd.init();
  // Inisialisasi komunikasi serial dan LCD
  Serial.begin(9600);
  lcd.setBacklight(HIGH);  // Menyalakan lampu latar LCD
  dht.begin();      // Inisialisasi sensor DHT22
  
  // Inisialisasi pin relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Pastikan relay mati saat awal

  // Menampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("Suhu & Kelembaban");
  delay(2000);  // Tunggu 2 detik
}

void loop() {
  // Membaca suhu dan kelembaban dari sensor DHT22
  float humidity, temperature;
  if(dht.getData()) {
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
  }

  // Cek jika pembacaan gagal
  if (isnan(humidity) || isnan(temperature)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gagal Membaca");
    lcd.setCursor(0, 1);
    lcd.print("Sensor DHT22");
    delay(2000);
    return;
  }

  // Tampilkan data suhu dan kelembaban di LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Kelembaban: ");
  lcd.print(humidity);
  lcd.print(" %");

  // Cek apakah suhu dibawah 22°C dan kelembaban dibawah 50%
  if (temperature < 28 || humidity < 50) {
    digitalWrite(RELAY_PIN, LOW);  // Nyalakan relay
  } else {
    digitalWrite(RELAY_PIN, HIGH);   // Matikan relay
  }
  delay(2000);
}
