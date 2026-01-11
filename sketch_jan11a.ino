#define BUZZER_PIN 32
#define BUTTON_PIN 18

#define NOTE_E2  82
#define NOTE_E3  165
#define NOTE_D3  147
#define NOTE_C3  131
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_FS3 185
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_G3  196
#define NOTE_E4  330
#define NOTE_G4  392
#define NOTE_B4  494

const int melody[] = {
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3, 
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, 
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3, 
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, 
  NOTE_FS3, NOTE_D3, NOTE_B2, NOTE_A3, NOTE_FS3, NOTE_B2, NOTE_D3, NOTE_FS3, 
  NOTE_A3, NOTE_FS3, NOTE_D3, NOTE_B2, 
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3, 
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, 
  NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_G3, NOTE_B3, 
  NOTE_E4, NOTE_B3, NOTE_G4, NOTE_B4 
};

// Tempo = 225 BPM (1 quarter note = 267ms)
const int noteDurations[] = {
  133, 133, 133, 133, 133, 133, 133, 133,
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 400,  
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 400,  
  67, 67, 67, 67, 67, 67, 67, 67, 
  67, 67, 67, 67,  
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 133, 133, 133, 133, 133,  
  133, 133, 133, 400,  
  67, 67, 67, 67, 67, 67, 67, 67,  
  67, 67, 67, 67  
};



volatile int speedLevel = 2;
const float speedMultiplier[] = { 2.0, 1.5, 1.0, 0.75, 0.5 };
volatile bool buttonPressed = false;

hw_timer_t *My_timer = NULL;
volatile int noteIndex = 0;
volatile bool isPlaying = false;

void IRAM_ATTR onTimer() {
  if (isPlaying) {
    digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));  // Toggle Buzzer
  }
}

void IRAM_ATTR handleButton() {
  buttonPressed = true;
}

void playNote() {
  if (noteIndex >= sizeof(melody) / sizeof(melody[0])) {
    noteIndex = 0;
  }

  int frequency = melody[noteIndex];
  int duration = noteDurations[noteIndex] * speedMultiplier[speedLevel];
  int halfPeriod = (1000000 / frequency) / 2;

  timerAlarm(My_timer, halfPeriod, true, 0);
  isPlaying = true;

  delay(duration);
  isPlaying = false;
  timerAlarm(My_timer, 1000000, true, 0);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);

  noteIndex++;
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON_PIN, handleButton, FALLING);

  My_timer = timerBegin(1000000);
  timerAttachInterrupt(My_timer, &onTimer);
  timerAlarm(My_timer, 1000000, true, 0);

  Serial.begin(115200);
}

void loop() {
  if (buttonPressed) {
    speedLevel = (speedLevel + 1) % 5;
    Serial.print("Speed Level: ");
    Serial.println(speedLevel + 1);
    buttonPressed = false;
  }
  playNote();
}