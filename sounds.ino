/*************************************************************
Arduino Alarm by jake@coffshire.com
*************************************************************/

// Sound related stuff
int length = 2; // the number of notes

int beats[] = { 1, 4 };  // hold the second note
int tempo = 200;         // relatively slow

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void play(const char notes[]) {
    for (int i = 0; i < length; i++) 
    {
      if (notes[i] == ' ') 
      {
        delay(beats[i] * tempo); // rest
      } 
      else 
      {
        playNote(notes[i], beats[i] * tempo);
      }
    
      // pause between notes
      delay(tempo / 2); 
    }  
}
