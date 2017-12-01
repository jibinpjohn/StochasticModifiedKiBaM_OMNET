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

network StocKiBaMNetwork
{
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        sender: EnergyConsumer {
            @display("p=234,50");
        }
        receiver: EnergyConsumer {
            @display("p=333,249");
        }
        senderBattery: BatteryModel {
            @display("p=103,100");
        }
        receiverBattery: BatteryModel {
            @display("p=490,249");
        }
    connections:
        sender.out --> {  delay = 0ms; } --> receiver.in;
        sender.in <-- {  delay = 0ms; } <-- receiver.out;
        sender.out1 --> {  delay = 0ms; } --> senderBattery.in;
        sender.in1 <-- {  delay = 0ms; } <-- senderBattery.out;
        receiver.out1 --> {  delay = 0ms; } --> receiverBattery.in;
        receiver.in1 <-- {  delay = 0ms; } <-- receiverBattery.out;

}
