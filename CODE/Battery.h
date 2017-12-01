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

#ifndef __KIBAM_BATTERYMODEL_H_
#define __KIBAM_BATTERYMODEL_H_

#include <omnetpp.h>
#include <math.h>

#include "batteryMsg_m.h"
#include "Energy.h"

/**
 * TODO - Generated class
 */
class BatteryModel : public cSimpleModule
{
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);


public:
    virtual void changeDisplay(double h4); //to display
    virtual void terminate(); // to finish the battery module
    void doSomething();    //act like destructor
    BatteryModel();
    virtual ~BatteryModel();
private:
    double h1;
    double h2;
    double i;
    double j;
    double c;
    double requestedcurrent;
    double requestedchargeunits;
    double pi;
    double r;
    double k;
    double kp;
    double ks;
    double T;
    double d0;
    double d;
    double qtotal;
    double hd;
    double tempcharge;
    double charge;
    double J;
    double sJ;
    double sI;

    int  Integer_multiple;
    int current_state;
    int qi;
    int I;
    //int count;
    BatteryMsg *patteryMsg;
    cMessage *eventSleep;
    cMessage *eventActive;
    // cMessage *eventdelay;
    simsignal_t availablewellsignal;
    simsignal_t boundwellsignal;
    simtime_t t;
    cModule *targetModule1;
    cModule *targetModule2;
};

#endif
