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

#include "Source.h"

Define_Module(Source);

Source::Source()
{
    sendMessageEvent = nullptr;
}

Source::~Source()
{
    cancelAndDelete(sendMessageEvent);
}

void Source::initialize()
{
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime(), sendMessageEvent);
}

void Source::handleMessage(cMessage *msg)
{
    ASSERT(msg == sendMessageEvent);

    if (simTime() > 4000) {
        endSimulation();
    }

    int priorityLevel; // Calculate the priority level here for the current user
    //generateIPPacket(priorityLevel);
    cMessage *job = new cMessage("job");
    send(job, "txPackets");
    double sendingTime;
    /*netwload = data_generated/ transfer_rate
         *
         * data_generated = nrUsers* nrPackets *PacketLength / sendingTime
         * PacketLength = 1
         * nrPackets = 1
         *
         * transfer_rate = 10 ^ 6
         *
         * netwload = (nrUsers*nrPackets*PacketLength)/(sendingTime*nr_of_channels)
         *
         *sendingTime =  (nrUsers*nrPackets*PacketLength*schedulingCycle)/(netwload*nr_of_channels)
         *
         * */
    sendingTime = par("sendIaTime").doubleValue();
    //scheduleAt(simTime()+ exponential(sendingTime));
    scheduleAt(simTime()+ exponential(sendingTime), sendMessageEvent);

    //scheduleAt(simTime()+par("sendIaTime").doubleValue(), sendMessageEvent);
}

