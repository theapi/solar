
unsigned char pwm_val = 0;

void setup()
{
  pinMode(0, OUTPUT); 
  pinMode(1, OUTPUT); 
  initPWM();
}

void loop() 
{
  delay(30);
  pwm_val++;
  
  if (pwm_val == 254) {
    pwm_val = 0; 
  }
  
  setPwm(pwm_val);
}

void setPwm(byte val)
{
  // Set the compare value
  OCR1A = val;
}

void initPWM()
{
  // Fast PWM on timer 2 
  
  // This does both PB0 & PB1 (opposite vals for each)
  //TCCR1 = (1 << PWM1A) | (1 << COM1A0) | (1 << CTC1) | (1 << CS10);
  
  // PB1 ONLY (non inverting)
  TCCR1 = (1 << PWM1A) | (1 << COM1A1) | (0 << COM1A0) | (1 << CTC1) | (1 << CS10);

  setPwm(pwm_val);
}

