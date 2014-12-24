// =============================================================

#include "ByteArray.h"

ByteArray::ByteArray(uint8_t* _bytes, uint16_t _length) {
    bytes = _bytes;
    length = _length;
}

// =============================================================

IndexedByteArray::IndexedByteArray(uint8_t* _bytes, uint16_t _length, uint16_t _index): ByteArray(_bytes, _length) {
    index = _index;
}

bool IndexedByteArray::isEmpty() {
    return index == 0;
}

bool IndexedByteArray::containsData() {
    return index > 0;
}

uint8_t IndexedByteArray::peekByte() {
    return bytes[index];
}

void IndexedByteArray::putByte(uint8_t b) {
    putByte(index, b);
    index++;
}

void IndexedByteArray::putByte(uint16_t index, uint8_t b) {
    bytes[index] = b;
}
        
void IndexedByteArray::putWord(uint16_t w) {
    putByte(w >> 8);
    putByte(w & 0x00ff);
}

void IndexedByteArray::putWord(uint16_t index, uint16_t w) {
    putByte(index, w >> 8);
    putByte(index + 1, w & 0x00ff);
}
        
void IndexedByteArray::putBytes(String s) {
    putBytes(reinterpret_cast<const uint8_t*>(s.c_str()), s.length());
}

void IndexedByteArray::putBytes(const uint8_t* bs, uint16_t len) {
    putBytes(index, bs, len);
    index += len;
}

void IndexedByteArray::putBytes(uint16_t index, String s) {
    putBytes(index, reinterpret_cast<const uint8_t*>(s.c_str()), s.length());
}

void IndexedByteArray::putBytes(uint16_t index, const uint8_t* bs, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        bytes[index + i] = bs[i];
    }
}

uint8_t IndexedByteArray::getByte() {
    uint8_t b = getByte(index);
    index++;
    return b;
}

uint8_t IndexedByteArray::getByte(uint16_t index) {
    return bytes[index];
}
        
uint16_t IndexedByteArray::getWord() {
    uint16_t w = getWord(index);
    index += 2;
    return w;
}

uint16_t IndexedByteArray::getWord(uint16_t index) {
    return bytes[index] * 256 + bytes[index + 1];
}
        
void IndexedByteArray::drop(uint16_t count) {
    index += count;
}

// =============================================================
