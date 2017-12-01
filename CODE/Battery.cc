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

#include "Battery.h"

Define_Module(BatteryModel);
BatteryModel::BatteryModel()
{
    eventActive= eventSleep = patteryMsg = nullptr;  // set variables to null
}


BatteryModel::~BatteryModel()
{
    cancelAndDelete(eventActive);         // cancel and delete event object
    cancelAndDelete(eventSleep);
    delete patteryMsg;             // delete message when destructor is called
}

//to initialize parameters
void BatteryModel::initialize()
{
    c=par("dCapacityratio");
    k=par("dRateconstant");

    i=par("chargeinAvailablewell");
    h1=i/c;

    j=par("chargeinBoundwell");
    h2=j/(1-c);

    I=0;

    kp=k/(c*(1-c));
    ks=k/h2;
    charge=0;
    qtotal=i+j;
    sJ=0;
    sI=0;
    current_state=0; //state 0 means idle, 1 means sending, 2 receiving

    T=par("dLeastCountTime");

    requestedcurrent=0;
    requestedchargeunits=0;
    Integer_multiple=1000;
    patteryMsg=nullptr;

    availablewellsignal = registerSignal("av_well");
    boundwellsignal=registerSignal("bound_well");
    changeDisplay(i);
    EV << "The i is"  << i<<"\n";
    EV << "The j is"  << j<<"\n";

    emit(availablewellsignal, i);
    emit(boundwellsignal, j);
    t=simTime();
    EV << "the current time"  << t<<"\n";
    eventSleep = new cMessage();
    eventActive=new cMessage();

}

void BatteryModel::handleMessage(cMessage *msg)
{

    // check if it is self message for sleep
    //The calculation details explained in section 4.1.2 in report
    if(msg==eventSleep)
    {
        //check the exit condition, if i<=0 the simulation ends

        if(i>0)
        {

            pi=uniform(0,100);
            EV << "Node is in sleep "  << "\n";
            EV << "the value of pi:" <<pi<< "\n";

            // check the probability of recovery, if condition is satisfied battery module would recover partially.
            if(pi<93)
            {

                //The calculation details explained in section 4.1.2 in report
                d0=h2-h1;
                qtotal=i+j;

                r=exp(-(ks*T*h2));
                d=d0*r;

                hd=qtotal-(d*(1-c));

                /*The following code allow to deduct only integer values from i and j,
                 * The values have been accumulated in sI*/


                sI=sI+((hd-h1)*c);

                J=floor(sI);
                sI=sI-J;
                EV << "Value of J: " << J<< "\n";
                i=i+J;
                j=j-J;

                t=simTime();
                EV << "Value of i: " << i << "\n";
                EV << "Value of j: " << j << "\n";
                EV << "Value of time: " << t << "\n";
                h1=i/c;
                h2=j/(1-c);

                //collecting statitics
                emit(availablewellsignal, i);
                emit(boundwellsignal, j);

                if (i>0)
                {
                    changeDisplay(i);

                    //scheduling next sleep after T.

                    if(!eventSleep->isScheduled())
                    {
                        scheduleAt(simTime()+T,eventSleep);
                    }



                }
                // if i<=0 the simulation ends, code to end the simulation
                else
                {
                    i=0;
                    changeDisplay(i);
                    // terminate();

                }


            }
            //when no recovery
            else
            {
                t=simTime();

                EV << "Value of i: " << i << "\n";
                EV << "Value of j: " << j << "\n";
                EV << "Value of time: " << t << "\n";

                if(!eventSleep->isScheduled())
                {
                    scheduleAt(simTime()+T,eventSleep);
                }


            }

        }
        // if i<=0 the simulation ends, code to end the simulation
        else
        {
            i=0;
            changeDisplay(i);
            terminate();

        }

    }


    // check if it is self message for sleep

    //The calculation details explained in section 4.1.1 in report
    else if(msg==eventActive)
    {



        //check the exit condition, if i<=0 the simulation ends
        if(i>0)
        {
            //the loop will be executed until battery delivers the requested charge unit.

            if (charge<=requestedchargeunits)
            {
                //qI is the probability that in one time
                //unit, called a time slot, I charge units are demanded

                qi=intuniform(1,100);
                I=qi;
                d0=h2-h1;
                J=ks*d0*h2;

                // EV << "Value of J: " << J << "\n";
                /*The following code allow to deduct only integer values from  j,
                                 * The values have been accumulated in sJ*/
                sJ=sJ+J;
                J=floor(sJ);
                sJ=sJ-J;
                i=i+J;
                j=j-J;
                charge=charge+I;
                if(charge<requestedchargeunits)
                {
                    i=i-I;
                    if(i<=0)
                    {
                        i=0;
                        changeDisplay(i);

                    }
                }

                else
                {
                    charge=charge-requestedchargeunits;
                    I=I-charge;
                    i=i-I;
                    if(i<=0)
                    {
                        i=0;

                        changeDisplay(i);
                    }
                    charge= requestedchargeunits+1;
                    // count++;
                }
                h1=i/c;
                h2=j/(1-c);

                t=simTime();

                EV << "Value of t: " << t << "\n";
                EV << "The i is"  << i<<"\n";
                EV << "The j is"  << j<<"\n";

                emit(availablewellsignal, i);
                emit(boundwellsignal, j);


                changeDisplay(i);

                //schedule self message to same instant until battery delivers the requested charge unit.

                if(!eventActive->isScheduled())
                {
                    scheduleAt(simTime(),eventActive);
                }
            }
            //schedule self message after simulation instance T.
            else {
                charge=0;
                scheduleAt(simTime()+T, eventActive);
            }
        }


        // if i<=0 the simulation ends, code to end the simulation
        else
        {
            changeDisplay(i);
            i=0;
            terminate();
        }

    }

    //to handle message recvd from Energy Consumption module

    else {
        // received message from the Energy module

        EV << "Message " << msg << " arrived.\n";


        simtime_t arrival_time;

        //receive and extract parameters
        patteryMsg=check_and_cast<BatteryMsg *>(msg);
        arrival_time=patteryMsg->getArrivalTime();
        EV << "rcv time: " << arrival_time<< "\n";

        current_state= patteryMsg->getState();
        requestedcurrent=patteryMsg->getCurrent();

        requestedchargeunits=requestedcurrent*Integer_multiple*T;
        EV << "requestedchargeunits are" <<requestedchargeunits<< "\n";


        if(current_state==0)
        {
            EV << "The energy consumer is Idle" << "\n";

            if(!eventSleep->isScheduled())
            {
                //sI=0;
                scheduleAt(simTime()+(T-.0001), eventSleep);  // schedule sleep for node
                                                               //.0001 to adjust timing
            }



            if(eventActive->isScheduled())
            {
                cancelAndDelete(eventActive);
                eventActive=new cMessage();// cancel the scheduled active state

            }
        }



        else
        {
            EV << "The energy consumer is Active" << "\n";
            if(!eventActive->isScheduled())      // schedule active state for the node
            {
                scheduleAt(simTime()+(T-.0001), eventActive); //.0001 to adjust timing
            }

            if(eventSleep->isScheduled())
            {
                cancelAndDelete(eventSleep);  // cancel the scheduled sleep state
                eventSleep = new cMessage();
            }


        }

        delete msg;
    }

}

