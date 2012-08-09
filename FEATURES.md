<<<<<<< HEAD
CBA - YAAFE feature extension
*****************************

Yaafe feature extension for music detection in the Cultural
Broadcasting Archive.
=======
**Yaafe feature extension** for music detection in the **Cultural
Broadcasting Archive** ([CBA](http://cba.fro.at)).
>>>>>>> doc changes


Available features
******************


AccumulateSameValues
====================

_class class cba_yaafe_extension.AccumulateSameValues_

   Counts the number of several consecutive same values and outputs as
   2-dimensional array [number, value].

   **Declaration example**:

      AccumulateSameValues


ContinuousFrequencyActivation
=============================

class class cba_yaafe_extension.ContinuousFrequencyActivation

   Compute Continuous Frequency Activation ([SP2007]).   Continuously
   (over several seconds) activated frequency bands are detected and
   their spectral peak values summed up to the CFA-value.

   [SP2007] Seyerlehner, Pohle, et al., *Automatic Music Detection in
            Television Productions.*, Proc. of the 10th Int.
            Conference on Digital Audio Effects (DAFx-07), Bordeaux,
            France, September 10-15, 2007, pp. 221-228.

   **Parameters**:

      * "BinThreshold" (default=0.1): Values less than BinThreshold
        will be set to 0, others to 1.

      * "FFTLength" (default=0): Frame's length on which perform FFT.
        Original frame is padded with zeros or truncated to reach this
        size. If 0 then use original frame length.

      * "FFTWindow" (default=Hanning): Weighting window to apply
        before fft. Hanning|Hamming|None

      * "NbPeaks" (default=5): Number of peaks to find

      * "NbRunAvgFrames" (default=21): Number of frames to integrate
        for running average

      * "NbSumFrames" (default=100): Number of frames to sum up

      * "NormWindow" (default=Hanning): Normalizing window to apply.
        Hanning|Hamming

      * "StepNbSumFrames" (default=50): Number of frames to skip
        between two sums

      * "blockSize" (default=1024): output frames size

      * "stepSize" (default=512): step between consecutive frames

   **Declaration example**:

      ContinuousFrequencyActivation BinThreshold=0.1  FFTLength=0  FFTWindow=Hanning  NbPeaks=5  NbRunAvgFrames=21  NbSumFrames=100  NormWindow=Hanning  StepNbSumFrames=50  blockSize=1024  stepSize=512

   See also:

      "MagnitudeSpectrum"


DilationFilter
==============

class class cba_yaafe_extension.DilationFilter

   Dilation Filter.

   **Parameters**:

      * "DFSize" (default=1): Size for Dilation Filter

   **Declaration example**:

      DilationFilter DFSize=1


ErosionFilter
=============

class class cba_yaafe_extension.ErosionFilter

   Erosion Filter.

   **Parameters**:

      * "EFSize" (default=1): Size for Erosion Filter

   **Declaration example**:

      ErosionFilter EFSize=1


MedianFilter
============

class class cba_yaafe_extension.MedianFilter

   Median Filter.

   **Parameters**:

      * "MFOrder" (default=5): Median filtering order, a number of
        frame ('7' or '9') or a duration ('0.5s' or '0.125s').

   **Declaration example**:

      MedianFilter MFOrder=5


SimpleNoiseGate
===============

class class cba_yaafe_extension.SimpleNoiseGate

   Signal values lower than the threshold are set to 0.

   **Parameters**:

      * "SNGThreshold" (default=0.001): Threshold for simple noise
        gate

   **Declaration example**:

      SimpleNoiseGate SNGThreshold=0.001


WindowConvolution
=================

class class cba_yaafe_extension.WindowConvolution

   Convolution Filter accomplished in the spectrum of the signal with
   a specific window form.

   **Parameters**:

      * "WCLength" (default=0): Length of window to perform
        convolution with. Signal is padded with zeros or truncated to
        reach this size. If 0 then use original signal length.

      * "WCWindow" (default=Hanning): Form of window to apply with
        convolution. Hanning|Hamming|None

   **Declaration example**:

      WindowConvolution WCLength=0  WCWindow=Hanning


Available feature transforms
****************************


SimpleThresholdClassification
=============================

class class cba_yaafe_extension.SimpleThresholdClassification

   Feature transform that compares each value to the threshold and
   outputs 1 if greater than or equal and 0 if lower than threshold.

   **Parameters**:

      * "STCThreshold" (default=1.0): Threshold for Simple Threshold
        Classification

   **Declaration example**:

      SimpleThresholdClassification STCThreshold=1.0
