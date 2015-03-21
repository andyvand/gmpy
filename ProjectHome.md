GMPY and GMPY2 are C-coded Python extension modules that support fast multiple-precision arithmetic.

GMPY only supports the GMP library and provides fast multiple-precision integer and rational arithmetic. The limited mpf type from GMP is also supported. GMPY is no longer being maintained.

GMPY2 supports the GMP library for integer and rational arithmetic but GMPY2 adds support for multiple-precision real and complex arithmetic as provided by the MPFR and MPC libraries. GMPY2 is being actively developed.

# Downloads have been moved to [PyPI](https://pypi.python.org/pypi/gmpy2) ! #

## News ##

**January 12, 2015**

GMPY2 2.0.5 has been released. The following bugs were fixed:

  1. Fix crash for repr(mpc) for precision >325 bits.
  1. Fix bug in qdiv() when denominator is a Python long larger than a C long.

Version 2.0.5 is highly recommended upgrade since the bugs can crash or corrupt the Python interpreter. It can be downloaded from https://pypi.python.org/pypi/gmpy2.

**October 8, 2014**

GMPY2 2.0.4 has been released. The following bugs were fixed:

  1. bit\_scan0() no longer returns -1 after the last 0-bit is reached.
  1. A performance regression with mpmath and Python 3.x has been fixed.
  1. divm() no longer mutates the arguments under certain circumstances.

Version 2.0.4 is a highly recommended upgrade. It can be downloaded from
https://pypi.python.org/pypi/gmpy2.

**August 4, 2014**

All new file downloads are only available at https://pypi.python.org/pypi/gmpy2.

GMPY2 2.0.4b1 has been released. The following changes were made:

  1. bit\_scan0() no longer returns -1 after the last 0-bit is reached.
  1. A performance regression with mpmath and Python 3.x has been fixed.
  1. Updated versions of MPIR, MPFR, and MPC (along with updated compiler options) are used in an to attempt to resolve a rare crash on some systems.

**November 29, 2013**

GMPY2 2.0.3 has been released. The new version corrects bugs in lucas2() and atanh() that caused incorrect results to be returned.

GMPY2 2.0.3 is a highly recommended upgrade.

**August 25, 2013**

GMPY2 2.0.2 has been released. The new version corrects an MPIR bug in the binary installers for Windows.

**July 27, 2013**

GMPY2 2.0.1 has been released. A few minor bugs have been fixed and the logic used in setup.py to find the required libraries has been updated.

A source-only update to GMPY 1.17 has been released. The only change is adding support for third-party extensions to use GMPY under Python 3 (support was working in Python 2 ).

**March 24, 2013**

GMPY2 2.0.0 has been released. There are a few minor changes and bug fixes. The most significant change is an updated setup.py that tries to simplify the build process.

Due to changes required to integrate support for MPFR and MPC, GMPY2 is not 100% compatible with GMPY. The development versions of mpmath and Sympy both support GMPY2.

Documentation is available at [Read The Docs](http://readthedocs.org/docs/gmpy2/en/latest/).


