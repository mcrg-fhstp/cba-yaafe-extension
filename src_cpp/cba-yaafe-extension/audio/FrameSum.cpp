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

#include "FrameSum.h"
#include "MathUtils.h"

#include <iostream>

using namespace std;

namespace YAAFE
{

FrameSum::FrameSum()
{
}

FrameSum::~FrameSum()
{
}

ParameterDescriptorList FrameSum::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;

    p.m_identifier = "NbSumFrames";         // parameter for number of frames to sum up
    p.m_description = "Number of frames to sum up";
    p.m_defaultValue = "100";               // default value
    pList.push_back(p);

    p.m_identifier = "StepNbSumFrames";     // parameter of step size
    p.m_description = "Number of frames to skip between two sums";
    p.m_defaultValue = "50";                // default value
    pList.push_back(p);

    return pList;
}

bool FrameSum::init(const ParameterMap& params, const Ports<StreamInfo>& inp)
{
	assert(inp.size()==1);
	const StreamInfo& in = inp[0].data;

	m_nbFrames = getIntParam("NbSumFrames",params);     // set parameter
    if (m_nbFrames<=0) {
    	cerr << "ERROR: Invalid NbSumFrames parameter !" << endl;
    	return false;
    }
    m_stepNbFrames = getIntParam("StepNbSumFrames",params);     // set parameter
    if (m_stepNbFrames<=0) {
    	cerr << "ERROR: Invalid stepNbSumFrames parameter !" << endl;
    	return false;
    }

    StreamInfo out;
    out.size = in.size;
    out.sampleRate = in.sampleRate;
    out.frameLength = in.frameLength + (m_nbFrames - 1) * in.sampleStep;
    out.sampleStep = (m_stepNbFrames * in.sampleStep);
    outStreamInfo().add(out);

    return true;
}

bool FrameSum::process(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
	assert(inp.size()==1);
	InputBuffer* in = inp[0].data;
	assert(outp.size()==1);
	OutputBuffer* out = outp[0].data;

	if ((out->tokenno()==0) && (in->tokenno()!=-m_nbFrames/2))       // prepends 0s at beginning
		in->prependZeros(m_nbFrames/2);
	if (!in->hasTokens(m_nbFrames)) return false;

	const int N = in->info().size;
	assert(out->info().size==2*in->info().size);
    while (in->hasTokens(m_nbFrames))
    {
        double* outPtr = out->writeToken();
        for (int j = 0; j < N; j++)         // loop horizontally
        {
        	double m = 0;
        	int count = 0;
        	for (int i=0;i<m_nbFrames;i++)      // loop vertically
        	{
        		double v = *(in->token(i) + j);     // get value
        		if (!isnan(v)) {
        			m += v;                         // sum up
        			count++;
        		}
        	}
            *outPtr++ = m/m_nbFrames;           // divide
        }
        in->consumeTokens(m_stepNbFrames);
    }
    return true;
}

void FrameSum::flush(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
	InputBuffer* in = inp[0].data;
	in->appendZeros((m_nbFrames-1)/2);          // append 0s at end
	 }

}
