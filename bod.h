/* 
MIT License

Copyright (c) 2022 George Hofmann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef BOD_H
#define BOD_H
#include <Arduino.h>

class detector 
{ 
/*****************************************************************************/
public:
    detector(uint8_t westPin, uint8_t eastPin); // TBD the type
    void setBlockWest(uint8_t westBlock);
    void setBlockEast(uint8_t eastBlock);
    void setWestKeeper(bool keeper);
    void setEastKeeper(bool keeper);
    void setDisplayDetect(bool displayOn);
    uint8_t getBlockWest();
    uint8_t getBlockEast();
    uint32_t getInterval();
    void zeroWheelInterval();
    bool check();
    bool detected();
    uint8_t direction();
    bool inProcess();
    uint16_t westCount;
    uint16_t eastCount;
    bool westKeeper;
    bool eastKeeper;
    bool displayDetect = false;

/*****************************************************************************/
private:
    uint32_t wheelInterval;
    uint32_t trippedTime;
    uint8_t _detectorPinWest;
    uint8_t _detectorPinEast;
    uint8_t _blockWest;
    uint8_t _blockEast;
    uint8_t _testByte;
    uint8_t _flagBits;
    bool _detected;
    bool _endLocked;
    bool _triplocked;
    enum Direction : uint8_t
    {
        WEST,
        EAST
    }; 
    Direction _direction;
    uint32_t _keepAlive;

};

#endif

  /*
    _flagBits[n] bit assignments
    bit 0  = east detector
        1  = west detector
        2  = entered from east, westbound
        3  = entered from west. eastbound
        4  = staged westbound
        5  = staged eastbound
  */