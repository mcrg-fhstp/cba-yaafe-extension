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

#include "DilationFilter.h"
#include "MathUtils.h"

using namespace std;

namespace YAAFE
{

DilationFilter::DilationFilter()
    :m_dfsize(0)
{
}

DilationFilter::~DilationFilter()
{
}

ParameterDescriptorList DilationFilter::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;
    p.m_identifier = "DFSize";
    p.m_description = "Size for Dilation Filter";
    p.m_defaultValue = "1";
    pList.push_back(p);
    return pList;
}

bool DilationFilter::init(const ParameterMap& params, const Ports<StreamInfo>& inp)
{
    string dfsizeStr = getStringParam("DFSize",params);
    m_dfsize = getIntParam("DFSize",params);
    if (m_dfsize<=0)
	{
		cerr << "ERROR: invalid parameter DFSize " << dfsizeStr << endl;
        return false;
	}
    
    assert(inp.size()==1);
	const StreamInfo& in = inp[0].data;
    
	outStreamInfo().add(StreamInfo(in,in.size));
    return true;
}

bool DilationFilter::process(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
    assert(inp.size()==1);
	InputBuffer* in = inp[0].data;
	if (in->empty()) return false;
	assert(outp.size()==1);
	OutputBuffer* out = outp[0].data;
    
    double buf;
    
    while (in->hasTokens(m_dfsize+1))      // loop through all data horizontally
    {
        
            // 1 -> 0
            if ((*(in->token(0)) == 1) && (*(in->token(1)) == 0)){         
                int m = in->availableTokens();
                buf = *(in->token(0));
                for (int j=0;(j <= m_dfsize) && (j < m);j++){
                    double* outData = out->writeToken();
                    outData[0] = buf;     // set next n values to 1
                    in->consumeToken();
                }
                buf = 0;
            }
            else{
                bool m_set = false;
                for (int i=0;i<m_dfsize;i++){   // next n values
                    // 0 -> 1
                    if ((*(in->token(i)) == 0) && (*(in->token(i+1)) == 1)){    
                        buf = *(in->token(i+1));
                        double* outData = out->writeToken();
                        outData[0] = buf;     // set actual value to 1
                        in->consumeToken();
                        m_set = true;
                    }
                }
                // same value
                if (!m_set){      
                    double* outData = out->writeToken();
                    buf = *(in->token(0));
                    outData[0] = buf;
                    in->consumeToken();
                }
            }
    }
    
    int m = in->availableTokens();
    for(int j=0; j<m; j++){       // write last values
        double* outData = out->writeToken();
        outData[0] = buf;
        in->consumeToken();
    }
    
    return true;
}

}
