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

#include "WindowNormalize.h"
#include "MathUtils.h"
#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

namespace YAAFE
{

WindowNormalize::WindowNormalize() : m_window()
{
}

WindowNormalize::~WindowNormalize()
{
}

ParameterDescriptorList WindowNormalize::getParameterDescriptorList() const
{
    ParameterDescriptorList pList;
    ParameterDescriptor p;

    p.m_identifier = "NormWindow";                      // window as parameter
    p.m_description = "Normalizing window to apply. Hanning|Hamming";
    p.m_defaultValue = "Hanning";                       // default window
    pList.push_back(p);

    return pList;
}

bool WindowNormalize::init(const ParameterMap& params, const Ports<StreamInfo>& inp)
{
	assert(inp.size()==1);
	const StreamInfo& in = inp[0].data;

	if (in.size<=1) {
		cerr << "WARNING: trying to normalize vector of size " << in.size << endl;
	}

	string w = getStringParam("NormWindow", params);    // define window

    if (w == "Hanning")
            m_window = ehanningPeriodic(in.size);       // window of same size as inputdata
    else if (w == "Hamming")
        m_window = ehammingPeriodic(in.size);
    else
    {
        cerr << "WindowNormalize: invalid Window parameter value " << w << " ignore it !" <<endl;
    }

	outStreamInfo().add(in);
	return true;
}

bool WindowNormalize::process(Ports<InputBuffer*>& inp, Ports<OutputBuffer*>& outp)
{
	assert(inp.size()==1);
	InputBuffer* in = inp[0].data;
	if (in->empty())
		return false;
	assert(outp.size()==1);
	OutputBuffer* out = outp[0].data;

	const int size = in->info().size;
	
    while (!in->empty()) {                              // as long as there is inputdata
        Map<ArrayXd> inData(in->readToken(),size);
        Map<ArrayXd> outData(out->writeToken(),size);
        outData = inData / m_window.sum();              // divide inputdata by sum of windowvalues
        in->consumeToken();
    }
    return true;
}

}
