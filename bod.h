// Copyright 2022 George F. Hofmann

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
    bool check();
    bool detected();
    uint8_t direction();
    uint16_t westCount;
    uint16_t eastCount;
    bool westKeeper;
    bool eastKeeper;
    bool displayDetect = false;

/*****************************************************************************/
private:
    uint8_t _detectorPinWest;
    uint8_t _detectorPinEast;
    uint8_t _blockWest;
    uint8_t _blockEast;
    uint8_t _testByte;
    uint8_t _flagBits;
    bool _detected;
    enum Direction : uint8_t
    {
        WEST,
        EAST
    }; 
    Direction _direction;

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