/*
 * Init Motor pin 3, 5, 9, 10
*/
void InitMotor()
{
  /*init pin 5 freq at 1khz*/
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS01) | _BV(CS00) | _BV(WGM02);
  OCR0A = 249;
  /*init pin 3 freq at 1khz*/
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(WGM22);
  OCR2A = 249;
  /*init pin 9, 10 freq at 1khz*/
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);
  ICR1  = 249;
  OCR2B = 249;
  OCR1B = 249;
  OCR1A = 249;
  OCR0B = 249;
  delay(600000);
  OCR2B = 0;
  OCR1B = 0;
  OCR1A = 0;
  OCR0B = 0;
  delay(600000);
}

void SetUpMotor()
{
  /* set frequency pin 5 at 450hz
   * PWM duty controlled by OCR0B register
  */
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS02) | _BV(WGM02);
  OCR0A = 138;
  /* set frequency pin 3 at 450hz
   * PWM duty controlled by OCR2B register
  */
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(WGM22);
  OCR2A = 138;
  /* init pin 9, 10 freq at 450hz
   * PWM duty controlled by OCR1A(pin 9), OCR1B(pin 10) register
  */
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);
  ICR1  = 138;
}

void SetPin3_Duty(double val)
{
  OCR2B = val;
}

void SetPin5_Duty(double val)
{
  OCR0B = val;
}

void SetPin9_Duty(double val)
{
  OCR1A = val;
}

void SetPin10_Duty(double val)
{
  OCR1B = val;
}
