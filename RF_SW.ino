#define RX315_pin 2
#define RX433_pin 3
#define TX315_pin 6
#define TX433_pin 4
#define TX433_pin_en 5

#include <RCSwitch.h>

RCSwitch RX_RC = RCSwitch();
RCSwitch TX315 = RCSwitch();
RCSwitch TX433 = RCSwitch();

String Type_RC;
String BinCode_RC;
String Protocol_RC;
String PulseLength_RC;
String RepeatTX;

boolean getStarted;
boolean recievedFlag;
byte index;
String string_convert = "";
void parsing() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();        // обязательно ЧИТАЕМ входящий символ
    if (getStarted) {                         // если приняли начальный символ (парсинг разрешён)
      if (incomingByte != '+' && incomingByte != '\r') {   // если это не пробел И не конец
        string_convert += incomingByte;       // складываем в строку
      } else {                                // если это пробел или конец пакета
        switch (index) { // строку кладём в соответствующую переменную
          case 0:
            Type_RC = string_convert;
            break;
          case 1:
            BinCode_RC = string_convert;
            break;
          case 2:
            Protocol_RC = string_convert;
            break;
          case 3:
            PulseLength_RC = string_convert;
            break;
          case 4:
            RepeatTX = string_convert;
            break;
        }
        string_convert = "";                  // очищаем строку
        index++;                              // переходим к парсингу следующего элемента массива
      }
    }
    if (getStarted == false) {                // если это новое сообщение
      getStarted = true;                      // поднимаем флаг, что можно парсить
      index = 0;                              // сбрасываем индекс
      string_convert = "";
      string_convert += incomingByte;
    }
    if (incomingByte == '\n') {                // если приняли символ конца строки - конец парсинга
      getStarted = false;                     // сброс
      recievedFlag = true;                    // флаг на принятие
    }
  }
}
void setup() {
  Serial.begin(57600);
  TX315.enableTransmit(TX315_pin);
  TX433.enableTransmit(TX433_pin);
  Serial.println("Started RF_SW");
  Serial.println("Format cmd: [Type_RC]+[BinCode_RC]+[Protocol_RC]+[PulseLength_RC](+[RepeatTX])\nType_RC = 315 or 433\nBinCode_RC - received binary code\nProtocol_RC - protocol number of received code\nPulseLength_RC - length of received code\nRepeatTX - optional argument, the number of times to resubmit the code.\nUsing: send the received command for repeat code or send the command 'l315' to switch to listening mode 315 MHz, for 433 MHz - 'l433'.");
  RX_RC.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  Serial.println("Listing\t315MHz");
}
boolean FrSet315_or_433 = true;
void loop() {
  parsing();       // функция парсинга
  if (recievedFlag) { // если получены данные
    recievedFlag = false;
    if (Type_RC == "315") {
      if (index >= 4 && index <= 5) {
        if (index >= 5) { // устанавливаем количество повтора пакета
          TX315.setRepeatTransmit(RepeatTX.toInt());
        } else {
          TX315.setRepeatTransmit(10);
        }
        TX315.setProtocol(Protocol_RC.toInt(), PulseLength_RC.toInt()); // задаем соотвествующий протокол и длину пакета
        int str_len = BinCode_RC.length() + 1;// Длина (с одним дополнительным символом нулевого терминатора)
        char BinCode_RC_ChA[str_len];// Подготавливаем массив символов (буфер)
        BinCode_RC.toCharArray(BinCode_RC_ChA, str_len);// Копируем его
        TX315.send(BinCode_RC_ChA); // отправляем пакет
      } else {
        Serial.println("Err. arg.");
      }
    }
    else if (Type_RC == "433") {
      if (index >= 4 && index <= 5) {
        if (index >= 5) { // устанавливаем количество повтора пакета
          TX433.setRepeatTransmit(RepeatTX.toInt());
        } else {
          TX433.setRepeatTransmit(10);
        }
        TX433.setProtocol(Protocol_RC.toInt(), PulseLength_RC.toInt()); // задаем соотвествующий протокол и длительность пакета
        int str_len = BinCode_RC.length() + 1;// Длина (с одним дополнительным символом нулевого терминатора)
        char BinCode_RC_ChA[str_len];// Подготавливаем массив символов (буфер)
        BinCode_RC.toCharArray(BinCode_RC_ChA, str_len);// Копируем его
        TX433.send(BinCode_RC_ChA); // отправляем пакет
      } else {
        Serial.println("Err. arg.");
      }
    }
    else if (Type_RC == "l315") { //listing 315
      RX_RC.disableReceive();
      RX_RC.enableReceive(0);
      FrSet315_or_433 = true;
      Serial.println("Listing\t315MHz");
    }
    else if (Type_RC == "l433") { //listing 433
      RX_RC.disableReceive();
      RX_RC.enableReceive(1);
      FrSet315_or_433 = false;
      Serial.println("Listing\t433MHz");
    } else {
      Serial.print("Err. index = ");
      Serial.print(index);
      Serial.print(", Type_RC = ");
      Serial.println(Type_RC);
    }
  }
  if (RX_RC.available()) { // если приняли пакет
    if (FrSet315_or_433) {// определяем на какой частоте
      Serial.print("315+");
    } else {
      Serial.print("433+");
    }
    const char* bin_code = dec2binWzerofill(RX_RC.getReceivedValue(), RX_RC.getReceivedBitlength());
    Serial.print(bin_code); // код
    Serial.print("+");
    Serial.print(RX_RC.getReceivedProtocol()); // протокол
    Serial.print("+");
    Serial.println(RX_RC.getReceivedDelay()); // длительность
    RX_RC.resetAvailable();
  }
}
static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64];
  unsigned int i = 0;
  while (Dec > 0) {
    bin[32 + i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }
  for (unsigned int j = 0; j < bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  return bin;
}
