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

    prioritySignalHq = registerSignal("prioritySignalHq");
    prioritySignalMq = registerSignal("prioritySignalMq");
    prioritySignalLq = registerSignal("prioritySignalLq");
}

void Scheduler::handleMessage(cMessage *msg)
{
    for(int i = 0; i<nrQueues;i++) {
        if (msg->arrivedOn("rxInfo", i)){
            q[i]= msg->par("ql_info");
            delete(msg);
        }
        else if (msg->arrivedOn("rxPriority", i)) {
            priority[i] = msg->par("q1_priority").doubleValue();

            double currPriority = priority[i];

            // Emit signal for data collection

            if (i == 2) {
                emit(prioritySignalHq, currPriority);
            }
            else if (i == 1) {
                emit(prioritySignalMq, currPriority);
            }
            else if (i == 0) {
                emit(prioritySignalLq, currPriority);
            }

            delete(msg);
        }
    }


    if (msg == selfMsg) {
        double currSimTime = simTime().dbl();
        double max = 3 * (currSimTime - priority[2]);
        int curr_index = 2;

        cMessage *cmd = new cMessage("cmd");

        std::string algorithm = par("algorithm").stringValue();

        // Weighted Round-Robin
        if (algorithm == "wrr") {
            for(int i = nrQueues;i>=0;i--){
                double currPriority = (i+1) * (int)(currSimTime - priority[i]);
                if(q[i] > 0 && currPriority > max){
                    curr_index = i;
                    EV << "Curr max: " << max << " updated to " << currPriority << endl;
                    max = currPriority;
                }

                if (i == 2)
                    EV << "Priority for HP is " << currPriority << endl;
                else if (i == 1)
                    EV << "Priority for MP is " << currPriority << endl;
                else if (i == 0)
                    EV << "Priority for LP is " << currPriority << endl;
            }

            EV << "Sending to index " << curr_index << " with priority of " << max << endl;
        }
        // Priority Queue
        else if (algorithm == "prio") {
            for (int i = nrQueues; i>=0; i--) {
                if (q[i] > 0) {
                    curr_index = i;
                    break;
                }
            }

            EV << "Sending to index " << curr_index << " with no. elements of " << q[curr_index] << endl;
        }

        send(cmd, "txScheduling", curr_index);
        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);
    }
}
