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

#include "SimpleThresholdClassification.h"
#include "MathUtils.h"

using namespace std;

namespace YAAFE
{

SimpleThresholdClassification::SimpleThresholdClassification()
    :m_threshold(0)
{
}

SimpleThresholdClassification::~SimpleThresholdClassification()
{
}

ParameterDescriptorList SimpleThresholdClassification::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;
    p.m_identifier = "STCThreshold";
    p.m_description = "Threshold for Simple Threshold Classification";
    p.m_defaultValue = "1.0";
    pList.push_back(p);
    return pList;
}

StreamInfo SimpleThresholdClassification::init(const ParameterMap& params, const StreamInfo& in)
{
    m_threshold = getDoubleParam("STCThreshold",params);
	return StreamInfo(in, in.size);
}

void SimpleThresholdClassification::processToken(double* inPtr, const int inSize, double* outPtr, const int outSize)
{
    for (int i=0;i<outSize;++i)                 // loop through all data
		outPtr[i] = (inPtr[i]>m_threshold) ? 1 : 0;       // >threshold -> 1, <threshold -> 0
}

}
