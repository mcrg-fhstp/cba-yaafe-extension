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

#include "SimpleNoiseGate.h"
//#include "yaafe-core/NumericUtils.h"
#include <Eigen/Dense>

using namespace std;
//using namespace blitz;
using namespace Eigen;


namespace YAAFE
{

    SimpleNoiseGate::SimpleNoiseGate():m_threshold(0)
{
}

SimpleNoiseGate::~SimpleNoiseGate()
{
}

ParameterDescriptorList SimpleNoiseGate::getParameterDescriptorList() const
{        
    ParameterDescriptorList pList;
    ParameterDescriptor p;
    p.m_identifier = "SNGThreshold";
    p.m_description = "Threshold for simple noise gate";
    p.m_defaultValue = "0.001";
    pList.push_back(p);
    return pList;
}
    
StreamInfo SimpleNoiseGate::init(const ParameterMap& params, const StreamInfo& in)
{
    m_threshold = getDoubleParam("SNGThreshold",params);
    return StreamInfo(in, in.size);
}


void SimpleNoiseGate::processToken(double* inData, const int inSize, double* outData, const int outSize)
{
	for (int i=0;i<outSize;++i)
		outData[i] = (abs(inData[i])>m_threshold) ? inData[i] : 0;       // >threshold -> inData, <threshold -> 0
}

}
