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
simple Sink
{
 parameters:
        int gateSize=default(3);
        @display("i=block/sink");
      //  @signal[lifetime](type="simtime_t");
      //  @statistic[lifetime](title="lifetime of arrived jobs"; unit=s; record=vector,mean,max; interpolationmode=none);
       @signal[prioritySignalHq](type="double");
        @signal[prioritySignalMq](type="double");
        @signal[prioritySignalLq](type="double");
        @statistic[currPriorityHq](title="last time served"; source="prioritySignalHq"; record=vector,stats; interpolationmode=none);
        @statistic[currPriorityMq](title="last time served"; source="prioritySignalMq"; record=vector,stats; interpolationmode=none);
        @statistic[currPriorityLq](title="last time served"; source="prioritySignalLq"; record=vector,stats; interpolationmode=none);
    gates:
        input rxPackets[gateSize];

}