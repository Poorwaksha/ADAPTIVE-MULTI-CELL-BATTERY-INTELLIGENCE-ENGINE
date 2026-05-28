// ==========================================================
// ADAPTIVE MULTI-CELL BATTERY INTELLIGENCE ENGINE
// PROFESSIONAL INTERNSHIP PROJECT
// ==========================================================

#include <LiquidCrystal.h>

// ==========================================================
// LCD CONNECTIONS
// RS, E, D4, D5, D6, D7
// ==========================================================

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ==========================================================
// BATTERY CELL INPUT PINS
// ==========================================================

const int cellPins[4] = {A0, A1, A2, A3};

// ==========================================================
// LED + BUZZER PINS
// ==========================================================

const int GREEN_LED  = 8;
const int YELLOW_LED = 9;
const int RED_LED    = 10;

const int BUZZER = 7;

// ==========================================================
// BATTERY VARIABLES
// ==========================================================

float cellVoltage[4];

float avgVoltage;
float packVoltage;

float maxVoltage;
float minVoltage;

float imbalance;

int strongestCell;
int weakestCell;

String healthState;

// ==========================================================
// READ BATTERY CELLS
// ==========================================================

void readCells() {

  for (int i = 0; i < 4; i++) {

    int adc = analogRead(cellPins[i]);

    // Simulate lithium cell voltage
    // Range: 3.0V -> 4.2V

    cellVoltage[i] =
      map(adc, 0, 1023, 300, 420) / 100.0;
  }
}

// ==========================================================
// ANALYZE BATTERY PACK
// ==========================================================

void analyzeBattery() {

  packVoltage = 0;

  maxVoltage = cellVoltage[0];
  minVoltage = cellVoltage[0];

  strongestCell = 0;
  weakestCell = 0;

  for (int i = 0; i < 4; i++) {

    packVoltage += cellVoltage[i];

    // Strongest Cell

    if (cellVoltage[i] > maxVoltage) {

      maxVoltage = cellVoltage[i];

      strongestCell = i;
    }

    // Weakest Cell

    if (cellVoltage[i] < minVoltage) {

      minVoltage = cellVoltage[i];

      weakestCell = i;
    }
  }

  avgVoltage = packVoltage / 4.0;

  imbalance =
    ((maxVoltage - minVoltage) / avgVoltage) * 100.0;
}

// ==========================================================
// BATTERY HEALTH CLASSIFICATION
// ==========================================================

void classifyHealth() {

  // Reset outputs

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  noTone(BUZZER);

  // ==========================================
  // HEALTHY
  // ==========================================

  if (imbalance <= 2) {

    healthState = "HEALTHY";

    digitalWrite(GREEN_LED, HIGH);
  }

  // ==========================================
  // MINOR IMBALANCE
  // ==========================================

  else if (imbalance <= 5) {

    healthState = "MINOR";

    digitalWrite(YELLOW_LED, HIGH);
  }

  // ==========================================
  // CRITICAL IMBALANCE
  // ==========================================

  else if (imbalance <= 10) {

    healthState = "CRITICAL";

    digitalWrite(RED_LED, HIGH);
  }

  // ==========================================
  // PACK FAILURE
  // ==========================================

  else {

    healthState = "FAILURE";

    digitalWrite(RED_LED, HIGH);

    tone(BUZZER, 1000);
  }
}

// ==========================================================
// LCD DISPLAY
// ==========================================================

void updateLCD() {

  lcd.clear();

  // ==========================================
  // FIRST ROW
  // ==========================================

  lcd.setCursor(0, 0);

  lcd.print("AVG:");

  lcd.print(avgVoltage, 2);

  lcd.print(" ");

  if (healthState == "HEALTHY") {

    lcd.print("OK");
  }

  else if (healthState == "MINOR") {

    lcd.print("MIN");
  }

  else if (healthState == "CRITICAL") {

    lcd.print("CRT");
  }

  else {

    lcd.print("FAIL");
  }

  // ==========================================
  // SECOND ROW
  // ==========================================

  lcd.setCursor(0, 1);

  lcd.print("W:");

  lcd.print(weakestCell + 1);

  lcd.print(" S:");

  lcd.print(strongestCell + 1);

  lcd.print(" ");

  lcd.print(imbalance, 1);

  lcd.print("%");
}

// ==========================================================
// PROFESSIONAL SERIAL DASHBOARD
// ==========================================================

void serialDashboard() {

  Serial.println();
  Serial.println("==================================================");
  Serial.println("     ADAPTIVE BATTERY INTELLIGENCE ENGINE");
  Serial.println("==================================================");

  for (int i = 0; i < 4; i++) {

    // ==========================================
    // STATE OF CHARGE %
    // ==========================================

    float soc =
      ((cellVoltage[i] - 3.0) / 1.2) * 100.0;

    // Limit values

    if (soc < 0) soc = 0;

    if (soc > 100) soc = 100;

    Serial.print("Cell ");
    Serial.print(i + 1);

    Serial.print(" : ");

    Serial.print(cellVoltage[i], 2);

    Serial.print("V");

    Serial.print("   SoC: ");

    Serial.print(soc, 0);

    Serial.print("%");

    // ==========================================
    // STRONGEST / WEAKEST TAGS
    // ==========================================

    if (i == strongestCell) {

      Serial.print("   << STRONGEST");
    }

    if (i == weakestCell) {

      Serial.print("   << WEAKEST");
    }

    Serial.println();
  }

  Serial.println("--------------------------------------------------");

  Serial.print("Pack Voltage      : ");

  Serial.print(packVoltage, 2);

  Serial.println("V");

  Serial.print("Average Voltage   : ");

  Serial.print(avgVoltage, 2);

  Serial.println("V");

  Serial.print("Highest Cell Volt : ");

  Serial.print(maxVoltage, 2);

  Serial.println("V");

  Serial.print("Lowest Cell Volt  : ");

  Serial.print(minVoltage, 2);

  Serial.println("V");

  Serial.print("Voltage Imbalance : ");

  Serial.print(imbalance, 2);

  Serial.println("%");

  Serial.println("--------------------------------------------------");

  Serial.print("Strongest Cell    : Cell ");

  Serial.println(strongestCell + 1);

  Serial.print("Weakest Cell      : Cell ");

  Serial.println(weakestCell + 1);

  Serial.println("--------------------------------------------------");

  Serial.print("BATTERY HEALTH : ");

  if (healthState == "HEALTHY") {

    Serial.println("[ HEALTHY ]");
  }

  else if (healthState == "MINOR") {

    Serial.println("[ MINOR IMBALANCE ]");
  }

  else if (healthState == "CRITICAL") {

    Serial.println("[ CRITICAL IMBALANCE ]");
  }

  else {

    Serial.println("[ !!! PACK FAILURE !!! ]");
  }

  Serial.println("==================================================");
}

// ==========================================================
// STARTUP SCREEN
// ==========================================================

void startupScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("Battery Engine");

  lcd.setCursor(0, 1);

  lcd.print("Initializing");

  delay(2000);

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print("System Ready");

  delay(1500);
}

// ==========================================================
// SETUP
// ==========================================================

void setup() {

  Serial.begin(9600);

  // LCD START

  lcd.begin(16, 2);

  // OUTPUT PINS

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  // STARTUP SCREEN

  startupScreen();
}

// ==========================================================
// MAIN LOOP
// ==========================================================

void loop() {

  // STEP 1
  readCells();

  // STEP 2
  analyzeBattery();

  // STEP 3
  classifyHealth();

  // STEP 4
  updateLCD();

  // STEP 5
  serialDashboard();

  delay(1500);
}
