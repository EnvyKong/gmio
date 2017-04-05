<img src="doc/gmio.png" height="91" alt="gmio_logo"/>


[![Build Status](https://travis-ci.org/fougue/gmio.svg?branch=develop)](https://travis-ci.org/fougue/gmio)
[![Build status](https://ci.appveyor.com/api/projects/status/0q4f69lqo6sw9108?svg=true)](https://ci.appveyor.com/project/HuguesDelorme/gmio) 
<a href="https://scan.coverity.com/projects/5721">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/5721/badge.svg"/>
</a>
[![Codecov](https://codecov.io/github/fougue/gmio/coverage.svg?branch=develop)](https://codecov.io/gh/fougue/gmio/branch/develop)
[![License](https://img.shields.io/badge/license-BSD%202--clause-blue.svg)](https://github.com/fougue/gmio/blob/develop/LICENSE.txt)
[![Version](https://img.shields.io/badge/version-v0.4.0-blue.svg?style=flat)](https://github.com/fougue/gmio/releases)  


C library for geometry input/output
===========================================

gmio is a reusable C library providing complete I/O support for various CAD file
formats(eg. [STL](https://en.wikipedia.org/wiki/STL_%28file_format%29))

gmio aims to be [fast](https://github.com/fougue/gmio/wiki/v0.4-Benchmarks),
portable(C99 conformance) and feature-rich.

Main highlights:

  * [x] "Abstract" streams that does not tie the user to C stream(`FILE*`)
  * [x] Buffering of input/ouput for efficient device usage
  * [x] Operations can be easily aborted
  * [x] Progress report about the I/O operation


 Supported formats        |  Read     |  Write
--------------------------|-----------|---------
AMF uncompressed          |  &#10007; | &#10004;
AMF zip                   |  &#10007; | &#10004;
AMF zip64                 |  &#10007; | &#10004;
STL ascii                 |  &#10004; | &#10004;
STL binary(little-endian) |  &#10004; | &#10004;
STL binary(big-endian)    |  &#10004; | &#10004;


Building gmio
=============

gmio can be built with CMake, by default a static library is generated.

Read the [Build instructions](https://github.com/fougue/gmio/wiki/v0.4-Build-instructions)


Documentation
=============

The HTML reference manual generated by Doxygen is [here](http://www.fougue.pro/docs/gmio/0.4)

There is also a [wiki](https://github.com/fougue/gmio/wiki)


Getting help
============

For now use the gmio issue tracking system provided by GitHub at
https://github.com/fougue/gmio/issues

Just create an issue with label `help wanted` or `question`.


How to report a bug
===================

The gmio bug tracking system is open to the public at
https://github.com/fougue/gmio/issues.

If you think you have found a bug in gmio, we would like to hear
about it so that we can fix it.

Always include the following information in your bug report:
  * the name and version number of your compiler
  * the name and version number of your operating system
  * the version of gmio you are using
  * what configure options it was compiled with.

If the problem you are reporting is only visible at run-time, try to
create a small test program that shows the problem when run.


License
=======

gmio is made available under a "2-clause" BSD [license](https://github.com/fougue/gmio/blob/develop/LICENSE.txt).


Credits
=======

"gmio" logo rendered with Prism font(thanks to Erik Yin !)
