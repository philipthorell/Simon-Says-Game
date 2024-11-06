const int RED_LED = 2;
const int BLUE_LED = 4;
const int YELLOW_LED = 6;
const int GREEN_LED = 8;

const int RED_BUTTON = 3;
const int BLUE_BUTTON = 5;
const int YELLOW_BUTTON = 7;
const int GREEN_BUTTON = 9;

const int PIEZO = 10;

int notes[] = {262, 294, 330, 349, 150};  // C4, D4, E4, F4, WRONG-SOUND

int start_melody[] = {262, 294, 330, 262, 330, 349, 392};  // C4, D4, E4, C4, E4, F4, G4
int start_note_durations[] = {200, 200, 200, 200, 200, 200, 400};  // Duration of each note in milliseconds


bool start_game = true;
bool waiting_for_response = false;
int game_round = 1;
int round_guess = 0;

enum Color {
  RED,
  BLUE,
  YELLOW,
  GREEN
};

Color current_color;

Color color_sequence[10];
int color_sequence_index = 0;

void setup() {
  pinMode(13, OUTPUT); // To prevent the "L" LED on the Arduino from lighting up

  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);

  noTone(PIEZO);

  randomSeed(analogRead(A0));

}

void loop() {
  if (start_game) {
    for (int i = 0; i < 7; i++) {
    int noteDuration = start_note_durations[i];
    tone(PIEZO, start_melody[i], noteDuration);
    delay(noteDuration * 1.3);
    }
    noTone(PIEZO);
    start_game = false;
  }

  if (waiting_for_response) {
    checkPlayerInput();
  } else {
    start_round();
  }
}

void start_round() {
  current_color = static_cast<Color>(random(4));
  color_sequence[color_sequence_index] = current_color;
  color_sequence_index++;

  displayCurrentColor();
  waiting_for_response = true;
}

void displayCurrentColor() {
  for (int i = 0; i < game_round; i++) {
    switch (color_sequence[i]) {
      case RED:
        blink_light(1);
        break;
      case BLUE:
        blink_light(2);
        break;
      case YELLOW:
        blink_light(3);
        break;
      case GREEN:
        blink_light(4);
        break;
    }
    delay(500);
  }
}

void checkPlayerInput() {
  if (digitalRead(RED_BUTTON) == LOW) {
    processGuess(RED, 1);
  } else if (digitalRead(BLUE_BUTTON) == LOW) {
    processGuess(BLUE, 2);
  } else if (digitalRead(YELLOW_BUTTON) == LOW) {
    processGuess(YELLOW, 3);
  } else if (digitalRead(GREEN_BUTTON) == LOW) {
    processGuess(GREEN, 4);
  }
  delay(200);
}

void processGuess(Color guessed_color, int led_number) {
  if (guessed_color == color_sequence[round_guess]) {
    blink_light(led_number); // Correct guess
    round_guess++;
  } else {
    blink_light(5);  // Wrong guess
    round_guess = 0;
    game_round = 0;
    color_sequence_index = 0;
    waiting_for_response = false;
  }
  if (round_guess == game_round) {
    game_round++;
    if (game_round == 10) {start_game = true;}
    round_guess = 0;
    waiting_for_response = false;
    delay(1200);
  }
}

void blink_light(int light) {
  switch (light) {
    case 1: // RED
      digitalWrite(RED_LED, HIGH);
      tone(PIEZO, notes[0]);
      delay(400);
      noTone(PIEZO);
      digitalWrite(RED_LED, LOW);
      break;
    case 2: // BLUE
      digitalWrite(BLUE_LED, HIGH);
      tone(PIEZO, notes[1]);
      delay(400);
      noTone(PIEZO);
      digitalWrite(BLUE_LED, LOW);
      break;
    case 3: // YELLOW
      digitalWrite(YELLOW_LED, HIGH);
      tone(PIEZO, notes[2]);
      delay(400);
      noTone(PIEZO);
      digitalWrite(YELLOW_LED, LOW);
      break;
    case 4: // GREEN
      digitalWrite(GREEN_LED, HIGH);
      tone(PIEZO, notes[3]);
      delay(400);
      noTone(PIEZO);
      digitalWrite(GREEN_LED, LOW);
      break;
    case 5: // WRONG (all LEDs flash)
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      tone(PIEZO, notes[4]);
      delay(400);
      noTone(PIEZO);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      break;
  }
}
