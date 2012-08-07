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

#include "ErosionFilter.h"
#include "MathUtils.h"

using namespace std;

namespace YAAFE
{

ErosionFilter::ErosionFilter()
    :m_efsize(0)
{
}

ErosionFilter::~ErosionFilter()
{
}

ParameterDescriptorList ErosionFilter::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;
    p.m_identifier = "EFSize";
    p.m_description = "Size for Erosion Filter";
    p.m_defaultValue = "1";
    pList.push_back(p);
    return pList;
}

bool ErosionFilter::init(const ParameterMap& params, const Ports<StreamInfo>& inp)
{
    string efsizeStr = getStringParam("EFSize",params);
    m_efsize = getIntParam("EFSize",params);
    if (m_efsize<=0)
	{
		cerr << "ERROR: invalid parameter EFSize " << efsizeStr << endl;
        return false;
	}
    
    assert(inp.size()==1);
	const StreamInfo& in = inp[0].data;
    
	outStreamInfo().add(StreamInfo(in,in.size));
    return true;
}

bool ErosionFilter::process(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
    assert(inp.size()==1);
	InputBuffer* in = inp[0].data;
	if (in->empty()) return false;
	assert(outp.size()==1);
	OutputBuffer* out = outp[0].data;
    
    double buf;
    
    while (in->hasTokens(m_efsize+1))      // loop through all data horizontally
    {
        
        // 0 -> 1
        if ((*(in->token(0)) == 0) && (*(in->token(1)) == 1)){         
            int m = in->availableTokens();
            buf = *(in->token(0));
            for (int j=0;(j <= m_efsize) && (j < m);j++){
                double* outData = out->writeToken();
                outData[0] = buf;     // set next n values to 0
                in->consumeToken();
            }
            buf = 1;
        }
        else{
            bool m_set = false;
            for (int i=0;i<m_efsize;i++){   // next n values
                // 1 -> 0
                if ((*(in->token(i)) == 1) && (*(in->token(i+1)) == 0)){    
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
