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

#include "Scheduler.h"

Define_Module(Scheduler);



Scheduler::Scheduler()
{
    selfMsg = nullptr;
}

Scheduler::~Scheduler()
{
    cancelAndDelete(selfMsg);
}


void Scheduler::initialize()
{
    nrQueues = par("gateSize").intValue();
    selfMsg = new cMessage("selfMsg");
    scheduleAt(simTime(), selfMsg);

    for (int i = 0; i<nrQueues; i++) {
        priority[i] = 0;
    }
}

void Scheduler::handleMessage(cMessage *msg)
{
    for(int i = 0; i<nrQueues;i++) {
        if (msg->arrivedOn("rxInfo", i)){
            q[i]= msg->par("ql_info");
            delete(msg);
        }
        else if (msg->arrivedOn("rxPriority", i)) {
            priority[i] = msg->par("q1_priority").doubleValue() * (double)(3-i);
            delete(msg);
        }
    }


    if (msg == selfMsg) {
        double min = priority[0];
        int curr_index = 0;

        cMessage *cmd = new cMessage("cmd");

        for(int i =0;i<nrQueues;i++){

            if(q[i] > 0 && priority[i] < min){
                curr_index = i;
                min = priority[i];
            }

            EV << "Priority for " << i << " is " << priority[i] << endl;
        }

        send(cmd, "txScheduling", curr_index);

        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);
    }
}
