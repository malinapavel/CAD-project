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
simple Scheduler
{
      parameters:
        @display("i=block/dispatch");
        string algorithm = default("wrr");
        double schedulingPeriod @unit(ms) = default(1ms);
        int gateSize=default(3);//me, dt
        @signal[prioritySignalHq](type="double");
        @signal[prioritySignalMq](type="double");
        @signal[prioritySignalLq](type="double");
        @statistic[currPriorityHq](title="last time served"; source="prioritySignalHq"; record=vector,stats; interpolationmode=none);
        @statistic[currPriorityMq](title="last time served"; source="prioritySignalMq"; record=vector,stats; interpolationmode=none);
        @statistic[currPriorityLq](title="last time served"; source="prioritySignalLq"; record=vector,stats; interpolationmode=none);

    gates:
        output txScheduling[gateSize];
        input rxInfo[gateSize];
        input rxPriority[gateSize];
}