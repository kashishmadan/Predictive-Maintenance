#define VIN A0
int led = 13;


int ThermistorPin = 0;
int Vo;
//float R1 = 10000;
float logR2, R2, T,Tc, Tf;
//Temp

float R1 = 2252;
float A = 1.484778004e-03, B = 2.348962910e-04, C = 1.006037158e-07;  // Steinhart-Hart and Hart Coefficients


char data = 0;//Bluetooth



int vs =9; // vibration sensor

const float VCC   = 5.0;// supply voltage is from 4.5 to 5.5V. Normally 5V.
const int model = 0;   // enter the model number (see below)

float cutOffLimit = 0.5;// set the current which below that value, doesn't matter. Or set 0.5

/*
          "ACS712ELCTR-05B-T",// for model use 0
          "ACS712ELCTR-20A-T",// for model use 1
          "ACS712ELCTR-30A-T"// for model use 2  
sensitivity array is holding the sensitivy of the  ACS712
current sensors. Do not change. All values are from page 5  of data sheet          
*/
float sensitivity[] ={
          0.185,// for ACS712ELCTR-05B-T
          0.100,// for ACS712ELCTR-20A-T
          0.066// for ACS712ELCTR-30A-T
     
         }; 


const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float voltage;// internal variable for voltage

//Current


void setup()
{ 
  pinMode(led, OUTPUT);
  pinMode(vs, INPUT); 
  Serial.begin(9600); 
}
void loop()
{
  Vib();
  temp2();
  bluetooth();
  current();
 
}

void current()
{
  float voltage_raw =   (5.0 / 1023.0)* analogRead(VIN);// Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.012;//-0.6 ;// 0.000 is a value to make voltage zero when there is no current
  float current = voltage / sensitivity[model];
 
  if(abs(current) > cutOffLimit ){
    Serial.print("V: ");
    Serial.print(voltage,3);// print voltage with 3 decimal places
    Serial.print("V, I: ");
    Serial.print(current,2); // print the current with 2 decimal places
    Serial.println("A");
  
  }else{
    
  Serial.println("No Current");
  }
  delay(500);
}


void Vib()
{
 long measurement =pulseIn (vs, HIGH);
  delay(50);
  Serial.println(measurement);
  if (measurement > 50){
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW); 
  } 
}

void bluetooth()
{
  if(Serial.available()>0)
{ 
char data= Serial.read(); // reading the data received from the bluetooth module
switch(data)
{
case '1': digitalWrite(13, HIGH);break; // when Z is pressed on the app Turn on Pin 13
case '0': digitalWrite(13, LOW);break; // when z is pressed on the app Turn off Pin 13
case 'Z': digitalWrite(13, HIGH);break; // when Z is pressed on the app Turn on Pin 13
case 'z': digitalWrite(13, LOW);break; // when z is pressed on the app Turn off Pin 13
default : break;
}
//BT.write(T);

Serial.println(data);
}
delay(50);

}

void temp2() 
{
  
  Vo = analogRead(ThermistorPin);
  R2 = R1* (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (A + B*logR2 + C*logR2*logR2*logR2))-32;  // Steinhart and Hart Equation.T  = 1 / {A + B[ln(R)] + C[ln(R)]^3}-32
  T =  T - 273.15;

  Serial.print("TEMP =");
  Serial.print(T);
  Serial.println("C");

  delay(1000);     // Time delay 10 Milliseconds
}

 
