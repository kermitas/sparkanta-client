// =============================================================

#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include "application.h"

class ByteArray
{
	public:
	    uint8_t* bytes;
	    uint16_t length;
	
		ByteArray(uint8_t* _bytes, uint16_t _length);
        
		virtual ~ByteArray();
};

#endif /* BYTE_ARRAY_H */

// =============================================================

#ifndef INDEXED_YTE_ARRAY_H
#define INDEXED_YTE_ARRAY_H

class IndexedByteArray: public ByteArray
{
	public:
	    uint16_t index;
	
		IndexedByteArray(uint8_t* _bytes, uint16_t _length, uint16_t _index);
        
        virtual bool isEmpty();
        virtual bool containsData();
        
        virtual void putByte(uint8_t b);
        virtual void putByte(uint16_t index, uint8_t b);
        
        virtual void putWord(uint16_t w);
        virtual void putWord(uint16_t index, uint16_t w);
        
        virtual void putBytes(String s);
        virtual void putBytes(const uint8_t* bs, uint16_t len);
        virtual void putBytes(uint16_t index, String s);        
        virtual void putBytes(uint16_t index, const uint8_t* bs, uint16_t len);
        
        virtual uint8_t getByte();
        virtual uint8_t getByte(uint16_t index);
        
        virtual uint16_t getWord();
        virtual uint16_t getWord(uint16_t index);
        
        virtual void drop(uint16_t count);
        
		virtual ~IndexedByteArray();
};

#endif /* INDEXED_YTE_ARRAY_H */

// =============================================================
