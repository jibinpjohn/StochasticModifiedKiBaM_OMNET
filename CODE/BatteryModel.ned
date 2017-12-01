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
simple BatteryModel
{


parameters:
    	@display("i=block/routing");
        double chargeinAvailablewell;
        double chargeinBoundwell;
        double dRateconstant;
        double dProbabilityrecovery;
        double dCapacityratio;
        double dLeastCountTime;
        
        @signal[av_well](type="double");
        @statistic[chargeinAVwell](title="charge in available well (i)"; source="av_well"; record=vector,stats; interpolationmode=none);
    	@signal[bound_well](type="double");
        @statistic[chargeinBDwell](title="charge in bound well (j)"; source="bound_well"; record=vector,stats; interpolationmode=none);
       
       
      gates:
        input in;
        output out;
}
