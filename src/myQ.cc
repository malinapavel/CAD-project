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

#include "myQ.h"

Define_Module(MyQ);

void MyQ::initialize()
{
    queue.setName("queue");
}

void MyQ::handleMessage(cMessage *msg)
{
    int ql;

    if (msg->arrivedOn("rxPackets")){
        queue.insert(msg);
    } else if (msg->arrivedOn("rxScheduling")){
        //read parameters from msg
        delete msg;
        //empty the queue !
        if(!queue.isEmpty()) {
          msg = (cMessage *)queue.pop();
          send(msg, "txPackets");
        }

        cMessage *qPriority = new cMessage("qPriority");
        qPriority->addPar("q1_priority");
        qPriority->par("q1_priority").setDoubleValue(simTime().dbl());
        send(qPriority, "txPriority");
    }

    ql = queue.getLength();

    cMessage *qInfo = new cMessage("qInfo");
    qInfo->addPar("ql_info");
    qInfo->par("ql_info").setLongValue(ql);
    sendDelayed(qInfo, 0.1, "txInfo");
}
