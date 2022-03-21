
#include "bod.h"
#include "routines.h"

detector::detector(uint8_t westPin, uint8_t eastPin)
{
    _detectorPinWest = westPin;
    pinMode(_detectorPinWest, INPUT_PULLUP);
    _detectorPinEast = eastPin;
    pinMode(_detectorPinEast, INPUT_PULLUP);
    _flagBits = B00000000; // clear all
    _detected = false;
}

void detector::setBlockWest(uint8_t westBlock)
{
    _blockWest = westBlock;
}

void detector::setBlockEast(uint8_t eastBlock)
{
    _blockEast = eastBlock;
}

void detector::setWestKeeper(bool keeper)
{
    westKeeper = keeper;
}

void detector::setEastKeeper(bool keeper)
{
    eastKeeper = keeper;
}

void detector::setDisplayDetect(bool displayOn)
{
    displayDetect = displayOn;
}

uint8_t detector::getBlockWest()
{
    return _blockWest;
}

uint8_t detector::getBlockEast()
{
    return _blockEast;
}

uint8_t detector::direction()
{
    return _direction;
}

bool detector::check()
{
    _detected = false;

    // read the detector bits from hardware
    //  return true if detected, false otherwise TBD maybe remove detected() routine and _detected var
    _testByte = 0;
    bitWrite(_testByte, 1, digitalRead(_detectorPinWest));
    bitWrite(_testByte, 0, digitalRead(_detectorPinEast));

    // replace the detector bits in _flagBits with the new ones
    _flagBits &= B00111100; // clear bits 0, 1 first
    _flagBits |= _testByte;  // insert the new bits 0, 1

    switch (_flagBits)
    {
        // on startup, if both detectors are totally obscured we have to ignore it
        // don't know where it came from
        // will result in an uncounted axle in a block which will be cleared when it leaves
        // storing the flagbits status prevents this from occurring

    case B00000000:
        break; // no more work to do

    case B00000001:            // detect entry westbound
        _flagBits = B00000100; // set the detected bit westbound
        if (displayDetect)
        {
            printMsg("detect westbound");
        }
        break;

    case B00000010:            // detect entry eastbound
        _flagBits = B00001000; // set the detected bit eastbound
        if (displayDetect)
        {
            printMsg("detect eastbound");
        }
        break; // no more work to do

    case B00000110:            // trailing detector triggered, westbound
        _flagBits = B00010000; // set the staged bit westbound
        if (displayDetect)
        {
            printMsg("staged westbound");
        }
        return false;

    case B00001001:            // trailing detector triggered, eastbound
        _flagBits = B00100000; // set the staged bit eastbound
        if (displayDetect)
        {
            printMsg("staged eastbound");
        }
        return false;

    case B00010000:            // detectors clear while staged, count westbound
        _flagBits = B00000000; // reset
        if (displayDetect)
        {
            printMsg("count westbound");
        }
        _direction = WEST;
        return true;

    case B00100000:            // detectors clear while staged, count eastbound
        _flagBits = B00000000; // reset
        if (displayDetect)
        {
            printMsg("count eastbound");
        }
        _direction = EAST;
        return true;

    case B00010001:            // trailing detector cleared while staged, entry now set, westbound
        _flagBits = B00000100; // clear the staged bit, set detected bit westbound
        break;

    case B00100010:            // trailing detector cleared while staged, entry now set, eastbound
        _flagBits = B00001000; // clear the staged bit, set detected bit eastbound
        break;

    case B00000100:            // cleared with detected bit set, westbound
        _flagBits = B00000000; // clear all
        break;

    case B00001000:            // cleared with detected bit set, eastbound
        _flagBits = B00000000; // clear all
        break;

    default:
        break;
    }
    return false;
}
