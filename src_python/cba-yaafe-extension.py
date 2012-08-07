# -*- coding: iso-8859-1 -*-
#
# Yaafe extension for the Cultural Broadcasting Archive.
#
# Copyright (c) 2012 University of Applied Sciences – Institute for Creative Media Technologies
#
# Author : Ewald Wieser B.Sc. (fhstp.ac.at)
#
# Co-Authors :
#      Dipl.-Ing. (FH) Matthias Husinsky (fhstp.ac.at)
#      FH-Prof. Dipl.-Ing. Markus Seidl (fhstp.ac.at)
#
# Contributes to : 
#      Dr. Klaus Seyerlehner (jku.at)
#
# This file is an extension for Standard-Yaafe, developed for Music
# detection with the Continuous Frequency Activation Feature (CFA) for
# the Cultural Broadcasting Archive (CBA).
#
# Yaafe is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Yaafe is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

'''
Yaafe extension module for the Cultural Broadcasting Archive.
'''

from yaafelib.audiofeature import AudioFeature, check_dataflow_params, dataflow_safe_append
from yaafelib.dataflow import DataFlow
import yaafefeatures as yf
import math
import decimal

       
        

class ContinuousFrequencyActivation(AudioFeature):
    '''
        Compute Continuous Frequency Activation ([SP2007]_).
        
        .. [SP2007] Seyerlehner, Pohle, et al., *Automatic Music Detection in Television Productions.*, Proc. of the 10th Int. Conference on Digital Audio Effects (DAFx-07), Bordeaux, France, September 10-15, 2007, pp. 221-228.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension','yaafe-components']
    PARAMS = [(yf.MagnitudeSpectrum,{}),
              ('WindowNormalize',{}),
              ('Sqr',{}),
              ('DCOffsetFilter',{}),
              ('DBConversion',{}),
              ('SubRunningAverage',{}),
              ('Binarization',{}),
              ('FrameSum',{}),
              ('PeakDetection',{}),
              ('Sum',{})]
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls, params, samplerate):
        # Power Spectrum
        df = yf.MagnitudeSpectrum.get_dataflow(yf.MagnitudeSpectrum.filter_params(params), samplerate)
        dataflow_safe_append(df,'WindowNormalize',{'NormWindow':'Hanning'})
        dataflow_safe_append(df,'Sqr',{})
        #dataflow_safe_append(df,'DCOffsetFilter',{})   #only good for special files
        dataflow_safe_append(df,'DBConversion',{})
        # Emphasize Local Peaks
        dataflow_safe_append(df,'SubRunningAverage',params)
        # Binarization
        dataflow_safe_append(df,'Binarization',params)
        # Calculate Frequency Activation
        dataflow_safe_append(df,'FrameSum',params)
        # Detect Strong Peaks
        dataflow_safe_append(df,'PeakDetection',params)
        # Quantify CFA
        dataflow_safe_append(df,'Sum',params)
        return df

class SimpleThresholdClassification(AudioFeature):
    '''
        Feature transform that compares each value to the threshold and outputs 1 if greater than or equal and 0 if lower than threshold.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension']
    PARAMS = [('SimpleThresholdClassification',{})]
    TRANSFORM = True
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls, params, samplerate):
        df = DataFlow()
        df.createNode('SimpleThresholdClassification',params)
        return df

class SimpleNoiseGate(AudioFeature):
    '''
        Simple Noise Gate.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension']
    PARAMS = [('SimpleNoiseGate',{}),]
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls,params,samplerate):
        df = DataFlow()
        df.createNode('SimpleNoiseGate',params)
        return df

class MedianFilter(AudioFeature):
    '''
        Median Filter.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension']
    PARAMS = [('MedianFilter',{}),]
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls,params,samplerate):
        df = DataFlow()
        df.createNode('MedianFilter',params)
        return df

class ErosionFilter(AudioFeature):
    '''
        Erosion Filter.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension']
    PARAMS = [('ErosionFilter',{}),]
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls,params,samplerate):
        df = DataFlow()
        df.createNode('ErosionFilter',params)
        return df

class DilationFilter(AudioFeature):
    '''
        Dilation Filter.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension']
    PARAMS = [('DilationFilter',{}),]
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls,params,samplerate):
        df = DataFlow()
        df.createNode('DilationFilter',params)
        return df

class WindowConvolution(AudioFeature):
    '''
        Median Filter.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension']
    PARAMS = [('WindowConvolution',{}),]
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls,params,samplerate):
        df = DataFlow()
        df.createNode('WindowConvolution',params)
        return df

class AccumulateSameValues(AudioFeature):
    '''
        Accumulate same values.
        '''
    COMPONENT_LIBS = ['cba-yaafe-extension']
    PARAMS = [('AccumulateSameValues',{}),]
    @classmethod
    @check_dataflow_params
    def get_dataflow(cls,params,samplerate):
        df = DataFlow()
        df.createNode('AccumulateSameValues',params)
        return df