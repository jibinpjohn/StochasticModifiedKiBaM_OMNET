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

#include "Energy.h"

Define_Module(EnergyConsumer);

EnergyConsumer::EnergyConsumer()
{
    eventActive= eventSleep = dataMsg = nullptr;  // set variables to null
}


EnergyConsumer::~EnergyConsumer()
{
    cancelAndDelete(eventActive);         // cancel and delete event object
    cancelAndDelete(eventSleep);
    delete dataMsg;             // delete message when destructor is called
}

void EnergyConsumer::initialize()
{
    // TODO - Generated method body

    dataMsg = nullptr;
    batteryMsg=nullptr;
    eventActive=new cMessage();    // event is used to schedule wakeup
    eventSleep = new cMessage();    // used to schedule sleep



    // consumer parameters
    iMessageInterval = 0;       // set message sending interval
    dMessageLength = 0;         //  message length in bytes

    dEnergyUsedInSend=par("energy_consumption_send");
    dEnergyUsedInReceive=par("energy_consumption_rcv");
    iSendingFrequency = par("SendingFrequency");

    state=0;



    changeDisplay();      // show the current state

    double dWakeupTime = par("nextMsgGenInterval");   // get a random number between 0 and interval, wake up device if not 0
    // double dWakeupTime=120;
    iMessageInterval=par("sending_interval");
    dMessageLength=par("MessageLength");
    frequency=par("SendingFrequency");
    transmissionTime=(dMessageLength*8)/frequency;
    scheduleAt((simTime() + dWakeupTime), eventActive);
    EV << "sending time :"  << transmissionTime<<"\n";

}

void EnergyConsumer::handleMessage(cMessage *msg)
{

    if(msg == eventActive)  // if it is a wake up self message
    {
        // if message interval is greater than 0, node is a sender
        if(iMessageInterval>0)
        {
            state=1;
            changeDisplay();
            // send message
            dataMsg = new DataMsg();   //create data message
            dataMsg->setByteLength(dMessageLength); //set the message length
            send(dataMsg,"out");  // send message
            batteryMsg= new BatteryMsg();   //notify the battery module
            batteryMsg->setCurrent(dEnergyUsedInSend);  //set the parameters, current and state
            batteryMsg->setState(state);
            send(batteryMsg,"out1");
            simtime_t sending;
            sending=batteryMsg->getSendingTime();   //to print the sending time
            EV << "sending time: " << sending << "\n";



            // go to sleep after sending the message, depending on transmission time
            if(!eventSleep->isScheduled())
            {
                scheduleAt((simTime() +transmissionTime), eventSleep);  // schedule self message
            }
        }

        else
        {
            // node receives only
            // schedule self message
            if(!eventSleep->isScheduled())
            {
                scheduleAt((simTime() + transmissionTime), eventSleep);  // schedule sleep for rcv node
            }
        }

    }
    //check whether self message for sleep/idle
    else if(msg == eventSleep)
    {
        if(iMessageInterval>0)
        {
            state=0;
            changeDisplay();
            batteryMsg= new BatteryMsg();         //Notify battery module about state change
            batteryMsg->setState(state);
            batteryMsg->setCurrent(0);
            send(batteryMsg,"out1");


            if(!eventActive->isScheduled())                   //if active is not schedule, scheduling it
            {
                scheduleAt((simTime() + iMessageInterval), eventActive);
            }
        }
        //for receiver, no need to schedule next active
        else
        {
            state=0;
            changeDisplay();
            batteryMsg= new BatteryMsg();
            batteryMsg->setState(state);
            batteryMsg->setCurrent(0);
            send(batteryMsg,"out1");

        }
    }

    //To handle the messages from other module
    else
    {
        //To handle the message from battery module notifying the termination.
        if(strcmp(msg->getName(),"battery") == 0)
        {
            EV << "Message from battery module arrived" << msg << " arrived.\n";
            batteryMsg=check_and_cast<BatteryMsg *>(msg);
            cancelAndDelete(eventSleep);
            cancelAndDelete(eventActive);
            eventSleep=eventActive=nullptr;
            //delete msg;
        }
        //To handle the message from Energy module
        else
        {
            state=2;
            changeDisplay();
            EV << "Message from sender has been arrived" << msg << " arrived.\n";
            batteryMsg= new BatteryMsg();
            batteryMsg->setState(state);
            batteryMsg->setCurrent(dEnergyUsedInReceive);
            send(batteryMsg,"out1");

            if(!eventSleep->isScheduled())
            {
                scheduleAt((simTime() + transmissionTime), eventSleep);  // schedule sleep for node
            }
            //delete msg;
        }
        delete msg;
    }



}
//To display the value of state
//state 0 means idle, 1 means sending, 2 receiving
void EnergyConsumer::changeDisplay()
{

    char buffer[30];

    if(state==1)
    {
        sprintf(buffer,"The node is in state:Sending");
        getDisplayString().setTagArg("t",0,buffer);


        getDisplayString().setTagArg("i",1,"green");
        getDisplayString().setTagArg("t",2,"green");

        getDisplayString().setTagArg("i2",0,"status/up");
    }

    else if(state==0)
    {
        sprintf(buffer,"The node is in state:Idle");
        getDisplayString().setTagArg("t",0,buffer);


        getDisplayString().setTagArg("i",1,"blue");
        getDisplayString().setTagArg("t",2,"blue");

        getDisplayString().setTagArg("i2",0,"status/down");

    }

    else
    {
        sprintf(buffer,"The node is in state:receiving");
        getDisplayString().setTagArg("t",0,buffer);


        getDisplayString().setTagArg("i",1,"yellow");
        getDisplayString().setTagArg("t",2,"yellow");

        getDisplayString().setTagArg("i2",0,"status/up");
    }
}

//Termination of Energy consumer module act like destructor
void EnergyConsumer::doSomething()
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

