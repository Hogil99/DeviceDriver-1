#pragma once
/*
 * This class represents the interface to a Flash Memory Device. The hardware has only two methods - 'read' and 'write'
 * However, the interface for using the device is a lot more complex than that. It is outlined in the top-level README file.
 */

#include "ReadFailException.h"
#include "WriteFailException.h"

const long ADDR_LIST[] = { 0x00, 0x01, 0x02, 0x03, 0x04 };

class FlashMemoryDevice
{
public:
    virtual unsigned char read(long address) = 0;
    virtual void write(long address, unsigned char data) = 0;
};