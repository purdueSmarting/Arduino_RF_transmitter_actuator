#include <VirtualWire.h>       //본 스케치는 VirtualWire 라이브러리를 사용합니다.
#include <Bounce2.h>             //바운싱 현상을 방지하기 위해 이 라이브러리를 사용합니다.
 
 int buttonPin = 2;            //버튼과 연결된 2번 핀

 boolean oneTimeFlag = false;        
 
 Bounce bouncer = Bounce();    
 
  void setup() {
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);
    bouncer.attach( buttonPin );
    bouncer.interval(5);
    
    vw_setup(2000);             //라이브러리 사용하기 위한 세팅 및 초기화
    vw_set_tx_pin(4);           //송신 핀 설정
    Serial.begin(9600);
  }
 
  void loop() {
   if ( bouncer.update() && bouncer.read() == LOW) {      //버튼이 눌리면
      if( oneTimeFlag == false) {                         //꺼진 상태라면
        Serial.println("on");
        sendMessage("1", "1");                            //명령 1 전송(on)
        oneTimeFlag = true;

      } else {                                            //켜저 있는 상태였다면
        Serial.println("off");                 
        sendMessage("1", "2");                            //명령 2전송(off)
        oneTimeFlag = false;

      }         
   }
 }
 
 //메세지 송신 함수
  void sendMessage(char* pinCode, char *data) {
     if (strlen(data) > 0) {           //data(command)가 있다면

      int msgSize = (strlen(data) + strlen(pinCode) + 1);  //송신 메세지의 전체 길이를 구하고
      char packetData[msgSize];                            //구한 길이를 바탕으로 메세지 저장할 문자배열변수 선언
      strcat(packetData, pinCode);                         //문자열 합치는 함수를 이용해 전송할 메세지 만듬
      strcat(packetData, ".");
      strcat(packetData, data);
 
      vw_send((uint8_t *)packetData, msgSize);             //메세지 전송
      vw_wait_tx();                                        //메세지 전송 완료시 까지 기다림

    }  
  }