//to display charge in the available well
void BatteryModel::changeDisplay(double h4)
{
    char buffer[30];
    sprintf(buffer,"available well(i): %.0f ",h4);
    getDisplayString().setTagArg("t",0,buffer);
    //    if(dCurrent >=50)
    //    {
    getDisplayString().setTagArg("i",1,"gold");
    getDisplayString().setTagArg("t",2,"blue");

}


//The code for terminating when i<0
void BatteryModel::terminate()
{

    //notify the Energy consumer module modules
    patteryMsg= new BatteryMsg();
    patteryMsg->setName("battery");

    send(patteryMsg,"out");

    //cancel the sheduled events
    if(eventActive->isScheduled())
    {
        cancelAndDelete(eventActive);
    }

    if(eventSleep->isScheduled())
    {
        cancelAndDelete(eventSleep);
    }

    //if it is sender battery module, terminate receiver module as well
    if (strcmp("senderBattery", getName()) == 0)
    {
        targetModule1=getModuleByPath("StocKiBaMNetwork.receiverBattery");
        targetModule2=getModuleByPath("StocKiBaMNetwork.receiver");

    }

    else
    {
        targetModule1=getModuleByPath("StocKiBaMNetwork.senderBattery");
        targetModule2=getModuleByPath("StocKiBaMNetwork.sender");

    }
    EnergyConsumer *target2 = check_and_cast<EnergyConsumer *>(targetModule2);
    BatteryModel *target1 = check_and_cast<BatteryModel *>(targetModule1);

    target1->doSomething();
    target2->doSomething();


}

//Act as destructor when program terminates
void BatteryModel::doSomething()
{
    if(eventActive->isScheduled())
    {
        cancelAndDelete(eventActive);
    }

    if(eventSleep->isScheduled())
    {
        cancelAndDelete(eventSleep);
    }
}
