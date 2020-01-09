
/*
 _   _           ___  ___           ______   _   _   
| | | |          |  \/  |           |  ___| | | | |  
| |_| |__   ___  | .  . | ___  ___  | |_ ___| |_| |_ 
| __| '_ \ / _ \ | |\/| |/ _ \/ __| |  _/ _ \ __| __|
| |_| | | |  __/ | |  | | (_) \__ \ | ||  __/ |_| |_ 
 \__|_| |_|\___| \_|  |_/\___/|___/ \_| \___|\__|\__|
                                                                                                         
  fettBox 
          Open Source MCU Control box + interface
          
Email: nBED@gmx.com
Website: www.n-bed.com (currently down)
Kris Bickham Janurary 2020                    
*/



#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int clkPin= 2; //the clk attach to pin2
const int dtPin= 3; //the dt attach to pin3
const int swPin= 4 ;//the number of the button
const int BtnPin = 5;
const int ButtonPin= 5 ;
int menuB;
int change;
const uint8_t menuAMAX = 3;
const uint8_t menuBMAX = 3;

 const char* selecttxt = "Select *button*";
 const char* menutxt[3]= {"Still Scan","Auto Scan","Manual Scan"};
 const char* functiontxt[3]= {"Servo Pos: ", "Object :", "Stop"};
typedef struct
{ int menuL; 
  unsigned long timerM;
  int menuV,subV,selV,tempV;
}menust, *pmenust;



typedef struct
{ menust MENU; 
  motorc MCTRL;
}fetbox, *pfetbox;

fetbox FBOX;
int ladder;


unsigned long buttonT,selectT=0;
     
uint8_t menuA,menustate;

bool select=false;
bool ignoreturn=false;
int buttonCASE,buttonB=0;

float mot1a,mot2a;
int mot1s,mot2s;
int inpmot;
int machinestate;
 //static 
 int oldA = HIGH; //set the oldA as HIGH
 // static
  int oldB = HIGH; //set the oldB as HIGH
  int oldchange;
  unsigned long refresht;
// menu messes up when scrolled out of range
// you need to double check encoder value is being reset when over max or under 0

void setup()
{ pinMode(BtnPin,INPUT);//initialize the buttonPin as an intput
  lcd.init();   
  lcd.init();        // initialize the lcd 
  // Print a message to the LCD. 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("mFETbox");
  lcd.setCursor(0,1);
  lcd.print("initializing....");
  
  //set clkPin,dePin,swPin as INPUT
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
  Serial.begin(9600); // initialize serial communications at 9600 bps
  lcd.clear();
  
  FBOX.MENU.timerM = millis();
  selectT = millis();
  refresht=millis();
  buttonT = millis();
   menuF();
}

void loop()
{buttonREAD();
 dialIN();   

 if (oldchange != change or select == true )
  {   lcd.clear();
      menuF();  //took out of dialIN
     // refresht = millis();
  };
}
void dialIN()
  { 
    oldchange=change;
    change = getEncoderTurn();
   FBOX.MENU.tempV = FBOX.MENU.tempV + change;
//  if (ignoreturn == false)  
               if(FBOX.MENU.menuL== 0)
                 { 
                  if (FBOX.MENU.tempV>2)
                     { FBOX.MENU.tempV=2;
                     //  change=0;
                     };  
                   if (FBOX.MENU.tempV<0)
                     { FBOX.MENU.tempV=0;
                     //  change=0;
                     };  
};
      

int getEncoderTurn(void)
{// static int oldA = HIGH; //set the oldA as HIGH
 // static int oldB = HIGH; //set the oldB as HIGH
  int result = 0;
  int newA = digitalRead(dtPin);//read the value of clkPin to newA
  int newB = digitalRead(clkPin);//read the value of dtPin to newB
  if (newA != oldA || newB != oldB) //if the value of clkPin or the dtPin has changed
    {// lcd.clear();  // something has changed
      if (oldA == HIGH && newA == LOW)
        {result = (oldB * 2 - 1);   
           };     
     //  menuF();  
     }; 
  oldA = newA;
  oldB = newB;
  return result;
};

void buttonREAD()
{ 
if (millis() - FBOX.MENU.timerM >80)
  {
    if (0 == digitalRead(BtnPin))   
         {  select=true;  
            selectT=millis();                    
         };
  };
};

void eventHandler()
{
 
};

void menuF()
{ 
                                       if (select==true)
                                          { lcd.clear();   
                                         //   if( FBOX.MENU.tempV != 4)
                                           //   { //FBOX.MENU.subV=FBOX.MENU.tempV;
                                              if (FBOX.MENU.menuL == 0)
                                                FBOX.MENU.menuL++; 
                                                }else{ FBOX.MENU.menuL--; };             
                                             select=false; 
                                             change=0;
                                             FBOX.MENU.timerM=millis();
                                          };
 switch (FBOX.MENU.menuL)
  {  
    case 0:
                                lcd.setCursor(0,0); 
                                lcd.print(menutxt[FBOX.MENU.tempV]);
                                lcd.setCursor(0,1);
                                lcd.print(selecttxt);      
                                break;
     case 1:                    lcd.setCursor(0,0);
                                lcd.print(functiontxt[0]);
                                lcd.setCursor(0,1);
                                lcd.print(functiontxt[1]);                           
                                break;
                                                
  };

};
