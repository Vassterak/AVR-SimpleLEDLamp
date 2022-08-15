#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

//PINOUT definitions
#define CHANGE_BUTTON_UP PB2
#define CHANGE_BUTTON_PIN_DOWN PB4
#define LED_PIN PB1
#define VOTLAGE_READ A3

//Constants
//#define F_CPU 8000000UL //8MHz
#define RAINBOW_SPEED 128 // 65 536 / 128 = 512 steps. 62steps per second = 512/62 = 8seconds (rounded) lasting one loop of animation
#define NUMPIXELS 1
#define HOLD_TIME 1000 //in ms
#define HOLD_TIME_DELAY 20 //in ms
#define BRIGHTNESS_STEPS 32 //PS: 256 / 32 = 8 brightness steps
#define BLINKING_SPEED 30
#define EEPROM_UPDATE_SPEED 480 //want to save every 30s if something has changed
#define EEPROM_EFFECT_ADDRESS 0
#define EEPROM_BRIGHTNESS_ADDRESS 1

//variables and instances
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
uint8_t butLasState = 1, butLasState2 = 1, effectIndex = 0, brightLevel = 255;
uint8_t isInHoldMode = 0, helpVarTimer = 0, helpVarDelay = 0;
uint16_t rainbowHue = 0, helpVarEEPROMDelay = 0;
long startTime = 0;

void timer_setup()
{
  TCNT1 = 0; //set timer1 to 0
  TCCR1 = 0b00001001; //setting up prescaler for timer1 to 8192x = every 1.024ms value will increse after 316.2ms timer will overflow
  TIMSK |= (1 << TOIE1); //set up overflow interrupt for timer1
  sei();  //Enable interrupts
}

void read_from_eeprom()
{
  effectIndex = EEPROM.read(EEPROM_EFFECT_ADDRESS);
  brightLevel = EEPROM.read(EEPROM_BRIGHTNESS_ADDRESS);
}

ISR (TIMER1_OVF_vect)
{
  rainbowHue += RAINBOW_SPEED;

  /*
  if (analogRead(VOTLAGE_READ) > 950 )
    PORTB |= 0x1;
  else
    PORTB &= !0x1;
  */

  if (helpVarDelay > BLINKING_SPEED)
  {
    if (isInHoldMode == 1 && helpVarTimer == 0)
    {
      pixels.setBrightness(1);
      helpVarTimer = 1;
    }
    else
    {
      pixels.setBrightness(brightLevel);
      helpVarTimer = 0;
    }
    helpVarDelay = 0;
  }
  else
    helpVarDelay++;

  if (helpVarEEPROMDelay > EEPROM_UPDATE_SPEED) //Save to EEPROM
  {
    EEPROM.update(EEPROM_EFFECT_ADDRESS, effectIndex);
    EEPROM.update(EEPROM_BRIGHTNESS_ADDRESS, brightLevel);
    helpVarEEPROMDelay = 0;
  }
  else
    helpVarEEPROMDelay++;
}

void brightness_set(uint8_t value)
{
  if (value == 1)
  {
    if (brightLevel > (UINT8_MAX - BRIGHTNESS_STEPS)) //check for uint overflow
      brightLevel = 255; //set max brightness if overflow would happened
    else
      brightLevel += BRIGHTNESS_STEPS;
  }
  else
  {
    if (brightLevel < BRIGHTNESS_STEPS + 1)
      brightLevel = BRIGHTNESS_STEPS;
    else
      brightLevel -= BRIGHTNESS_STEPS;
  }

  pixels.setBrightness(brightLevel);
}

void button_state_check1()
{
  //get current button state, LOW(0) = Pressed, HIGH(1) = not pressed
  uint8_t butCurState = digitalRead(CHANGE_BUTTON_PIN_DOWN);

  if (butCurState == 1 && butLasState == 1)
    return;
  else if (butCurState == 0 && butLasState == 1)
    startTime = millis();
  else if (butCurState == 0 && butLasState == 0)
  {
    if ((millis() - startTime) > HOLD_TIME && (millis() - startTime) < HOLD_TIME + HOLD_TIME_DELAY)
      isInHoldMode = 1; //isInHoldMode = !isInHoldMode; //toggle value
  }
  else if (butCurState == 1 && butLasState == 0)
  {
      if (isInHoldMode == 1)
        brightness_set(1);
      else
        effectIndex++;
  }

  butLasState = butCurState;
}

void button_state_check2()
{  
  uint8_t butCurState = digitalRead(CHANGE_BUTTON_UP);

  if (butCurState == 1 && butLasState2 == 1)
    return;
  else if (butCurState == 0 && butLasState2 == 1)
    startTime = millis();
  else if (butCurState == 0 && butLasState2 == 0)
  {
    if ((millis() - startTime) > HOLD_TIME && (millis() - startTime) < HOLD_TIME + HOLD_TIME_DELAY)
      isInHoldMode = 0; //isInHoldMode = !isInHoldMode; //toggle value
  }
  else if (butCurState == 1 && butLasState2 == 0)
  {
      if (isInHoldMode == 1)
        brightness_set(0);
      else
        effectIndex--;
  }

  butLasState2 = butCurState;
}

void select_effect()
{
  if (isInHoldMode == 1)
    return;

  switch (effectIndex)
  {
    case 0:
      pixels.setPixelColor(0, pixels.Color(255, 255, 255)); //white
      break;

    case 1:
      pixels.setPixelColor(0, pixels.Color(255, 244, 150)); //warm white
      break;
    
    //Start of color spectrum from red to blue (simple rainbow spectrum)
    case 2:
      pixels.setPixelColor(0, pixels.Color(255,0,0)); //red
      break;

    case 3:
      pixels.setPixelColor(0, pixels.Color(255,127,0)); //orange
      break;

    case 4:
      pixels.setPixelColor(0, pixels.Color(255,255,0)); //yellow
      break;

    case 5:
      pixels.setPixelColor(0, pixels.Color(128,255,0)); //light green
      break;

    case 6:
      pixels.setPixelColor(0, pixels.Color(0,255,0)); //green
      break;

    case 7:
      pixels.setPixelColor(0, pixels.Color(0,255,128)); //green-blue
      break;

    case 8:
      pixels.setPixelColor(0, pixels.Color(0,255,255)); //turquoise
      break;

    case 9:
      pixels.setPixelColor(0, pixels.Color(0,128,255)); //light blue
      break;

    case 10:
      pixels.setPixelColor(0, pixels.Color(0,0,255)); //blue
      break;

    case 11:
      pixels.setPixelColor(0, pixels.Color(128,0,255)); //dark-purple
      break;

    case 12:
      pixels.setPixelColor(0, pixels.Color(255,0,255)); //purple/slightly ping?
      break;

    case 13:
      pixels.setPixelColor(0, pixels.ColorHSV(rainbowHue,255,255));
      break;

    default:
      effectIndex = 0;
      break;
  }
}

void setup()
{
  read_from_eeprom();
  timer_setup();
  pinMode(CHANGE_BUTTON_UP, INPUT_PULLUP);
  pinMode(CHANGE_BUTTON_PIN_DOWN, INPUT_PULLUP);
  pixels.begin();

  //pinMode(PB0, OUTPUT);
  //pinMode(VOTLAGE_READ, INPUT);
}

void loop()
{
  button_state_check1();
  button_state_check2();
  select_effect();
  pixels.show();
  /*
  PORTB = PORTB | 0b00000001;
  delayMicroseconds(500);
  PORTB = PORTB & !0b00000001;
  delayMicroseconds(500);*/
}