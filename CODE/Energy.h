//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __ENERGYCONSUMPTION_DEVICENODE_H_
#define __ENERGYCONSUMPTION_DEVICENODE_H_
#include <omnetpp.h>
//using namespace omnetpp;

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "dataMsg_m.h"
#include "Battery.h"
#include "batteryMsg_m.h"


/**
 * TODO - Generated class
 */
class EnergyConsumer : public cSimpleModule
{
private:
    //consumer parameters
    int iMessageInterval;        // message sending interval
    double dMessageLength;          // message length in bytes
    double dEnergyUsedInSleep;      // energy used in sleep
    double dEnergyUsedInSend;       // energy consumed in send mode
    double dEnergyUsedInReceive;    // energy consumed while sending
    double dMinimumBatteryLevel;    // The cutoff freequency
    int iSendingFrequency;       // sending Frequency
    int state;
    double frequency;
    double transmissionTime;

    cMessage *eventActive;  // pointer to the event object which we'll use for timing
    cMessage *eventSleep;  // pointer to the event object which we'll use for timing
    DataMsg *dataMsg;  // variable to remember the message until we send it back
    BatteryMsg *batteryMsg;



    simsignal_t batterySignal;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void changeDisplay();



public:
    void doSomething(); //Termination of Energy consumer module act like destructor
    EnergyConsumer();
    virtual ~EnergyConsumer();
};

#endif
