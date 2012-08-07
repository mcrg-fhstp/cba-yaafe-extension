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

#include "SubRunningAverage.h"

using namespace std;

namespace YAAFE
{

SubRunningAverage::SubRunningAverage()
{
}

SubRunningAverage::~SubRunningAverage()
{
}

ParameterDescriptorList SubRunningAverage::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;
    
    p.m_identifier = "NbRunAvgFrames";              // parameter number of frames to integrate
    p.m_description = "Number of frames to integrate for running average";
    p.m_defaultValue = "21";                        // default value for number of frames
    pList.push_back(p);
    
    return pList;
}
    
StreamInfo SubRunningAverage::init(const ParameterMap& params, const StreamInfo& in)
{
    m_nbFrames = getIntParam("NbRunAvgFrames",params);      // get parameter
    if (m_nbFrames<=0) {
    	cerr << "ERROR: Invalid NbRunAvgFrames parameter !" << endl;
    }

	return StreamInfo(in,in.size);
}

void SubRunningAverage::processToken(double* inData, const int N, double* outData, const int outSize)
{
	double sum = 0;
    
	for (int i=0;i<N;i++) {             // loop through all data
        sum = 0;
        for (int j=-m_nbFrames/2; j<=m_nbFrames/2; j++) {   // calculate sum over n values
            if (i+j < 0)    sum += inData[0];                   // limit values <0 
            else if (i+j >= N) sum += inData[N-1];              // limit values >=N
            else sum += inData[i+j];                            // sum up
            
        }
		
		outData[i] = inData[i] - (sum/m_nbFrames);      // out = in - runningAverage
	}
}

}
