/**
 * Yaafe extension for the Cultural Broadcasting Archive.
 *
 * Copyright (c) 2012 University of Applied Sciences Ð Institute for Creative Media Technologies
 *
 * Author : Ewald Wieser B.Sc. (fhstp.ac.at)
 *
 * Co-Authors :
 *      Dipl.-Ing. (FH) Matthias Husinsky (fhstp.ac.at)
 *      FH-Prof. Dipl.-Ing. Markus Seidl (fhstp.ac.at)
 *
 * Contributes to : 
 *      Dr. Klaus Seyerlehner (jku.at)
 *
 * This file is an extension for Standard-Yaafe, developed for Music
 * detection with the Continuous Frequency Activation Feature (CFA) for
 * the Cultural Broadcasting Archive (CBA).
 *
 * Yaafe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Yaafe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "PeakDetection.h"
#include "MathUtils.h"

using namespace std;
using namespace Eigen;

namespace YAAFE
{

PeakDetection::PeakDetection()
{
}

PeakDetection::~PeakDetection()
{
}

ParameterDescriptorList PeakDetection::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;
        
    p.m_identifier = "NbPeaks";         // parameter for number of peaks
    p.m_description = "Number of peaks to find";
    p.m_defaultValue = "5";             // default value
    pList.push_back(p);
    
    return pList;
}
    
bool PeakDetection::init(const ParameterMap& params, const Ports<StreamInfo>& inp)
{
	assert(inp.size()==1);
	const StreamInfo& in = inp[0].data;

    m_nbPeaks = getIntParam("NbPeaks",params);      // get parameter
    if (m_nbPeaks<=0) {
    	cerr << "ERROR: Invalid NbPeaks parameter !" << endl;
    	return false;
    }
    
	outStreamInfo().add(StreamInfo(in, m_nbPeaks));  // size of output stream
    return true;
}

bool PeakDetection::process(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
	assert(inp.size()==1);
	InputBuffer* in = inp[0].data;
	if (in->empty()) return false;
	assert(outp.size()==1);
	OutputBuffer* out = outp[0].data;
	const int N = in->info().size;

    while (!in->empty())
    {
        double* input = in->readToken();

    	double peaks[N];    // list for peaks
        // initialize list
        for (int i=0; i< N; i++)
            peaks[i] = 0.0;
        
        double maxList[N];  // list for maxima
        double minList[N];  // list for minima
        // initialize lists
        for (int i=0; i< N; i++){
            maxList[i] = -1;
            minList[i] = -1;
        }
        
        int lastMaxIndex = 0;
        int lastMinIndex = 0;
        
        bool direction = 0; // 0 = down, 1 = up
    	{
    		// count forward when there is a steady value at the beginning
    		int cf = 0;
    		while ((input[cf] == input[cf+1]) && cf < N )
    			cf++;
    			
            // is start a max or min
			if (input[cf] > input[cf+1])	// max
            {
                maxList[cf] = input[cf];
                lastMaxIndex = cf;
                direction = 0;
            }    
            else if (input[cf] < input[cf+1])	// min
            { 
                minList[cf] = input[cf];
                lastMinIndex = cf;
                direction = 1;
            }
            int count = 1;
            
            
            // loop through, to find all minima and maxima and calculate slope
			for (int i=cf+1;i<N;i++)
			{
                if ((input[i] > input[i-1]) && (direction == 0))			// minimum detected
                {
                        // look backwards for same value, when steady minimum for several values
                        int cb = i-1;
                        while (cb > 1 && (input[cb-1] == input[cb]))
                            cb --;
                        
                        // save to list
                        minList[cb] = input[cb];	// save first minimum
                        minList[i-1] = input[i-1];	// save second minimum, may be the same as first
                        count ++;
                        direction = 1;
                        
                        // calculate area of peak
                        if (count < 3)      // first value was a maximum
                            peaks[lastMaxIndex] = (maxList[lastMaxIndex] - minList[cb])/(cb - lastMaxIndex);	// hmax-hmin / w
                        else                // all other values
                        {
                        	if (minList[lastMinIndex] > minList[cb])	// hminl > hminr
                        		peaks[lastMaxIndex] = (maxList[lastMaxIndex] - minList[lastMinIndex]) / (lastMaxIndex - lastMinIndex);	// hmax-hminl / w
                        	else										// hminl <= hminr
                        		peaks[lastMaxIndex] = (maxList[lastMaxIndex] - minList[cb]) / (cb - lastMaxIndex); // hmax-hminr / w 
                        }
                        lastMinIndex = i-1;
                }    
                else if ((input[i] < input[i-1]) && (direction == 1))		// maximum detected
                {
                        // save to list
                        maxList[i-1] = input[i-1];
                        count ++;
                        direction = 0;
                        lastMaxIndex = i-1;
                }
			}
            
            // sort peaks descending
            double temp;
        	for (int i=0; i<m_nbPeaks; i++)
        		for (int j=(N-1); j>i; j--)
        			if(peaks[j] > peaks[j-1])
        			{
        				temp = peaks[j];
        				peaks[j] = peaks[j-1];
        				peaks[j-1] = temp;
        			}
    	}

        double* output = out->writeToken();

        // output wanted number of peaks
        for (int i=0; i< m_nbPeaks; i++)
            output[i] = peaks[i];

        in->consumeToken();
    }
    return true;
}

}
