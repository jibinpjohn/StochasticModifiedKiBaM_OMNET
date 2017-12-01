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

package final;

//
// TODO auto-generated module
//
simple EnergyConsumer
{
    parameters:
        @display("i=device/antennatower");
        double dMessageSendingInterval;
        int SendingFrequency; // bits/s
        double MessageLength; //bytes
        
        
        volatile double nextMsgGenInterval @unit(s) =  default(uniform(1.0s, 60.0s));
           
        int sending_interval;
        double energy_consumption_sleep;
        double energy_consumption_send;
        double energy_consumption_rcv;
        double dMinBatteryLevel;
    gates:
        input in;
        input in1;
        output out;
        output out1;
}