# RF_SW
### Transceiver 315MHz / 433MHz on arduino. Устройство для управления домашними устройствами (радиореле) с ПК или смартфона по Bluetooth.  

Данное устройство, управляемое по Bluetooth, представляет собой приемник (одной из двух доступных частот) и передатчика 
(передача на частотах 315MHz / 433MHz производится без необходимости переключения режима) кодов радиопультов.  
Основой программной части является библиотека RCSwitch, а аппаратной - Arduino Pro Mini 3.3V 8MHz, MX-RM-5V and FS1000A (315MHz), 
WL101-341 and WL102-341 (433MHz) и Bluetooth модуль JDY-31.  

```
Format cmd: [Type_RC]+[BinCode_RC]+[Protocol_RC]+[PulseLength_RC](+[RepeatTX])
Type_RC = 315 or 433
BinCode_RC - received binary code
Protocol_RC - protocol number of received code
PulseLength_RC - length of received code
RepeatTX - optional argument, the number of times to resubmit the code.
Using: send the received command for repeat code or send the command 'l315' to switch to listening mode 315 MHz, for 433 MHz - 'l433'.
```

Тестирование.    
<details><summary>Прием кода пульта 433MHz, копирование его и управление радиореле 2201-30A, эмулируя пульт с помощью RF_SW</summary>  

![Тест](https://github.com/AlexanderChad/RF_SW/blob/main/Screenshot_terminal_0.jpg)  
</details>
 
<details><summary>Видео с демонстрацией работы.</summary>  


https://user-images.githubusercontent.com/55314805/189281198-2b6f32af-1966-4131-bc19-83414e48bc60.mp4


</details>
