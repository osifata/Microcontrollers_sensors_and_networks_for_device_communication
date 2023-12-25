#define RxTxPin		6
#define clOff		0
#define clYellow	1

#define spdPin		0
#define RxTxSelect	15
#define ManualPin	16

#define LEDRx		13
#define LEDTx		12
#define LEDBusy		11

#define latchPin	3	//	12
#define clockPin	4	//	11
#define dataPin		2	//	13

#define ShRegCl		5
#define ShOutRegCl	4
#define ShOutEn		3
#define ShInput		2

//--------------------------------------------------
bool	RxTxMode;
char	RxBuf[32];
byte	RxBuflen = 0, codeNum=0;
int		DelayDispCnt=0, DispPos=0, SegmDelay=0;
long		DelayCnt=0;

String ENCODE[]  = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
char  LETTERS[]  = {'A', 'B', 'C','D','E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
byte  SEGMENTS[] = { 119, 124, 57, 94, 121,113, 61, 118, 48, 30,    8,   56, 84, 84, 63,     115, 92, 80, 91, 120, 62, 28, 28, 118, 102, 91};
//--------------------------------------------------
struct TxSys{
  byte		txPos=0, txMax=0, txTime=0;
  String  	txCode="";
  char		txChar;
  bool		txEmpty = true;
  byte		txPhase = 0;
  byte      txSpd=0;
} MTx;
//--------------------------------------------------
struct RxSys{
  bool		rxEmpty = true;
  bool		rxRec	= true;
  
  byte		rxTimeH  = 0;
  byte		rxTimeL  = 0;
  String  	rxCode   = "";
  String	rxString;
  char		rxChar;
  byte		rxPhase  = 0;
  byte      rxSpd    = 0;

} MRx;
//--------------------------------------------------
void setup()
{
  Serial.begin(9600);
  pinMode(LEDRx, 	OUTPUT);
  pinMode(LEDTx, 	OUTPUT);
  pinMode(LEDBusy,	OUTPUT);

  for ( byte i=2; i<10; i++ ){
    pinMode(i,	OUTPUT); 
    digitalWrite(i, LOW);
  }
 
  digitalWrite(6, HIGH);
  SetLed( 0 );
  
  delay(100);
  if (  digitalRead( RxTxSelect ) ){ RxTxMode = false; pinMode(RxTxPin, OUTPUT); }	// RxTxPin = true  - Передатчик
  delay(500);
  if ( !digitalRead( RxTxSelect ) ){ RxTxMode = true;  pinMode(RxTxPin, INPUT);  }	// RxTxPin = false - Приёмник
}
//--------------------------------------------------
void loop(){
  if( Serial.available() ){
    RxBuf[ RxBuflen ] = Serial.read();
    RxBuflen++;
  }
  
  DelayCnt++;
  if ( DelayCnt >= (300-analogRead(spdPin)/4) ){
    DelayCnt=0;
      if ( digitalRead( RxTxSelect ) ) { TxMode(); }
      else						       { RxMode(); }
  }
  
  SegmDelay++;
  if ( MTx.txEmpty && !MRx.rxRec ){
    if ( SegmDelay >= 10000 ){ 
      DrawSegments();
      SegmDelay = 5000;
    }
  } else {SegmDelay = 0; DelayDispCnt=0; }

}
//--------------------------------------------------
void DrawSegments(){
  byte out, pOut;
  char dChar;
  bool segPIN;
  
  byte digit;
  DelayDispCnt++;
  if ( DelayDispCnt >= 2 ){
    DelayDispCnt=0;
    if ( MRx.rxString.length() > 0 ){
      dChar = MRx.rxString[DispPos];
      digit = SEGMENTS[ dChar-65 ];
      DispPos++;
      if ( DispPos > MRx.rxString.length() ){
        DispPos = 0;
        MRx.rxString=""; 
        for ( byte i=0; i<7; i++ ){
          if (i<4) {out=i+2; } else { out=i+3; }
          digitalWrite(out, 0);
        }
      } else {
        digitalWrite(latchPin, LOW);	delay(1);
        for ( byte i=7; i>0; i-- ){
          digitalWrite(clockPin, LOW);	delay(1);
          segPIN = (digit>>(i-1))&0x01;
          if (segPIN){ digitalWrite(dataPin, HIGH); delay(1); }
          else       { digitalWrite(dataPin, LOW);  delay(1); }
          digitalWrite(clockPin, HIGH);	delay(1);
        }
        digitalWrite(latchPin, HIGH);
      }
    } else {
        digitalWrite(latchPin, LOW);	delay(1);
        for ( byte i=7; i>0; i-- ){
          digitalWrite(clockPin, LOW);	delay(1);
		  digitalWrite(dataPin, LOW);  delay(1);
          digitalWrite(clockPin, HIGH);	delay(1);
        }
        digitalWrite(latchPin, HIGH);
    }
  }
}
//--------------------------------------------------
void TxMode(){

  if ( RxTxMode == false ){
    RxTxMode = true;
    pinMode(RxTxPin, OUTPUT);
    digitalWrite(RxTxPin, HIGH);
    SetLed(LEDTx);
  }
  
  if ( RxBuflen>0 && MTx.txEmpty ){
    for ( byte i=0; i<26; i++ ){
      if ( RxBuf[ 0 ] == LETTERS[i] ){
        MTx.txEmpty = false;
        MTx.txCode  = ENCODE[i];
        MTx.txMax   = MTx.txCode.length();
        MTx.txPos   = 0;
        MTx.txPhase = 0;
        MTx.txChar  = LETTERS[i];
        Serial.println("Put ["+(String)RxBuf[ 0 ]+"] in Tx");
        for ( byte i=0; i<RxBuflen; i++ ){
          RxBuf[ i ]=RxBuf[ i+1 ];
        }
        RxBuflen--;
        SetLed(LEDBusy);
        break;
      }
    }
  }

  if ( !MTx.txEmpty && MTx.txTime==0 ){
    if (  MTx.txPhase == 0 ){
      if ( MTx.txCode[MTx.txPos] == '.' ){
        MTx.txTime = 10;
        digitalWrite(RxTxPin, LOW);
        Serial.print(".");
        SetLed(clYellow);
      }
      if ( MTx.txCode[MTx.txPos] == '-' ){
        MTx.txTime = 20;
        digitalWrite(RxTxPin, LOW);
        Serial.print("-");
        SetLed(clYellow);
      }
      MTx.txPos++;
      if ( MTx.txPos > MTx.txMax ){
        MTx.txEmpty = true;
        MTx.txTime = 30;
        digitalWrite(RxTxPin, HIGH);
        Serial.println("["+(String)MTx.txChar+"]");
        SetLed(LEDTx);
      }
    }
    if (  MTx.txPhase == 1 ){
      digitalWrite(RxTxPin, HIGH);
      MTx.txTime = 10;
      SetLed(clOff);
    }
    MTx.txPhase++;
    if ( MTx.txPhase==2 ){ MTx.txPhase = 0; }
  } 
  if ( MTx.txEmpty ){
    digitalWrite(RxTxPin, digitalRead( ManualPin ) );
  }
  if ( MTx.txTime > 0 ){ MTx.txTime--; }
}
//--------------------------------------------------
void RxMode(){
  if ( RxTxMode == true ){
    RxTxMode = false;
    pinMode(RxTxPin, INPUT);
    digitalWrite(RxTxPin, HIGH);
    SetLed(LEDRx);
  }

  if ( !MRx.rxRec && !digitalRead( RxTxPin ) ){
    MRx.rxRec = true;
    MRx.rxPhase = 0;
//    MRx.rxString="";
    SetLed(LEDBusy);
  }
  if ( MRx.rxRec ){
    if ( digitalRead( RxTxPin ) ){
      MRx.rxTimeH++;
      SetLed(clOff);
      if ( MRx.rxPhase == 1 ){
        MRx.rxPhase = 0;
        Serial.println("Rx.time L ["+(String)MRx.rxTimeL+"]");
        if ( MRx.rxTimeL <15 ){ MRx.rxCode += "."; }
        else				  { MRx.rxCode += "-"; }
        MRx.rxTimeL = 0;
      }
      if ( MRx.rxTimeH > 50 ){
        MRx.rxRec   = false;
        MRx.rxPhase = 0;
        MRx.rxTimeL = 0;
        MRx.rxTimeH = 0;
        Serial.print("Rx.get word["+MRx.rxCode+"] -> ");
        SetLed(LEDRx);
		for ( byte i=0; i<26; i++ ){
      	  if ( MRx.rxCode == ENCODE[i] ){
            MRx.rxChar = LETTERS[i];
             break;
          }
        }
        Serial.println( (String)MRx.rxChar );
        MRx.rxString += (String)MRx.rxChar;
        MRx.rxCode = "";
        SetLed(LEDRx);
      }
      if ( MRx.rxTimeH > 15 ){
        MRx.rxRec   = false;
        MRx.rxPhase = 0;
        MRx.rxTimeL = 0;
        MRx.rxTimeH = 0;
          Serial.print("Rx.get ["+MRx.rxCode+"] -> ");
		  for ( byte i=0; i<26; i++ ){
      		if ( MRx.rxCode == ENCODE[i] ){
              MRx.rxChar = LETTERS[i];
               break;
            }
          }
          Serial.println( (String)MRx.rxChar );
          MRx.rxString = MRx.rxString + (String)MRx.rxChar;
          MRx.rxCode = "";
          SetLed(LEDRx);
        }
      }
      else {
        MRx.rxTimeL++;
        SetLed(clYellow);
        if ( MRx.rxPhase == 0 ){
          MRx.rxPhase = 1;
          Serial.println("Rx.time H ["+(String)MRx.rxTimeH+"]");
          MRx.rxTimeH = 0;
      }
    }
  }
}
//--------------------------------------------------
void SetLed( byte led ){
      
  digitalWrite(LEDRx,	LOW);
  digitalWrite(LEDTx,	LOW);
  digitalWrite(LEDBusy,	LOW);
  if ( led == LEDRx )  { digitalWrite(LEDRx,	HIGH); }
  if ( led == LEDTx )  { digitalWrite(LEDTx,	HIGH); }
  if ( led == LEDBusy ){ digitalWrite(LEDBusy,	HIGH); }
  if ( led == clYellow ){
    digitalWrite(LEDRx,		HIGH);
    digitalWrite(LEDBusy,	HIGH);
  }
  
}