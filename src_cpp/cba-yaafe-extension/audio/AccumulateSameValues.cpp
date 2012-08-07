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

#include "AccumulateSameValues.h"
#include <Eigen/Dense>

using namespace Eigen;

namespace YAAFE
{

AccumulateSameValues::AccumulateSameValues()
{
}

AccumulateSameValues::~AccumulateSameValues()
{
}

bool AccumulateSameValues::init(const ParameterMap& params, const Ports<StreamInfo>& inp)
{
	assert(inp.size()==1);
	const StreamInfo& in = inp[0].data;

	outStreamInfo().add(StreamInfo(in,2));
    return true;
}

bool AccumulateSameValues::process(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
	assert(inp.size()==1);
	InputBuffer* in = inp[0].data;
	if (in->empty()) return false;
	assert(outp.size()==1);
	OutputBuffer* out = outp[0].data;

    double sum = 1;
    double* inData;
    
	while (!in->empty())
	{
		inData = in->readToken();
        
        if (inData[0] == inData[1])
            sum++;                                  // accumulate same values
        else{
            double* outData = out->writeToken();
            outData[0] = sum;                       // write values
            outData[1] = inData[0];
            sum = 1;
        }
        in->consumeToken();
	}
    
    if (sum > 1){                                   // write last value
        double* outData = out->writeToken();
        outData[0] = sum-1;
        outData[1] = inData[0];
    }
    
    return true;
}

}
