CBA-YAAFE-extension
===================

This is an extension for [YAAFE](http://yaafe.sourceforge.net) (Yet Another Audio Feature Extractor), developed for music detection with the **Continuous Frequency Activation feature** (CFA) for the **Cultural Broadcasting Archive** ([CBA](http://cba.fro.at)).

## Extensions

This extension provides some new features as well as the possibility to get the output directly to the commandline for implementing it e.g. in PHP,...

<code>
$cmd = 'yaafe_cba.py -r 11025 --resample -f "cfa: ContinuousFrequencyActivation" wavefile.wav';
</code>
<code>
exec( $cmd, $output, $return_value);
</code>

## Installation

Before installing this extension, you have to install [YAAFE](http://yaafe.sourceforge.net)!

* Download the package and unzip
* <code>mkdir build</code>
* <code>cd build</code>
* <code>ccmake ..</code> type 'c' for configure and 'g' for generate
* <code>make</code>
* <code>sudo make install</code>
* <code>sudo chmod 755 /user/local/bin/yaafe_cba.py</code>

## Run

## License

The CBA-extension for YAAFE is released under GNU-LGPL v3.

## Contact

For questions, comments and complaints please use Issues on GitHub.