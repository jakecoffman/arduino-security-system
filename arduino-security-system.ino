/*************************************************************
Arduino Alarm by jake@coffshire.com
*************************************************************/

// Pins 
int speakerPin = 9;  // made it 9 to remind me ethernet has 10+ 
int doorPin = 2;

// Misc configurables
const int GRACE = 10000; // 10 seconds to vacate after reset
const int WAIT = 200; // time between door opening and alarm triggering

// The tones it plays in certain conditions
const char OPENCHIME[] = "gc ";  // a space represents a rest
const char READYCHIME[] = "cg "; // sounds like ok to me
const char ERRORCHIME[] = "cc "; // sounds like error to me

// Non-configurables
const char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
const int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
unsigned long pingtime = 0;
unsigned long time;
unsigned long time_since_open;
bool rearm;

void setup() 
{
  Serial.begin(9600);  // for debug
  
  // Set pins
  pinMode(speakerPin, OUTPUT);
  pinMode(doorPin, INPUT);
  
  time = 0;
  time_since_open = millis();
  
  // get me some ip addressage!
  setupComm();  
  
  // Give some grace to leave when you hit the reset button.
  while(millis() - time_since_open < GRACE);
  
  // Tells you the alarm is set.
  play(READYCHIME);             
}

/// This function watches a pin for the specified change in value and debounces for an
/// amount of time. When the value is held for that amount of time, the callback is executed.
/// @param pin The pin that will be watched.
/// @param val The value the pin will be watched for (LOW or HIGH)
/// @param debounce_millis The amount of time the value needs to be held to execute the callback.
/// @param callback The function to execute when the debounce is satisfied.
void detectChange(uint8_t pin, uint8_t val, int debounce_millis, void (*callback)(void))
{
  if (digitalRead(pin) == val)
  {
    time = millis();
    // To make sure it wasn't just static, wait the specified amount of time before continuing
    while(digitalRead(pin) == val)
    {
      if (millis() - time > debounce_millis)
      {
        // It wasn't static, execute the callback.
        callback();
        break;
      }
    }
  }
}  

// To rearm, simply set the bit and play the ready chime.
void rearmAlarm()
{
  rearm = true;
  play(READYCHIME);
}

// To trigger the alarm, send an email, play the open sound, then enter the rearm phase.
void triggerAlarm()
{
  email("Door has been opened.");
  play(OPENCHIME);
  rearm = false;
  time_since_open = time;
  bool once = true;
  
  while(rearm == false)
  {
    detectChange(doorPin, LOW, 10000, rearmAlarm);
    if (once && rearm == false && millis() - time_since_open > 20000)
    {
      // Door seems to be stuck open, send one email and play an extra buzz
      email("Door is stuck open");
      once = false;      
      play(OPENCHIME);
    }
  }
}

void loop() 
{
  // When the door opens, the value goes HIGH. If the door pin shows low for 200 millis, trigger the alarm.
  detectChange(doorPin, HIGH, WAIT, triggerAlarm);
  
  if(millis() - pingtime > 10000)
  {
    pingtime = millis();
  }
}
