#include "InterfaceDcmEth.hpp"

#include "InterfaceRingBuffer.hpp"
using TypeRingBuffer = InterfaceRingBuffer<Message, SIZE_RING_BUFFER>;
unique_ptr<TypeRingBuffer> ringBuffer = TypeRingBuffer::Instantiate();
void InterfaceDcmEth_vRxIndication(Message& message){
   ringBuffer->push(message);
}

typedef int uint8;

uint8 Ascii2HexNibble(char chAscii){
   uint8 lu8ValueReturn;
        if(chAscii >= '0' && chAscii <= '9'){lu8ValueReturn = chAscii - '0'     ;}
   else if(chAscii >= 'a' && chAscii <= 'f'){lu8ValueReturn = chAscii - 'a' + 10;}
   else if(chAscii >= 'A' && chAscii <= 'F'){lu8ValueReturn = chAscii - 'A' + 10;}
   else                                     {lu8ValueReturn = 0                 ;}
   return lu8ValueReturn;
}

uint8 Ascii2HexByte(char* lptri8Ascii){
   return(
         Ascii2HexNibble(lptri8Ascii[0]) * 16
      +  Ascii2HexNibble(lptri8Ascii[1])
   );
}

#include "InterfaceEthDcm.hpp"
#include <string.h>
void Dcm_vExecuteService_TesterPresent(char* ptrchBuffer){
   Message message;
   switch(Ascii2HexByte(ptrchBuffer)){
      case 0x01: memcpy(message.data, "027E01", 7); break;
      default:                                      break;
   }
   InterfaceEthDcm_vTransmit(message);
}

void Dcm_vExecuteService(char* ptrchBuffer){
   switch(Ascii2HexByte(&ptrchBuffer[2])){
      case 0x3E: Dcm_vExecuteService_TesterPresent(&ptrchBuffer[4]); break;
      default:                                                       break;
   }
}

#include "InterfaceDcmSchm.hpp"
void InterfaceDcmSchm_vMainFunction(void){
   Dcm_vExecuteService(ringBuffer->pop()->data);
}
