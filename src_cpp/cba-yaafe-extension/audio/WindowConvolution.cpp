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

#include "WindowConvolution.h"
#include "MathUtils.h"

#include <iostream>
#include <string.h>

using namespace std;
using namespace Eigen;

namespace YAAFE
{

WindowConvolution::WindowConvolution()
{
}

WindowConvolution::~WindowConvolution()
{
}

ParameterDescriptorList WindowConvolution::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;
    
    p.m_identifier = "WCLength";
    p.m_description
    = "Length of window to perform convolution with. Signal is padded with zeros or truncated to reach this size. If 0 then use original signal length.";
    p.m_defaultValue = "0";
    pList.push_back(p);
    
    p.m_identifier = "WCWindow";
    p.m_description
    = "Form of window to apply with convolution. Hanning|Hamming|None";
    p.m_defaultValue = "Hanning";
    pList.push_back(p);
    
    return pList;
}
    
bool WindowConvolution::init(const ParameterMap& params, const Ports<StreamInfo>& inp)
{
    assert(inp.size()==1);
	const StreamInfo& in = inp[0].data;
    
    m_len = getIntParam("WCLength", params);
    if (m_len == 0)
        m_len = in.size;
    string w = getStringParam("WCWindow", params);
    if (w != "None")
    {
        if (w == "Hanning")
            m_window = ehanningPeriodic(m_len);
        else if (w == "Hamming")
            m_window = ehammingPeriodic(m_len);
        else
        {
            cerr << "WC: invalid Window parameter value " << w << " ignore it !" << endl;
        }
        m_window /= m_window.sum();     // normalize window
    }

	StreamInfo out;
    out.size = in.size;
    out.sampleRate = in.sampleRate;
    out.frameLength = in.frameLength ;
    out.sampleStep = in.sampleStep;
    outStreamInfo().add(out);
    
    return true;
}

bool WindowConvolution::process(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
    assert(inp.size()==1);
    InputBuffer* in = inp[0].data;
    assert(outp.size()==1);
    OutputBuffer* out = outp[0].data;
    
    if ((out->tokenno()==0) && (in->tokenno()!=-m_len/2))
        in->prependZeros(m_len/2);
    if (!in->hasTokens(m_len)) return false;
    
    const int N = in->info().size;
    assert(out->info().size==2*in->info().size);
    while (in->hasTokens(m_len))
    {
        double* outPtr = out->writeToken();
        for (int j = 0; j < N; j++)
        {
            int nb = 0;
            double m = 0;
            for (int i=0;i<m_len;i++)
            {
                double v = *(in->token(i) + j);
                if (!isnan(v)) {
                    ++nb;
                    m += v * m_window[i];
                }
            }
            //m /= nb;
            
            *outPtr++ = m;
        }
        in->consumeToken();
    }
    return true;
}

void WindowConvolution::flush(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
    InputBuffer* in = inp[0].data;
    in->appendZeros((m_len-1)/2);
    process(inp,outp);
}

}
