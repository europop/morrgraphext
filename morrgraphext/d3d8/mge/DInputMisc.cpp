#define _DInputMisc
#include "DInputHeader.h"

void FakeKeyPress(BYTE key,BYTE data) {
    FakeBuffer[FakeBufferEnd].dwOfs=key;
    FakeBuffer[FakeBufferEnd++].dwData=data;
}
void FakeString(BYTE chars[],BYTE data[],BYTE length) {
    for(int i=0;i<length;i++) {
        FakeKeyPress(chars[i],data[i]);
    }
}
