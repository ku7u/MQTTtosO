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
    _keepAlive = 0;
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

uint32_t detector::getInterval()
{
    // set the interval to zero if train has passed or has stopped
    if (millis() - _keepAlive > 10000)
        wheelInterval = 0;
    return wheelInterval;
}

void detector::zeroWheelInterval()
{
    wheelInterval = 0;
}

bool detector::detected()
{
    bool myDetected;

    myDetected = _detected;
    _detected = false;
    return myDetected;
}

bool detector::check()
{
    /*
      _flagBits bit assignments
      bit 0  = east detector
          1  = west detector
          2  = entered from east, westbound
          3  = entered from west. eastbound
          4  = staged westbound
          5  = staged eastbound
    */

    // read the detector bits from hardware
    // replace the detector bits in _flagBits with the new ones
    bitWrite(_flagBits, 1, digitalRead(_detectorPinWest));
    bitWrite(_flagBits, 0, digitalRead(_detectorPinEast));
    pinMode(2, OUTPUT);
    switch (_flagBits)
    {
    case B00000000:
        _triplocked = false;
        _endLocked = false;
        break; // no more work to do

    case B00000001:            // detect entry westbound
        _flagBits = B00000100; // set the detected bit westbound
        if (!_triplocked && !_endLocked)
        {
            trippedTime = micros(); // for speedometer
            digitalWrite(2, HIGH);
            _triplocked = true;
        }
        break;

    case B00000010:            // detect entry eastbound
        _flagBits = B00001000; // set the detected bit eastbound
        if (!_triplocked && !_endLocked)
        {
            trippedTime = micros(); // for speedometer
            digitalWrite(2, HIGH);
            _triplocked = true;
        }
        break; // no more work to do

    case B00000100:            // cleared with detected bit set, westbound
        _flagBits = B00000000; // clear all
        digitalWrite(2, LOW);
        _triplocked = false;
        break;

    case B00001000:            // cleared with detected bit set, eastbound
        _flagBits = B00000000; // clear all
        digitalWrite(2, LOW);
        _triplocked = false;
        break;

    case B00000111: // both detectors triggered, westbound
        if ((!_endLocked) && _triplocked)
            wheelInterval = micros() - trippedTime; // this interval can be used to calculate speed
        digitalWrite(2, LOW);
        _direction = WEST;
        _triplocked = false;
        _endLocked = true;
        return false;

    case B00001011: // both detectors triggered, eastbound
        if ((!_endLocked) && _triplocked)
            wheelInterval = micros() - trippedTime; // this interval can be used to calculate speed
        digitalWrite(2, LOW);
        _direction = EAST;
        _triplocked = false;
        _endLocked = true;
        return false;

    case B00000110:            // trailing detector triggered, westbound
        _flagBits = B00010000; // set the staged bit westbound
        return false;

    case B00001001:            // trailing detector triggered, eastbound
        _flagBits = B00100000; // set the staged bit eastbound
        return false;

    case B00010000:            // detectors clear while staged, count westbound
        _flagBits = B00000000; // reset
        _keepAlive = millis();
        _detected = true;
        _direction = WEST;
        _triplocked = false;
        _endLocked = false; // enable speedometer
        return true;

    case B00100000:            // detectors clear while staged, count eastbound
        _flagBits = B00000000; // reset
        _keepAlive = millis();
        _detected = true;
        _direction = EAST;
        _triplocked = false;
        _endLocked = false; // enable speedometer
        return true;

    case B00010001:            // trailing detector cleared while staged, back to entry now set, westbound
        _flagBits = B00000100; // clear the staged bit, set detected bit westbound
        break;

    case B00100010:            // trailing detector cleared while staged, back to entry now set, eastbound
        _flagBits = B00001000; // clear the staged bit, set detected bit eastbound
        break;

    default:
        break;
    }
    return false;
}
