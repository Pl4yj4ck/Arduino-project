#include <dht_nonblocking.h>
#include <LiquidCrystal.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22
float temperature_old;
float temperature;
float humidity;
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Definizione di un simbolo personalizzato
byte nordsx1[8] = {
  0b00011,
  0b00100,
  0b00111,
  0b00100,
  0b00111,
  0b00101,
  0b00111,
  0b00101
};

byte norddx1[8] = {
  0b11000,
  0b00100,
  0b00100,
  0b00100,
  0b10100,
  0b10100,
  0b10100,
  0b10100
};

byte sudsx1[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00101,
  0b00111,
  0b00011,
  0b00001,
  0b00001
};

byte suddx1[8] = {
  0b10100,
  0b10100,
  0b10100,
  0b10100,
  0b11100,
  0b11000,
  0b10000,
  0b10000
};

byte nordsx2[8] = {
  0b00011,
  0b00100,
  0b00111,
  0b00100,
  0b00111,
  0b00100,
  0b00111,
  0b00100
};

byte norddx2[8] = {
  0b11000,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte sudsx2[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00100,
  0b00111,
  0b00011,
  0b00001,
  0b00001
};

byte suddx2[8] = {
  0b00100,
  0b10100,
  0b10100,
  0b10100,
  0b11100,
  0b11000,
  0b10000,
  0b10000
};


void setup(){
  pinMode(5, OUTPUT);
  Serial.begin(9600);

  lcd.begin(16,2);      // inizializzazione dell'LCD

  // creazione nuovi caratteri
  lcd.createChar(0, nordsx1);
  lcd.createChar(1, norddx1);
  lcd.createChar(2, sudsx1);
  lcd.createChar(3, suddx1);
  lcd.createChar(4, nordsx2);
  lcd.createChar(5, norddx2);
  lcd.createChar(6, sudsx2);
  lcd.createChar(7, suddx2);
  // Cancella il display
  resetta();
}

static bool measure_environment( float *temperature, float *humidity ){
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop(){
  temperature_old=temperature;
  analizza();
  if(temperature_old!=temperature){
  resetta();
  }
  if(temperature>21){
    digitalWrite(5, HIGH);
    disegno("temp_alta");
  } else{
    digitalWrite(5, LOW);
    disegno("temp_bassa");
  }
}

void analizza(){
  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true ){
    Serial.print( temperature, 1 );
    Serial.print( "C," );
    Serial.print( humidity, 1);
    Serial.println( "%" );
  }
}

void resetta(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Temperatura =");
  lcd.setCursor(3, 1);
  lcd.print(temperature,1);
  int lunghezza =strlen((int)temperature);
  lcd.setCursor(3+lunghezza,1);
  lcd.print((char)223);
  lcd.setCursor(4+lunghezza,1);
  lcd.print("C");
} 

void disegno(String i){
  if(i=="temp_alta"){
    lcd.setCursor(0, 0);
    lcd.write(byte(0));

    lcd.setCursor(1, 0);
    lcd.write(byte(1));

    lcd.setCursor(0, 1);
    lcd.write(byte(2));

    lcd.setCursor(1, 1);
    lcd.write(byte(3));
  }
  if(i=="temp_bassa"){
    lcd.setCursor(0, 0);
    lcd.write(byte(4));

    lcd.setCursor(1, 0);
    lcd.write(byte(5));

    lcd.setCursor(0, 1);
    lcd.write(byte(6));

    lcd.setCursor(1, 1);
    lcd.write(byte(7));
  }
}