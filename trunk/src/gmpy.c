/* gmpy.c
 *
 * Python interface to the GMP multiple precision library,
 * Copyright (C) 2000 - 2008 Alex Martelli
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 *************************************************************************
 *
 * originally written for GMP-2.0 (by AMK...?)
 * Rewritten by Niels M�ller, May 1996
 *
 * Version for GMP-4, Python-2.{2,3,4,5,6,...}, with support for MSVC++6,
 * addition of mpf's, &c: Alex Martelli (now aleaxit@gmail.com, Nov 2000).
 * cleanups & reorgs leading to 1.0: Alex Martelli (until Aug 2003)
 * further cleanups and bugfixes leading to 1.01, Alex Martelli (Nov 2005)
 * minor bugfixes+new decimal (&c) support to 1.02, Alex Martelli (Feb 2006)
 * various bugfixes for 64-bit platforms, 1.03, aleaxit and casevh (Jun 2008)
 * rich comparisons, 1.04, aleaxit and casevh (Jan 2009)
 *
 * Some hacks by Gustavo Niemeyer <niemeyer@conectiva.com>.
 *
 *   0.1, pre-alpha; date: 2000-11-06  first placed on sourceforge
 *
 *   0.2, still pre-alpha: 2000-11-15: bugfixes re formatting (tx, Peanu!)
 *   no tags on oct() and hex() of mpz's
 *   insert 'tagoff' in options (gmpy.mpz() vs mpz() in repr) (for Peanu!)
 *   speedups for _nonzero & _cmp (tx, Peanu!)
 *   slight speedup (7/8%?) for excess reallocs 4<->8 bytes (Peanu's help!)
 *   added copy/fcopy; bin; fib; remove
 *
 *   0.3, still pre-alpha, but...:
 *   performance tweaks via mpz-caching & fixed-constants
 *   added get/set functions for zcache, zco min/max
 *   added get-only function for versions (of gmp, and of gmpy)
 *   removed all 'traces' of mutability (to be re-done... much later!)
 *   cleaned up all of the mpz_cmp_ui(X,0) to mpz_sgn(X)
 *   cleaned up Py_BuildValue usage (N vs O, explicit-() for tuples)
 *   added numdigits, lowbits, root, next_prime, invert, popcount,
 *      hamdist, scan0, scan1
 *   renamed bin to bincoef
 *
 *   0.4:
 *   split gmpy.c/gmpy.h introducing C-API interface (Pearu's suggestion)
 *   cleanup some casts using Pearu's new macros
 *   further cache-tweaks at Pearu's suggestion (macros introduced)
 *   added sign (Pearu's request), getbit, setbit
 *   added docstrings
 *   renamed copy functions to start with _ ('internal, private')
 *   added .comb as a synonym of .bincoef
 *
 *   0.5:
 *   added jacobi, legendre, kronecker
 *   added random-number generation, seed set/save, shuffling
 *   added mpq (at last!-)
 *
 *   0.6: (lots of good ideas from Pearu once more!-):
 *   fixed silly bugs in kronecker and mpq_abs
 *   gmpy-level workaround for scan0/scan1 bugs (?) in GMP 3.1.1
 *   added qdiv; anynum->mpq substituted for all such conversions
 *       (also anynum->mpz and anynum->mpf by analogy, with care!)
 *   added options.fcoform for optional use of intermediate string in
 *       float2mpf (used for any float->mpf conversion)
 *   added set_fcoform function for options.fcoform access
 *   general cleanup of sources; added alloca for MSVC++;
 *       many sundry minor bugfixes & uniformization;
 *       a little useful refactoring (more would be good...)
 *   added caching of mpq objects
 *   power for mpq
 *   Stern-Brocot algorithm for mpf->mpq (also exposed as f2q)
 *       also used for float->mpq
 *       with stricter tracking of mpf's requested-precision
 *       added getrprec method to mpf, getrprec module-function
 *   exposed ceil, floor and trunc methods/functions for mpf's
 *   changed a couple exceptions from Value to ZeroDivision
 *   added 'qual' and 'floa' options to gmpy.rand
 *
 *   0.7: (good feedback from Keith Briggs, some advice from Tim Peters
 *      and Fred Lundh -- thanks all!):
 *   fixed bug of '"%d" where "%ld" was meant' in many places
 *      and other sundry minor warnings given by gcc
 *   fixed hash (delegating to Python) so mp[nqz](x) will
 *      produce the same value as hash(x) for any Python number x
 *   workaround for GMP 3.1.1 bug, mpz_root wrongly returning
 *      'exact' for non-exact root if dest==source, which stopped
 *      needed value-error for inexact mpq**mpq operations
 *   determined correct 'actual precision' of floats
 *   explicitly stored precision with binary-form mpf's
 *   extended explicit-bits request to all ->mpf operations
 *     (good in itself, plus, preparing for future MPFR)
 *   removed the limitation of no binary-form for <0 mpz
 *   introduced macros to parse args, for conciseness
 *
 *   0.8: (again, requests & suggestions by great Pearu!)
 *   raise test coverage 72.5% -> 90.0%
 *   introduced callbacks (not documented/tested for now;
 *       Pearu will test/support/document in PySymbolic)
 *   some errors went undiagnosed, caused crash: now fixed
 *   workaround for GMP bug(?s?) in mpz_fits_... (?)
 *   added exposure of mpf_ sqrt and pow_ui
 *
 *   0.9: (ditto)
 *   change ValueError to OverflowError for 'too-large' errors
 *   fix bug in mpq_pow (negative base, exp. with odd denominator)
 *       (fix now corrected -- _even_ denominator is the error!)
 *   fixed gcc warnings reported by K. Briggs
 *
 *   0.9b:
 *   support GMP 4 (but added no GMP4-only functionality yet)
 *
 *   0.9c:
 *   updated tests to 0.9, better coverage
 *
 *   1.0:
 *   minor cleanups, ensure support for Python 2.3
 *   fixed misdiagnosis of some argument counts in macro
 *     SELF_ONE_ARG_CONVERTED (tx to Paul Rubin!)
 *
 *   1.01:
 *   cleanups, ensure support for Python 2.4.1 on MacOSX 10.4/XCode 2.1
 *     as well as Python 2.2 and 2.3 (on MacOSX and Linux)
 *   fixed memory leak on divm (thanks to mensanator@aol.com)
 *   fixed bug on mpq('123') [[str2mpq on string w/o a slash]]
 *   added floordiv and truediv operators, and tests for them
 *   NOT tested on GMP 3 (have none left around...), ONLY on GMP 4.*
 *
 *   1.02:
 *   fix warning in comparison of mpq's
 *   added support of mpq('12.34') [[string w/o a slash, but with a dot]]
 *   fixes for 64-bit build (thanks to a patch by dmcooke)
 *   added experimental support for decimal.Decimal (and user-coded types)
 *      via wider use of special conversion methods (if present) and their
 *      sly insertion on-the-fly into the decimal.Decimal class (!)
 *   two bugfixes, thanks to Simon Burton
 *   Brought back into C89 compliance (thanks to Chip Turner), had
 *      drifted to C99 (declarations in the middle of the code).
 *   Python 2.5 support (Py_ssize_t, __index__) thanks to Chip Turner
 *   Pushed coverage to 93.3% (missing only "sanity check" level error
 *      tests [mostly for out-of-memory conditions], output to stderr
 *      conditioned by options.debug, & a couple of very obscure cases)
 *
 *   1.03:
 *   Fixed the bug that caused crashes on gmpy.mpf(float('inf')) and
 *      other such conversions, implicit and explicit
 *   Fixed a bug in get_zconst's prototype affecting 64-bit machines,
 *      thanks to Gary Bunting
 *   Fixed a bug in hashing on 64-bit systems. hash(long) now equals
 *      hash(mpz) for large values. (casevh)
 *   Changed int() to return a long value instead of OverFlowError.
 *      Complies with PEP 237. (casevh)
 *   Added support in setup.py for darwinports/macports build of GMP
 *      on MacOSX. (aleaxit)
 *
 *   1.04:
 *   Avoid GMP/mingw32 bug when converting very small floats to mpz.
 *      (casevh)
 *   Significant performance improvement for long->mpz and mpz->long.
 *      (casevh)
 *   Added "rich comparisons" to mpz, mpq and mpf types (aleaxit)
 *   Added additional tests (casevh, aleaxit)
 *   Fixed bug when converting very large mpz to str (casevh)
 *   Faster conversion from mpz->binary and binary->mpz (casevh)
 *   Added support for pickling (casevh)
 *   Added divexact (casevh)
 *   Fixed mpf comparisons by rounding mpf results when GMP returns
 *      a longer result. Added fround() (casevh)
 *   Added bit_length (Thanks Mario Pernici)
 *   Added helper functions for mpmath (casevh)
 *   Faster conversion from mpq->binary and binary->mpq (casevh)
 *   Recognize MPIR, mpir_version() (casevh)
 *
 *   1.10:
 *   Remove dependancy on pymemcompat.h (casevh)
 *   Remove callback (casevh)
 *   Added support for -DMPIR to include MPIR instead of GMP (casevh)
 *   Major code revisions to add support for Python 3.x (casevh)
 *   Changed version number to 1.10 to allow release of 1.05 (casevh)
 *   Fixed bug in binary() and qbinary() (casevh)
 */
#include "Python.h"

#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

#define GMPY_MODULE
#include "gmpy.h"

/* Define the minimum memory amount allocated. 8 has historically been
 * used, but 16 might be better for some applications or 64-bit systems.
 */
#define GMPY_ALLOC_MIN 8

#if defined(MS_WIN32) && defined(_MSC_VER)
/* so one won't need to link explicitly to gmp.lib...: */
#if defined(MPIR)
#pragma comment(lib,"mpir.lib")
#else
#pragma comment(lib,"gmp.lib")
#endif
#ifdef _MSC_VER
  #define isnan _isnan
  #define isinf !_finite
#endif
#define USE_ALLOCA 1
#define alloca _alloca
#endif

/* Define various macros to deal with differences between Python 2 and 3. */

#if PY_MAJOR_VERSION >= 3
#define Py2or3Int_Check PyLong_Check
#define Py2or3Int_AsLong PyLong_AsLong
#define Py2or3Int_FromLong PyLong_FromLong
#define Py2or3Long_SHIFT PyLong_SHIFT
#define Py2or3String_FromString PyUnicode_FromString
#define Py2or3String_Check PyUnicode_Check
#define Py2or3String_Format PyUnicode_Format
#define Py2or3String_AsString PyUnicode_AS_DATA
#define Py2or3String_FromStringAndSize PyUnicode_FromStringAndSize
#define Py2or3Bytes_ConcatAndDel PyBytes_ConcatAndDel
#define Py2or3Bytes_FromString PyBytes_FromString
#define Py2or3Bytes_AS_STRING PyBytes_AS_STRING
#define Py2or3Bytes_FromStringAndSize PyBytes_FromStringAndSize
#else
#define Py2or3Int_Check PyInt_Check
#define Py2or3Int_AsLong PyInt_AsLong
#define Py2or3Int_FromLong PyInt_FromLong
#define Py2or3Long_SHIFT SHIFT
#define Py2or3String_FromString PyString_FromString
#define Py2or3String_Check PyString_Check
#define Py2or3String_Format PyString_Format
#define Py2or3String_AsString PyString_AsString
#define Py2or3String_FromStringAndSize PyString_FromStringAndSize
#define Py2or3Bytes_ConcatAndDel PyString_ConcatAndDel
#define Py2or3Bytes_FromString PyString_FromString
#define Py2or3Bytes_AS_STRING PyString_AS_STRING
#define Py2or3Bytes_FromStringAndSize PyString_FromStringAndSize
#endif

#ifdef __MPIR_VERSION
#define MPIR_VER \
__MPIR_VERSION * 10000 + \
__MPIR_VERSION_MINOR * 100 + \
__MPIR_VERSION_PATCHLEVEL
char gmpy_license[] = "\
The GMPY source code is licensed under LGPL 2.1 or later. \
The MPIR libarary is licensed under LGPL 2.1 or later. \
Therefore, this combined module is licensed under LGPL 2.1 or later.\
";
#else
#define GNU_MP_VER \
__GNU_MP_VERSION * 10000 + \
__GNU_MP_VERSION_MINOR * 100 + \
__GNU_MP_VERSION_PATCHLEVEL
#if GNU_MP_VER > 40201
char gmpy_license[] = "\
The GMPY source code is licensed under LGPL 2.1 or later. \
This version of the GMP library is licensed under LGPL 3 or later. \
Therefore, this combined module is licensed under LGPL 3 or later.\
";
#else
char gmpy_license[] = "\
The GMPY source code is licensed under LGPL 2.1 or later. \
This version of the GMP library is licensed under LGPL 2.1 or later. \
Therefore, this combined module is licensed under LGPL 2.1 or later.\
";
#endif
#endif
#undef GNU_MP_VER

char gmpy_version[] = "1.10";

char _gmpy_cvs[] = "$Id$";

/*
 * we do have a dependence on Python's internals, specifically:
 * how Python "long int"s are internally represented.
 */
#include "longintrepr.h"

#ifdef __GNUC__
#define USE_ALLOCA 1
#endif

#define ALLOC_THRESHOLD 8192

#ifdef USE_ALLOCA
#define TEMP_ALLOC(B, S) \
    if(S < ALLOC_THRESHOLD) { \
        B = alloca(S); \
    } else { \
        if(!(B = malloc(S))) { \
            mpz_cloc(temp); \
            PyErr_NoMemory(); \
            return NULL; \
        } \
    }
#define TEMP_FREE(B, S) if(S >= ALLOC_THRESHOLD) free(B)
#else
#define TEMP_ALLOC(B, S) \
    if(!(B = malloc(S)))  { \
        mpz_cloc(temp); \
        PyErr_NoMemory(); \
        return NULL; \
    }
#define TEMP_FREE(B, S) free(B)
#endif

/*
 * global data declarations
 */
static PyObject *gmpy_module = NULL;

static struct gmpy_options {
    int debug;     /* != 0 if debug messages desired on stderr */
    unsigned long minprec;   /* min #of bits' precision on new mpf's built */
    int tagoff;    /* 0 for full tags 'gmpy.mpz()', else 5 for 'mpz()' */
    int zcache;    /* size of cache for mpz objects */
    int minzco;    /* min mpz that will come as constant (inclusive) */
    int maxzco;    /* max mpz that will come as constant (exclusive) */
    int qcache;    /* size of cache for mpq objects */
    PyObject* fcoform;  /* if non-NULL, format for float->mpf (via string) */
} options = { 0, 0, 5, 20, -2, 11, 20, 0 };

/* sanity check: do NOT let cache sizes become wildly large! */
#define MAX_CACHE 1000

/* caching macro (later expanded for mpz, mpq) */
#define DEFCACHE(mpX_t,Xcache,in_Xcache,set_Xcache,new_Xcache,mpX_clear) \
static mpX_t* Xcache; \
static int in_Xcache; \
static void set_Xcache(int new_Xcache) \
{ \
    if(in_Xcache > new_Xcache) { \
        int i; \
        if(options.debug) \
            fprintf(stderr, "Clean %d from " #Xcache "\n", \
                    in_Xcache-new_Xcache); \
        for(i=new_Xcache; i<in_Xcache; ++i) \
            mpX_clear(Xcache[i]); \
        in_Xcache = new_Xcache; \
    } \
    Xcache = realloc(Xcache, sizeof(mpX_t)*new_Xcache); \
    options.Xcache = new_Xcache; \
}

DEFCACHE(mpz_t,zcache,in_zcache,set_zcache,new_zcache,mpz_clear)
DEFCACHE(mpq_t,qcache,in_qcache,set_qcache,new_qcache,mpq_clear)

/* init-or-cache macro & function -- fetch from cache, else init, an MPZ */
#define mpz_inoc_m(newo) \
{ \
    if(in_zcache) { \
        if(options.debug) \
            fprintf(stderr, "Getting %d from zcache\n", in_zcache); \
        newo[0] = (zcache[--in_zcache])[0]; \
    } else { \
        if(options.debug) \
            fprintf(stderr, "Initing new not in zcache\n"); \
        mpz_init(newo); \
    } \
}
static void mpz_inoc(mpz_t newo) mpz_inoc_m(newo)

/* clear-or-cache macro & function -- stash into cache, else clear, an MPZ */
#define mpz_cloc_m(oldo) \
{ \
    if(in_zcache<options.zcache) { \
        (zcache[in_zcache++])[0] = oldo[0]; \
        if(options.debug) \
            fprintf(stderr, "Stashed %d to zcache\n", in_zcache); \
    } else { \
        if(options.debug) \
            fprintf(stderr, "Not placing in full zcache(%d/%d)\n", \
                    in_zcache, options.zcache); \
        mpz_clear(oldo); \
    } \
}
static void mpz_cloc(mpz_t oldo) mpz_cloc_m(oldo)

static PympzObject *mpz_from_c_long(long i); /* forward... */

/* extra cache-of-small-constants for MPZ */
static PympzObject** zconst;
/* change the zconst range to 'minzco included upto maxzco excluded' */
static void set_zconst(int new_minzco, int new_maxzco)
{
    int i;
    if(zconst) { /* a previous zconst-cache, remove it */
        for(i=options.minzco; i<options.maxzco; ++i)
            Py_DECREF((PyObject*)zconst[i-options.minzco]);
        free(zconst); zconst = 0;
    }
    if(new_maxzco > new_minzco) { /* non-empty new zconst-cache, build it */
        PympzObject** new_zconst;
        options.minzco = options.maxzco = 0;
        new_zconst = malloc(sizeof(PympzObject*)*(new_maxzco-new_minzco));
        for(i=new_minzco; i<new_maxzco; ++i)
            new_zconst[i-new_minzco] = mpz_from_c_long(i);
        zconst = new_zconst;
    }
    options.minzco = new_minzco;
    options.maxzco = new_maxzco;
}
/* return incref'd mpz from zconst with value 'i', or else 0 */
static PympzObject* get_zconst(long i)
{
    if(i>=options.minzco && i<options.maxzco) { /* i in zconst range */
        PympzObject* result = zconst[i-options.minzco];
        Py_INCREF((PyObject*)result);
        return result;
    } else {
        return 0;
    }
}

/* init-or-cache macro & function -- fetch from cache, else init, an MPQ */
#define mpq_inoc_m(newo) \
{ \
    if(in_qcache) { \
        if(options.debug) \
            fprintf(stderr, "Getting %d from qcache\n", in_qcache); \
        newo[0] = (qcache[--in_qcache])[0]; \
    } else { \
        if(options.debug) \
            fprintf(stderr, "Initing new not in qcache\n"); \
        mpq_init(newo); \
    } \
}
static void mpq_inoc(mpq_t newo) mpq_inoc_m(newo)

/* clear-or-cache macro & function -- stash into cache, else clear, an MPQ */
#define mpq_cloc_m(oldo) \
{ \
    if(in_qcache<options.qcache) { \
        (qcache[in_qcache++])[0] = oldo[0]; \
        if(options.debug) \
            fprintf(stderr, "Stashed %d to qcache\n", in_qcache); \
    } else { \
        if(options.debug) \
            fprintf(stderr, "Not placing in full qcache(%d/%d)\n", \
                    in_qcache, options.qcache); \
        mpq_clear(oldo); \
    } \
}
static void mpq_cloc(mpq_t oldo) mpq_cloc_m(oldo)

/* forward declarations of type-objects and method-arrays for them */
#ifdef _MSC_VER
PyTypeObject Pympz_Type;
PyTypeObject Pympq_Type;
PyTypeObject Pympf_Type;
PyMethodDef Pympz_methods [];
PyMethodDef Pympq_methods [];
PyMethodDef Pympf_methods [];
#else
static PyTypeObject Pympz_Type;
static PyTypeObject Pympq_Type;
static PyTypeObject Pympf_Type;
static PyMethodDef Pympz_methods [];
static PyMethodDef Pympq_methods [];
static PyMethodDef Pympf_methods [];
#endif

/* utility macros for argument parsing */
#define NO_ARGS() if(!PyArg_ParseTuple(args, "")) { return NULL; }

#define ONE_ARG(nm, fm, var) \
    if(!PyArg_ParseTuple(args, fm, var)) { return NULL; }

/* Define three different versions of the SELF_NO_ARG macro. Under Python
   2.x, self is NULL when a function is called via gmpy.fname(..). But
   under Python 3.x, self is a module. Don't understand. */

#define SELF_MPZ_NO_ARG \
    if(self && Pympz_Check(self)) { \
        if(!PyArg_ParseTuple(args, "")) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&", Pympz_convert_arg, &self)) \
            return NULL; \
    }
#define SELF_MPQ_NO_ARG \
    if(self && Pympq_Check(self)) { \
        if(!PyArg_ParseTuple(args, "")) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&", Pympq_convert_arg, &self)) \
            return NULL; \
    }
#define SELF_MPF_NO_ARG \
    if(self && Pympf_Check(self)) { \
        if(!PyArg_ParseTuple(args, "")) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&", Pympf_convert_arg, &self)) \
            return NULL; \
    }


#define SELF_MPZ_ONE_ARG(fm, var) \
    if(self && Pympz_Check(self)) { \
        if(!PyArg_ParseTuple(args, fm, var)) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&" fm, Pympz_convert_arg, &self, var)) \
            return NULL; \
    }
#define SELF_MPQ_ONE_ARG(fm, var) \
    if(self && Pympq_Check(self)) { \
        if(!PyArg_ParseTuple(args, fm, var)) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&" fm, Pympq_convert_arg, &self, var)) \
            return NULL; \
    }
#define SELF_MPF_ONE_ARG(fm, var) \
    if(self && Pympf_Check(self)) { \
        if(!PyArg_ParseTuple(args, fm, var)) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&" fm, Pympf_convert_arg, &self, var)) \
            return NULL; \
    }


#define SELF_MPZ_ONE_ARG_CONVERTED(var) \
    if(self && Pympz_Check(self)) { \
        if(args && !PyArg_ParseTuple(args, "O&", Pympz_convert_arg, var)) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&O&", Pympz_convert_arg,&self, \
                Pympz_convert_arg,var)) \
            return NULL; \
    }
#define SELF_MPQ_ONE_ARG_CONVERTED(var) \
    if(self && Pympq_Check(self)) { \
        if(args && !PyArg_ParseTuple(args, "O&", Pympq_convert_arg, var)) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&O&", Pympq_convert_arg,&self, \
                Pympq_convert_arg,var)) \
            return NULL; \
    }
#define SELF_MPF_ONE_ARG_CONVERTED(var) \
    if(self && Pympf_Check(self)) { \
        if(args && !PyArg_ParseTuple(args, "O&", Pympf_convert_arg, var)) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&O&", Pympf_convert_arg,&self, \
                Pympf_convert_arg,var)) \
            return NULL; \
    }


#define SELF_MPF_ONE_ARG_CONVERTED_OPT(var) \
    if(self && Pympf_Check(self)) { \
        if(args && !PyArg_ParseTuple(args, "|O&", Pympf_convert_arg,var)) \
            return NULL; \
        Py_INCREF(self); \
    } else { \
        if(!PyArg_ParseTuple(args, "O&|O&", Pympf_convert_arg,&self, \
                Pympf_convert_arg,var)) \
            return NULL; \
    }


#define TWO_ARG_CONVERTED(converter, var1, var2) \
    if(!PyArg_ParseTuple(args, "O&O&", converter,var1, converter,var2)) \
        return NULL;

/* Number of bits that are significant in a float */
static unsigned int double_mantissa = 0;

/* generation of new, uninitialized objects; deallocations */
static PympzObject *
Pympz_new(void)
{
    PympzObject * self;

    if(!(self = PyObject_New(PympzObject, &Pympz_Type)))
        return NULL;
    /* MPOBCAL(self)=0; MPOBFLA(self)=0; */
    mpz_inoc_m(self->z);
    return self;
}
static PympqObject *
Pympq_new(void)
{
    PympqObject * self;

    if(!(self = PyObject_New(PympqObject, &Pympq_Type)))
        return NULL;
    /* MPOBCAL(self)=0; MPOBFLA(self)=0; */
    mpq_inoc_m(self->q);
    return self;
}
static PympfObject *
Pympf_new(unsigned int bits)
{
    PympfObject * self;

    if(!(self = PyObject_New(PympfObject, &Pympf_Type)))
        return NULL;
    /* MPOBCAL(self)=0; MPOBFLA(self)=0; */
    if(bits < options.minprec)
        bits = options.minprec;
    mpf_init2(self->f, bits);
    self->rebits = bits;
    return self;
}

static void
Pympz_dealloc(PympzObject *self)
{
    if(options.debug)
        fprintf(stderr, "Pympz_dealloc: %p\n", self);
    mpz_cloc_m(self->z);
    /* Py_XDECREF(MPOBCAL(self)); */
    PyObject_Del(self);
} /* Pympz_dealloc */
static void
Pympq_dealloc(PympqObject *self)
{
    if(options.debug)
        fprintf(stderr, "Pympq_dealloc: %p\n", self);
    mpq_cloc_m(self->q);
    /* Py_XDECREF(MPOBCAL(self)); */
    PyObject_Del(self);
} /* Pympq_dealloc */
static void
Pympf_dealloc(PympfObject *self)
{
    if(options.debug)
        fprintf(stderr, "Pympf_dealloc: %p\n", self);
    mpf_clear(self->f);
    /* Py_XDECREF(MPOBCAL(self)); */
    PyObject_Del(self);
} /* Pympz_dealloc */

/* Return license information. */
static char doc_license[]="\
license(): returns string giving license information\n\
";
static PyObject *
Pygmpy_get_license(PyObject *self, PyObject *args)
{
    NO_ARGS();
    return Py_BuildValue("s", gmpy_license);
}

/* return GMPY, resp. GMP, versions, or CVS Id, as strings */
static char doc_version[]="\
version(): returns string giving current GMPY version\n\
";
static PyObject *
Pygmpy_get_version(PyObject *self, PyObject *args)
{
    NO_ARGS();
    return Py_BuildValue("s", gmpy_version);
}
static char doc_cvsid[]="\
_cvsid(): returns string giving current GMPY cvs Id\n\
";
static PyObject *
Pygmpy_get_cvsid(PyObject *self, PyObject *args)
{
    NO_ARGS();
    return Py_BuildValue("s", _gmpy_cvs);
}
static char doc_gmp_version[]="\
gmp_version(): returns string giving current GMP version. Empty string\n\
returned if MPIR was used.\n\
";
static PyObject *
Pygmpy_get_gmp_version(PyObject *self, PyObject *args)
{
    NO_ARGS();
#ifndef __MPIR_VERSION
    return Py_BuildValue("s", gmp_version);
#else
    return Py_BuildValue("s", "");
#endif
}
static char doc_mpir_version[]="\
mpir_version(): returns string giving current MPIR version. Empty string\n\
returned if GMP was used.\n\
";
static PyObject *
Pygmpy_get_mpir_version(PyObject *self, PyObject *args)
{
    NO_ARGS();
#ifdef __MPIR_VERSION
    return Py_BuildValue("s", mpir_version);
#else
    return Py_BuildValue("s", "");
#endif
}
static char doc_gmp_limbsize[]="\
gmp_limbsize(): returns the number of bits per limb\n\
";
static PyObject *
Pygmpy_get_gmp_limbsize(PyObject *self, PyObject *args)
{
    NO_ARGS();
    return Py_BuildValue("i", GMP_NUMB_BITS);
}

/*
 * access cache & constants options
 */
static char doc_get_zcache[]="\
get_zcache(): returns the current cache-size (number of objects)\n\
for mpz objects.\n\
";
static PyObject *
Pygmpy_get_zcache(PyObject *self, PyObject *args)
{
    NO_ARGS();
    return Py_BuildValue("i", options.zcache);
}

static char doc_get_qcache[]="\
get_qcache(): returns the current cache-size (number of objects)\n\
for mpq objects.\n\
";
static PyObject *
Pygmpy_get_qcache(PyObject *self, PyObject *args)
{
    NO_ARGS();
    return Py_BuildValue("i", options.qcache);
}

static char doc_get_zconst[]="\
get_zconst(): returns a 2-element tuple, the min (inclusive) and\n\
max (exclusive) mpz values for which pre-registered constant\n\
Python objects are supplied by the gmpy module.\n\
";
static PyObject *
Pygmpy_get_zconst(PyObject *self, PyObject *args)
{
    NO_ARGS();
    return Py_BuildValue("(ii)", options.minzco, options.maxzco);
}

static char doc_set_zcache[]="\
set_zcache(n): sets the current cache-size (number of objects)\n\
for mpz objects to n (does not immediately flush or enlarge the\n\
cache, but rather lets it grow/shrink during later normal use).\n\
Note: cache size n must be between 0 and 1000, included.\n\
";
static PyObject *
Pygmpy_set_zcache(PyObject *self, PyObject *args)
{
    int newval;
    ONE_ARG("set_zcache", "i", &newval);
    if(newval<0 || newval>MAX_CACHE) {
        PyErr_SetString(PyExc_ValueError, "cache must between 0 and 1000");
        return 0;
    }
    set_zcache(newval);
    return Py_BuildValue("");
}

static char doc_set_qcache[]="\
set_qcache(n): sets the current cache-size (number of objects)\n\
for mpq objects to n (does not immediately flush or enlarge the\n\
cache, but rather lets it grow/shrink during later normal use).\n\
Note: cache size n must be between 0 and 1000, included.\n\
";
static PyObject *
Pygmpy_set_qcache(PyObject *self, PyObject *args)
{
    int newval;
    ONE_ARG("set_qcache", "i", &newval);
    if(newval<0 || newval>MAX_CACHE) {
        PyErr_SetString(PyExc_ValueError, "cache must between 0 and 1000");
        return 0;
    }
    set_qcache(newval);
    return Py_BuildValue("");
}

static char doc_set_zconst[]="\
set_zconst(min,max): sets the min (inclusive) and max (exclusive)\n\
mpz values for which pre-registered constant Python objects are\n\
supplied by the gmpy module (the set of objects uses is immediately\n\
re-built [as and if needed] when set_zconst changes min and/or max).\n\
Note: cache size (max-min) must be between 0 and 1000, included.\n\
";
static PyObject *
Pygmpy_set_zconst(PyObject *self, PyObject *args)
{
    int newmin, newmax;
    if(!PyArg_ParseTuple(args, "ii", &newmin, &newmax))
        return NULL;
    if(newmin>newmax || (newmax-newmin)>MAX_CACHE) {
        PyErr_SetString(PyExc_ValueError, "cache must between 0 and 1000");
        return 0;
    }
    set_zconst(newmin, newmax);
    return Py_BuildValue("");
}

/* set a module-global flag, return previously-set value */
static char doc_set_debug[]="\
set_debug(n): resets (if n==0) or sets (if n!=0) the module\n\
level 'debug' setting, giving detailed info to stderr; also\n\
returns the previous value of this module-level setting.\n\
Note: only useful to debug gmpy's own internals!\n\
";
static PyObject *
Pygmpy_set_debug(PyObject *self, PyObject *args)
{
    long old = options.debug;

    ONE_ARG("set_debug", "l", &options.debug);
    return Py_BuildValue("l", old);
}

static char doc_set_tagoff[]="\
set_tagoff(n): resets (if n==0) or sets (if n!=0) the module\n\
level 'tagoff' setting, removing the 'gmpy.' prefix of the tag\n\
strings used by repr and (optionally) digits/fdigits/qdigits;\n\
also returns the previous value of this module-level setting.\n\
";
static PyObject *
Pygmpy_set_tagoff(PyObject *self, PyObject *args)
{
    int old = options.tagoff;

    ONE_ARG("set_tagoff", "i", &options.tagoff);
    if(options.tagoff) options.tagoff=5;
    return Py_BuildValue("i", old!=0);
}

static char doc_set_minprec[]="\
set_minprec(n): sets number of bits of precision to be at\n\
least n for all mpf objects generated from now on; also\n\
returns the previous value of this module-level setting.\n\
";
static PyObject *
Pygmpy_set_minprec(PyObject *self, PyObject *args)
{
    long old = options.minprec;
    int i;

    ONE_ARG("set_minprec", "i", &i);
    if(i<0) {
        PyErr_SetString(PyExc_ValueError,
            "minimum precision must be >= 0");
        return 0;
    }
    options.minprec = i;
    return Py_BuildValue("l", old);
}

static char doc_set_fcoform[]="\
set_fcoform(s=None): resets (if s is None) or sets the module level\n\
'fcoform' setting, the format in which to build an intermediate string\n\
to be used in float->mpf conversion (direct, if no fcoform); also\n\
returns the previous value of this module-level setting.  Note that\n\
s must be a string usable for s%f formatting; or, s may be a Python\n\
int, 0<s<=30, in which case format is set to '%.<s>e'.\n\
";
static PyObject *
Pygmpy_set_fcoform(PyObject *self, PyObject *args)
{
    PyObject *old = options.fcoform;
    PyObject *new = 0;

    ONE_ARG("set_fcoform", "|O", &new);
    if(new == Py_None) { /* none == missing-argument (reset string use) */
        new = 0;
    } else if(new) {
        char buf[20];
        if(Py2or3Int_Check(new)) {
            /* int arg (1 to 30) used as # of digits for intermediate string */
            long inew = Py2or3Int_AsLong(new);
            if(inew==-1 && PyErr_Occurred()) {
                PyErr_SetString(PyExc_ValueError,
                    "number of digits n must be 0<n<=30");
                return 0;
            }
            /* check range for number-of-digits setting */
            if(inew<=0 || inew>30) {
                PyErr_SetString(PyExc_ValueError,
                    "number of digits n must be 0<n<=30");
                return 0;
            }
            /* prepare Python format-string '%.12e' or whatever */
            sprintf(buf,"%%.%lde",inew);
            new = Py2or3String_FromString(buf);
        } else { /* else arg must be string directly usable in formatting */
            if(!Py2or3String_Check(new)) {
                PyErr_SetString(PyExc_TypeError,
                    "set_fcoform argument must be int, string, or None");
                return 0;
            }
            Py_INCREF(new);
        }
    }
    /* set new 'float conversion format' and return old one if any */
    options.fcoform = new;
    if(old)
        return old;
    else
        return Py_BuildValue("");
}

/*
 * Normalize the internal representation of an mpf. GMP allocates 1
 * or more additional limbs to store the mantissa of an mpf. The
 * additional limbs may or may not be used but when used, they can
 * confuse comparisions. We will normalize all mpf such that the additional
 * limbs, if used, are set to 0.
 */

static void mpf_normalize(PympfObject *i)
{
    long size, prec, toclear, temp;
    mp_limb_t bit1, rem, carry;

    prec = mpf_get_prec(i->f);
    size = mpf_size(i->f);
    toclear = size - ((prec / GMP_NUMB_BITS) + 1);
    if(toclear>0) {
        bit1 = (i->f->_mp_d[toclear-1] & ((mp_limb_t)1 << (GMP_NUMB_BITS - 1))) ? 1 : 0;
        rem = (i->f->_mp_d[toclear-1] & (((mp_limb_t)1 << (GMP_NUMB_BITS - 1)) - 1)) ? 1 : 0;
        carry = bit1 && ((i->f->_mp_d[toclear] & 1) || rem);
    } else {
        carry = 0;
    }
    if(options.debug) {
        fprintf(stderr, "prec %ld size %ld toclear %ld carry %ld\n",
               prec, size, toclear, carry
               );
    }
    temp = toclear;
    if(temp>0) {
        i->f->_mp_d[--temp] = 0;
    }
    if(carry) {
        if(options.debug) {
            fprintf(stderr, "adding carry bit\n");
        }
        carry = mpn_add_1(i->f->_mp_d + toclear, i->f->_mp_d + toclear, size-toclear, carry);
        if(carry) {
            if(options.debug) {
                fprintf(stderr, "carry bit extended\n");
            }
            i->f->_mp_d[size-1] = 1;
            i->f->_mp_exp++;
        }
    }
}

/* CONVERSIONS AND COPIES */
static PympzObject *
mpz2mpz(PympzObject *i)
{
    PympzObject *newob;

    assert(Pympz_Check(i));
    if(!(newob = Pympz_new()))
        return NULL;
    mpz_set(newob->z, i->z);
    return newob;
}

static PympqObject *
mpq2mpq(PympqObject *q)
{
    PympqObject *newob;

    assert(Pympq_Check(q));
    if(!(newob = Pympq_new()))
        return NULL;
    mpq_set(newob->q, q->q);
    return newob;
}

static PympfObject *
mpf2mpf(PympfObject *f, unsigned int bits)
{
    PympfObject *newob;

    assert(Pympf_Check(f));
    if(!(newob = Pympf_new(bits)))
        return NULL;
    mpf_set(newob->f, f->f);
    mpf_normalize(newob);
    return newob;
}

static PympzObject *
mpz_from_c_long(long i)
{
    PympzObject *newob = get_zconst(i);

    if(!newob) {
        if(!(newob = Pympz_new()))
            return NULL;
        mpz_set_si(newob->z, i);
    }
    return newob;
}

/* Forward declaration for long2mpz */
static PympzObject * long2mpz(PyObject * obj);

static PympzObject *
int2mpz(PyObject *i)
{
#if PY_MAJOR_VERSION >= 3
    return long2mpz(i);
#else
    assert(PyInt_Check(i));
    return mpz_from_c_long(PyInt_AsLong(i));
#endif
}

/* Forward declaration for long2mpq */
static PympqObject * long2mpq(PyObject * obj);

static PympqObject *
int2mpq(PyObject *i)
{
#if PY_MAJOR_VERSION >= 3
    return long2mpq(i);
#else
    PympqObject *newob;

    assert(Py2or3Int_Check(i));

    if(!(newob = Pympq_new()))
        return NULL;
    mpq_set_si(newob->q, PyInt_AsLong(i), 1);
    return newob;
#endif
}

/* Forward declaration for long2mpq */
static PympfObject * long2mpf(PyObject * obj, unsigned int bits);

static PympfObject *
int2mpf(PyObject *i, unsigned int bits)
{
#if PY_MAJOR_VERSION >= 3
    return long2mpf(i, bits);
#else
    PympfObject *newob;
    long li;

    assert(Py2or3Int_Check(i));
    li = PyInt_AsLong(i);
    /* on a 64-bit machine, SIZEOF_LONG*8 > double_mantissa, so to simplify
       the representation, only use that many bits if we have an integer that
       won't fit in an int. */
    if(!bits) {
        if ((li > INT_MAX) || (li < (-INT_MAX-1))) {
            bits = SIZEOF_LONG*8;
        } else {
            bits = SIZEOF_INT*8;
        }
    }

    if(!(newob = Pympf_new(bits)))
        return NULL;
    mpf_set_si(newob->f, li);
    mpf_normalize(newob);
    return newob;
#endif
}

static PympzObject *
float2mpz(PyObject *f)
{
    PympzObject *newob;

    assert(PyFloat_Check(f));

    if((newob = Pympz_new()))
    {
        double d = PyFloat_AsDouble(f);
        if (isinf(d)) {
            PyErr_SetString(PyExc_ValueError,
                "gmpy does not handle infinity");
            return NULL;
        }
        if (isnan(d)) {
            PyErr_SetString(PyExc_ValueError,
                "gmpy does not handle nan");
            return NULL;
        }
        if(fabs(d) < 1.0) d = 0.0;
        mpz_set_d(newob->z, d);
    }
    return newob;
}

/* forward...: */
static PyObject *f2q_internal(PympfObject* self, PympfObject* err,
        unsigned int bits, int mayz);
static PyObject* Pympf_f2q(PyObject *self, PyObject *args);
static PympfObject* anynum2mpf(PyObject* obj, unsigned int bits);

static PympqObject *
float2mpq(PyObject *f)
{
    PympfObject *self = Pympf_new(double_mantissa);
    if(!self) return NULL;
    assert(PyFloat_Check(f));
    {
        double d = PyFloat_AsDouble(f);
        if (isinf(d)) {
            PyErr_SetString(PyExc_ValueError,
                "gmpy does not handle infinity");
            return NULL;
        }
        if (isnan(d)) {
            PyErr_SetString(PyExc_ValueError,
                "gmpy does not handle nan");
            return NULL;
        }
        mpf_set_d(self->f, d);
    }
    return (PympqObject*)f2q_internal(self, 0, double_mantissa, 0);
}

static PympfObject *str2mpf(PyObject *s, long base, unsigned int bits); /* forward */

static PympfObject *
float2mpf(PyObject *f, unsigned int bits)
{
    PympfObject *newob = 0;

    assert(PyFloat_Check(f));
    if(!bits) bits=double_mantissa;
    if(options.debug)
        fprintf(stderr, "float2mpf(%p,%d)\n", f, bits);

    if(options.fcoform) {
        /* 2-step float->mpf conversion process: first, get a
         * Python string by formatting the Python float; then,
         * use str2mpf to build the mpf from the string.
         */
        PyObject* tuple=Py_BuildValue("(O)",f);
        PyObject* s;
        if(!tuple) return 0;
        s=Py2or3String_Format(options.fcoform, tuple);
        Py_DECREF(tuple);
        if(options.debug)
            fprintf(stderr,"f2mp(%s,%f->%s)\n",
                    Py2or3String_AsString(options.fcoform),
                    PyFloat_AsDouble(f),
                    s?Py2or3String_AsString(s):"<NoString>");

        if(!s) return 0;
        newob = str2mpf(s, 10, bits);
        Py_DECREF(s);
    } else { /* direct float->mpf conversion, faster but rougher */
        if((newob = Pympf_new(bits))) {
            double d = PyFloat_AsDouble(f);
            if (isinf(d)) {
                PyErr_SetString(PyExc_ValueError,
                    "gmpy does not handle infinity");
                return NULL;
            }
            if (isnan(d)) {
                PyErr_SetString(PyExc_ValueError,
                    "gmpy does not handle nan");
                return NULL;
            }
            mpf_set_d(newob->f, d);
        }
    }
    mpf_normalize(newob);
    return newob;
}

static PympfObject *
mpz2mpf(PyObject * obj, unsigned int bits)
{
    PympfObject *newob;

    assert(Pympz_Check(obj));
    if(!bits) bits = mpz_sizeinbase(Pympz_AS_MPZ(obj),2)+2;

    if(!(newob = Pympf_new(bits)))
        return NULL;
    mpf_set_z(newob->f, Pympz_AS_MPZ(obj));
    mpf_normalize(newob);
    return newob;
}

static PympzObject *
mpf2mpz(PyObject * obj)
{
    PympzObject *newob;

    assert(Pympf_Check(obj));

    if(!(newob = Pympz_new()))
        return NULL;
    mpz_set_f(newob->z, Pympf_AS_MPF(obj));

    return newob;
}

static PympqObject *
mpz2mpq(PyObject * obj)
{
    PympqObject *newob;

    assert(Pympz_Check(obj));

    if(!(newob = Pympq_new()))
        return NULL;
    mpq_set_z(newob->q, Pympz_AS_MPZ(obj));

    return newob;
}

static PympqObject *
mpf2mpq(PyObject * obj)
{
    return (PympqObject*) Pympf_f2q(obj,0);
}

static PympfObject *
mpq2mpf(PyObject * obj, unsigned int bits)
{
    PympfObject *newob;

    assert(Pympq_Check(obj));

    if(!(newob = Pympf_new(bits)))
        return NULL;
    mpf_set_q(newob->f, Pympq_AS_MPQ(obj));
    mpf_normalize(newob);
    return newob;
}

static PympzObject *
mpq2mpz(PyObject * obj)
{

    PympzObject *newob;

    assert(Pympq_Check(obj));

    if(!(newob = Pympz_new()))
        return NULL;
    mpz_set_q(newob->z, Pympq_AS_MPQ(obj));

    return newob;
}

/*
 * to convert-from-long, we have a dependence on longs' internals:
 * we concentrate this dependence _right here_.
 */

static PympzObject *
long2mpz(PyObject * obj)
{
    PympzObject *newob;
    int len, negative;
    PyLongObject *l = (PyLongObject *) obj;

    assert(PyLong_Check(obj));

    if(!(newob = Pympz_new()))
        return NULL;
    mpz_set_si(newob->z, 0);

#if PY_MAJOR_VERSION >= 3
    if(l->ob_base.ob_size < 0) {
        len = - l->ob_base.ob_size;
        negative = 1;
    } else {
        len = l->ob_base.ob_size;
        negative = 0;
    }
#else
    if(l->ob_size < 0) {
        len = - l->ob_size;
        negative = 1;
    } else {
        len = l->ob_size;
        negative = 0;
    }
#endif
    mpz_import(newob->z, len, -1, sizeof(l->ob_digit[0]), 0,
                sizeof(l->ob_digit[0])*8 - Py2or3Long_SHIFT, l->ob_digit);
    if(negative)
        mpz_neg(newob->z, newob->z);
    return newob;
}

/*
 * long->mpf delegates via long->mpz->mpf to avoid duplicating
 * the above-seen dependencies; ditto long->mpq
 */
static PympfObject *
long2mpf(PyObject * obj, unsigned int bits)
{
    PympfObject *newob;
    PyObject *intermediate = (PyObject*)long2mpz(obj);
    if(!intermediate) return 0;

    newob = mpz2mpf(intermediate, bits);
    Py_DECREF(intermediate);
    return newob;
}
static PympqObject *
long2mpq(PyObject * obj)
{
    PympqObject *newob;
    PyObject *intermediate = (PyObject*)long2mpz(obj);
    if(!intermediate) return 0;

    newob = mpz2mpq(intermediate);
    Py_DECREF(intermediate);
    return newob;
}


/*
 * mpz conversion from string includes from-binary (base-256 LSB string
 * of bytes) and 'true' from-string (bases 2 to 36; bases 8 and 16 are
 * special -- decorations of leading 0/0x are allowed (not required).
 *
 * Binary form was previously (0.6) limited to >=0 values; now (0.7)
 * extended to <0 values, by adding one last sign-byte, 0xFF for <0,
 * 0x00 for >0 (the latter only if the #bits is exact multiple of 8).
 */
static PympzObject *
str2mpz(PyObject *s, long base)
{
    PympzObject *newob;
    unsigned char *cp;
    Py_ssize_t len;
    int i;
    PyObject *ascii_str = NULL;

#if PY_MAJOR_VERSION >= 3
    assert(PyBytes_Check(s) || PyUnicode_Check(s));
#else
    assert(PyString_Check(s));
#endif

    if(!(newob = Pympz_new()))
        return NULL;

#if PY_MAJOR_VERSION >= 3
    if(PyBytes_Check(s)) {
        len = PyBytes_Size(s);
        cp = (unsigned char*)PyBytes_AsString(s);
    } else {
        ascii_str = PyUnicode_AsASCIIString(s);
        if(!ascii_str) {
            PyErr_SetString(PyExc_ValueError,
                    "string contains non-ASCII characters");
            return NULL;
        }
        len = PyBytes_Size(ascii_str);
        cp = (unsigned char*)PyBytes_AsString(ascii_str);
    }
#else
    len = PyString_Size(s);
    cp = (unsigned char*)PyString_AsString(s);
#endif

    if(256 == base) {
        /* Least significant octet first */
        mpz_t digit;
        int negative = 0;

        if(cp[len-1] == 0xFF) {
            negative = 1;
            --len;
        }
        mpz_set_si(newob->z, 0);
        mpz_inoc(digit);
        mpz_import(newob->z, len, -1, sizeof(char), 0, 0, cp);
        if(negative)
            mpz_neg(newob->z, newob->z);
        mpz_cloc(digit);
    } else {
        /* Don't allow NULL characters */
        for(i=0; i<len; i++) {
            if(cp[i] == '\0') {
                PyErr_SetString(PyExc_ValueError,
                    "string without NULL characters expected");
                Py_DECREF((PyObject *) newob);
                Py_XDECREF(ascii_str);
                return NULL;
            }
        }
        /* delegate rest to GMP's _set_str function */
        if(-1 == mpz_set_str(newob->z, (char*)cp, base)) {
            PyErr_SetString(PyExc_ValueError, "invalid digits");
            Py_DECREF((PyObject *) newob);
            Py_XDECREF(ascii_str);
            return NULL;
        }
    }
    Py_XDECREF(ascii_str);
    return newob;
}

/*
 * mpq conversion from string includes from-binary (base-256 LSB string
 * of bytes) and 'true' from-string (bases 2 to 36; bases 8 and 16 are
 * special -- decorations of leading 0/0x are allowed (but not required).
 * For 'true-bases' 2..36 there is a '/' separator between numerator and
 * denominator (if none, just numerator!); decimal point NOT allowed.
 *
 * Added in gmpy 1.02: also support a string of the form '12.34', i.e.,
 * WITH a decimal point and WITHOUT a slash
 *
 * Binary-form: 4-byte numerator length (upper bit set if <0), then
 * numerator (as above for mpz), then denominator (ditto).
 */
static PympqObject *
str2mpq(PyObject *stringarg, long base)
{
    PympqObject *newob;
    unsigned char *cp;
    Py_ssize_t len;
    int i;
    PyObject *ascii_str = NULL;

#if PY_MAJOR_VERSION >= 3
    assert(PyBytes_Check(stringarg) || PyUnicode_Check(stringarg));
#else
    assert(PyString_Check(stringarg));
#endif

    if(!(newob = Pympq_new()))
        return NULL;

#if PY_MAJOR_VERSION >= 3
    if(PyBytes_Check(stringarg)) {
        len = PyBytes_Size(stringarg);
        cp = (unsigned char*)PyBytes_AsString(stringarg);
    } else {
        ascii_str = PyUnicode_AsASCIIString(stringarg);
        if(!ascii_str) {
            PyErr_SetString(PyExc_ValueError,
                    "string contains non-ASCII characters");
            return NULL;
        }
        len = PyBytes_Size(ascii_str);
        cp = (unsigned char*)PyBytes_AsString(ascii_str);
    }
#else
    len = PyString_Size(stringarg);
    cp = (unsigned char*)PyString_AsString(stringarg);
#endif

    if(256 == base) {
        /* TODO: better factoring of str2mpz (for speed) */
        int topper, isnega, numlen;
        PyObject *s;
        PympzObject *numerator, *denominator;
        if(len < 6) {
            PyErr_SetString(PyExc_ValueError, "invalid mpq binary (too short)");
            Py_DECREF((PyObject*)newob);
            Py_XDECREF(ascii_str);
            return 0;
        }
        topper = cp[3] & 0x7f;
        isnega = cp[3] & 0x80;
        numlen = cp[0]+256*(cp[1]+256*(cp[2]+256*topper));
        if(len < (4+numlen+1)) {
            PyErr_SetString(PyExc_ValueError, "invalid mpq binary (num len)");
            Py_DECREF((PyObject*)newob);
            Py_XDECREF(ascii_str);
            return 0;
        }
        s = Py2or3Bytes_FromStringAndSize((char*)cp+4, numlen);
        numerator = str2mpz(s,256);
        Py_DECREF(s);
        if (!numerator) {
            Py_DECREF((PyObject*)newob);
            Py_XDECREF(ascii_str);
            return 0;
        }
        if(mpz_sgn(numerator->z) < 0) {
            PyErr_SetString(PyExc_ValueError, "invalid mpq binary (num sgn)");
            Py_DECREF((PyObject*)newob);
            Py_DECREF((PyObject*)numerator);
            Py_XDECREF(ascii_str);
            return 0;
        }
        if(isnega)
            mpz_neg(numerator->z, numerator->z);
        s = Py2or3Bytes_FromStringAndSize((char*)cp+4+numlen, len-4-numlen);
        denominator = str2mpz(s,256);
        Py_DECREF(s);
        if (!denominator) {
            Py_DECREF((PyObject*)newob);
            Py_DECREF((PyObject*)numerator);
            Py_XDECREF(ascii_str);
            return 0;
        }
        if(mpz_sgn(denominator->z) != 1) {
            PyErr_SetString(PyExc_ValueError, "invalid mpq binary (den sgn)");
            Py_DECREF((PyObject*)newob);
            Py_DECREF((PyObject*)numerator);
            Py_DECREF((PyObject*)denominator);
            Py_XDECREF(ascii_str);
            return 0;
        }
        mpq_set_num(newob->q, numerator->z);
        mpq_set_den(newob->q, denominator->z);
        mpq_canonicalize(newob->q);
        Py_DECREF((PyObject*)numerator);
        Py_DECREF((PyObject*)denominator);
    } else {
        /* Don't allow NULL characters */
        for(i=0; i<len; i++) {
            if(cp[i] == '\0') {
                PyErr_SetString(PyExc_ValueError,
                    "string without NULL characters expected");
                Py_DECREF((PyObject *) newob);
                Py_XDECREF(ascii_str);
                return NULL;
            }
        }
        /* trickily delegate the rest to GMP avoiding allocations/copies */
        {
            char* whereslash = strchr((char*)cp,'/');
            char* wheredot = 0;
            if(whereslash) *whereslash = 0;
            else {
                wheredot = strchr((char*)cp, '.');
                if(wheredot) {
                    PympfObject* temp = str2mpf(stringarg, base, 4*len);
                    if(temp) {
                        newob = mpf2mpq((PyObject*)temp);
                        Py_DECREF((PyObject*)temp);
                    }
                    return newob;
                }
            }
            if(-1 == mpz_set_str(mpq_numref(newob->q), (char*)cp, base)) {
                if(whereslash) *whereslash = '/';
                PyErr_SetString(PyExc_ValueError, "invalid digits");
                Py_DECREF((PyObject *) newob);
                Py_XDECREF(ascii_str);
                return NULL;
            }
            if(whereslash) {
                *whereslash = '/';
                if(-1==mpz_set_str(mpq_denref(newob->q), whereslash+1, base)) {
                    PyErr_SetString(PyExc_ValueError, "invalid digits");
                    Py_DECREF((PyObject *) newob);
                    Py_XDECREF(ascii_str);
                    return NULL;
                }
                if(0==mpz_sgn(mpq_denref(newob->q))) {
                    Py_DECREF((PyObject *) newob);
                    Py_XDECREF(ascii_str);
                    PyErr_SetString(PyExc_ZeroDivisionError, "mpq: zero denominator");
                    return NULL;
                }
                mpq_canonicalize(newob->q);
            } else {
                mpz_set_ui(mpq_denref (newob->q), 1);
            }
        }
    }
    Py_XDECREF(ascii_str);
    return newob;
}


/*
 * mpf conversion from string includes from-binary (base-256, format is
 * explained later) and 'true' from-string (bases 2 to 36), where exponent
 * if any is denoted by 'e' if base<=10, else by '@', and is always decimal.
 */
static PympfObject *
str2mpf(PyObject *s, long base, unsigned int bits)
{
    PympfObject *newob;
    unsigned char *cp;
    Py_ssize_t len;
    int precision, i;
    PyObject *ascii_str = NULL;

#if PY_MAJOR_VERSION >= 3
    assert(PyBytes_Check(s) || PyUnicode_Check(s));
#else
    assert(PyString_Check(s));
#endif

#if PY_MAJOR_VERSION >= 3
    if(PyBytes_Check(s)) {
        len = PyBytes_Size(s);
        cp = (unsigned char*)PyBytes_AsString(s);
    } else {
        ascii_str = PyUnicode_AsASCIIString(s);
        if(!ascii_str) {
            PyErr_SetString(PyExc_ValueError,
                    "string contains non-ASCII characters");
            return NULL;
        }
        len = PyBytes_Size(ascii_str);
        cp = (unsigned char*)PyBytes_AsString(ascii_str);
    }
#else
    len = PyString_Size(s);
    cp = (unsigned char*)PyString_AsString(s);
#endif
    if(bits>0) {
        precision = bits;
    } else { /* precision to be defaulted or fetched */
        if(base == 256) {  /* it may be encoded for fetching */
            precision = 8*(len-5);      /* default precision */
            if((len>=5) && (cp[0]&8)) { /* precision must be fetched */
                precision = 0;
                for(i=4; i>0; --i) {
                    precision = (precision<<8) | cp[i];
                }
            }
        } else { /* true-string, never encoded, just default it */
            precision = double_mantissa;
        }
        if(precision<=0) precision=1;
    }

    if(!(newob = Pympf_new(precision)))
        return NULL;

    if(256 == base) {
        /*
         * binary format for MP floats: first, a code-byte, then, a LSB
         * 4-byte unsigned int (exponent magnitude), then the "mantissa"
         * (actually, "significand", but "mantissa" is the usual term...)
         * in MSB form.
         *
         * The codebyte encodes both the signs, exponent and result, or
         * also the zeroness of the result (in which case, nothing more).
         */
        mpf_t digit;
        int codebyte = cp[0];
        int resusign = codebyte & 1;
        int exposign = codebyte & 2;
        int resuzero = codebyte & 4;
        int precilen = (codebyte & 8)?4:0;
        unsigned int expomag = 0;

        /* mpf zero has a very compact (1-byte) binary encoding!-) */
        if(resuzero) {
            mpf_set_ui(newob->f, 0);
            return newob;
        }

        /* all other numbers are 6+ bytes: codebyte, 4-byte exp, 1+
         * bytes for the mantissa; check this string is 6+ bytes
         */
        if(len<6+precilen) {
            PyErr_SetString(PyExc_ValueError,
                "string too short to be a gmpy.mpf binary encoding");
            Py_DECREF((PyObject *) newob);
            Py_XDECREF(ascii_str);
            return NULL;
        }
        /* reconstruct exponent */
        for(i=4+precilen; i>precilen; --i) {
            expomag = (expomag<<8) | cp[i];
        }

        /* reconstruct 'mantissa' (significand) */
        mpf_set_si(newob->f, 0);
        mpf_init2(digit, newob->rebits);
        for(i=5+precilen; i<len; i++) {
            mpf_set_ui(digit, cp[i]);
            mpf_div_2exp(digit, digit, (i-4-precilen) * 8);
            mpf_add(newob->f, newob->f, digit);
        }
        mpf_clear(digit);
        /* apply exponent, with its appropriate sign */
        if(exposign)
            mpf_div_2exp(newob->f, newob->f, 8*expomag);
        else
            mpf_mul_2exp(newob->f, newob->f, 8*expomag);
        /* apply significand-sign (sign of the overall number) */
        if(resusign)
            mpf_neg(newob->f, newob->f);
    } else {
        /* Don't allow NULL characters */
        for(i=0; i<len; i++) {
            if(cp[i] == '\0') {
                PyErr_SetString(PyExc_ValueError,
                    "string without NULL characters expected");
                Py_DECREF((PyObject *) newob);
                Py_XDECREF(ascii_str);
                return NULL;
            }
        }
        /* delegate the rest to GMP */
        if(-1 == mpf_set_str(newob->f, (char*)cp, base)) {
            PyErr_SetString(PyExc_ValueError,
                "invalid digits");
            Py_DECREF((PyObject *) newob);
            Py_XDECREF(ascii_str);
            return NULL;
        }
    }
    mpf_normalize(newob);
    Py_XDECREF(ascii_str);
    return newob;
}

/*
 * to convert-to-long, we have a dependence on longs' internals,
 * just as for the reverse-trip.  We concentrate dependence here.
 */
static PyObject *
mpz2long(PympzObject *x)
{
    int negative;
    int size;
    int i;
    size_t count;
    PyLongObject *newob;
    mpz_t temp;

    /* Assume gmp uses limbs as least as large as the builtin longs do */
    assert(mp_bits_per_limb >= Py2or3Long_SHIFT);

    mpz_inoc(temp);
    if(mpz_sgn(x->z) < 0) {
        negative = 1;
        mpz_neg(temp, x->z);
    } else {
        negative = 0;
        mpz_set(temp, x->z);
    }

    size = (mpz_sizeinbase(temp, 2) + Py2or3Long_SHIFT - 1) / Py2or3Long_SHIFT;

    if(!(newob = _PyLong_New(size))) {
        mpz_cloc(temp);
        return NULL;
    }
    mpz_export(newob->ob_digit, &count, -1, sizeof(newob->ob_digit[0]), 0,
            sizeof(newob->ob_digit[0])*8 - Py2or3Long_SHIFT, temp);
    if (count == 0) newob->ob_digit[0] = 0;
    mpz_cloc(temp);

    /* long_normalize() is file-static so we must reimplement it */
    /* longobjp = long_normalize(longobjp); */
    i = size;
    while ( (i>0) && (newob->ob_digit[i-1] == 0))
        i--;
    if(negative) i = -i;
#if PY_MAJOR_VERSION >= 3
    newob->ob_base.ob_size = i;
#else
    newob->ob_size = i;
#endif

    return (PyObject *) newob;
}

/*
 * mpf->long delegates via mpf->mpz->long to avoid duplicating
 * the above-seen thorny dependencies; ditto mpq->long
 */
static PyObject *
mpf2long(PympfObject *x)
{
    PyObject* result;

    PympzObject *intermediate = mpf2mpz((PyObject*)x);
    if(!intermediate) return 0;

    result = mpz2long(intermediate);
    Py_DECREF((PyObject*)intermediate);
    return result;
}
static PyObject *
mpq2long(PympqObject *x)
{
    PyObject* result;

    PympzObject *intermediate = mpq2mpz((PyObject*)x);
    if(!intermediate) return 0;

    result = mpz2long(intermediate);
    Py_DECREF((PyObject*)intermediate);
    return result;
}

static int
notanint(mpz_t z)
{
    return !mpz_fits_slong_p(z) && mpz_size(z);
}

static PyObject *
mpz2int(PympzObject *x)
{
#if PY_MAJOR_VERSION >= 3
    return mpz2long(x);
#else
    if(notanint(x->z)) {
        return mpz2long(x);
    }
    return PyInt_FromLong(mpz_get_si(x->z));
#endif
}

#if Py_TPFLAGS_HAVE_INDEX || (PY_MAJOR_VERSION >= 3)
static PyObject*
Pympz_asindex(PympzObject *x)
{
    PyObject* result = mpz2int(x);
    if(result) return result;
    PyErr_Clear();
    return mpz2long(x);
}
#endif

/*
 * mpf->int delegates via mpf->mpz->int for convenience; ditto mpq->int
 */
static PyObject *
mpf2int(PympfObject *x)
{
    PyObject* result;

    PympzObject *intermediate = mpf2mpz((PyObject*)x);
    if(!intermediate) return 0;

    result = mpz2int(intermediate);
    Py_DECREF((PyObject*)intermediate);
    return result;
}
static PyObject *
mpq2int(PympqObject *x)
{
    PyObject* result;

    PympzObject *intermediate = mpq2mpz((PyObject*)x);
    if(!intermediate) return 0;

    result = mpz2int(intermediate);
    Py_DECREF((PyObject*)intermediate);
    return result;
}


static PyObject *
mpz2float(PympzObject *x)
{
    double res = mpz_get_d(x->z);
    return PyFloat_FromDouble(res);
}
static PyObject *
mpf2float(PympfObject *x)
{
    double res = mpf_get_d(x->f);
    return PyFloat_FromDouble(res);
}
static PyObject *
mpq2float(PympqObject *x)
{
    double res = mpq_get_d(x->q);
    return PyFloat_FromDouble(res);
}


/*
 *  build binary representation of mpz (base-256 little-endian)
 *  Note: design limitation used to forbid binary repr of <0 mpz;
 *  this has now been remedied, but at the price of full compatibility
 *  with files saved in gmpy releases 0.6 and earlier.
 */
static PyObject *
mpz2binary(PympzObject *x)
{
    mpz_t temp;
    size_t size, usize;
    int negative, needtrail;
    char *buffer;
    PyObject *s;

    assert(Pympz_Check( (PyObject *) x));

    mpz_inoc(temp);
    if(mpz_sgn(x->z) < 0) {
        negative = 1;
        mpz_neg(temp, x->z);
    } else {
        negative = 0;
        mpz_set(temp, x->z);
    }

    size = mpz_sizeinbase(temp, 2);
    needtrail = (size%8)==0;
    usize = size = (size + 7) / 8;
    if(negative || needtrail)
        ++size;

    TEMP_ALLOC(buffer, size);
    buffer[0] = 0x00;
    mpz_export(buffer, NULL, -1, sizeof(char), 0, 0, temp);
    if(usize < size) {
        buffer[usize] = negative?0xff:0x00;
    }
    mpz_cloc(temp);
    s = Py2or3Bytes_FromStringAndSize(buffer, size);
    TEMP_FREE(buffer, size);
    return s;
}

/*
 *  build binary representation of mpq (base-256 little-endian
 *  for num, then den; before those, 4 bytes for _length_ of
 *  numerator, which also encode sign as the single top bit).
 */
static PyObject *
mpq2binary(PympqObject *x)
{
    int sizenum, sizeden, size, sizetemp;
    int negative=0;
    char *buffer;
    int i;
    PyObject *s;
    mpq_t qtemp;

    assert(Pympq_Check( (PyObject *) x));
    mpq_inoc(qtemp);
    mpq_set(qtemp, x->q);

    if(mpq_sgn(qtemp) < 0) {
        negative = 1;
        mpz_abs(mpq_numref(qtemp), mpq_numref(qtemp));
    }
    assert(mpz_sgn(mpq_denref(qtemp))>0);

    sizenum = (mpz_sizeinbase(mpq_numref(qtemp), 2) + 7) / 8;
    sizeden = (mpz_sizeinbase(mpq_denref(qtemp), 2) + 7) / 8;
    size = sizenum+sizeden+4;

    if(!(buffer = malloc(size))) {
        mpq_cloc(qtemp);
        PyErr_NoMemory();
        return NULL;
    }
    sizetemp = sizenum;
    for(i=0; i<4; i++) {
        buffer[i] = (char)(sizetemp & 0xff);
        sizetemp >>= 8;
    }
    if(negative) buffer[3] |= 0x80;
    buffer[4] = 0x00;

    mpz_export(buffer+4, NULL, -1, sizeof(char), 0, 0, mpq_numref(qtemp));
    mpz_export(buffer+sizenum+4, NULL, -1, sizeof(char), 0, 0, mpq_denref(qtemp));
    mpq_cloc(qtemp);
    s = Py2or3Bytes_FromStringAndSize(buffer, size);
    free(buffer);
    return s;
}

/*
 * helper functions for mpf->binary conversion
 * hof: maps a hex-digit character into 0..15
 * di256: maps two hex-digits chars into 0..255
 */
static int hof(int hedi)
{
    static char table[] = "0123456789abcdef";
    char* p = strchr(table, tolower(hedi));
    assert(hedi && p);
    return p-table;
}
static char di256(int di1, int di2)
{
    return (char)(hof(di2)+16*hof(di1));
}

/*
 * build binary representation of mpf (see format description above)
 */
static PyObject *
mpf2binary(PympfObject *x)
{
    int size, hexdigs;
    char *buffer, *aux;
    int i, j;
    PyObject *s;
    int sign, codebyte;
    mp_exp_t the_exp;
    long lexp, lprec;
    int lexpodd, extrabyte;

    assert(Pympf_Check( (PyObject *) x));

    /* prepare codebyte */
    sign = mpf_sgn(x->f);
    if(sign==0) {
        /* 0 -> single codebyte with 'zerovalue' bit set */
#if PY_MAJOR_VERSION >= 3
        return Py_BuildValue("y", "\004");
#else
        return Py_BuildValue("s", "\004");
#endif
        /* codebyte = 0; */
    } else if(sign<0) {
        codebyte = 1;
        mpf_neg(x->f, x->f); /* note we TEMPORARILY change sign!!! */
    } else {
        codebyte = 0;
    }

    /* get buffer of base-16 digits */
    buffer  = mpf_get_str(0, &the_exp, 16, 0, x->f);
    /* no need to worry about null-buffer as x->f==0.0 was
     * already handled above (see first test on 'sign').
     */
    /* restore correct sign to x->f if it was changed! */
    if(codebyte) {
        mpf_neg(x->f, x->f);
    }
    hexdigs = strlen(buffer);
    /* adjust exponent, & possibly set codebyte's expo-sign bit.
     * note the_exp is base-16 exp, while we need to have it in
     * base-256 -- so it's basically halved (but, with care...!).
     */
    if(the_exp<0) {
        codebyte |= 2;
        the_exp = -the_exp;
    }
    lexp = the_exp;
    lexpodd = lexp&1;
    lexp = lexp/2 + lexpodd;
    if(lexpodd && (codebyte&2))
        --lexp;
    /* remember we also store precision explicitly */
    codebyte |= 8;

    /* allocate suitably-sized, uninitialized Python string */
    size = (hexdigs+1)/2;
    /* allocate an extra byte if lexpodd and hexdigs is even */
    extrabyte = lexpodd & ~hexdigs;
    s = Py2or3Bytes_FromStringAndSize(0, 1+4+size+4+extrabyte);
    if(!s) return 0;
    /* set the data to the new Python string's buffer */
    aux = Py2or3Bytes_AS_STRING(s);
    /* codebyte first */
    aux[0] = (char)codebyte;
    /* then precision */
    lprec = x->rebits;
    for(i=0; i<4; ++i) {
        aux[i+1] = (char)(lprec & 0xFF);
        lprec >>= 8;
    }
    /* then exponent */
    for(i=0; i<4; ++i) {
        aux[4+i+1] = (char)(lexp & 0xFF);
        lexp >>= 8;
    }
    /* then mantissa, grouping 2 hex digits per base-256 digits;
     * with some care for the first & last ones...
     */
    j=0; i=0;
    if(lexpodd) {
        aux[i+9] = di256('0',buffer[0]);
        j=1; i=1;
    }
    for(; i<size+extrabyte; ++i) {
        int secdig = (j+1)<hexdigs? buffer[j+1] : '0';
        aux[i+9] = di256(buffer[j], secdig);
        j += 2;
    }

    free(buffer);
    return s;
}

/*
 * format mpz into any base (2 to 36), optionally with
 * a "gmpy.mpz(...)" tag around it so it can be recovered
 * through a Python eval of the resulting string
 * Note: tag can be just mpz() if options.tagoff=5
 */
static char* ztag = "gmpy.mpz(";
static PyObject *
mpz_ascii(mpz_t z, int base, int with_tag)
{
    PyObject *s;
    char *buffer, *p;
    mpz_t temp;
    int minus, size;

    if((base != 0) && ((base < 2) || (base > 36))) {
        PyErr_SetString(PyExc_ValueError,
            "base must be either 0 or in the interval 2 ... 36");
        return NULL;
    }

    mpz_inoc(temp);
    if(mpz_sgn(z) < 0) {
        minus = 1;
        mpz_neg(temp, z);
    } else {
        minus = 0;
        mpz_set(temp, z);
    }

    /* Allocate extra space for:
     *
     * minus sign and trailing NULL byte (2)
     * 'gmpy.mpz()' tag                  (10)
     * '0x' prefix                       (2)
     * 'L' suffix                        (1)
     *                                  -----
     *                                   15
     */
    size = mpz_sizeinbase(z, base) + 16;
    TEMP_ALLOC(buffer, size);
    p = buffer;
    if(with_tag) {
       strcpy(p, ztag+options.tagoff);
       p += strlen(p);
    }
    if(minus)
        *(p++) = '-';
    if(base == 8)
        *(p++) = '0';
    else if(base == 16) {
        *(p++) = '0';
        *(p++) = 'x';
    }

    mpz_get_str(p, base, temp); /* Doesn't return number of characters */
    p = buffer + strlen(buffer); /* Address of NULL byte */
#if PY_MAJOR_VERSION < 3
    if(with_tag && notanint(temp))
        *(p++) = 'L';
#endif
    if(with_tag)
        *(p++) = ')';
    s = Py2or3Bytes_FromStringAndSize(buffer, p - buffer);
    mpz_cloc(temp);
    TEMP_FREE(buffer, size);
    return s;
}
static PyObject *
Pympz_ascii(PympzObject *self, int base, int with_tag)
{
#if PY_MAJOR_VERSION >= 3
    PyObject *s, *t;
    assert(Pympz_Check( (PyObject *) self));
    t = mpz_ascii(self->z, base, with_tag);
    if(!t) return NULL;
    s = PyUnicode_FromString(PyBytes_AS_STRING(t));
    Py_DECREF(t);
    return s;
#else
    assert(Pympz_Check( (PyObject *) self));
    return mpz_ascii(self->z, base, with_tag);
#endif
}

static char* qtag = "gmpy.mpq(";
static int qden_1(mpq_t q)
{
    return 0==mpz_cmp_ui(mpq_denref(q),1);
}
static PyObject *
Pympq_ascii(PympqObject *self, int base, int with_tag)
{
    PyObject *result = 0;
    PyObject *numstr = mpz_ascii(mpq_numref(self->q), base, 0);
    PyObject *denstr = 0;
    PyObject *temp = 0;

    if(!numstr) return 0;

    if(!qden_1(self->q)) {
        denstr = mpz_ascii(mpq_denref(self->q), base, 0);
        if(!denstr) {
            Py_DECREF(numstr);
            return 0;
        }
    }

    if(with_tag) {
        result = Py2or3Bytes_FromString(qtag+options.tagoff);
        if(result) Py2or3Bytes_ConcatAndDel(&result, numstr);
        if(!result) {
            Py_XDECREF(denstr);
            return 0;
        }
#if PY_MAJOR_VERSION < 3
        if(notanint(mpq_numref(self->q))) {
            temp = Py2or3Bytes_FromString("L");
            Py2or3Bytes_ConcatAndDel(&result, temp);
            if(!result) {
                Py_XDECREF(denstr);
                return 0;
            }
        }
#endif
    } else {
        result = numstr;
        numstr = 0;
    }
    if(denstr) {
        char* separator = with_tag?",":"/";
        temp = Py2or3Bytes_FromString(separator);
        Py2or3Bytes_ConcatAndDel(&result, temp);
        if(!result) {
            Py_DECREF(denstr);
            return 0;
        }
        Py2or3Bytes_ConcatAndDel(&result, denstr);
#if PY_MAJOR_VERSION < 3
        if(with_tag && notanint(mpq_denref(self->q))) {
            temp = Py2or3Bytes_FromString("L");
            Py2or3Bytes_ConcatAndDel(&result, temp);
            if(!result) {
                return 0;
            }
        }
#endif
    }
    if(with_tag && result) {
        temp = Py2or3Bytes_FromString(")");
        Py2or3Bytes_ConcatAndDel(&result, temp);
    }
#if PY_MAJOR_VERSION >= 3
    temp = PyUnicode_FromString(PyBytes_AS_STRING(result));
    Py_DECREF(result);
    return temp;
#else
    return result;
#endif
}

#define OP_TAG 1
#define OP_RAW 2
static char ftag[]="gmpy.mpf('";
/*
 * format mpf into any base (2 to 36), optionally with
 * a "gmpy.mpf('...')" tag around it so it can be recovered
 * through a Python eval of the resulting string.
 * Note: tag can be just mpf() if options.tagoff=5
 * digits: number of digits to ask GMP for (0=all of
 *     them) -- fewer will be given, if fewer significant
 * minexfi: format as mantissa-exponent if exp<minexfi
 * maxexfi: format as mantissa-exponent if exp>maxexfi
 *     note that, e.g., minexfi=0, maxexfi=-1, means
 *     "always format as mantissa-exponent".  If not
 *     mantissa-exponent, the number will be formatted
 *     as "fixed point" (FP).  Note the decimal point
 *     is _always_ explicitly inserted by this function
 *     (except when bit OP_RAW is set in optionflags).
 * optionflags: bitmap of option-values; currently:
 *     OP_TAG (1): add the gmpy.mpf('...') tag
 *     OP_RAW (2): ignore minexfi/maxexfi/OP_TAG
 *         and return a 3-element tuple digits/exponent/rprec
 *         (as GMP gives them) for Python formatting;
 *         'digits' may include a '-' sign, but no decimal
 *         point, nor tag, nor any exponent-indicator.
 *     other bits are currently ignored
 */
static PyObject *
Pympf_ascii(PympfObject *self, int base, int digits,
    int minexfi, int maxexfi, int optionflags)
{
    PyObject *res;
#if PY_MAJOR_VERSION >= 3
    PyObject *temp;
#endif
    char *buffer;
    mp_exp_t the_exp;

    /* check arguments are valid */
    assert(Pympf_Check((PyObject*)self));
    if(! ( (base==0) || ((base >= 2) && (base <= 36))))    {
        PyErr_SetString(PyExc_ValueError,
            "base must be either 0 or in the interval 2 ... 36");
        return NULL;
    }
    if(digits < 0) {
        PyErr_SetString(PyExc_ValueError, "digits must be >= 0");
        return NULL;
    }

    /* obtain digits-string and exponent */
    buffer = mpf_get_str(0, &the_exp, base, digits, self->f);
    if(!*buffer) {
        /* need to use malloc here for uniformity with mpf_get_str */
        free(buffer);
        buffer = malloc(2);
        strcpy(buffer, "0");
        the_exp = 1;
    }

    if(optionflags & OP_RAW) {
        res = Py_BuildValue("(sii)", buffer, the_exp, self->rebits);
        free(buffer);
        return res;
    } else {
        /* insert formatting elements (decimal-point, leading or
         * trailing 0's, other indication of exponent...)
         */
        int buflen = strlen(buffer);
        /* account for the decimal point that is always inserted */
        int size = buflen+1;
        char expobuf[24];
        char auprebuf[24];
        int isfp=1;   /* flag: fixed-point format (FP)? */
        int isnegative=0;
        if(buffer[0]==0x2d) isnegative=1;

        /* compute size of needed Python string */
        if(optionflags & OP_TAG) {
            size += strlen(ftag+options.tagoff) + 2;
            if(self->rebits != double_mantissa) {
                sprintf(auprebuf,",%d",self->rebits);
                size += strlen(auprebuf);
            }
        }
        if(the_exp<minexfi || the_exp>maxexfi) { /* exponential format */
            /* add exponent-length + 1 for '@' or 'e' marker */
            sprintf(expobuf,"%ld",the_exp-1);
            size += strlen(expobuf) + 1;
            isfp = 0;
        } else { /* 'fixed-point' format */
            /* add number of leading or trailing 0's */
            if(the_exp <= 0) {
                /* add leading 0's */
                size += abs(the_exp)+1;
            } else {
                /* add trailing 0's if needed */
                if(the_exp >= (buflen-isnegative))
                    size += (the_exp-(buflen-isnegative))+1;
            }
        }

        /* allocate the string itself (uninitialized, as yet) */
        res = Py2or3Bytes_FromStringAndSize(0, size);

        {
            /* proceed with building the string-buffer value */
            char* pd = Py2or3Bytes_AS_STRING(res);
            char* ps = buffer;

            /* insert leading tag if requested */
            if(optionflags & OP_TAG) {
                char* pt = ftag+options.tagoff;
                while(*pt) *pd++ = *pt++;
            }

            /* copy sign if it's there */
            if(*ps=='-') {
                *pd++ = *ps++;
            }

            /* insert a leading-0 if needed for non-positive-exp FP,
             * else just copy the leading digit (goes before '.')
             */
            if(isfp && the_exp<=0)
                *pd++ = '0';
            else if(*ps)
                *pd++ = *ps++;
            else
                *pd++ = '0';

            /* insert what else goes before '.' for FP */
            if(isfp && the_exp>1) {
                /* number of digits-to-copy before the '.' */
                int dtc = the_exp-1;
                /* copy requested # of digits as long as there
                 * are still digits to copy in the buffer
                 */
                while(dtc && *ps) {
                    *pd++ = *ps++;
                    --dtc;
                }
                /* insert trailing 0's before the '.' if
                 * needed to make up the total # digits
                 * that go before the '.' in FP/large exp
                 */
                while(dtc>0) {
                    *pd++ = '0';
                    --dtc;
                }
            }

            /* the decimal-point is _always_ explicitly there */
            *pd++ = '.';

            /* as is at least 1 trailing-digit after it, if FP,
             * so put a 0 if no more digits to copy
             */
            if(isfp && !*ps)
                *pd++ = '0';

            /* in FP with negative exp, we have more leading 0's
             * after the decimal-point before copying the digits
             * from the buffer
             */
            if(isfp && the_exp<0) {
                int dtc = abs(the_exp);
                while(dtc>0) {
                    *pd++ = '0';
                    --dtc;
                }
            }

            /* copy all remaining digits from buffer, if any */
            while(*ps) *pd++ = *ps++;

            /* insert marker-and-exponent if _not_ FP */
            if(!isfp) {
                char* pe = expobuf;
                *pd++ = (base<=10)?'e':'@';
                while(*pe) *pd++ = *pe++;
            }

            /* insert trailing-part of the tag if needed */
            if(optionflags & OP_TAG) {
                char* pe = auprebuf;
                *pd++ = '\'';
                if(self->rebits != double_mantissa)
                    while(*pe) *pd++ = *pe++;
                *pd++ = ')';
            }
        }
        free(buffer);
#if PY_MAJOR_VERSION >= 3
        temp = PyUnicode_FromString(PyBytes_AS_STRING(res));
        Py_DECREF(res);
        return temp;
#else
        return res;
#endif
    }
}

/*
 * generic conversions
 */
static PympqObject*
anynum2mpq(PyObject* obj)
{
    PympqObject* newob = 0;

    if(Pympq_Check(obj)) {
        Py_INCREF(obj);
        newob = (PympqObject *) obj;
    } else if(Pympz_Check(obj)) {
        newob = mpz2mpq(obj);
#if PY_MAJOR_VERSION == 2
    } else if(PyInt_Check(obj)) {
        newob = int2mpq(obj);
#endif
    } else if(Pympf_Check(obj)) {
        newob = mpf2mpq(obj);
    } else if(PyFloat_Check(obj)) {
        newob = float2mpq(obj);
    } else if(PyLong_Check(obj)) {
        newob = long2mpq(obj);
    }

    if(options.debug)
        fprintf(stderr,"any2mpq(%p)->%p\n", obj, newob);

    return newob;
}

/* Convert an integer or mpz to mpq. */

static PympqObject*
anyrational2mpq(PyObject* obj)
{
    PympqObject* newob = 0;

    if(Pympq_Check(obj)) {
        Py_INCREF(obj);
        newob = (PympqObject *) obj;
    } else if(Pympz_Check(obj)) {
        newob = mpz2mpq(obj);
#if PY_MAJOR_VERSION == 2
    } else if(PyInt_Check(obj)) {
        newob = int2mpq(obj);
#endif
    } else if(PyLong_Check(obj)) {
        newob = long2mpq(obj);
    }

    if(options.debug)
        fprintf(stderr,"anyrational2mpq(%p)->%p\n", obj, newob);

    return newob;
}

static PympzObject*
anynum2mpz(PyObject* obj)
{
    PympzObject* newob = 0;

    if(Pympz_Check(obj)) {
        Py_INCREF(obj);
        newob = (PympzObject *) obj;
#if PY_MAJOR_VERSION == 2
    } else if(PyInt_Check(obj)) {
        newob = int2mpz(obj);
#endif
    } else if(PyLong_Check(obj)) {
        newob = long2mpz(obj);
    } else if(Pympq_Check(obj)) {
        newob = mpq2mpz(obj);
    } else if(Pympf_Check(obj)) {
        newob = mpf2mpz(obj);
    } else if(PyFloat_Check(obj)) {
        newob = float2mpz(obj);
    }
    if(options.debug)
        fprintf(stderr,"anynum2mpz(%p)->%p\n", obj, newob);

    return newob;
}

/* Convert an integer-like object to an mpz.
   Todo: possibly add support for objects that support an index method. */

static PympzObject*
anyint2mpz(PyObject* obj)
{
    PympzObject* newob = 0;

    if(Pympz_Check(obj)) {
        Py_INCREF(obj);
        newob = (PympzObject *) obj;
#if PY_MAJOR_VERSION == 2
    } else if(PyInt_Check(obj)) {
        newob = int2mpz(obj);
#endif
    } else if(PyLong_Check(obj)) {
        newob = long2mpz(obj);
    }
    if(options.debug)
        fprintf(stderr,"anyint2mpz(%p)->%p\n", obj, newob);

    return newob;
}

static PympfObject*
anynum2mpf(PyObject* obj, unsigned int bits)
{
    PympfObject* newob = 0;

    if(Pympf_Check(obj)) {
        newob = (PympfObject *) obj;
        if(!bits || newob->rebits==bits) {
            Py_INCREF(obj);
        } else {
            newob = mpf2mpf(newob, bits);
        }
    } else if(PyFloat_Check(obj)) {
        newob = float2mpf(obj, bits);
#if PY_MAJOR_VERSION == 2
    } else if(PyInt_Check(obj)) {
        newob = int2mpf(obj, bits);
#endif
    } else if(Pympq_Check(obj)) {
        newob = mpq2mpf(obj, bits);
    } else if(Pympz_Check(obj)) {
        newob = mpz2mpf(obj, bits);
    } else if(PyLong_Check(obj)) {
        newob = long2mpf(obj, bits);
    }

    if(options.debug)
        fprintf(stderr, "anynum2mpf(%p,%d)->%p (%d)\n", obj,
                bits, newob, newob != 0 ? newob->rebits : -1);

    return newob;
}

/* Convert any int, mpz, mpq to mpf. If input is already an mpf, the reference
   count will be incremented and the new reference will be returned. */

static PympfObject*
anyreal2mpf(PyObject* obj, unsigned int bits)
{
    PympfObject* newob = 0;

    if(Pympf_Check(obj)) {
        newob = (PympfObject *) obj;
        if(!bits || newob->rebits==bits) {
            Py_INCREF(obj);
        } else {
            newob = mpf2mpf(newob, bits);
        }
    } else if(PyFloat_Check(obj)) {
        newob = float2mpf(obj, bits);
#if PY_MAJOR_VERSION == 2
    } else if(PyInt_Check(obj)) {
        newob = int2mpf(obj, bits);
#endif
    } else if(Pympq_Check(obj)) {
        newob = mpq2mpf(obj, bits);
    } else if(Pympz_Check(obj)) {
        newob = mpz2mpf(obj, bits);
    } else if(PyLong_Check(obj)) {
        newob = long2mpf(obj, bits);
    } else if(!strcmp(obj->ob_type->tp_name, "Decimal")) {
        PyObject *s = PyObject_Str(obj);
        if(s) {
            newob = str2mpf(s, 10, bits);
            Py_DECREF(s);
        }
    }

    if(options.debug)
        fprintf(stderr, "anyreal2mpf(%p,%d)->%p (%d)\n", obj,
                bits, newob, newob != 0 ? newob->rebits : -1);

    return newob;
}

static PympfObject*
anyrational2mpf(PyObject* obj, unsigned int bits)
{
    PympfObject* newob = 0;

    if(Pympf_Check(obj)) {
        newob = (PympfObject *) obj;
        if(!bits || newob->rebits==bits) {
            Py_INCREF(obj);
        } else {
            newob = mpf2mpf(newob, bits);
        }
#if PY_MAJOR_VERSION == 2
    } else if(PyInt_Check(obj)) {
        newob = int2mpf(obj, bits);
#endif
    } else if(Pympq_Check(obj)) {
        newob = mpq2mpf(obj, bits);
    } else if(Pympz_Check(obj)) {
        newob = mpz2mpf(obj, bits);
    } else if(PyLong_Check(obj)) {
        newob = long2mpf(obj, bits);
    }

    if(options.debug)
        fprintf(stderr, "anyreal2mpf(%p,%d)->%p (%d)\n", obj,
                bits, newob, newob != 0 ? newob->rebits : -1);

    return newob;
}

/*
 * coerce any number to a mpz
 */
int
Pympz_convert_arg(PyObject *arg, PyObject **ptr)
{
    PympzObject* newob = anyint2mpz(arg);
    if(options.debug)
        fprintf(stderr, "mpz_conv_arg(%p)->%p\n", arg, newob);

    if(newob) {
        *ptr = (PyObject*)newob;
        return 1;
    } else {
        PyErr_SetString(PyExc_TypeError,
            "argument cannot be converted to mpz");
        return 0;
    }
}

/*
 * coerce any number to a mpq
 */
int
Pympq_convert_arg(PyObject *arg, PyObject **ptr)
{
    PympqObject* newob = anyrational2mpq(arg);
    if(options.debug)
        fprintf(stderr, "mpq_conv_arg(%p)->%p\n", arg, newob);

    if(newob) {
        *ptr = (PyObject*)newob;
        return 1;
    } else {
        if(!PyErr_Occurred()) {
            PyErr_SetString(PyExc_TypeError,
                "argument can not be converted to mpq");
        }
        return 0;
    }
}

/*
 * coerce any number to a mpf
 */
int
Pympf_convert_arg(PyObject *arg, PyObject **ptr)
{
    PympfObject* newob = anyreal2mpf(arg,0);
    if(options.debug)
        fprintf(stderr, "mpf_conv_arg(%p)->%p\n", arg, newob);

    if(newob) {
        *ptr = (PyObject*)newob;
        return 1;
    } else {
        PyErr_SetString(PyExc_TypeError,
            "argument can not be converted to mpf");
        return 0;
    }
}

/* str and repr implementations for mpz */
static PyObject *
mpz2str(PympzObject *self)
{
    /* base-10, no tag */
    return Pympz_ascii(self, 10, 0);
}
static PyObject *
mpz2repr(PympzObject *self)
{
    /* base-10, with tag */
    return Pympz_ascii(self, 10, 1);
}

/* str and repr implementations for mpq */
static PyObject *
mpq2str(PympqObject *self)
{
    /* base-10, no tag */
    return Pympq_ascii(self, 10, 0);
}
static PyObject *
mpq2repr(PympqObject *self)
{
    /* base-10, with tag */
    return Pympq_ascii(self, 10, 1);
}

/* str and repr implementations for mpz */
static PyObject *
mpf2str(PympfObject *self)
{
    /* base-10, FP for exp -2 to 8, no tag */
    return Pympf_ascii(self, 10, 0, -2, 8, 0);
}
static PyObject *
mpf2repr(PympfObject *self)
{
    /* base-10, always mantissa+exp, with tag */
    return Pympf_ascii(self, 10, 0, 0, -1, OP_TAG);
}

static PyObject *
Pympz_copy(PyObject *self, PyObject *args)
{
    PyObject* temp;
    if(self && Pympz_Check(self)) {
        if(PyTuple_GET_SIZE(args) != 0) {
            PyErr_SetString(PyExc_TypeError, "_copy() takes exactly 1 argument");
            return NULL;
        }
        return (PyObject*)mpz2mpz((PympzObject*)self);
    } else {
        if(PyTuple_GET_SIZE(args) != 1){
            PyErr_SetString(PyExc_TypeError, "_copy() takes exactly 1 argument");
            return NULL;
        }
        temp = PyTuple_GET_ITEM(args, 0);
        if(Pympz_Check(temp)) {
            return (PyObject*)mpz2mpz((PympzObject*)temp);
        } else {
            PyErr_SetString(PyExc_TypeError, "unsupported operand type for _copy(): mpz required");
            return NULL;
        }
    }
}

/* copy mpf object */
static PyObject *
Pympf_copy(PyObject *self, PyObject *args)
{
    PyObject *s;
    unsigned int bits=0;
    SELF_MPF_ONE_ARG("|I",&bits);

    assert(Pympf_Check(self));
    if(!bits) bits = ((PympfObject*)self)->rebits;
    s = (PyObject*)mpf2mpf((PympfObject*)self, bits);
    Py_DECREF(self);
    return s;
}

/* copy mpq object */
static PyObject *
Pympq_copy(PyObject *self, PyObject *args)
{
    PyObject *s;

    SELF_MPQ_NO_ARG;
    assert(Pympq_Check(self));
    s = (PyObject*)mpq2mpq((PympqObject*)self);
    Py_DECREF(self);
    return s;
}


/* produce portable binary form for mpz object */
static char doc_binarym[]="\
x.binary(): returns a Python string that is a portable binary\n\
representation of x (the string can later be passed to the mpz\n\
constructor function to obtain an exact copy of x's value).\n\
";
static char doc_binaryg[]="\
binary(x): returns a Python string that is a portable binary\n\
representation of x (the string can later be passed to the mpz\n\
constructor function to obtain an exact copy of x's value).\n\
x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_binary(PyObject *self, PyObject *args)
{
    PyObject *s;
    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));
    s = mpz2binary((PympzObject*)self);
    Py_DECREF(self);
    return s;
}

/* produce portable binary form for mpq object */
static char doc_qbinarym[]="\
x.binary(): returns a Python string that is a portable binary\n\
representation of x (the string can later be passed to the mpq\n\
constructor function to obtain an exact copy of x's value).\n\
";
static char doc_qbinaryg[]="\
qbinary(x): returns a Python string that is a portable binary\n\
representation of x (the string can later be passed to the mpq\n\
constructor function to obtain an exact copy of x's value).\n\
x must be an mpq, or else gets coerced to one.\n\
";
static PyObject *
Pympq_binary(PyObject *self, PyObject *args)
{
    PyObject *s;
    SELF_MPQ_NO_ARG;
    assert(Pympq_Check(self));
    s = mpq2binary((PympqObject*)self);
    Py_DECREF(self);
    return s;
}

/* produce portable binary form for mpf object */
static char doc_fbinarym[]="\
x.binary(): returns a Python string that is a portable binary\n\
representation of x (the string can later be passed to the mpf\n\
constructor function to obtain an exact copy of x's value).\n\
";
static char doc_fbinaryg[]="\
fbinary(f): returns a Python string that is a portable binary\n\
representation of x, which is an mpf or else gets coerced to one.\n\
The string can later be passed to the mpf constructor function\n\
to obtain an exact copy of x's mpf value.\n\
";
static PyObject *
Pympf_binary(PyObject *self, PyObject *args)
{
    PyObject *s;
    SELF_MPF_NO_ARG;
    assert(Pympf_Check(self));
    s = mpf2binary((PympfObject*)self);
    Py_DECREF(self);
    return s;
}

/* produce digits for an mpz in requested base, default 10 */
static char doc_digitsm[]="\
x.digits([base]): returns Python string representing x in the\n\
given base (2 to 36, default 10 if omitted or 0); leading '-'\n\
is present if x<0, but no leading '+' if x>=0.\n\
";
static char doc_digitsg[]="\
digits(x[,base]): returns Python string representing x in the\n\
given base (2 to 36, default 10 if omitted or 0); leading '-'\n\
present if x<0, but no leading '+' if x>=0. x must be an mpz,\n\
or else gets coerced into one.\n\
";
static PyObject *
Pympz_digits(PyObject *self, PyObject *args)
{
    int base = 10;
    PyObject *s;

    SELF_MPZ_ONE_ARG("|i",&base);
    assert(Pympz_Check(self));
    s = Pympz_ascii((PympzObject*)self, base, 0);
    Py_DECREF(self);
    return s;
}

/* return number-of-digits for an mpz in requested base, default 10 */
static char doc_numdigitsm[]="\
x.numdigits([base]): returns length of string representing x in\n\
the given base (2 to 36, default 10 if omitted or 0); the value\n\
returned may sometimes be 1 more than necessary; no provision\n\
for any 'sign' character, nor leading '0' or '0x' decoration,\n\
is made in the returned length.\n\
";
static char doc_numdigitsg[]="\
numdigits(x[,base]): returns length of string representing x in\n\
the given base (2 to 36, default 10 if omitted or 0); the value\n\
returned may sometimes be 1 more than necessary; no provision\n\
for any 'sign' character, nor leading '0' or '0x' decoration,\n\
is made in the returned length.  x must be an mpz, or else gets\n\
coerced into one.\n\
";
static PyObject *
Pympz_numdigits(PyObject *self, PyObject *args)
{
    int base = 10;
    PyObject *s;

    SELF_MPZ_ONE_ARG("|i",&base);
    assert(Pympz_Check(self));
    if(base==0) base=10;
    if((base < 2) || (base > 36)) {
        PyErr_SetString(PyExc_ValueError,
            "base must be either 0 or in the interval 2 ... 36");
        return NULL;
    }
    s = Py_BuildValue("l", (long) mpz_sizeinbase(Pympz_AS_MPZ(self), base));
    Py_DECREF(self);
    return s;
}

static char doc_bit_lengthm[]="\
x.bit_length(): returns length of string representing x in base 2\n\
";
static char doc_bit_lengthg[]="\
bit_length(x): returns length of string representing x in base 2\n\
";
static PyObject *
Pympz_bit_length(PyObject *self, PyObject *args)
{
    long i = 0;
    PympzObject* newob;
    //~ PyObject *s;

    //~ fprintf(stderr,"self is %p\n", self);
    //~ if(self) fprintf(stderr,"type is %s\n", self->ob_type->tp_name);
    //~ fprintf(stderr,"args is %p\n", args);
    //~ if(args) fprintf(stderr,"args is %s\n", args->ob_type->tp_name);

    //~ if(self) {
    if(self && Pympz_Check(self)) {
        //~ fprintf(stderr,"hello\n");
        if(PyTuple_GET_SIZE(args) != 0) {
            PyErr_SetString(PyExc_TypeError,
                "bit_length() takes exactly 1 argument");
            return NULL;
        }
        assert(Pympz_Check(self));
        if ((i=mpz_sizeinbase(Pympz_AS_MPZ(self), 2))==1)
            return Py2or3Int_FromLong((long) mpz_size(Pympz_AS_MPZ(self)));
        else
            return Py2or3Int_FromLong(i);
    } else {
        //~ fprintf(stderr,"hello again\n");
        if(PyTuple_GET_SIZE(args) != 1){
            PyErr_SetString(PyExc_TypeError,
                "bit_length() takes exactly 1 argument");
            return NULL;
        }
        newob = anyint2mpz(PyTuple_GET_ITEM(args, 0));
        if(newob) {
            assert(Pympz_Check(newob));
            if (mpz_size(Pympz_AS_MPZ(newob)))
                i = (long) mpz_sizeinbase(Pympz_AS_MPZ(newob), 2);
            Py_DECREF((PyObject*)newob);
            return Py2or3Int_FromLong(i);
        }
        else {
            //~ fprintf(stderr, "Why?\n");
            //~ SELF_NO_ARG("bit_length", Pympz_convert_arg);
            //~ assert(Pympz_Check(self));
            //~ if (mpz_size(Pympz_AS_MPZ(self)))
                //~ i = (long) mpz_sizeinbase(Pympz_AS_MPZ(self), 2);
            //~ s = Py_BuildValue("l", i);
            //~ Py_DECREF(self);
            //~ return s;
            PyErr_SetString(PyExc_TypeError,
                "unsupported operand type for bit_length: integer required");
            return NULL;
        }
    }
}

/* produce digits for an mpq in requested base, default 10 */
static char doc_qdigitsm[]="\
x.digits([base]): returns Python string representing x in the\n\
given base (2 to 36, default 10 if omitted or 0); leading '-'\n\
is present if x<0, but no leading '+' if x>=0.\n\
";
static char doc_qdigitsg[]="\
qdigits(x[,base]): returns Python string representing x in the\n\
given base (2 to 36, default 10 if omitted or 0); leading '-'\n\
present if x<0, but no leading '+' if x>=0. x must be an mpq,\n\
or else gets coerced into one.\n\
";
static PyObject *
Pympq_digits(PyObject *self, PyObject *args)
{
    int base = 10;
    PyObject *s;

    SELF_MPQ_ONE_ARG("|i",&base);
    assert(Pympq_Check(self));
    s = Pympq_ascii((PympqObject*)self, base, 0);
    Py_DECREF(self);
    return s;
}

/* return scan0/scan1 for an mpz */
static char doc_scan0m[]="\
x.scan0(n=0): returns the bit-index of the first 0-bit of x (that\n\
is at least n); n must be an ordinary Python int, >=0.  If no more\n\
0-bits are in x at or above bit-index n (which can only happen for\n\
x<0, notionally extended with infinite 1-bits), None is returned.\n\
";
static char doc_scan0g[]="\
scan0(x, n=0): returns the bit-index of the first 0-bit of x (that\n\
is at least n); n must be an ordinary Python int, >=0.  If no more\n\
0-bits are in x at or above bit-index n (which can only happen for\n\
x<0, notionally extended with infinite 1-bits), None is returned.\n\
x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_scan0(PyObject *self, PyObject *args)
{
    long starting_bit = 0;
    long maxbit;
    PyObject *s;

    SELF_MPZ_ONE_ARG("|l",&starting_bit);
    assert(Pympz_Check(self));
    if(starting_bit < 0) {
        PyErr_SetString(PyExc_ValueError, "starting bit must be >= 0");
        return NULL;
    }
    maxbit = mpz_sizeinbase(Pympz_AS_MPZ(self), 2);
    if(starting_bit > maxbit) {
        int sig = mpz_sgn(Pympz_AS_MPZ(self));
        if(options.debug)
            fprintf(stderr,"scan0 start=%ld max=%ld sig=%d\n",
                starting_bit, maxbit, sig);

        if(sig<0)
            s = Py_BuildValue("");
        else
            s = Py_BuildValue("l", starting_bit);
    } else {
        s = Py_BuildValue("l", mpz_scan0(Pympz_AS_MPZ(self), starting_bit));
    }
    Py_DECREF(self);
    return s;
}

static char doc_scan1m[]="\
x.scan1(n=0): returns the bit-index of the first 1-bit of x (that\n\
is at least n); n must be an ordinary Python int, >=0.  If no more\n\
1-bits are in x at or above bit-index n (which can only happen for\n\
x>=0, notionally extended with infinite 0-bits), None is returned.\n\
";
static char doc_scan1g[]="\
scan1(x, n=0): returns the bit-index of the first 1-bit of x (that\n\
is at least n); n must be an ordinary Python int, >=0.  If no more\n\
1-bits are in x at or above bit-index n (which can only happen for\n\
x>=0, notionally extended with infinite 0-bits), None is returned.\n\
x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_scan1(PyObject *self, PyObject *args)
{
    long starting_bit = 0;
    long maxbit;
    PyObject *s;

    SELF_MPZ_ONE_ARG("|l",&starting_bit);
    assert(Pympz_Check(self));
    if(starting_bit < 0) {
        PyErr_SetString(PyExc_ValueError, "starting bit must be >= 0");
        return NULL;
    }
    maxbit = mpz_sizeinbase(Pympz_AS_MPZ(self), 2);
    if(starting_bit >= maxbit) {
        int sig = mpz_sgn(Pympz_AS_MPZ(self));
        if(options.debug)
            fprintf(stderr,"scan1 start=%ld max=%ld sig=%d\n",
                starting_bit, maxbit, sig);

        if(sig>=0)
            s = Py_BuildValue("");
        else
            s = Py_BuildValue("l", starting_bit);
    } else {
        s = Py_BuildValue("l", mpz_scan1(Pympz_AS_MPZ(self), starting_bit));
    }
    Py_DECREF(self);
    return s;
}

/* return population-count (# of 1-bits) for an mpz */
static char doc_popcountm[]="\
x.popcount(): returns the number of 1-bits set in x; note that\n\
this is 'infinite' if x<0, and in that case, -1 is returned.\n\
";
static char doc_popcountg[]="\
popcount(x): returns the number of 1-bits set in x; note that\n\
this is 'infinite' if x<0, and in that case, -1 is returned.\n\
x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_popcount(PyObject *self, PyObject *args)
{
    PyObject *s;

    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));
    s = Py_BuildValue("l", mpz_popcount(Pympz_AS_MPZ(self)));
    Py_DECREF(self);
    return s;
}

/* return N lowest bits from an mpz */
static char doc_lowbitsm[]="\
x.lowbits(n): returns the n lowest bits of x; n must be an\n\
ordinary Python int, >0.\n\
";
static char doc_lowbitsg[]="\
lowbits(x,n): returns the n lowest bits of x; n must be an\n\
ordinary Python int, >0; x must be an mpz, or else gets\n\
coerced to one.\n\
";
static PyObject *
Pympz_lowbits(PyObject *self, PyObject *args)
{
    long nbits;
    PympzObject *s;

    SELF_MPZ_ONE_ARG("l",&nbits);
    assert(Pympz_Check(self));
    if(nbits <= 0) {
        PyErr_SetString(PyExc_ValueError, "nbits must be > 0");
        return NULL;
    }
    if(!(s = Pympz_new()))
        return NULL;
    mpz_fdiv_r_2exp(s->z, Pympz_AS_MPZ(self), nbits);
    Py_DECREF(self);
    return (PyObject*)s;
}

/* get & return one bit from an mpz */
static char doc_getbitm[]="\
x.getbit(n): returns 0 or 1, the bit-value of bit n of x;\n\
n must be an ordinary Python int, >=0.\n\
";
static char doc_getbitg[]="\
getbit(x,n): returns 0 or 1, the bit-value of bit n of x;\n\
n must be an ordinary Python int, >=0; x is an mpz, or else\n\
gets coerced to one.\n\
";
static PyObject *
Pympz_getbit(PyObject *self, PyObject *args)
{
    long bit_index;
    PyObject *s;

    SELF_MPZ_ONE_ARG("l",&bit_index);
    assert(Pympz_Check(self));
    if(bit_index < 0) {
        PyErr_SetString(PyExc_ValueError, "bit_index must be >= 0");
        return NULL;
    }
    s = Py_BuildValue("i", mpz_tstbit(Pympz_AS_MPZ(self), bit_index));
    Py_DECREF(self);
    return s;
}

static char doc_setbitm[]="\
x.setbit(n,v=1): returns a copy of the value of x, with bit n set\n\
to value v; n must be an ordinary Python int, >=0; v, 0 or !=0.\n\
";
static char doc_setbitg[]="\
setbit(x,n,v=1): returns a copy of the value of x, with bit n set\n\
to value v; n must be an ordinary Python int, >=0; v, 0 or !=0;\n\
x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_setbit(PyObject *self, PyObject *args)
{
    long bit_index;
    long bit_value=1;
    PympzObject *s;

    if(self && Pympz_Check(self)) {
        if(!PyArg_ParseTuple(args, "l|l", &bit_index, &bit_value))
            return NULL;
        Py_INCREF(self);
    } else {
        if(!PyArg_ParseTuple(args, "O&l|l", Pympz_convert_arg, &self,
                    &bit_index, &bit_value))
            return NULL;
    }
    assert(Pympz_Check(self));
    if(bit_index < 0) {
        PyErr_SetString(PyExc_ValueError, "bit_index must be >= 0");
        return NULL;
    }
    if(!(s = mpz2mpz((PympzObject*)self)))
        return NULL;
    Py_DECREF(self);
    if(bit_value) {
        mpz_setbit(s->z, bit_index);
    } else {
        mpz_clrbit(s->z, bit_index);
    }
    return (PyObject*)s;
}


/* return nth-root of an mpz (in a 2-el tuple: 2nd is int, non-0 iff exact) */
static char doc_rootm[]="\
x.root(n): returns a 2-element tuple (y,m), such that y is the\n\
(possibly truncated) n-th root of x; m, an ordinary Python int,\n\
is 1 if the root is exact (x==y**n), else 0.  n must be an ordinary\n\
Python int, >=0.\n\
";
static char doc_rootg[]="\
root(x,n): returns a 2-element tuple (y,m), such that y is the\n\
(possibly truncated) n-th root of x; m, an ordinary Python int,\n\
is 1 if the root is exact (x==y**n), else 0.  n must be an ordinary\n\
Python int, >=0. x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_root(PyObject *self, PyObject *args)
{
    long n;
    int exact;
    PympzObject *s;

    SELF_MPZ_ONE_ARG("l",&n);
    assert(Pympz_Check(self));
    if(n <= 0) {
        PyErr_SetString(PyExc_ValueError, "n must be > 0");
        return NULL;
    } else if(n>1) {
        int sign = mpz_sgn(Pympz_AS_MPZ(self));
        if(sign<0) {
            PyErr_SetString(PyExc_ValueError, "root of negative number");
            return NULL;
        }
    }
    if(!(s = Pympz_new()))
        return NULL;
    exact = mpz_root(s->z, Pympz_AS_MPZ(self), n);
    Py_DECREF(self);
    return Py_BuildValue("(Ni)", s, exact);
}

/* produce string for an mpf with requested/defaulted parameters */
static char doc_fdigitsm[]="\
x.digits(base=10, digs=0, mine=0, maxe=-1, opts=0): formats x.\n\
\n\
Returns up to digs digits in the given base (if digs is 0, as many\n\
digits as are available), but no more than available given x's\n\
precision; the resulting string is formatted in fixed point\n\
if the exponent is >=mine and <=maxe, else in exponential (the\n\
exponent-separator is 'e' for base up to 10, else '@' -- the\n\
exponent is always output as a signed, base-10 integer). If opts\n\
has bit 1 set, the whole is wrapped in 'gmpy.mpf(...)', to ease\n\
later approximate reconstruction via builtin function eval\n\
(Or, in just mpf(...) if gmpy.set_tagoff(1) was called).\n\
\n\
If opts has bit 2 set, then opts bit 1, mine, and maxe, are\n\
ignored; the result is then a 2-element tuple, first element\n\
the raw string of base-digits without formatting, second the\n\
exponent in base as a Python int.\n\
";

static char doc_fdigitsg[]="\
fdigits(x, base=10, digs=0, mine=0, maxe=-1, opts=0): formats x,\n\
which is an mpf or else gets coerced to one.\n\
\n\
Returns up to digs digits in the given base (if digs is 0, as many\n\
digits as are available), but no more than available given x's\n\
precision; the resulting string is formatted in fixed point\n\
if the exponent is >=mine and <=maxe, else in exponential (the\n\
exponent-separator is 'e' for base up to 10, else '@' -- the\n\
exponent is always output as a signed, base-10 integer). If opts\n\
has bit 1 set, the whole is wrapped in 'gmpy.mpf(...)', to ease\n\
later approximate reconstruction via builtin function eval\n\
(Or, in just mpf(...) if gmpy.set_tagoff(1) was called).\n\
\n\
If opts has bit 2 set, then opts bit 1, mine, and maxe, are\n\
ignored; the result is then a 2-element tuple, first element\n\
the raw string of base-digits without formatting, second the\n\
exponent in base as a Python int.\n\
";

static PyObject *
Pympf_digits(PyObject *self, PyObject *args)
{
    int base = 10;
    int digs = 0;
    int mine = 0;
    int maxe = -1;
    int opts = 0;
    PyObject *s;

    if(self && Pympf_Check(self)) {
        if(!PyArg_ParseTuple(args, "|iiiii", &base, &digs, &mine, &maxe, &opts))
            return NULL;
        Py_INCREF(self);
    } else {
        if(!PyArg_ParseTuple(args, "O&|iiiii", Pympf_convert_arg, &self, &base,
                &digs, &mine, &maxe, &opts))
            return NULL;
    }
    assert(Pympf_Check(self));
    s = Pympf_ascii( (PympfObject *) self, base, digs, mine, maxe, opts);
    Py_DECREF(self);
    return s;
}

static char doc_signm[]="\
x.sign(): returns -1, 0, or +1, if x is negative, 0, positive.\n\
";
static char doc_signg[]="\
sign(x): returns -1, 0, or +1, if x is negative, 0, positive;\n\
x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_sign(PyObject *self, PyObject *args)
{
    PyObject *s;

    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));
    s = Py_BuildValue("i", mpz_sgn(Pympz_AS_MPZ(self)));
    Py_DECREF(self);
    return s;
}

static char doc_qsignm[]="\
x.sign(): returns -1, 0, or +1, if x is negative, 0, positive.\n\
";
static char doc_qsigng[]="\
qsign(x): returns -1, 0, or +1, if x is negative, 0, positive;\n\
x must be an mpq, or else gets coerced to one.\n\
";
static PyObject *
Pympq_sign(PyObject *self, PyObject *args)
{
    PyObject *s;

    SELF_MPQ_NO_ARG;
    assert(Pympq_Check(self));
    s = Py_BuildValue("i", mpq_sgn(Pympq_AS_MPQ(self)));
    Py_DECREF(self);
    return s;
}

static char doc_numerm[]="\
x.numer(): returns numerator of x.\n\
";
static char doc_numerg[]="\
numer(x): returns numerator of x;\n\
x must be an mpq, or else gets coerced to one.\n\
";
static PyObject *
Pympq_numer(PyObject *self, PyObject *args)
{
    PympzObject *s;

    if(!(s = Pympz_new()))
        return NULL;

    SELF_MPQ_NO_ARG;
    assert(Pympq_Check(self));

    mpz_set(s->z, mpq_numref(Pympq_AS_MPQ(self)));

    Py_DECREF(self);
    return (PyObject*)s;
}

static char doc_denomm[]="\
x.denom(): returns denominator of x.\n\
";
static char doc_denomg[]="\
denom(x): returns denominator of x;\n\
x must be an mpq, or else gets coerced to one.\n\
";
static PyObject *
Pympq_denom(PyObject *self, PyObject *args)
{
    PympzObject *s;

    if(!(s = Pympz_new()))
        return NULL;

    SELF_MPQ_NO_ARG;
    assert(Pympq_Check(self));

    mpz_set(s->z, mpq_denref(Pympq_AS_MPQ(self)));

    Py_DECREF(self);
    return (PyObject*)s;
}

static char doc_qdivm[]="\
x.qdiv(y=1): returns x/y as mpz if possible, or as mpq\n\
if x is not exactly divisible by y.\n\
";
static char doc_qdivg[]="\
qdiv(x,y=1): returns x/y as mpz if possible, or as mpq\n\
if x is not exactly divisible by y.\n\
";
static int isOne(PyObject* obj)
{
    if(!obj) return 1;

    if(Pympq_Check(obj)) {
        return (0==mpz_cmp_ui(mpq_denref(Pympq_AS_MPQ(obj)),1)) &&
               (0==mpz_cmp_ui(mpq_numref(Pympq_AS_MPQ(obj)),1));
    } else if(Pympz_Check(obj)) {
        return 0==mpz_cmp_ui(Pympz_AS_MPZ(obj),1);
#if PY_MAJOR_VERSION < 3
    } else if(PyInt_Check(obj)) {
        return PyInt_AS_LONG(obj)==1;
#endif
    } else if(Pympf_Check(obj)) {
        return mpf_get_d(Pympf_AS_MPF(obj))==1.0;
    } else if(PyFloat_Check(obj)) {
        return PyFloat_AS_DOUBLE(obj)==1.0;
    } else if (PyLong_Check(obj)) {
        return PyLong_AsLong(obj)==1;
    }
    return 0;
}
static PyObject *
Pympq_qdiv(PyObject *self, PyObject *args)
{
    PyObject *other = 0;
    PyObject *s = 0;
    int wasone;

    if(self && Pympq_Check(self)) {
        if(!PyArg_ParseTuple(args, "|O", &other))
            return NULL;
    } else {
        if(!PyArg_ParseTuple(args, "O|O", &self, &other))
            return NULL;
    }
    wasone = isOne(other);
    /* optimize if self must be returned unchanged */
    if(Pympq_Check(self) && wasone) {
        /* optimize if self is mpq and result must==self */
        if(mpz_cmp_ui(mpq_denref(Pympq_AS_MPQ(self)), 1) != 0) {
            Py_INCREF(self);
            return self;
        } else {
            /* denominator is 1, optimize returning an mpz */
            s = (PyObject*)Pympz_new();
            mpz_set(Pympz_AS_MPZ(s), mpq_numref(Pympq_AS_MPQ(self)));
            return s;
        }
    } else if(Pympz_Check(self) && wasone) {
        /* optimize if self is mpz and result must==self */
        Py_INCREF(self);
        return self;
    }
    /* normal, non-optimized case: must make new object as result */
    self = (PyObject*)anyrational2mpq(self);
    if(!self) {
        if(!PyErr_Occurred()) {
            PyErr_SetString(PyExc_TypeError,
                "first argument to qdiv not a number");
        }
        return NULL;
    }
    if(wasone) { /* self was mpf, float, int, long... */
        s = self;
    } else {     /* other explicitly present and !=1... must compute */
        other = (PyObject*)anyrational2mpq(other);
        if(!other) {
            Py_DECREF(self);
            if(!PyErr_Occurred()) {
                PyErr_SetString(PyExc_TypeError,
                    "second argument to qdiv not a number");
            }
            return NULL;
        }
        if(mpq_sgn(Pympq_AS_MPQ(other))==0) {
            PyObject* result = 0;
            PyErr_SetString(PyExc_ZeroDivisionError,"qdiv: zero divisor");
            Py_DECREF(self);
            Py_DECREF(other);
            return result;
        }
        s = (PyObject*)Pympq_new();
        mpq_div(Pympq_AS_MPQ(s), Pympq_AS_MPQ(self), Pympq_AS_MPQ(other));
        Py_DECREF(self);
        Py_DECREF(other);
    }
    if(mpz_cmp_ui(mpq_denref(Pympq_AS_MPQ(s)), 1) != 0) {
        return s;
    } else {
        /* denominator is 1, return an mpz */
        PyObject* ss = (PyObject*)Pympz_new();
        if(ss) mpz_set(Pympz_AS_MPZ(ss), mpq_numref(Pympq_AS_MPQ(s)));
        Py_DECREF(s);
        return ss;
    }
}

static char doc_f2qm[]="\
x.f2q([err]): returns the 'best' mpq approximating x to\n\
within relative error err (default, x's precision); 'best'\n\
rationals as per Stern-Brocot tree; mpz if denom is 1.\n\
If err<0, error sought is 2.0 ** err.\n\
";
static char doc_f2qg[]="\
f2q(x[,err]): returns the 'best' mpq approximating x to\n\
within relative error err (default, x's precision); 'best'\n\
rationals as per Stern-Brocot tree; mpz if denom is 1.\n\
If err<0, error sought is 2.0 ** err.\n\
";
static PyObject *
Pympf_f2q(PyObject *self, PyObject *args)
{
    PympfObject *err = 0;
    PympfObject *fself;

    if(options.debug)
        fprintf(stderr, "Pympf_f2q: %p, %p\n", self, args);

    SELF_MPF_ONE_ARG_CONVERTED_OPT(&err);
    assert(Pympf_Check(self));
    fself = (PympfObject*)self;

    return f2q_internal(fself, err, fself->rebits, args!=0);
}

static PyObject *
f2q_internal(PympfObject* self, PympfObject* err, unsigned int bits, int mayz)
{
    PympqObject *res = 0;
    int i, negative, errsign;
    mpf_t f, al, a, r1[3], r2[3], minerr, curerr, newerr, temp;

    assert(!err || Pympf_Check(err));
    errsign = err?mpf_sgn(err->f):0;
    if(errsign == 0) {
        if(err) { Py_DECREF((PyObject*)err); }
        if(!(err = Pympf_new(20))) {
            Py_DECREF((PyObject*)self);
            return NULL;
        }
        mpf_set_si(err->f, 1);
        mpf_div_2exp(err->f, err->f, bits);
    } else if(errsign < 0) {
        int ubits;
        mpf_floor(err->f, err->f);
        ubits = (int)mpf_get_d(err->f);
        mpf_set_si(err->f, 1);
        mpf_div_2exp(err->f, err->f, -ubits);
    }
    if(!(res = Pympq_new())) return NULL;
    mpf_init2(minerr, 20); mpf_set(minerr, err->f);
    Py_DECREF((PyObject*)err);

    mpf_init2(f, bits);
    if(mpf_sgn(self->f)<0) {
        negative = 1;
        mpf_abs(f, self->f);
    } else {
        negative = 0;
        mpf_set(f, self->f);
    }
    Py_DECREF((PyObject*)self);
    mpf_init2(al, bits);
    mpf_set(al, f);
    mpf_init2(a, bits); mpf_floor(a, al);
    mpf_init2(temp, bits);
    for(i=0; i<3; ++i) {
        mpf_init2(r1[i], bits);
        mpf_init2(r2[i], bits);
    }
    mpf_set_si(r1[0], 0); mpf_set_si(r1[1], 0); mpf_set_si(r1[2], 1);
    mpf_set_si(r2[0], 0); mpf_set_si(r2[1], 1); mpf_set(r2[2], a);
    mpf_init2(curerr, 20); mpf_init2(newerr, 20);
    mpf_reldiff(curerr, f, a);
    while(mpf_cmp(curerr, minerr) > 0) {
        mpf_sub(temp, al, a);
        mpf_ui_div(al, 1, temp);
        mpf_floor(a, al);
        mpf_swap(r1[0], r1[1]); mpf_swap(r1[1], r1[2]);
        mpf_mul(r1[2], r1[1], a); mpf_add(r1[2], r1[2], r1[0]);
        mpf_swap(r2[0], r2[1]); mpf_swap(r2[1], r2[2]);
        mpf_mul(r2[2], r2[1], a); mpf_add(r2[2], r2[2], r2[0]);
        mpf_div(temp, r2[2], r1[2]);
        mpf_reldiff(newerr, f, temp);
        if(mpf_cmp(curerr, newerr) <= 0) {
            mpf_swap(r1[1],r1[2]);
            mpf_swap(r2[1],r2[2]);
            break;
        }
        mpf_swap(curerr, newerr);
    }
    if(mayz && (mpf_cmp_ui(r1[2],1)==0)) {
        Py_DECREF((PyObject*)res);
        res = (PympqObject*)Pympz_new();
        mpz_set_f(Pympz_AS_MPZ(res), r2[2]);
        if(negative)
            mpz_neg(Pympz_AS_MPZ(res),Pympz_AS_MPZ(res));
    } else {
        mpz_set_f(mpq_numref(res->q), r2[2]);
        mpz_set_f(mpq_denref(res->q), r1[2]);
        if(negative)
            mpz_neg(mpq_numref(res->q),mpq_numref(res->q));
    }
    mpf_clear(minerr); mpf_clear(al); mpf_clear(a); mpf_clear(f);
    for(i=0; i<3; ++i) {
        mpf_clear(r1[i]);
        mpf_clear(r2[i]);
    }
    mpf_clear(curerr); mpf_clear(newerr); mpf_clear(temp);
    return (PyObject*)res;
}

/* CONSTRUCTORS */
static char doc_mpz[] = "\
mpz(n): builds an mpz object with a numeric value n (truncating n\n\
        to its integer part if it's a float or mpf)\n\
mpz(s,base=10): builds an mpz object from a string s made up of\n\
        digits in the given base.  If base=0, hex and oct Python\n\
        strings may also be interpreted (started with '0x' and '0'\n\
        respectively), as well as decimal.  If base=256, s must be\n\
        a gmpy.mpz portable binary representation as built by the\n\
        function gmpy.binary (and the .binary method of mpz objects).\n\
";
static PyObject *
Pygmpy_mpz(PyObject *self, PyObject *args)
{
    PympzObject *newob;
    PyObject *obj;
    Py_ssize_t argc;

    if(options.debug)
        fputs("Pygmpy_mpz() called...\n", stderr);

    assert(PyTuple_Check(args));

    argc = PyTuple_Size(args);
    if((argc < 1) || (argc > 2)) {
        PyErr_SetString(PyExc_TypeError,
            "gmpy.mpz() requires 1 or 2 arguments");
        return NULL;
    }

    obj = PyTuple_GetItem(args, 0);
#if PY_MAJOR_VERSION >= 3
    if(PyBytes_Check(obj) || PyUnicode_Check(obj)) {
#else
    if(PyString_Check(obj)) {
#endif
        /* build-from-string (ascii or binary) */
        long base=10;
        if(argc == 2) {
            PyObject *pbase = PyTuple_GetItem(args, 1);
            if(!Py2or3Int_Check(pbase)) {
                PyErr_SetString(PyExc_TypeError,
                    "gmpy.mpz(): base must be an integer");
                return NULL;
            }
            base = Py2or3Int_AsLong(pbase);
            if((base!=0) && (base!=256) && ((base<2)||(base>36))) {
                PyErr_SetString(PyExc_ValueError,
                    "base for gmpy.mpz must be 0, 256, or in the "
                    "interval 2 ... 36 .");
                return NULL;
            }
        }
        newob = str2mpz(obj, base);
        if (!newob) {
            return NULL;
        }
    } else {
        if(argc==2) {
            PyErr_SetString(PyExc_TypeError,
                "gmpy.mpz() with numeric argument needs exactly 1 argument");
            return NULL;
        }
        newob = anynum2mpz(obj);
        if(!newob) {
            if (!PyErr_Occurred()) {
                PyErr_SetString(PyExc_TypeError,
                    "gmpy.mpz() expects numeric or string argument");
            }
            return NULL;
        }
    }
    if(options.debug)
        fprintf(stderr, "Pygmpy_mpz: created mpz = %ld\n",
                mpz_get_si(newob->z));


    return (PyObject *) newob;
}

#define FORWARD_MPQ_BINOP(NAME) \
static PyObject * \
Py##NAME(PyObject *a, PyObject *b)

FORWARD_MPQ_BINOP(mpq_div);

static char doc_mpq[] = "\
mpq(n): builds an mpq object with a numeric value n\n\
mpq(n,m): builds an mpq object with a numeric value n/m\n\
mpq(s,base=10): builds an mpq object from a string s made up of\n\
        digits in the given base.  s may be made up of two\n\
        numbers in the same base separated by a '/' character.\n\
        If base=256, s must be a gmpy.mpq portable binary\n\
        representation as built by the gmpy.qbinary (and the\n\
        .binary method of mpq objects).\n\
";
static PyObject *
Pygmpy_mpq(PyObject *self, PyObject *args)
{
    PympqObject *newob;
    PyObject *obj;
    int wasnumeric;
    int argc;

    if(options.debug)
        fputs("Pygmpy_mpq() called...\n", stderr);

    assert(PyTuple_Check(args));

    argc = PyTuple_Size(args);
    if((argc < 1) || (argc > 2)) {
        PyErr_SetString(PyExc_TypeError,
            "gmpy.mpq() requires 1 or 2 arguments");
        return NULL;
    }

    obj = PyTuple_GetItem(args, 0);
#if PY_MAJOR_VERSION >= 3
    if(PyBytes_Check(obj) || PyUnicode_Check(obj)) {
#else
    if(PyString_Check(obj)) {
#endif
        /* build-from-string (ascii or binary) */
        long base=10;
        wasnumeric=0;
        if(argc == 2) {
            PyObject *pbase = PyTuple_GetItem(args, 1);
            if(!Py2or3Int_Check(pbase)) {
                PyErr_SetString(PyExc_TypeError,
                    "gmpy.mpq(): base must be an integer");
                return NULL;
            }
            base = Py2or3Int_AsLong(pbase);
            if((base!=0) && (base!=256) && ((base<2)||(base>36))) {
                PyErr_SetString(PyExc_ValueError,
                    "base for gmpy.mpq() must be 0, 256, or in the "
                    "interval 2 ... 36 .");
                return NULL;
            }
        }
        newob = str2mpq(obj, base);
        if (!newob) {
            return NULL;
        }
    } else {
        wasnumeric=1;
        newob = anynum2mpq(obj);
        if(!newob) {
            if(!PyErr_Occurred()) {
                PyErr_SetString(PyExc_TypeError,
                    "gmpy.mpq() expects numeric or string argument");
            }
            return NULL;
        }
    }
    if(options.debug) {
        fputs("Pygmpy_mpq: created mpq = ", stderr);
        mpq_out_str(stderr, 10, newob->q);
        putc('\n', stderr);
    }
    if(wasnumeric && argc==2) {
        PyObject *secondarg = PyTuple_GetItem(args, 1);
        PyObject *denominator;
        PyObject *result;
        if(!Pympq_convert_arg(secondarg,&denominator)) {
            Py_DECREF((PyObject*)newob);
            return 0;
        }
        if(0==mpq_sgn(Pympq_AS_MPQ(denominator))) {
            PyObject* result = 0;
            PyErr_SetString(PyExc_ZeroDivisionError,"mpq: zero denominator");
            Py_DECREF((PyObject *) newob);
            Py_DECREF(denominator);
            return result;
        }
        result = Pympq_div((PyObject*)newob,(PyObject*)denominator);
        Py_DECREF((PyObject*)newob);
        Py_DECREF(denominator);
        newob = (PympqObject*)result;
    }

    return (PyObject *) newob;
}

static char doc_mpf[] = "\
mpf(n): builds an mpf object with a numeric value n (n may be any\n\
        Python number, or an mpz, mpq, or mpf object) and a default\n\
        precision (in bits) depending on the nature of n\n\
mpf(n,bits=0): as above, but with the specified number of bits (0\n\
        means to use default precision, as above)\n\
mpf(s,bits=0,base=10): builds an mpf object from a string s made up of\n\
        digits in the given base, possibly with fraction-part (with\n\
        period as a separator) and/or exponent-part (with exponent\n\
        marker 'e' for base<=10, else '@'). If base=256, s must be\n\
        a gmpy.mpf portable binary representation as built by the\n\
        function gmpy.fbinary (and the .binary method of mpf objects).\n\
        The resulting mpf object is built with a default precision (in\n\
        bits) if bits is 0 or absent, else with the specified number\n\
        of bits.\n\
";
static PyObject *
Pygmpy_mpf(PyObject *self, PyObject *args)
{
    PympfObject *newob;
    PyObject *obj;
    int argc;
    unsigned int bits=0;

    if(options.debug)
        fputs("Pygmpy_mpf() called...\n", stderr);

    assert(PyTuple_Check(args));

    argc = PyTuple_Size(args);
    if((argc < 1) || (argc > 3)) {
        PyErr_SetString(PyExc_TypeError,
            "gmpy.mpf() requires 1 to 3 arguments");
        return NULL;
    }

    obj = PyTuple_GetItem(args, 0);

    if(2 <= argc) {
        int sbits;
        PyObject *pbits = PyTuple_GetItem(args, 1);
        if(!Py2or3Int_Check(pbits)) {
            PyErr_SetString(PyExc_TypeError,
                "gmpy.mpf(): bits must be an integer");
            return NULL;
        }
        sbits = Py2or3Int_AsLong(pbits);
        if(sbits<0) {
            PyErr_SetString(PyExc_ValueError,
                "bits for gmpy.mpf must be >= 0");
            return NULL;
        }
        bits = sbits;
    }

#if PY_MAJOR_VERSION >= 3
    if(PyBytes_Check(obj) || PyUnicode_Check(obj)) {
#else
    if(PyString_Check(obj)) {
#endif
        /* build-from-string (ascii or binary) */
        long base=10;
        if(3 == argc) {
            PyObject *pbase = PyTuple_GetItem(args, 2);
            if(!Py2or3Int_Check(pbase)) {
                PyErr_SetString(PyExc_TypeError,
                    "gmpy.mpf(): base must be an integer");
                return NULL;
            }
            base = Py2or3Int_AsLong(pbase);
            if((base!=0) && (base!=256) && ((base<2)||(base>36))) {
                PyErr_SetString(PyExc_ValueError,
                    "base for gmpy.mpf must be 0, 256, or in the "
                    "interval 2 ... 36 .");
                return NULL;
            }
        }
        newob = str2mpf(obj, base, bits);
        if (!newob) {
            return NULL;
        }
    } else {
        if(argc==3) {
            PyErr_SetString(PyExc_TypeError,
                "gmpy.mpf() with numeric 1st argument needs 1 or 2 arguments");
            return NULL;
        }
        newob = anynum2mpf(obj, bits);
        if(!newob) {
            if(!PyErr_Occurred())
                PyErr_SetString(PyExc_TypeError,
                    "gmpy.mpf() expects numeric or string argument");
            return NULL;
        }
    }
    if(options.debug) {
        fputs("Pygmpy_mpf: created mpf = ", stderr);
        mpf_out_str(stderr, 10, 0, newob->f);
        fprintf(stderr," bits=%d (%d)\n", newob->rebits, bits);
    }

    return (PyObject *) newob;
} /* Pygmpy_mpf() */

/* ARITHMETIC */

#define MPZ_BINOP(NAME) \
static PyObject * \
Py##NAME(PyObject *a, PyObject *b) \
{ \
  PympzObject *r; \
  PympzObject *pa = 0; \
  PympzObject *pb = 0; \
  pa = anyint2mpz(a); \
  pb = anyint2mpz(b); \
  if(!pa || !pb) { \
    PyObject *r = Py_NotImplemented; \
    Py_XDECREF(pa); \
    Py_XDECREF(pb); \
    Py_INCREF(r); \
    return r; \
  } \
  if (options.debug) fprintf(stderr, "Py" #NAME ": %p, %p\n", pa, pb); \
  if (!(r = Pympz_new())) { \
    Py_DECREF((PyObject*)pa); \
    Py_DECREF((PyObject*)pb); \
    return NULL; \
  } \
  NAME(r->z, pa->z, pb->z); \
  Py_DECREF((PyObject*)pa); \
  Py_DECREF((PyObject*)pb); \
  if (options.debug) fprintf(stderr, "Py" #NAME "-> %p\n", r); \
  return (PyObject *) r; \
}

#define MPF_BINOP(NAME) \
static PyObject * \
Py##NAME(PyObject *a, PyObject *b) \
{ \
  unsigned int bits, bbits; \
  PympfObject *r; \
  PympfObject *pa = 0; \
  PympfObject *pb = 0; \
  if(Pympf_Check(a) && Pympf_Check(b)) { \
    bits = ((PympfObject*)a)->rebits; \
    bbits = ((PympfObject*)b)->rebits; \
    if(bits>bbits) bits=bbits; \
    if (!(r = Pympf_new(bits))) { \
      return NULL; \
    } \
    NAME(r->f, ((PympfObject*)a)->f, ((PympfObject*)b)->f); \
    if (options.debug) fprintf(stderr, "Py" #NAME "-> %p", r); \
    mpf_normalize(r); \
    return (PyObject *) r; \
  } else { \
    if(Pympf_Check(a)) { \
      bits = ((PympfObject*)a)->rebits; \
    } else { \
      bits = ((PympfObject*)b)->rebits; \
    } \
    pa = anyreal2mpf(a, bits); \
    pb = anyreal2mpf(b, bits); \
    if(!pa || !pb) { \
      PyObject *r = Py_NotImplemented; \
      Py_XDECREF(pa); \
      Py_XDECREF(pb); \
      Py_INCREF(r); \
      return r; \
    } \
    if (options.debug) fprintf(stderr, "Py" #NAME ": %p, %p", pa, pb); \
    if (!(r = Pympf_new(bits))) { \
      Py_DECREF((PyObject*)pa); \
      Py_DECREF((PyObject*)pb); \
      return NULL; \
    } \
    NAME(r->f, pa->f, pb->f); \
    Py_DECREF((PyObject*)pa); \
    Py_DECREF((PyObject*)pb); \
    if (options.debug) fprintf(stderr, "Py" #NAME "-> %p", r); \
    mpf_normalize(r); \
    return (PyObject *) r; \
  } \
}

#define MPQ_BINOP(NAME) \
static PyObject * \
Py##NAME(PyObject *a, PyObject *b) \
{ \
  PympqObject *r; \
  PympqObject *pa = 0; \
  PympqObject *pb = 0; \
  pa = anyrational2mpq(a); \
  pb = anyrational2mpq(b); \
  if(!pa || !pb) { \
    PyObject *r = Py_NotImplemented; \
    Py_XDECREF(pa); \
    Py_XDECREF(pb); \
    Py_INCREF(r); \
    return r; \
  } \
  if (options.debug) fprintf(stderr, "Py" #NAME ": %p, %p", pa, pb); \
  if (!(r = Pympq_new())) { \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
    return NULL; \
  } \
  NAME(r->q, pa->q, pb->q); \
  Py_DECREF(pa); \
  Py_DECREF(pb); \
  if (options.debug) fprintf(stderr, "Py" #NAME "-> %p", r); \
  return (PyObject *) r; \
}

MPZ_BINOP(mpz_add)
MPZ_BINOP(mpz_sub)
MPZ_BINOP(mpz_mul)

MPF_BINOP(mpf_add)
MPF_BINOP(mpf_sub)
MPF_BINOP(mpf_mul)

MPQ_BINOP(mpq_add)
MPQ_BINOP(mpq_sub)
MPQ_BINOP(mpq_mul)

MPF_BINOP(mpf_reldiff)

#define MPZ_DIVOP(NAME, OP) \
static PyObject * \
NAME(PyObject *a, PyObject *b) \
{ \
  PympzObject *r; \
  PympzObject *pa = 0; \
  PympzObject *pb = 0; \
  pa = anyint2mpz(a); \
  pb = anyint2mpz(b); \
  if(!pa || !pb) { \
    PyObject *r = Py_NotImplemented; \
    Py_XDECREF(pa); \
    Py_XDECREF(pb); \
    Py_INCREF(r); \
    return r; \
  } \
  if (options.debug) fprintf(stderr, #NAME ": %p, %p", pa, pb); \
  if  (mpz_sgn(pb->z) == 0) { \
    PyErr_SetString(PyExc_ZeroDivisionError, #NAME " by zero"); \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
    return NULL; \
  } \
  if (!(r = Pympz_new())) { \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
    return NULL; \
  } \
  OP(r->z, pa->z, pb->z); \
  Py_DECREF(pa); \
  Py_DECREF(pb); \
  if (options.debug) fprintf(stderr, #NAME "-> %p", r); \
  return (PyObject *) r; \
}

#define MPQ_DIVOP(NAME, OP) \
static PyObject * \
NAME(PyObject *a, PyObject *b) \
{ \
  PympqObject *r; \
  PympqObject *pa = 0; \
  PympqObject *pb = 0; \
  pa = anyrational2mpq(a); \
  pb = anyrational2mpq(b); \
  if(!pa || !pb) { \
    PyObject *r = Py_NotImplemented; \
    Py_XDECREF(pa); \
    Py_XDECREF(pb); \
    Py_INCREF(r); \
    return r; \
  } \
  if (options.debug) fprintf(stderr, #NAME ": %p, %p", pa, pb); \
  if (mpq_sgn(pb->q) == 0) { \
    PyErr_SetString(PyExc_ZeroDivisionError, #NAME " by zero"); \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
    return NULL; \
  } \
  if (!(r = Pympq_new())) { \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
    return NULL; \
  } \
  OP(r->q, pa->q, pb->q); \
  Py_DECREF(pa); \
  Py_DECREF(pb); \
  if (options.debug) fprintf(stderr, #NAME "-> %p", r); \
  return (PyObject *) r; \
}

#define MPF_DIVOP(NAME, OP) \
static PyObject * \
NAME(PyObject *a, PyObject *b) \
{ \
  unsigned int bits, bbits; \
  PympfObject *r; \
  PympfObject *pa = 0; \
  PympfObject *pb = 0; \
  pa = anyreal2mpf(a, 0); \
  pb = anyreal2mpf(b, 0); \
  if(!pa || !pb) { \
    PyObject *r = Py_NotImplemented; \
    Py_XDECREF(pa); \
    Py_XDECREF(pb); \
    Py_INCREF(r); \
    return r; \
  } \
  if (options.debug) fprintf(stderr, #NAME ": %p, %p", pa, pb); \
  if (mpf_sgn(pb->f) == 0) { \
    PyErr_SetString(PyExc_ZeroDivisionError, #NAME " by zero"); \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
    return NULL; \
  } \
  bits = pa->rebits; \
  bbits = pb->rebits; \
  if(bits>bbits) bits=bbits; \
  if (!(r = Pympf_new(bits))) { \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
    return NULL; \
  } \
  OP(r->f, pa->f, pb->f); \
  Py_DECREF(pa); \
  Py_DECREF(pb); \
  if (options.debug) fprintf(stderr, #NAME "-> %p", r); \
  return (PyObject *) r; \
}

MPZ_DIVOP(Pympz_div, mpz_fdiv_q)
MPZ_DIVOP(Pympz_rem, mpz_fdiv_r)
MPQ_DIVOP(Pympq_div, mpq_div)
MPF_DIVOP(Pympf_div, mpf_div)

static PyObject *
Pympany_truediv(PyObject *a, PyObject *b)
{
    PympfObject *pa = anyreal2mpf(a, 0);
    PympfObject *pb = anyreal2mpf(b, 0);
    PyObject *result;
    if (!pa || !pb) {
        PyObject *result = Py_NotImplemented;
        Py_XDECREF(pa);
        Py_XDECREF(pb);
        Py_INCREF(result);
        return result;
    }
    result = Pympf_div((PyObject*)pa, (PyObject*)pb);
    Py_DECREF((PyObject*)pa);
    Py_DECREF((PyObject*)pb);
    return result;
}

static PyObject *
Pympq_floordiv(PyObject *a, PyObject *b)
{
    PympfObject *pa = anyrational2mpf(a, 0);
    PympfObject *pb = anyrational2mpf(b, 0);
    PyObject *result;
    if (!pa || !pb) {
        PyObject *result = Py_NotImplemented;
        Py_XDECREF(pa);
        Py_XDECREF(pb);
        Py_INCREF(result);
        return result;
    }
    result = Pympf_div((PyObject*)pa, (PyObject*)pb);
    Py_DECREF((PyObject*)pa);
    Py_DECREF((PyObject*)pb);
    if(result) {
        result = (PyObject*) anyint2mpz(result);
        if (result)
            result = (PyObject*) anyrational2mpq(result);
    }
    return result;
}

static PyObject *
Pympf_floordiv(PyObject *a, PyObject *b)
{
    PympfObject *pa = anyreal2mpf(a, 0);
    PympfObject *pb = anyreal2mpf(b, 0);
    PyObject *result;
    if (!pa || !pb) {
        PyObject *result = Py_NotImplemented;
        Py_XDECREF(pa);
        Py_XDECREF(pb);
        Py_INCREF(result);
        return result;
    }
    result = Pympf_div((PyObject*)pa, (PyObject*)pb);
    Py_DECREF((PyObject*)pa);
    Py_DECREF((PyObject*)pb);
    if(result) {
        result = (PyObject*) anyint2mpz(result);
        if (result)
            result = (PyObject*) anyreal2mpf(result, 0);
    }
    return result;
}

static PyObject *
Pympz_divmod(PyObject *a, PyObject *b)
{
    PympzObject *q, *r;
    PympzObject *pa = 0;
    PympzObject *pb = 0;
    pa = anyint2mpz(a);
    pb = anyint2mpz(b);
    if(!pa || !pb) {
        PyObject *r = Py_NotImplemented;
        Py_XDECREF(pa);
        Py_XDECREF(pb);
        Py_INCREF(r);
        return r;
    }
    if (options.debug)
        fprintf(stderr, "Pympz_divmod: %p, %p\n", pa, pb);
    if (mpz_sgn(pb->z) == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError,"mpz.divmod by zero");
        return NULL;
    }
    if (!(q = Pympz_new()))
        return NULL;
    if (!(r = Pympz_new())) {
        Pympz_dealloc(q);
        return NULL;
    }
    mpz_fdiv_qr(q->z, r->z, pa->z, pb->z);
    Py_DECREF(pa);
    Py_DECREF(pb);
    if (options.debug)
        fprintf(stderr, "Pympz_divmod -> %p, %p\n", q, r);
    return Py_BuildValue("(NN)", q, r);
}

#define MPZ_MONOP(NAME) \
static PyObject * \
Py##NAME(PympzObject *x) \
{ \
  PympzObject *r; \
  if (options.debug) fprintf(stderr, "Py" #NAME ": %p\n", x); \
  if (!(r = Pympz_new())) return NULL; \
  NAME(r->z, x->z); \
  if (options.debug) fprintf(stderr, "Py" #NAME "-> %p\n", r); \
  return (PyObject *) r; \
}

#define MPF_MONOP(NAME) \
static PyObject * \
Py##NAME(PympfObject *x) \
{ \
  PympfObject *r; \
  if (options.debug) fprintf(stderr, "Py" #NAME ": %p\n", x); \
  if (!(r = Pympf_new(x->rebits))) return NULL; \
  NAME(r->f, x->f); \
  if (options.debug) fprintf(stderr, "Py" #NAME "-> %p\n", r); \
  return (PyObject *) r; \
}

#define MPQ_MONOP(NAME) \
static PyObject * \
Py##NAME(PympqObject *x) \
{ \
  PympqObject *r; \
  if (options.debug) fprintf(stderr, "Py" #NAME ": %p\n", x); \
  if (!(r = Pympq_new())) return NULL; \
  NAME(r->q, x->q); \
  if (options.debug) fprintf(stderr, "Py" #NAME "-> %p\n", r); \
  return (PyObject *) r; \
}

MPZ_MONOP(mpz_abs)
MPZ_MONOP(mpz_neg)

/* MPQ_MONOP(mpq_inv) */

MPQ_MONOP(mpq_neg)

static PyObject *
Pympq_abs(PympqObject *x)
{
    PympqObject *r;
    if (options.debug) fprintf(stderr, "Pympq_abs: %p\n", x);
    if (!(r = Pympq_new())) return NULL;
    mpq_set(r->q, x->q);
    mpz_abs(mpq_numref(r->q),mpq_numref(r->q));
    if (options.debug) fprintf(stderr, "Pympq_abs-> %p\n", r);
    return (PyObject *) r;
}

MPF_MONOP(mpf_abs)
MPF_MONOP(mpf_neg)

static PyObject *
Pympz_pos(PympzObject *x)
{
    Py_INCREF((PyObject*)x);
    return (PyObject *) x;
}
static PyObject *
Pympq_pos(PympqObject *x)
{
    Py_INCREF((PyObject*)x);
    return (PyObject *) x;
}
static PyObject *
Pympf_pos(PympfObject *x)
{
    Py_INCREF((PyObject*)x);
    return (PyObject *) x;
}

static PyObject *
Pympz_pow(PyObject *in_b, PyObject *in_e, PyObject *in_m)
{
    PympzObject *r;
    PympzObject *b = anyint2mpz(in_b);
    PympzObject *e = anyint2mpz(in_e);
    PympzObject *m = anyint2mpz(in_m);

    if(!b || !e || (!m && ((PyObject*)in_m != Py_None))) {
        PyObject *r;
        Py_XDECREF(b);
        Py_XDECREF(e);
        Py_XDECREF(m);
        r = Py_NotImplemented;
        Py_INCREF(r);
        return r;
    }

    if(options.debug)
        fprintf(stderr, "Pympz_pow: %p, %p, %p\n", b, e, m);

    if(mpz_sgn(e->z) < 0) {
        PyErr_SetString(PyExc_ValueError, "mpz.pow with negative power");
        Py_XDECREF(b);
        Py_XDECREF(e);
        Py_XDECREF(m);
        return NULL;
    }

    if((PyObject*)in_m == Py_None) {
        /* When no modulo is present, the exponent must fit in C ulong */
        unsigned long el;
        if(notanint(e->z)) {
            PyErr_SetString(PyExc_ValueError, "mpz.pow outrageous exponent");
            Py_XDECREF(b);
            Py_XDECREF(e);
            Py_XDECREF(m);
            return NULL;
        }
        el = mpz_get_ui(e->z);
        if(!(r = Pympz_new())) {
            Py_XDECREF(b);
            Py_XDECREF(e);
            Py_XDECREF(m);
            return NULL;
        }
        mpz_pow_ui(r->z, b->z, el);
        if(options.debug)
            fprintf(stderr, "Pympz_pow (ui) -> %p\n", r);
    } else { /* Modulo exponentiation */
        int sign;
        mpz_t mm;

        sign = mpz_sgn(m->z);
        if(sign == 0) {
            PyErr_SetString(PyExc_ValueError, "mpz.pow divide by zero");
            Py_XDECREF(b);
            Py_XDECREF(e);
            Py_XDECREF(m);
            return NULL;
        }
        if(!(r = Pympz_new())) {
            Py_XDECREF(b);
            Py_XDECREF(e);
            Py_XDECREF(m);
            return NULL;
        }
        mpz_inoc(mm);
        mpz_abs(mm, m->z);
        mpz_powm(r->z, b->z, e->z, mm);
        mpz_cloc(mm);
        if((sign<0) && (mpz_sgn(r->z) > 0)) {
        /* Python uses a rather peculiar convention for negative modulos
         * If the modulo is negative, result should be in the interval
         * m < r <= 0 .
         */
            mpz_add(r->z, r->z, m->z);
        }
        if(options.debug)
            fprintf(stderr, "Pympz_pow -> %p\n", r);
    }
    Py_XDECREF(b);
    Py_XDECREF(e);
    Py_XDECREF(m);
    return (PyObject*)r;
}

static PyObject *
Pympq_pow(PyObject *in_b, PyObject *in_e, PyObject *m)
{
    PympqObject *r;
    PympqObject *b = anyrational2mpq(in_b);
    PympqObject *e = anyrational2mpq(in_e);

    int esign;
    unsigned long ultem;

    assert(Pympq_Check(b));
    assert(Pympq_Check(e));

    if(!b || !e) {
        PyObject *r;
        Py_XDECREF(b);
        Py_XDECREF(e);
        r = Py_NotImplemented;
        Py_INCREF(r);
        return r;
    }

    if(options.debug)
        fprintf(stderr, "Pympq_pow: %p, %p, %p\n", b, e, m);

    if((PyObject*)m != Py_None) {
        PyErr_SetString(PyExc_ValueError, "mpq.pow no modulo allowed");
        Py_DECREF(b);
        Py_DECREF(e);
        return NULL;
    }
    if(notanint(mpq_numref(e->q))) {
        PyErr_SetString(PyExc_ValueError, "mpq.pow outrageous exp num");
        Py_DECREF(b);
        Py_DECREF(e);
        return NULL;
    }
    if(notanint(mpq_denref(e->q))) {
        PyErr_SetString(PyExc_ValueError, "mpq.pow outrageous exp den");
        Py_DECREF(b);
        Py_DECREF(e);
        return NULL;
    }
    if(!(r = Pympq_new())) {
        Py_DECREF(b);
        Py_DECREF(e);
        return NULL;
    }

    esign = mpq_sgn(e->q);
    if(esign == 0) {
        if(options.debug)
            fprintf(stderr, "Pympq_pow (ui,0) -> %p\n", r);
        mpq_set_si(r->q, 1, 1);
        Py_DECREF(b);
        Py_DECREF(e);
        return (PyObject*)r;
    } else if(esign < 0) {
        int bsign = mpq_sgn(b->q);
        if(bsign == 0) {
            PyObject* result = 0;
            PyErr_SetString(PyExc_ZeroDivisionError,"mpq.pow 0 base to <0 exponent");
            Py_DECREF((PyObject*)r);
            Py_DECREF(b);
            Py_DECREF(e);
            return result;
        }
        if(bsign < 0) {
            mpz_neg(mpq_numref(r->q), mpq_denref(b->q));
        } else {
            mpz_set(mpq_numref(r->q), mpq_denref(b->q));
        }
        mpz_abs(mpq_denref(r->q), mpq_numref(b->q));
        ultem = -mpz_get_si(mpq_numref(e->q));
    } else {
        mpq_set(r->q, b->q);
        ultem = mpz_get_ui(mpq_numref(e->q));
    }
    if(ultem>1) {
        mpz_pow_ui(mpq_numref(r->q), mpq_numref(r->q), ultem);
        mpz_pow_ui(mpq_denref(r->q), mpq_denref(r->q), ultem);
    }
    ultem = mpz_get_ui(mpq_denref(e->q));
    if(ultem>1) {
        static char* msgi = "mpq.pow fractional exponent, inexact-root";
        char* msg = msgi;
        int exact=0;
        if(mpq_sgn(r->q)<0) {
            static char* msgi = "mpq.pow fractional exponent, nonreal-root";
            msg = msgi;
        } else {
            mpz_t temp; /* workaround mpz_root bug in GMP 3.1.1 */
            mpz_inoc(temp);
            exact = mpz_root(temp, mpq_numref(r->q), ultem);
            if(exact) {
                mpz_set(mpq_numref(r->q), temp);
                exact = mpz_root(temp, mpq_denref(r->q), ultem);
                mpz_set(mpq_denref(r->q), temp);
            }
        }
        if(!exact) {
            Py_DECREF((PyObject*)r);
            PyErr_SetString(PyExc_ValueError, msg);
            Py_DECREF(b);
            Py_DECREF(e);
            return NULL;
        }
    }
    if(options.debug)
        fprintf(stderr, "Pympq_pow (ui) -> %p\n", r);
    Py_DECREF(b);
    Py_DECREF(e);
    return (PyObject*)r;
}

static PyObject *
Pympf_pow(PyObject *xb, PyObject *xe, PyObject *m)
{
    PympqObject *qb, *qe;
    PyObject *r;
    unsigned int bits;
    int iexpo;
    PympfObject *b = 0, *e = 0;

    if((PyObject*)m != Py_None) {
        PyErr_SetString(PyExc_ValueError, "mpf.pow no modulo allowed");
        return NULL;
    }

    if((Pympf_Check(xb) && Pympf_Check(xe))) {
        b = anyreal2mpf(xb, 0);
        e = anyreal2mpf(xe, 0);
    } else {
        if(Pympf_Check(xb)) {
            b = anyreal2mpf(xb, 0);
            e = anyreal2mpf(xe, ((PympfObject*)xb)->rebits);
        }
        if(Pympf_Check(xe)) {
            b = anyreal2mpf(xb, ((PympfObject*)xe)->rebits);
            e = anyreal2mpf(xe, 0);
        }
    }

    if(!e || !b) {
        PyObject *r = Py_NotImplemented;
        Py_INCREF(r);
        Py_XDECREF(e);
        Py_XDECREF(b);
        return r;
    }

    bits = b->rebits;
    if(bits > e->rebits)
        bits = e->rebits;
    if(options.debug)
        fprintf(stderr, "Pympf_pow(%d): %p, %p, %p\n", bits, b, e, m);

    iexpo = (int)mpf_get_d(e->f);
    if(iexpo>0 && 0==mpf_cmp_si(e->f, iexpo)) {
        r = (PyObject*)Pympf_new(b->rebits);
        if(!r) {
            Py_DECREF(e);
            Py_DECREF(b);
            return 0;
        }
        mpf_pow_ui(Pympf_AS_MPF(r), b->f, iexpo);
    } else {
        qb = mpf2mpq((PyObject*)b);
        qe = mpf2mpq((PyObject*)e);
        r = Pympq_pow((PyObject*)qb, (PyObject*)qe, (PyObject*)m);
        Py_DECREF((PyObject*)qb); Py_DECREF((PyObject*)qe);
        if(!r || !Pympq_Check(r)) {
            Py_DECREF(e);
            Py_DECREF(b);
            return r;
        }
        qb = (PympqObject*)r;
        r = (PyObject*)mpq2mpf((PyObject*)qb, bits);
        Py_DECREF((PyObject*)qb);
    }
    mpf_normalize((PympfObject*)r);
    Py_DECREF(e);
    Py_DECREF(b);
    return r;
}

/* COMPARING */
static int sign(int result)
{
    if(result) {
       if(result>0) {
           return 1;
       } else {
           return -1;
       }
    } else {
        return 0;
    }
}
//~ static int
//~ Pympz_cmp(PympzObject *a, PympzObject *b)
//~ {
    //~ if(options.debug)
        //~ fprintf(stderr, "Pympz_cmp: compare %ld to %ld\n",
                //~ mpz_get_si(a->z),
                //~ mpz_get_si(b->z)
                //~ );
    //~ return sign(mpz_cmp(a->z, b->z));
//~ }
//~ static int
//~ Pympq_cmp(PympqObject *a, PympqObject *b)
//~ {
    //~ return sign(mpq_cmp(a->q, b->q));
//~ }
//~ static int
//~ Pympf_cmp(PympfObject *a, PympfObject *b)
//~ {
    //~ return sign(mpf_cmp(a->f, b->f));
//~ }

//~ static int Pympz_coerce(PyObject **pv, PyObject **pw);
//~ static int Pympf_coerce(PyObject **pv, PyObject **pw);
//~ static int Pympq_coerce(PyObject **pv, PyObject **pw);

static PyObject *_cmp_to_object(int c, int op)
{
    PyObject *result;
    switch (op) {
    case Py_LT: c = c <  0; break;
    case Py_LE: c = c <= 0; break;
    case Py_EQ: c = c == 0; break;
    case Py_NE: c = c != 0; break;
    case Py_GT: c = c >  0; break;
    case Py_GE: c = c >= 0; break;
    }
    result = c ? Py_True : Py_False;
    Py_INCREF(result);
    return result;
}
static PyObject *
mpz_richcompare(PympzObject *a, PyObject *b, int op)
{
    int c;
    PympzObject *bb = anyint2mpz(b);
    if(!bb) {
        if(PyFloat_Check(b)) {
            c = sign(mpz_cmp_d(a->z, PyFloat_AS_DOUBLE(b)));
        } else {
            PyObject *result = Py_NotImplemented;
            Py_INCREF(result);
            return result;
        }
    } else {
        c = sign(mpz_cmp(a->z, bb->z));
        Py_DECREF(bb);
    }
    return _cmp_to_object(c, op);
}

static PyObject *
mpq_richcompare(PympqObject *a, PyObject *b, int op)
{
    int c;
    mpq_t temp;
    PympqObject *bb = anyrational2mpq(b);
    if(!bb) {
        if(PyFloat_Check(b)) {
            mpq_inoc(temp);
            mpq_set_d(temp, PyFloat_AS_DOUBLE(b));
            c = sign(mpq_cmp(a->q, temp));
            mpq_cloc(temp);
        } else {
            PyObject *result = Py_NotImplemented;
            Py_INCREF(result);
            return result;
        }
    } else {
        c = sign(mpq_cmp(a->q, bb->q));
        Py_DECREF(bb);
    }
    return _cmp_to_object(c, op);
}
static PyObject *
mpf_richcompare(PympfObject *a, PyObject *b, int op)
{
    int c;
    PympfObject *bb = anyreal2mpf(b, 0);
    if(!bb) {
        PyObject *result = Py_NotImplemented;
        Py_INCREF(result);
        return result;
    } else {
        c = sign(mpf_cmp(a->f, bb->f));
        Py_DECREF(bb);
    }
    return _cmp_to_object(c, op);
}

static int
Pympz_nonzero(PympzObject *x)
{
    return mpz_sgn(x->z) != 0;
}
static int
Pympq_nonzero(PympqObject *x)
{
    return mpq_sgn(x->q) != 0;
}
static int
Pympf_nonzero(PympfObject *x)
{
    return mpf_sgn(x->f) != 0;
}

/* float-truncations (return still a float!) */

#define MPF_UNIOP(NAME) \
static PyObject * \
Py##NAME(PyObject* self, PyObject *args) \
{ \
  PympfObject *r; \
  if(self && Pympf_Check(self)) { \
      if(args && !PyArg_ParseTuple(args, "")) return NULL; \
      Py_INCREF(self); \
  } else { \
      if(!PyArg_ParseTuple(args, "O&", Pympf_convert_arg, &self)) return NULL; \
  } \
  assert(Pympf_Check(self)); \
  if (options.debug) fprintf(stderr, "Py" #NAME ": %p\n", self); \
  if (!(r = Pympf_new(((PympfObject*)self)->rebits))) return NULL; \
  NAME(r->f, Pympf_AS_MPF(self)); \
  if (options.debug) fprintf(stderr, "Py" #NAME "-> %p\n", r); \
  Py_DECREF(self); \
  mpf_normalize(r); \
  return (PyObject *) r; \
}

static char doc_ceilm[]="\
x.ceil(): returns an mpf that is the smallest integer >= x\n\
";
static char doc_ceilg[]="\
ceil(x): returns an mpf that is the smallest integer >= x\n\
x must be an mpf, or else gets coerced to one.\n\
";
MPF_UNIOP(mpf_ceil)

static char doc_floorm[]="\
x.floor(): returns an mpf that is the smallest integer <= x\n\
";
static char doc_floorg[]="\
floor(x): returns an mpf that is the smallest integer <= x\n\
x must be an mpf, or else gets coerced to one.\n\
";
MPF_UNIOP(mpf_floor)

static char doc_truncm[]="\
x.trunc(): returns an mpf that is x truncated towards 0\n\
(same as x.floor() if x>=0, x.ceil() if x<0).\n\
";
static char doc_truncg[]="\
trunc(x): returns an mpf that is x truncated towards 0\n\
(same as x.floor() if x>=0, x.ceil() if x<0).\n\
x must be an mpf, or else gets coerced to one.\n\
";
MPF_UNIOP(mpf_trunc)

/* BIT OPERATIONS (mpz-only) */
MPZ_MONOP(mpz_com)
MPZ_BINOP(mpz_and)
MPZ_BINOP(mpz_ior)
MPZ_BINOP(mpz_xor)

#define MPZ_SHIFTOP(NAME, OP) \
static PyObject * \
NAME(PyObject *a, PyObject *b) \
{ \
  PympzObject *r; \
  PympzObject *pa = anyint2mpz(a); \
  PympzObject *pb = 0; \
  long count; \
  if(!pa) { \
    PyObject *r = Py_NotImplemented; \
    Py_INCREF(r); \
    return r; \
  } \
  if(Py2or3Int_Check(b) && ((count = Py2or3Int_AsLong(b)) > 0)) { \
      if (!(r = Pympz_new())) { \
        Py_DECREF(pa); \
        return NULL; \
      } \
      OP(r->z, ((PympzObject*)a)->z, count); \
  } else { \
    pb = anyint2mpz(b); \
    if(!pb) { \
      PyObject *r; \
      Py_DECREF(pa); \
      r = Py_NotImplemented; \
      Py_INCREF(r); \
      return r; \
    } \
    if(mpz_sgn(pb->z) < 0) { \
      static char* msg = #NAME " negative shift count"; \
      PyErr_SetString(PyExc_ValueError, msg); \
      Py_DECREF(pa); \
      Py_DECREF(pb); \
      return NULL; } \
    if(!mpz_fits_slong_p(pb->z)) { \
      static char* msg = #NAME " outrageous shift count"; \
      PyErr_SetString(PyExc_ValueError, msg); \
      Py_DECREF(pa); \
      Py_DECREF(pb); \
      return NULL; } \
    count = mpz_get_si(pb->z); \
    if (!(r = Pympz_new())) { \
      Py_DECREF(pa); \
      Py_DECREF(pb); \
      return NULL; \
    } \
    OP(r->z, pa->z, count); \
    Py_DECREF(pa); \
    Py_DECREF(pb); \
  } \
  return (PyObject *) r; \
}

MPZ_SHIFTOP(Pympz_rshift, mpz_fdiv_q_2exp)
MPZ_SHIFTOP(Pympz_lshift, mpz_mul_2exp)

/* hex/oct formatting (mpz-only) */
static PyObject *
Pympz_oct(PympzObject *self)
{
    return Pympz_ascii(self, 8, 0);
}
static PyObject *
Pympz_hex(PympzObject *self)
{
    return Pympz_ascii(self, 16, 0);
}

/* coercion (in the Python sense) */

//~ int
//~ Pympz_coerce(PyObject **pv, PyObject **pw)
//~ {
    //~ PyObject *z;

    //~ if(options.debug)
        //~ fprintf(stderr, "Pympz.coerce(%p, %p) called...\n", *pv, *pw);
    //~ assert(Pympz_Check(*pv));

    //~ /* if other arg is float, mpf, mpq, then mpz gets converted to its type */
    //~ if(PyFloat_Check(*pw)) {
        //~ /* convert mpz to float, instead */
        //~ if (options.debug)
            //~ fprintf(stderr, "Pympz.coerce(): float \n");
        //~ if (!(z = mpz2float((PympzObject*)*pv)))
            //~ return -1;
        //~ *pv = z;
        //~ Py_INCREF(*pw);
        //~ return 0;
    //~ } else if(Pympf_Check(*pw)) {
        //~ return Pympf_coerce(pw,pv);
    //~ } else if(Pympq_Check(*pw)) {
        //~ return Pympq_coerce(pw,pv);
    //~ }
    //~ /* else, try converting other-number (Python int or long) to mpz */
    //~ z = (PyObject*)anynum2mpz(*pw);
    //~ if(z) {
        //~ Py_INCREF(*pv);
        //~ *pw = z;
        //~ return 0;
    //~ } else {
        //~ if(!PyErr_Occurred())
            //~ PyErr_SetString(PyExc_TypeError,
                    //~ "coercion to gmpy.mpz type failed");
        //~ return -1;
    //~ }
//~ }

//~ static int
//~ Pympq_coerce(PyObject **pv, PyObject **pw)
//~ {
    //~ PympqObject *q;

    //~ if(options.debug)
        //~ fprintf(stderr, "Pympq.coerce(%p, %p) called...\n", *pv, *pw);
    //~ assert(Pympq_Check(*pv));

    //~ q = anynum2mpq(*pw);
    //~ if(q) {
        //~ *pw = (PyObject*)q;
        //~ Py_INCREF(*pv);
        //~ return 0;
    //~ } else {
        //~ if(!PyErr_Occurred()) {
            //~ PyErr_SetString(PyExc_TypeError,
                            //~ "coercion to gmpy.mpq type failed");
        //~ }
        //~ return -1;
    //~ }
//~ }

//~ static int
//~ Pympf_coerce(PyObject **pv, PyObject **pw)
//~ {
    //~ PyObject *z;
    //~ PympfObject* self;

    //~ if(options.debug)
        //~ fprintf(stderr, "Pympf.coerce(%p, %p) called...\n", *pv, *pw);
    //~ assert(Pympf_Check(*pv));
    //~ self = (PympfObject*)(*pv);

    //~ /* if other arg is mpq, then mpf gets converted to its type */
    //~ if(Pympq_Check(*pw)) {
        //~ return Pympq_coerce(pw,pv);
    //~ }
    //~ /* else, try converting other-number (Python int or long) to mpf */
    //~ z = (PyObject*)anynum2mpf(*pw, self->rebits);
    //~ if(z) {
        //~ Py_INCREF(*pv);
        //~ *pw = z;
        //~ return 0;
    //~ } else {
        //~ if(!PyErr_Occurred())
            //~ PyErr_SetString(PyExc_TypeError,
                    //~ "coercion to gmpy.mpf type failed");
        //~ return -1;
    //~ }
//~ }

/* hashing */
static long
dohash(PyObject* tempPynum)
{
    long hash;
    if(!tempPynum) return -1;
    hash = PyObject_Hash(tempPynum);
    Py_DECREF(tempPynum);
    return hash;
}
static long
Pympz_hash(PympzObject *self)
{
    return dohash(mpz2long(self));
}
static long
Pympf_hash(PympfObject *self)
{
    return dohash(mpf2float(self));
}
static long
Pympq_hash(PympqObject *self)
{
    return dohash(mpq2float(self));
}

/* Miscellaneous gmpy functions */
static char doc_gcd[]="\
gcd(a,b): returns the greatest common denominator of numbers a and b\n\
(which must be mpz objects, or else get coerced to mpz)\n\
";
static PyObject *
Pygmpy_gcd(PyObject *self, PyObject *args)
{
    PympzObject *a, *b, *c;

    TWO_ARG_CONVERTED(Pympz_convert_arg,&a,&b);

    assert(Pympz_Check((PyObject*)a));
    assert(Pympz_Check((PyObject*)b));

    if(!(c = Pympz_new())) {
        Py_DECREF((PyObject*)a); Py_DECREF((PyObject*)b);
        return NULL;
    }
    mpz_gcd(c->z, a->z, b->z);
    Py_DECREF((PyObject*)a); Py_DECREF((PyObject*)b);
    return (PyObject*)c;
}

static char doc_lcm[]="\
lcm(a,b): returns the lowest common multiple of numbers a and b\n\
(which must be mpz objects, or else get coerced to mpz)\n\
";
static PyObject *
Pygmpy_lcm(PyObject *self, PyObject *args)
{
    PympzObject *a, *b, *c;

    TWO_ARG_CONVERTED(Pympz_convert_arg,&a,&b);

    assert(Pympz_Check((PyObject*)a));
    assert(Pympz_Check((PyObject*)b));

    if(!(c = Pympz_new())) {
        Py_DECREF((PyObject*)a); Py_DECREF((PyObject*)b);
        return NULL;
    }
    mpz_lcm(c->z, a->z, b->z);
    Py_DECREF((PyObject*)a); Py_DECREF((PyObject*)b);
    return (PyObject*)c;
}

static char doc_gcdext[]="\
gcdext(a,b): returns a 3-element tuple (g,s,t) such that\n\
    g==gcd(a,b) and g == a*s + b*t\n\
(a and b must be mpz objects, or else get coerced to mpz)\n\
";
static PyObject *
Pygmpy_gcdext(PyObject *self, PyObject *args)
{
    PympzObject *a, *b, *g=0, *s=0, *t=0;
    PyObject *res;

    TWO_ARG_CONVERTED(Pympz_convert_arg,&a,&b);
    assert(a && Pympz_Check((PyObject*)a));
    assert(b && Pympz_Check((PyObject*)b));

    g = Pympz_new(); s = Pympz_new(); t = Pympz_new();
    if(!g||!s||!t) {
        Py_DECREF((PyObject*)a); Py_DECREF((PyObject*)b);
        Py_XDECREF((PyObject*)g); Py_XDECREF((PyObject*)s);
        Py_XDECREF((PyObject*)t);
        return NULL;
    }
    mpz_gcdext(g->z, s->z, t->z, a->z, b->z);
    Py_DECREF((PyObject*)a); Py_DECREF((PyObject*)b);
    res = Py_BuildValue("(NNN)", g, s, t); /* Does NOT increment refcounts! */

    return (PyObject *) res;
}

static char doc_divm[]="\
divm(a,b,m): returns x such that b*x==a modulo m, or else raises\n\
a ZeroDivisionError exception if no such value x exists\n\
(a, b and m must be mpz objects, or else get coerced to mpz)\n\
";
static PyObject *
Pygmpy_divm(PyObject *self, PyObject *args)
{
    PympzObject *num, *den, *mod, *res;
    mpz_t numz, denz, modz;
    int ok;

    if(!PyArg_ParseTuple(args, "O&O&O&",
        Pympz_convert_arg, &num,
        Pympz_convert_arg, &den,
        Pympz_convert_arg, &mod))
    {
        return NULL;
    }
    if(!(res = Pympz_new())) {
        return NULL;
        Py_DECREF((PyObject*)num);
        Py_DECREF((PyObject*)den);
        Py_DECREF((PyObject*)mod);
    }
    mpz_init(numz); mpz_init(denz); mpz_init(modz);
    mpz_set(numz, num->z); mpz_set(denz, den->z); mpz_set(modz, mod->z);

    if(mpz_invert(res->z, denz, modz)) { /* inverse exists */
      ok = 1;
    } else {
      /* last-ditch attempt: do num, den AND mod have a gcd>1 ? */
      PympzObject *gcd;
      if(!(gcd = Pympz_new()))
          return NULL;
      mpz_gcd(gcd->z, numz, denz);
      mpz_gcd(gcd->z, gcd->z, modz);
      mpz_divexact(numz, numz, gcd->z);
      mpz_divexact(denz, denz, gcd->z);
      mpz_divexact(modz, modz, gcd->z);
      Py_DECREF((PyObject*)gcd);
      ok = mpz_invert(res->z, denz, modz);
    }

    if (ok) {
        mpz_mul(res->z, res->z, numz);
        mpz_mod(res->z, res->z, modz);
        mpz_clear(numz); mpz_clear(denz); mpz_clear(modz);
        Py_DECREF((PyObject*)num);
        Py_DECREF((PyObject*)den);
        Py_DECREF((PyObject*)mod);
        return (PyObject*)res;
    } else {
        PyObject* result = 0;
        PyErr_SetString(PyExc_ZeroDivisionError, "not invertible");
        mpz_clear(numz); mpz_clear(denz); mpz_clear(modz);
        Py_DECREF((PyObject*)num);
        Py_DECREF((PyObject*)den);
        Py_DECREF((PyObject*)mod);
        Py_DECREF((PyObject*)res);
        return result;
    }
}

static char doc_fac[]="\
fac(n): returns the factorial of n; takes O(n) time; n must be\n\
an ordinary Python int, >=0.\n\
";
static PyObject *
Pygmpy_fac(PyObject *self, PyObject *args)
{
    PympzObject *fac;
    long n;

    ONE_ARG("fac", "l", &n);

    if(n < 0) {
        PyErr_SetString(PyExc_ValueError, "factorial of negative number");
        return NULL;
    }
    if(!(fac = Pympz_new()))
        return NULL;
    mpz_fac_ui(fac->z, n);

    return (PyObject *) fac;
}

static char doc_fib[]="\
fib(n): returns the n-th Fibonacci number; takes O(n) time; n must be\n\
an ordinary Python int, >=0.\n\
";
static PyObject *
Pygmpy_fib(PyObject *self, PyObject *args)
{
    PympzObject *fib;
    long n;

    ONE_ARG("fib", "l", &n);

    if(n < 0) {
        PyErr_SetString(PyExc_ValueError, "Fibonacci of negative number");
        return NULL;
    }
    if(!(fib = Pympz_new()))
        return NULL;
    mpz_fib_ui(fib->z, n);

    return (PyObject *) fib;
}

static char doc_pi[]="\
pi(n): returns pi with n bits of precision in an mpf object\n\
";

/* This function was originally from netlib, package bmp, by
 * Richard P. Brent. Paulo Cesar Pereira de Andrade converted
 * it to C and used it in his LISP interpreter.
 *
 * Original comments:
 *
 *   sets mp pi = 3.14159... to the available precision.
 *   uses the gauss-legendre algorithm.
 *   this method requires time o(ln(t)m(t)), so it is slower
 *   than mppi if m(t) = o(t**2), but would be faster for
 *   large t if a faster multiplication algorithm were used
 *   (see comments in mpmul).
 *   for a description of the method, see - multiple-precision
 *   zero-finding and the complexity of elementary function
 *   evaluation (by r. p. brent), in analytic computational
 *   complexity (edited by j. f. traub), academic press, 1976, 151-176.
 *   rounding options not implemented, no guard digits used.
*/
static PyObject *
Pygmpy_pi(PyObject *self, PyObject *args)
{
    PympfObject *pi;
    int precision;
    mpf_t r_i2, r_i3, r_i4;
    mpf_t ix;

    ONE_ARG("pi", "i", &precision);
    if(!(pi = Pympf_new(precision))) {
        return NULL;
    }

    mpf_set_si(pi->f, 1);

    mpf_init(ix);
    mpf_set_ui(ix, 1);

    mpf_init2(r_i2, precision);

    mpf_init2(r_i3, precision);
    mpf_set_d(r_i3, 0.25);

    mpf_init2(r_i4, precision);
    mpf_set_d(r_i4, 0.5);
    mpf_sqrt(r_i4, r_i4);

    for (;;) {
        mpf_set(r_i2, pi->f);
        mpf_add(pi->f, pi->f, r_i4);
        mpf_div_ui(pi->f, pi->f, 2);
        mpf_mul(r_i4, r_i2, r_i4);
        mpf_sub(r_i2, pi->f, r_i2);
        mpf_mul(r_i2, r_i2, r_i2);
        mpf_mul(r_i2, r_i2, ix);
        mpf_sub(r_i3, r_i3, r_i2);
        mpf_sqrt(r_i4, r_i4);
        mpf_mul_ui(ix, ix, 2);
        /* Check for convergence */
        if (!(mpf_cmp_si(r_i2, 0) &&
              mpf_get_prec(r_i2) >= (unsigned)precision)) {
            mpf_mul(pi->f, pi->f, r_i4);
            mpf_div(pi->f, pi->f, r_i3);
            break;
        }
    }

    mpf_clear(ix);
    mpf_clear(r_i2);
    mpf_clear(r_i3);
    mpf_clear(r_i4);

    mpf_normalize(pi);
    return (PyObject*)pi;
}

static char doc_bincoefm[]="\
x.bincoef(n): returns the 'binomial coefficient' that is 'x\n\
over n'; n is an ordinary Python int, >=0.\n\
";
static char doc_bincoefg[]="\
bincoef(x,n): returns the 'binomial coefficient' that is 'x\n\
over n'; n is an ordinary Python int, >=0; x must be an mpz,\n\
or else gets converted to one.\n\
";
static char doc_combm[]="\
x.comb(n): returns the 'number of combinations' of 'x things,\n\
taken n at a time'; n is an ordinary Python int, >=0.\n\
";
static char doc_combg[]="\
comb(x,n): returns the 'number of combinations' of 'x things,\n\
taken n at a time'; n is an ordinary Python int, >=0; x must be\n\
an mpz, or else gets converted to one.\n\
";
static PyObject *
Pympz_bincoef(PyObject *self, PyObject *args)
{
    PympzObject *bincoef;
    long k;

    SELF_MPZ_ONE_ARG("l",&k);

    assert(Pympz_Check(self));

    if(k < 0) {
        PyErr_SetString(PyExc_ValueError, "binomial coefficient with negative k");
        Py_DECREF(self);
        return NULL;
    }

    if(!(bincoef = Pympz_new())) {
        Py_DECREF(self);
        return NULL;
    }
    mpz_bin_ui(bincoef->z, Pympz_AS_MPZ(self), k);
    Py_DECREF(self);
    return (PyObject*)bincoef;
}


static char doc_fsqrtm[]="\
x.fsqrt(): returns the square root of x.  x must be >= 0.\n\
";
static char doc_fsqrtg[]="\
fsqrt(x): returns the square root of x.  x must be an mpf, or\n\
else gets coerced to one; further, x must be >= 0.\n\
";
static PyObject *
Pympf_sqrt(PyObject *self, PyObject *args)
{
    PympfObject *root;

    SELF_MPF_NO_ARG;

    assert(Pympf_Check(self));

    if(mpf_sgn(Pympf_AS_MPF(self)) < 0) {
        PyErr_SetString(PyExc_ValueError, "sqrt of negative number");
        Py_DECREF(self);
        return NULL;
    }

    if(!(root = Pympf_new(((PympfObject*)self)->rebits))) {
        Py_DECREF(self);
        return NULL;
    }
    mpf_sqrt(root->f, Pympf_AS_MPF(self));
    Py_DECREF(self);
    mpf_normalize(root);
    return (PyObject *) root;
}

static char doc_sqrtm[]="\
x.sqrt(): returns the integer, truncated square root of x, i.e. the\n\
largest y such that x>=y*y. x must be >= 0.\n\
";
static char doc_sqrtg[]="\
sqrt(x): returns the integer, truncated square root of x, i.e. the\n\
largest y such that x>=y*y. x must be an mpz, or else gets coerced\n\
to one; further, x must be >= 0.\n\
";
static PyObject *
Pympz_sqrt(PyObject *self, PyObject *args)
{
    PympzObject *root;

    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));

    if(mpz_sgn(Pympz_AS_MPZ(self)) < 0) {
        PyErr_SetString(PyExc_ValueError, "sqrt of negative number");
        Py_DECREF(self);
        return NULL;
    }

    if(!(root = Pympz_new())) {
        Py_DECREF(self);
        return NULL;
    }
    mpz_sqrt(root->z, Pympz_AS_MPZ(self));
    Py_DECREF(self);
    return (PyObject *) root;
}

static char doc_sqrtremm[]="\
x.sqrtrem(): returns a 2-element tuple (s,t), such that\n\
s==x.sqrt() and x==s*s+t. x must be >= 0.\n\
";
static char doc_sqrtremg[]="\
sqrtrem(x): returns a 2-element tuple (s,t), such that\n\
s==sqrt(x) and x==s*s+t. x must be an mpz, or else gets\n\
coerced to one; further, x must be >= 0.\n\
";
static PyObject *
Pympz_sqrtrem(PyObject *self, PyObject *args)
{
    PympzObject *root=0, *rem=0;
    PyObject *res;

    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));

    if(mpz_sgn(Pympz_AS_MPZ(self)) < 0) {
        PyErr_SetString(PyExc_ValueError, "sqrt of negative number");
        Py_DECREF(self);
        return NULL;
    }

    root = Pympz_new();
    rem = Pympz_new();
    if(!root || !rem) {
        Py_XDECREF((PyObject*)rem); Py_XDECREF((PyObject*)root);
        Py_DECREF((PyObject*)self);
        return NULL;
    }
    mpz_sqrtrem(root->z, rem->z, Pympz_AS_MPZ(self));

    res = Py_BuildValue("(NN)", root, rem);
    Py_DECREF(self);
    return res;
}

static char doc_removem[]="\
x.remove(f): returns a 2-element tuple (y,m) such that\n\
x==y*(f**m), and y%f==0; i.e., y is x with any factor f\n\
removed, and m (an ordinary Python int) is the multiplicity\n\
of the factor f in x (m=0, and y=x, unless x%f==0). f must\n\
be > 0.\n\
";
static char doc_removeg[]="\
remove(x,f): returns a 2-element tuple (y,m) such that\n\
x==y*(f**m), and y%f==0; i.e., y is x with any factor f\n\
removed, and m (an ordinary Python int) is the multiplicity\n\
of the factor f in x (m=0, and y=x, unless x%f==0). x must\n\
be an mpz, or else gets coerced to one; f must be > 0.\n\
";
static PyObject *
Pympz_remove(PyObject *self, PyObject *args)
{
    PympzObject *result;
    PyObject *factor;
    PyObject *restuple;
    int multiplicity;

    SELF_MPZ_ONE_ARG_CONVERTED(&factor);
    assert(Pympz_Check(self));
    assert(Pympz_Check(factor));

    if(mpz_sgn(Pympz_AS_MPZ(factor)) <= 0) {
        PyErr_SetString(PyExc_ValueError, "factor must be > 0");
        return NULL;
    }

    if(!(result = Pympz_new())) {
        Py_DECREF(self);
        Py_DECREF(factor);
        return NULL;
    }
    multiplicity = mpz_remove(result->z, Pympz_AS_MPZ(self),
        Pympz_AS_MPZ(factor));
    restuple = Py_BuildValue("(Ni)", result, multiplicity);
    Py_DECREF(self);
    Py_DECREF(factor);
    return restuple;
}

static char doc_invertm[]="\
x.invert(m): returns the inverse of x modulo m, i.e., that y\n\
such that x*y==1 modulo m, or 0 if no such y exists.\n\
m must be an ordinary Python int, !=0.\n\
";
static char doc_invertg[]="\
invert(x,m): returns the inverse of x modulo m, i.e., that y\n\
such that x*y==1 modulo m, or 0 if no such y exists.\n\
m must be an ordinary Python int, !=0; x must be an mpz,\n\
or else gets converted to one.\n\
";
static PyObject *
Pympz_invert(PyObject *self, PyObject *args)
{
    PympzObject *result;
    PyObject *modulo;
    int success;

    SELF_MPZ_ONE_ARG_CONVERTED(&modulo);
    assert(Pympz_Check(self));
    assert(Pympz_Check(modulo));

    if(!(result = Pympz_new())) {
        Py_DECREF(self);
        Py_DECREF(modulo);
        return NULL;
    }
    success = mpz_invert(result->z, Pympz_AS_MPZ(self), Pympz_AS_MPZ(modulo));
    if(!success) {
        mpz_set_ui(result->z, 0);
    }
    Py_DECREF(self);
    Py_DECREF(modulo);
    return (PyObject*)result;
}


static char doc_hamdistm[]="\
x.hamdist(y): returns the Hamming distance (number of bit-positions\n\
where the bits differ) between x and y.  y must be an mpz, or else\n\
gets coerced to one.\n\
";
static char doc_hamdistg[]="\
hamdist(x,y): returns the Hamming distance (number of bit-positions\n\
where the bits differ) between x and y.  x and y must be mpz, or else\n\
get coerced to mpz.\n\
";
static PyObject *
Pympz_hamdist(PyObject *self, PyObject *args)
{
    PyObject *result;
    PyObject *other;

    SELF_MPZ_ONE_ARG_CONVERTED(&other);
    assert(Pympz_Check(self));
    assert(Pympz_Check(other));

    result = Py_BuildValue("i",
                 mpz_hamdist(Pympz_AS_MPZ(self),Pympz_AS_MPZ(other)));

    Py_DECREF(self);
    Py_DECREF(other);
    return (PyObject*)result;
}

static char doc_divexactm[]="\
x.divexact(y): returns the quotient of x divided by y. Faster than\n\
standard division but requires the remainder is zero!  y must be an mpz,\n\
or else gets coerced to one.\n\
";
static char doc_divexactg[]="\
divexact(x,y): returns the quotient of x divided by y. Faster than\n\
standard division but requires the remainder is zero!  x and y must\n\
be mpz, or else get coerced to mpz.\n\
";
static PyObject *
Pympz_divexact(PyObject *self, PyObject *args)
{
    PyObject *other;
    PympzObject *result;

    SELF_MPZ_ONE_ARG_CONVERTED(&other);
    assert(Pympz_Check(self));
    assert(Pympz_Check(other));

    result = Pympz_new();
    if(!result) {
        Py_XDECREF((PyObject*)result);
        Py_DECREF((PyObject*)self);
        Py_DECREF((PyObject*)other);
        return NULL;
    }

    mpz_divexact(result->z,Pympz_AS_MPZ(self),Pympz_AS_MPZ(other));

    Py_DECREF(self);
    Py_DECREF(other);
    return (PyObject*)result;
}

/* Internal helper function for mpmath. */

static PyObject *
mpmath_build_mpf(long sign, PympzObject *man, PyObject *exp, long bc)
{
    PyObject *tup, *tsign, *tbc;
    if(!(tup = PyTuple_New(4))){
        Py_DECREF((PyObject*)man);
        Py_DECREF(exp);
        return NULL;
    }
    if(!(tsign=Py2or3Int_FromLong(sign))){
        Py_DECREF((PyObject*)man);
        Py_DECREF(exp);
        Py_DECREF(tup);
        return NULL;
    }
    if(!(tbc=Py2or3Int_FromLong(bc))){
        Py_DECREF((PyObject*)man);
        Py_DECREF(exp);
        Py_DECREF(tup);
        Py_DECREF(tsign);
        return NULL;
    }
    PyTuple_SET_ITEM(tup, 0, tsign);
    PyTuple_SET_ITEM(tup, 1, (PyObject*)man);
    PyTuple_SET_ITEM(tup, 2, (exp)?exp:Py2or3Int_FromLong(0));
    PyTuple_SET_ITEM(tup, 3, tbc);
    return tup;
}

static char doc_mpmath_normalizeg[]="\
_mpmath_normalize(...): helper function for mpmath.\n\
";
static PyObject *
Pympz_mpmath_normalize(PyObject *self, PyObject *args)
{
    long sign = 0, bc = 0, prec = 0, shift, zbits, carry = 0;
    PyObject *exp = 0, *newexp = 0, *newexp2 = 0, *tmp = 0;
    PympzObject *man = 0, *upper = 0, *lower = 0;
    char rnd = 0;

    if(PyTuple_GET_SIZE(args) == 6){
        /* Need better error-checking here. Under Python 3.0, overflow into
           C-long is possible. */
        sign = Py2or3Int_AsLong(PyTuple_GET_ITEM(args, 0));
        man = (PympzObject *)PyTuple_GET_ITEM(args, 1);
        exp = PyTuple_GET_ITEM(args, 2);
        bc = Py2or3Int_AsLong(PyTuple_GET_ITEM(args, 3));
        prec = Py2or3Int_AsLong(PyTuple_GET_ITEM(args, 4));
        rnd = Py2or3Bytes_AS_STRING(PyTuple_GET_ITEM(args, 5))[0];
        if(PyErr_Occurred()){
            PyErr_SetString(PyExc_TypeError, "arguments long, PympzObject*,"
                "PyObject*, long, long, char needed");
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "6 arguments required");
        return NULL;
    }
    if(!Pympz_Check(man)){
        PyErr_SetString(PyExc_TypeError, "argument is not an mpz");
	return NULL;
    }

    /* If the mantissa is 0, return the normalized representation. */
    if(!mpz_sgn(man->z)) {
        Py_INCREF((PyObject*)man);
        return mpmath_build_mpf(0, man, 0, 0);
    }

    /* if bc <= prec and the number is odd return it */
    if ((bc <= prec) && mpz_odd_p(man->z)) {
        Py_INCREF((PyObject*)man);
        Py_INCREF((PyObject*)exp);
        return mpmath_build_mpf(sign, man, exp, bc);
    }

    upper = Pympz_new();
    lower = Pympz_new();
    if(!upper||!lower) {
        Py_DECREF((PyObject*)man);
        Py_XDECREF((PyObject*)upper);
        Py_XDECREF((PyObject*)lower);
        return NULL;
    }

    shift = bc - prec;
    if(shift>0) {
        switch(rnd) {
            case 'f':
                if(sign) {
                    mpz_cdiv_q_2exp(upper->z, man->z, shift);
                } else {
                    mpz_fdiv_q_2exp(upper->z, man->z, shift);
                }
                break;
            case 'c':
                if(sign) {
                    mpz_fdiv_q_2exp(upper->z, man->z, shift);
                } else {
                    mpz_cdiv_q_2exp(upper->z, man->z, shift);
                }
                break;
            case 'd':
                mpz_fdiv_q_2exp(upper->z, man->z, shift);
                break;
            case 'u':
                mpz_cdiv_q_2exp(upper->z, man->z, shift);
                break;
            case 'n':
            default:
                mpz_tdiv_r_2exp(lower->z, man->z, shift);
                mpz_tdiv_q_2exp(upper->z, man->z, shift);
                if(mpz_sgn(lower->z)) {
                    /* lower is not 0 so it must have at least 1 bit set */
                    if(mpz_sizeinbase(lower->z, 2)==shift) {
                        /* lower is >= 1/2 */
                        if(mpz_scan1(lower->z, 0)==shift-1) {
                            /* lower is exactly 1/2 */
                            if(mpz_odd_p(upper->z))
                                carry = 1;
                        } else {
                            carry = 1;
                        }
                    }
                }
                if(carry)
                    mpz_add_ui(upper->z, upper->z, 1);
        }
        if (!(tmp = Py2or3Int_FromLong(shift))) {
            Py_DECREF((PyObject*)upper);
            Py_DECREF((PyObject*)lower);
            return NULL;
        }
        if (!(newexp = PyNumber_Add(exp, tmp))) {
            Py_DECREF((PyObject*)upper);
            Py_DECREF((PyObject*)lower);
            Py_DECREF(tmp);
            return NULL;
        }
        Py_DECREF(tmp);
        bc = prec;
    } else {
        mpz_set(upper->z, man->z);
        newexp = exp;
        Py_INCREF(newexp);
    }

    /* Strip trailing 0 bits. */
    if((zbits = mpz_scan1(upper->z, 0)))
        mpz_tdiv_q_2exp(upper->z, upper->z, zbits);

    if (!(tmp = Py2or3Int_FromLong(zbits))) {
        Py_DECREF((PyObject*)upper);
        Py_DECREF((PyObject*)lower);
        Py_DECREF(newexp);
        return NULL;
    }
    if (!(newexp2 = PyNumber_Add(newexp, tmp))) {
        Py_DECREF((PyObject*)upper);
        Py_DECREF((PyObject*)lower);
        Py_DECREF(tmp);
        Py_DECREF(newexp);
        return NULL;
    }
    Py_DECREF(newexp);
    Py_DECREF(tmp);

    bc -= zbits;
    /* Check if one less than a power of 2 was rounded up. */
    if(!mpz_cmp_ui(upper->z, 1))
        bc = 1;

    Py_DECREF((PyObject*)lower);
    return mpmath_build_mpf(sign, upper, newexp2, bc);
}

static char doc_mpmath_createg[]="\
_mpmath_create(...): helper function for mpmath.\n\
";
static PyObject *
Pympz_mpmath_create(PyObject *self, PyObject *args)
{
    long sign, bc, shift, zbits, carry = 0;
    PyObject *exp = 0, *newexp = 0, *newexp2 = 0, *tmp = 0, *precobj = 0;
    PympzObject *man = 0, *upper = 0, *lower = 0;

    char rnd = 'f';
    long prec = 0;

    if(!PyArg_ParseTuple(args, "O&O|Oc", Pympz_convert_arg, &man, &exp, &precobj, &rnd))
        return NULL;
    assert(Pympz_Check(man));

    /* If the mantissa is 0, return the normalized representation. */
    if(!mpz_sgn(man->z)) {
        return mpmath_build_mpf(0, man, 0, 0);
    }

    upper = Pympz_new();
    lower = Pympz_new();
    if(!upper||!lower) {
        //~ Py_DECREF((PyObject*)precobj);
        Py_DECREF((PyObject*)man);
        Py_XDECREF((PyObject*)upper);
        Py_XDECREF((PyObject*)lower);
        return NULL;
    }

    /* Extract sign, make man positive, and set bit count */
    sign = (mpz_sgn(man->z) == -1);
    mpz_abs(upper->z, man->z);
    bc = mpz_sizeinbase(upper->z, 2);

    /* Check desired precision */
    if((precobj)&&(Py2or3Int_Check(precobj))) prec = abs(Py2or3Int_AsLong(precobj));
    if(!prec) prec = bc;

    shift = bc - prec;
    if(shift>0) {
        switch(rnd) {
            case 'f':
                if(sign) {
                    mpz_cdiv_q_2exp(upper->z, upper->z, shift);
                } else {
                    mpz_fdiv_q_2exp(upper->z, upper->z, shift);
                }
                break;
            case 'c':
                if(sign) {
                    mpz_fdiv_q_2exp(upper->z, upper->z, shift);
                } else {
                    mpz_cdiv_q_2exp(upper->z, upper->z, shift);
                }
                break;
            case 'd':
                mpz_fdiv_q_2exp(upper->z, upper->z, shift);
                break;
            case 'u':
                mpz_cdiv_q_2exp(upper->z, upper->z, shift);
                break;
            case 'n':
            default:
                mpz_tdiv_r_2exp(lower->z, upper->z, shift);
                mpz_tdiv_q_2exp(upper->z, upper->z, shift);
                if(mpz_sgn(lower->z)) {
                    /* lower is not 0 so it must have at least 1 bit set */
                    if(mpz_sizeinbase(lower->z, 2)==shift) {
                        /* lower is >= 1/2 */
                        if(mpz_scan1(lower->z, 0)==shift-1) {
                            /* lower is exactly 1/2 */
                            if(mpz_odd_p(upper->z))
                                carry = 1;
                        } else {
                            carry = 1;
                        }
                    }
                }
                if(carry)
                    mpz_add_ui(upper->z, upper->z, 1);
        }
        if (!(tmp = Py2or3Int_FromLong(shift))) {
            Py_DECREF((PyObject*)upper);
            Py_DECREF((PyObject*)lower);
            return NULL;
        }
        if (!(newexp = PyNumber_Add(exp, tmp))) {
            Py_DECREF((PyObject*)man);
            Py_DECREF((PyObject*)upper);
            Py_DECREF((PyObject*)lower);
            Py_DECREF(tmp);
            return NULL;
        }
        Py_DECREF(tmp);
        bc = prec;
    } else {
        newexp = exp;
        Py_INCREF(newexp);
    }

    /* Strip trailing 0 bits. */
    if((zbits = mpz_scan1(upper->z, 0)))
        mpz_tdiv_q_2exp(upper->z, upper->z, zbits);

    if (!(tmp = Py2or3Int_FromLong(zbits))) {
        Py_DECREF((PyObject*)man);
        Py_DECREF((PyObject*)upper);
        Py_DECREF((PyObject*)lower);
        Py_DECREF(newexp);
        return NULL;
    }
    if (!(newexp2 = PyNumber_Add(newexp, tmp))) {
        Py_DECREF((PyObject*)man);
        Py_DECREF((PyObject*)upper);
        Py_DECREF((PyObject*)lower);
        Py_DECREF(tmp);
        Py_DECREF(newexp);
        return NULL;
    }
    Py_DECREF(newexp);
    Py_DECREF(tmp);

    bc -= zbits;
    /* Check if one less than a power of 2 was rounded up. */
    if(!mpz_cmp_ui(upper->z, 1))
        bc = 1;

    Py_DECREF((PyObject*)lower);
    Py_DECREF((PyObject*)man);
    return mpmath_build_mpf(sign, upper, newexp2, bc);
}

static char doc_is_squarem[]="\
x.is_square(): returns 1 if x is a perfect square, else 0.\n\
";
static char doc_is_squareg[]="\
is_square(x): returns 1 if x is a perfect square, else 0.\n\
x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_is_square(PyObject *self, PyObject *args)
{
    PyObject *res;

    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));

    res = Py_BuildValue("i", mpz_perfect_square_p(Pympz_AS_MPZ(self)));
    Py_DECREF(self);
    return res;
}

static char doc_is_powerm[]="\
x.is_power(): returns 1 if x is a perfect power, i.e., there exist\n\
y, and n>1, such that x==y**n; else, 0.\n\
";
static char doc_is_powerg[]="\
is_power(x): returns 1 if x is a perfect power, i.e., there exist\n\
y, and n>1, such that x==y**n; else, 0. x must be an mpz, or else\n\
gets coerced to one.\n\
";
static PyObject *
Pympz_is_power(PyObject *self, PyObject *args)
{
    PyObject *res;

    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));

    res = Py_BuildValue("i", mpz_perfect_power_p(Pympz_AS_MPZ(self)));
    Py_DECREF(self);
    return res;
}

static char doc_is_primem[]="\
x.is_prime(n=25): returns 2 if x is _certainly_ prime, 1 if x is\n\
_probably_ prime (probability > 1 - 1/2**n), 0 if x is composite.\n\
If x<0, GMP considers x 'prime' iff -x is prime; gmpy reflects this\n\
GMP design choice.\n\
";
static char doc_is_primeg[]="\
is_prime(x,n=25): returns 2 if x is _certainly_ prime, 1 if x is\n\
_probably_ prime (probability > 1 - 1/2**n), 0 if x is composite.\n\
If x<0, GMP considers x 'prime' iff -x is prime; gmpy reflects this\n\
GMP design choice. x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_is_prime(PyObject *self, PyObject *args)
{
    PyObject *res;
    int reps = 25;

    SELF_MPZ_ONE_ARG("|i",&reps);
    assert(Pympz_Check(self));
    if(reps <= 0) {
        PyErr_SetString(PyExc_ValueError,
            "repetition count for is_prime must be positive");
        return NULL;
    }
    res = Py_BuildValue("i", mpz_probab_prime_p(Pympz_AS_MPZ(self), reps));
    Py_DECREF(self);
    return res;
}


static char doc_next_primem[]="\
x.next_prime(): returns the smallest prime number > x.  Note that\n\
GMP may use a probabilistic definition of 'prime', and also that\n\
if x<0 GMP considers x 'prime' iff -x is prime; gmpy reflects these\n\
GMP design choices.\n\
";
static char doc_next_primeg[]="\
next_prime(x): returns the smallest prime number > x.  Note that\n\
GMP may use a probabilistic definition of 'prime', and also that\n\
if x<0 GMP considers x 'prime' iff -x is prime; gmpy reflects these\n\
GMP design choices. x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_next_prime(PyObject *self, PyObject *args)
{
    PympzObject *res;

    SELF_MPZ_NO_ARG;
    assert(Pympz_Check(self));
    if(!(res = Pympz_new()))
        return NULL;
    mpz_nextprime(res->z, Pympz_AS_MPZ(self));
    Py_DECREF(self);
    return (PyObject*)res;
}

static char doc_jacobim[]="\
x.jacobi(y): returns the Jacobi symbol (x|y) (y should be odd\n\
and must be positive).\n\
";
static char doc_jacobig[]="\
jacobi(x,y): returns the Jacobi symbol (x|y) (y should be odd and\n\
must be positive); x and y must be mpz, or else get coerced to mpz.\n\
";
static PyObject *
Pympz_jacobi(PyObject *self, PyObject *args)
{
    PyObject* other;
    PyObject* res;

    SELF_MPZ_ONE_ARG_CONVERTED(&other);
    assert(Pympz_Check(self));
    assert(Pympz_Check(other));
    if(mpz_sgn(Pympz_AS_MPZ(other))<=0) {
        PyErr_SetString(PyExc_ValueError, "jacobi's y must be odd prime > 0");
        Py_DECREF(self); Py_DECREF(other);
        return NULL;
    }
    res = Py_BuildValue("i",
        mpz_jacobi(Pympz_AS_MPZ(self), Pympz_AS_MPZ(other)));
    Py_DECREF(self); Py_DECREF(other);
    return res;
}

static char doc_legendrem[]="\
x.legendre(y): returns the Legendre symbol (x|y) (y should be odd\n\
and must be positive).\n\
";
static char doc_legendreg[]="\
legendre(x,y): returns the Legendre symbol (x|y) (y should be odd\n\
and must be positive); x must be an mpz, or else gets coerced to one.\n\
";
static PyObject *
Pympz_legendre(PyObject *self, PyObject *args)
{
    PyObject* other;
    PyObject* res;

    SELF_MPZ_ONE_ARG_CONVERTED(&other);
    assert(Pympz_Check(self));
    assert(Pympz_Check(other));
    if(mpz_sgn(Pympz_AS_MPZ(other))<=0) {
        PyErr_SetString(PyExc_ValueError, "legendre's y must be odd and > 0");
        Py_DECREF(self); Py_DECREF(other);
        return NULL;
    }
    res = Py_BuildValue("i",
        mpz_legendre(Pympz_AS_MPZ(self), Pympz_AS_MPZ(other)));
    Py_DECREF(self); Py_DECREF(other);
    return res;
}

static char doc_kroneckerm[]="\
x.kronecker(y): returns the Kronecker-Jacobi symbol (x|y).\n\
(At least one of x and y must fit in a plain int).\n\
";
static char doc_kroneckerg[]="\
kronecker(x,y): returns the Kronecker-Jacobi symbol (x|y).\n\
x and y must be mpz, or else get coerced to mpz (at least\n\
one of x and y, however, must also fit in a plain int).\n\
";
static PyObject *
Pympz_kronecker(PyObject *self, PyObject *args)
{
    PyObject* other;
    PyObject* res=0;
    int ires;

    SELF_MPZ_ONE_ARG_CONVERTED(&other);
    assert(Pympz_Check(self));
    assert(Pympz_Check(other));
    if(mpz_fits_ulong_p(Pympz_AS_MPZ(self))) {
        ires = mpz_ui_kronecker(mpz_get_ui(Pympz_AS_MPZ(self)),
            Pympz_AS_MPZ(other));
    } else if(mpz_fits_ulong_p(Pympz_AS_MPZ(other))) {
        ires = mpz_kronecker_ui(Pympz_AS_MPZ(self),
            mpz_get_ui(Pympz_AS_MPZ(other)));
    } else if(mpz_fits_slong_p(Pympz_AS_MPZ(self))) {
        ires = mpz_si_kronecker(mpz_get_si(Pympz_AS_MPZ(self)),
            Pympz_AS_MPZ(other));
    } else if(mpz_fits_slong_p(Pympz_AS_MPZ(other))) {
        ires = mpz_kronecker_si(Pympz_AS_MPZ(self),
            mpz_get_si(Pympz_AS_MPZ(other)));
    } else {
        PyErr_SetString(PyExc_ValueError, "Either arg in Kronecker must fit in an int");
        Py_DECREF(self); Py_DECREF(other);
        return NULL;
    }
    res = Py_BuildValue("i", ires);
    Py_DECREF(self); Py_DECREF(other);
    return res;
}

static char doc_getprecm[]="\
x.getprec(): returns the number of bits of precision in x.\n\
";
static char doc_getprecg[]="\
getprec(x): returns the number of bits of precision in x,\n\
which must be an mpf or else gets coerced to one.\n\
";
static PyObject *
Pympf_getprec(PyObject *self, PyObject *args)
{
    unsigned long precres;

    SELF_MPF_NO_ARG;
    assert(Pympf_Check(self));

    precres = mpf_get_prec(Pympf_AS_MPF(self));
    Py_DECREF(self);
    return Py_BuildValue("l", precres);
}

static char doc_getrprecm[]="\
x.getrprec(): returns the number of bits of precision in x\n\
_that were requested_ (.getprec may return a higher value).\n\
";
static char doc_getrprecg[]="\
getrprec(x): returns the number of bits of precision in x,\n\
_that were requested_ (getprec may return a higher value).\n\
x must be an mpf, or else gets coerced to one.\n\
";
static PyObject *
Pympf_getrprec(PyObject *self, PyObject *args)
{
    int precres;

    SELF_MPF_NO_ARG;
    assert(Pympf_Check(self));

    precres = ((PympfObject*)self)->rebits;
    Py_DECREF(self);
    return Py_BuildValue("i", precres);
}

static char doc_setprecm[]="\
x.setprec(n): sets the number of bits of precision in x to\n\
be _at least_ n (n>0).  ***note that this alters x***!!!\n\
Please use x.round(); it returns a new value instead of\n\
altering the existing value. setprec() will be removed in a\n\
future release.\n\
";
static PyObject *
Pympf_setprec(PyObject *self, PyObject *args)
{
    long precres;

    assert(self);   /* exists _as method, ONLY_ */
    assert(Pympf_Check(self));

    if(PyErr_Warn(PyExc_DeprecationWarning,
        "setprec() will be removed, use round() instead")) {
        return 0;
    }
    ONE_ARG("setprec", "l", &precres);
    if(precres<0) {
        PyErr_SetString(PyExc_ValueError, "n must be >=0");
        return 0;
    }

    mpf_set_prec(Pympf_AS_MPF(self), precres);
    ((PympfObject*)self)->rebits = precres;
    mpf_normalize((PympfObject*)self);
    return Py_BuildValue("");
}

static char doc_froundm[] = "\
x.round(n): returns x rounded to least n bits. Actual precision will\n\
be a multiple of gmp_limbsize().\n\
";
static char doc_froundg[] = "\
fround(x, n): returns x rounded to least n bits. Actual precision will\n\
be a multiple of gmp_limbsize(). x an mpf or coerced to an mpf.\n\
";
static PyObject *
Pympf_round(PyObject *self, PyObject *args)
{
    /* Should really get default precision. */
    long prec = 64;
    PyObject *s;

    SELF_MPF_ONE_ARG("|l",&prec);
    assert(Pympf_Check(self));
    s = (PyObject*)mpf2mpf((PympfObject*)self, prec);
    Py_DECREF(self);
    return s;
}


static char doc_reldiffm[] = "\
x.reldiff(y): returns the relative difference between x and y,\n\
where y can be any number and gets coerced to an mpf; result is\n\
a non-negative mpf roughly equal to abs(x-y)/((abs(x)+abs(y))/2).\n\
";
static char doc_reldiffg[] = "\
reldiff(x,y): returns the relative difference between x and y,\n\
where x and y can be any numbers and get coerced to mpf; result is\n\
a non-negative mpf roughly equal to abs(x-y)/((abs(x)+abs(y))/2).\n\
";
static PyObject *
Pympf_doreldiff(PyObject *self, PyObject *args)
{
    PympfObject *op;
    PyObject *res;

    SELF_MPF_ONE_ARG_CONVERTED(&op);
    assert(Pympf_Check(self));

    res = Pympf_reldiff((PyObject*)self, (PyObject*)op);
    Py_DECREF(self); Py_DECREF((PyObject*)op);

    return res;
}

static char doc_fsignm[]="\
x.sign(): returns -1, 0, or +1, if x is negative, 0, positive.\n\
";
static char doc_fsigng[]="\
fsign(x): returns -1, 0, or +1, if x is negative, 0, positive;\n\
x must be an mpf, or else gets coerced to one.\n\
";
static PyObject *
Pympf_sign(PyObject *self, PyObject *args)
{
    int sign;

    SELF_MPF_NO_ARG;
    assert(Pympf_Check(self));

    sign = mpf_sgn(Pympf_AS_MPF(self));
    Py_DECREF(self);
    return Py_BuildValue("i", sign);
}

/* random-number issues -- TODO: repackage as separate functions */
static char doc_rand[]="\
rand(opt[,arg]): expose various GMP random-number operations,\n\
    depending on value of parameter 'opt' (a string) -- arg is\n\
    normally an int or mpz (or else gets coerced to mpz), but\n\
    must be a Python mutable sequence when opt is 'shuf':\n\
'init': initialize random-state to support arg bits of 'good\n\
    randomness', for arg between 1 and 128 (default 32).\n\
    May be called again to change this 'random-quality'.\n\
'qual': returns the number-of-bits-of-good-randomness (0 if\n\
    the random-generator not yet initialized), arg ignored.\n\
'seed': set/reset random-state's seed to arg.\n\
'save': get random-state seed (for saving) - arg is ignored.\n\
'next': get random mpz, 0 (included) to arg (excluded)\n\
    (default range is 0..2**31).\n\
'floa': get random mpf, range 0<=x<1, with arg meaningful bits\n\
    (default, if arg missing or 0, is current 'random quality').\n\
'shuf': random shuffle of Python list (or other mutable\n\
    sequence) 'arg'; shuffle is in-place, None returned.\n\
";
static gmp_randstate_t randstate;
static int randinited=0;
static int randquality=0;
#if (__GNU_MP_VERSION==4) && (__GNU_MP_VERSION_MINOR>=2)
#  define do_randinit(state, size) gmp_randinit_lc_2exp_size(state, size)
#  define SEEDOF(x)  ( *(mpz_t*)((x)->_mp_seed->_mp_d) )
#else
#  define do_randinit(state, size) gmp_randinit(state, GMP_RAND_ALG_LC, size)
#  if (__GNU_MP_VERSION==4)
#    define SEEDOF(x) ((x)->_mp_seed)
#  else
#    define SEEDOF(x) ((x)->seed)
#  endif
#endif

static int randbits(PyObject* arg)
{
    int req = arg?mpz_get_si(Pympz_AS_MPZ(arg)):0;
    return req?req:randquality;
}
static int randinit(int size)
{
    if(size==-1) size = 32;
    if(size<=0 || size>128) {
        PyErr_SetString(PyExc_ValueError, "size must be in 1..128");
        return 0;
    }
    if(randinited)
        gmp_randclear(randstate);
    do_randinit(randstate, size);
    randquality = size;
    randinited = 1;
    return 1;
}
static PyObject *random_shuffle(PyObject* seq)
{
    int i, j;
    int len = PySequence_Length(seq);
    PyObject* result;
    mpz_t temp1, temp2;
    mpz_inoc(temp1);
    mpz_inoc(temp2);
    mpz_set_si(temp1,len);

    result = Py_BuildValue("");
    for(i=0; i<(len-1); ++i) {
        mpz_urandomm(temp2, randstate, temp1);
        j = mpz_get_si(temp2);
        if(j!=0) {
            int rc;
            PyObject* temp = PySequence_GetItem(seq, i);
            rc = PySequence_SetItem(seq, i, PySequence_GetItem(seq, i+j));
            if(!rc) {
                rc = PySequence_SetItem(seq, i+j, temp);
            }
            if(rc) {
                Py_DECREF(result);
                result = 0;
                break;
            }
        }
        mpz_sub_ui(temp1, temp1, 1);
    }

    mpz_cloc(temp1);
    mpz_cloc(temp2);
    return result;
}
static PyObject *
Pygmpy_rand(PyObject *self, PyObject *args)
{
    char* opt;
    int iseq=0;
    PyObject* arg=0;
    PyObject* result=0;

    if(!PyArg_ParseTuple(args, "s|O&", &opt, Pympz_convert_arg, &arg)) {
        int retry = PyArg_ParseTuple(args, "sO", &opt, &arg);
        if(retry && 0==strncmp(opt,"shuf",4) && PySequence_Check(arg)) {
            PyErr_Clear();
            iseq=1;
            Py_INCREF(arg);
        } else {
            return 0;
        }
    }
    assert(!arg || iseq || Pympz_Check(arg));

    if(0==strncmp(opt,"init",4)) {
        int ok = randinit(arg?mpz_get_si(Pympz_AS_MPZ(arg)):-1);
        if(ok)
            result = Py_BuildValue("");
    } else if(0==strncmp(opt,"qual",4)) {
        result = Py_BuildValue("i", randquality);
    } else if(0==strncmp(opt,"seed",4)) {
        int ok=1;
        if(!randinited) {
            ok = randinit(-1);
        }
        if(ok) {
            if(arg) gmp_randseed(randstate, Pympz_AS_MPZ(arg));
            else gmp_randseed_ui(randstate, rand());
            result = Py_BuildValue("");
        }
    } else if(0==strncmp(opt,"save",4)) {
        if(!randinited) {
            PyErr_SetString(PyExc_RuntimeError, "can't save before init");
        } else {
            PympzObject *resob = Pympz_new();
            if(resob) {
                mpz_set(resob->z, SEEDOF(randstate));
                result = (PyObject*)resob;
            }
        }
    } else if(0==strncmp(opt,"next",4)) {
        int ok=1;
        if(!randinited) {
            ok = randinit(-1);
        }
        if(ok) {
            PympzObject *resob = Pympz_new();
            if(resob) {
                if(arg) mpz_urandomm(resob->z, randstate, Pympz_AS_MPZ(arg));
                else mpz_urandomb(resob->z, randstate, 31);
                result = (PyObject*)resob;
            }
        }
    } else if(0==strncmp(opt,"floa",4)) {
        int ok=1;
        if(!randinited) {
            ok = randinit(-1);
        }
        if(ok) {
            int bits = randbits(arg);
            PympfObject *resob = Pympf_new(bits);
            if(bits>0 && resob) {
                mpf_urandomb(resob->f, randstate, bits);
                mpf_normalize(resob);
                result = (PyObject*)resob;
            } else if(bits<=0) {
                if(resob)
                    mpf_clear(resob->f);
                PyErr_SetString(PyExc_ValueError,
                    "'floa' needs arg>=0");
            }
        }
    } else if(0==strncmp(opt,"shuf",4)) {
        if(!iseq) {
            PyErr_SetString(PyExc_TypeError, "'shuf' needs mutable sequence");
        } else {
            int ok=1;
            if(!randinited) {
                ok = randinit(-1);
            }
            if(ok) {
                result = random_shuffle(arg);
            }
        }
    } else {
        char buff[128];
        sprintf(buff,"unknown option '%s'",opt);
        PyErr_SetString(PyExc_ValueError, buff);
    }
    if(arg) { Py_DECREF(arg); }
    return result;
}

/* method-tables */

#if PY_MAJOR_VERSION >= 3
static PyNumberMethods mpz_number_methods =
{
    (binaryfunc) Pympz_add,        /* binaryfunc nb_add;                  */
    (binaryfunc) Pympz_sub,        /* binaryfunc nb_subtract;             */
    (binaryfunc) Pympz_mul,        /* binaryfunc nb_multiply;             */
    (binaryfunc) Pympz_rem,        /* binaryfunc nb_remaider;             */
    (binaryfunc) Pympz_divmod,     /* binaryfunc nb_divmod;               */
    (ternaryfunc) Pympz_pow,       /* ternaryfunc nb_power;               */
    (unaryfunc) Pympz_neg,         /* unaryfunc nb_negative;              */
    (unaryfunc) Pympz_pos,         /* unaryfunc nb_positive;              */
    (unaryfunc) Pympz_abs,         /* unaryfunc nb_absolute;              */
    (inquiry) Pympz_nonzero,       /* inquiry nb_bool;                    */
    (unaryfunc) Pympz_com,         /* unaryfunc nb_invert;                */
    (binaryfunc) Pympz_lshift,     /* binaryfunc nb_lshift;               */
    (binaryfunc) Pympz_rshift,     /* binaryfunc nb_rshift;               */
    (binaryfunc) Pympz_and,        /* binaryfunc nb_and;                  */
    (binaryfunc) Pympz_xor,        /* binaryfunc nb_xor;                  */
    (binaryfunc) Pympz_ior,        /* binaryfunc nb_or;                   */
    (unaryfunc) mpz2long,          /* unaryfunc nb_int                    */
        0,                         /* void *nb_reserved;                  */
    (unaryfunc) mpz2float,         /* unaryfunc nb_float;                 */
        0,                         /* binaryfunc nb_inplace_add;          */
        0,                         /* binaryfunc nb_inplace_subtract;     */
        0,                         /* binaryfunc nb_inplace_multiply;     */
        0,                         /* binaryfunc nb_inplace_remainder;    */
        0,                         /* ternaryfunc nb_inplace_power;       */
        0,                         /* binaryfunc nb_inplace_lshift;       */
        0,                         /* binaryfunc nb_inplace_rshift;       */
        0,                         /* binaryfunc nb_inplace_and;          */
        0,                         /* binaryfunc nb_inplace_xor;          */
        0,                         /* binaryfunc nb_inplace_or;           */
    (binaryfunc) Pympz_div,        /* binaryfunc nb_floor_divide;         */
    (binaryfunc) Pympany_truediv,  /* binaryfunc nb_true_divide;          */
        0,                         /* binaryfunc nb_inplace_floor_divide; */
        0,                         /* binaryfunc nb_inplace_true_divide;  */
    (unaryfunc)  Pympz_asindex,    /* unaryfunc nb_index;                 */
};

#else
static PyNumberMethods mpz_number_methods =
{
    (binaryfunc) Pympz_add,
    (binaryfunc) Pympz_sub,
    (binaryfunc) Pympz_mul,
    (binaryfunc) Pympz_div,
    (binaryfunc) Pympz_rem,
    (binaryfunc) Pympz_divmod,
    (ternaryfunc) Pympz_pow,
    (unaryfunc) Pympz_neg,
    (unaryfunc) Pympz_pos,  /* nb_positive */
    (unaryfunc) Pympz_abs,
    (inquiry) Pympz_nonzero,
    (unaryfunc) Pympz_com,
    (binaryfunc) Pympz_lshift,
    (binaryfunc) Pympz_rshift,
    (binaryfunc) Pympz_and,
    (binaryfunc) Pympz_xor,
    (binaryfunc) Pympz_ior,
    (coercion) 0,
    (unaryfunc) mpz2int,
    (unaryfunc) mpz2long,
    (unaryfunc) mpz2float,
    (unaryfunc) Pympz_oct,
    (unaryfunc) Pympz_hex,
        0, /* binaryfunc nb_inplace_add;        */
        0, /* binaryfunc nb_inplace_subtract;   */
        0, /* binaryfunc nb_inplace_multiply;   */
        0, /* binaryfunc nb_inplace_divide;     */
        0, /* binaryfunc nb_inplace_remainder;  */
        0, /* ternaryfunc nb_inplace_power;     */
        0, /* binaryfunc nb_inplace_lshift;     */
        0, /* binaryfunc nb_inplace_rshift;     */
        0, /* binaryfunc nb_inplace_and;        */
        0, /* binaryfunc nb_inplace_xor;        */
        0, /* binaryfunc nb_inplace_or;         */

    (binaryfunc) Pympz_div,             /* binaryfunc nb_floor_divide; */
    (binaryfunc) Pympany_truediv,       /* binaryfunc nb_true_divide; */
        0, /* binaryfunc nb_inplace_floor_divide;       */
        0, /* binaryfunc nb_inplace_true_divide;        */
#if Py_TPFLAGS_HAVE_INDEX
    (unaryfunc) Pympz_asindex,            /* unaryfunc nb_index; */
#endif
};
#endif

#if PY_MAJOR_VERSION >= 3
static PyNumberMethods mpq_number_methods =
{
    (binaryfunc) Pympq_add,        /* binaryfunc nb_add;                  */
    (binaryfunc) Pympq_sub,        /* binaryfunc nb_subtract;             */
    (binaryfunc) Pympq_mul,        /* binaryfunc nb_multiply;             */
        0,                         /* binaryfunc nb_remaider;             */
        0,                         /* binaryfunc nb_divmod;               */
    (ternaryfunc) Pympq_pow,       /* ternaryfunc nb_power;               */
    (unaryfunc) Pympq_neg,         /* unaryfunc nb_negative;              */
    (unaryfunc) Pympq_pos,         /* unaryfunc nb_positive;              */
    (unaryfunc) Pympq_abs,         /* unaryfunc nb_absolute;              */
    (inquiry) Pympq_nonzero,       /* inquiry nb_bool;                    */
        0,                         /* unaryfunc nb_invert;                */
        0,                         /* binaryfunc nb_lshift;               */
        0,                         /* binaryfunc nb_rshift;               */
        0,                         /* binaryfunc nb_and;                  */
        0,                         /* binaryfunc nb_xor;                  */
        0,                         /* binaryfunc nb_or;                   */
    (unaryfunc) mpq2long,          /* unaryfunc nb_int                    */
        0,                         /* void *nb_reserved;                  */
    (unaryfunc) mpq2float,         /* unaryfunc nb_float;                 */
        0,                         /* binaryfunc nb_inplace_add;          */
        0,                         /* binaryfunc nb_inplace_subtract;     */
        0,                         /* binaryfunc nb_inplace_multiply;     */
        0,                         /* binaryfunc nb_inplace_remainder;    */
        0,                         /* ternaryfunc nb_inplace_power;       */
        0,                         /* binaryfunc nb_inplace_lshift;       */
        0,                         /* binaryfunc nb_inplace_rshift;       */
        0,                         /* binaryfunc nb_inplace_and;          */
        0,                         /* binaryfunc nb_inplace_xor;          */
        0,                         /* binaryfunc nb_inplace_or;           */
    (binaryfunc) Pympq_floordiv,        /* binaryfunc nb_floor_divide;         */
    (binaryfunc) Pympq_div,        /* binaryfunc nb_true_divide;          */
        0,                         /* binaryfunc nb_inplace_floor_divide; */
        0,                         /* binaryfunc nb_inplace_true_divide;  */
        0,                         /* unaryfunc nb_index;                 */
};
#else
static PyNumberMethods mpq_number_methods =
{
    (binaryfunc) Pympq_add,
    (binaryfunc) Pympq_sub,
    (binaryfunc) Pympq_mul,
    (binaryfunc) Pympq_div,
    (binaryfunc) 0,     /* no rem */
    (binaryfunc) 0,     /* no divmod */
    (ternaryfunc) Pympq_pow,
    (unaryfunc) Pympq_neg,
    (unaryfunc) Pympq_pos,  /* nb_positive */
    (unaryfunc) Pympq_abs,
    (inquiry) Pympq_nonzero,
    (unaryfunc) 0,      /* no bit-complement */
    (binaryfunc) 0,     /* no left-shift */
    (binaryfunc) 0,     /* no right-shift */
    (binaryfunc) 0,     /* no bit-and */
    (binaryfunc) 0,     /* no bit-xor */
    (binaryfunc) 0,     /* no bit-ior */
    (coercion) 0,
    (unaryfunc) mpq2int,
    (unaryfunc) mpq2long,
    (unaryfunc) mpq2float,
    (unaryfunc) 0,      /* no oct */
    (unaryfunc) 0,      /* no hex */
        0, /* binaryfunc nb_inplace_add;        */
        0, /* binaryfunc nb_inplace_subtract;   */
        0, /* binaryfunc nb_inplace_multiply;   */
        0, /* binaryfunc nb_inplace_divide;     */
        0, /* binaryfunc nb_inplace_remainder;  */
        0, /* ternaryfunc nb_inplace_power;     */
        0, /* binaryfunc nb_inplace_lshift;     */
        0, /* binaryfunc nb_inplace_rshift;     */
        0, /* binaryfunc nb_inplace_and;        */
        0, /* binaryfunc nb_inplace_xor;        */
        0, /* binaryfunc nb_inplace_or;         */

    (binaryfunc) Pympq_floordiv,                /* binaryfunc nb_floor_divide; */
    (binaryfunc) Pympany_truediv,       /* binaryfunc nb_true_divide; */
        0, /* binaryfunc nb_inplace_floor_divide;       */
        0, /* binaryfunc nb_inplace_true_divide;        */
};
#endif

#if PY_MAJOR_VERSION >= 3
static PyNumberMethods mpf_number_methods =
{
    (binaryfunc) Pympf_add,        /* binaryfunc nb_add;                  */
    (binaryfunc) Pympf_sub,        /* binaryfunc nb_subtract;             */
    (binaryfunc) Pympf_mul,        /* binaryfunc nb_multiply;             */
        0,                         /* binaryfunc nb_remaider;             */
        0,                         /* binaryfunc nb_divmod;               */
    (ternaryfunc) Pympf_pow,       /* ternaryfunc nb_power;               */
    (unaryfunc) Pympf_neg,         /* unaryfunc nb_negative;              */
    (unaryfunc) Pympf_pos,         /* unaryfunc nb_positive;              */
    (unaryfunc) Pympf_abs,         /* unaryfunc nb_absolute;              */
    (inquiry) Pympf_nonzero,       /* inquiry nb_bool;                    */
        0,                         /* unaryfunc nb_invert;                */
        0,                         /* binaryfunc nb_lshift;               */
        0,                         /* binaryfunc nb_rshift;               */
        0,                         /* binaryfunc nb_and;                  */
        0,                         /* binaryfunc nb_xor;                  */
        0,                         /* binaryfunc nb_or;                   */
    (unaryfunc) mpf2long,          /* unaryfunc nb_int                    */
        0,                         /* void *nb_reserved;                  */
    (unaryfunc) mpf2float,         /* unaryfunc nb_float;                 */
        0,                         /* binaryfunc nb_inplace_add;          */
        0,                         /* binaryfunc nb_inplace_subtract;     */
        0,                         /* binaryfunc nb_inplace_multiply;     */
        0,                         /* binaryfunc nb_inplace_remainder;    */
        0,                         /* ternaryfunc nb_inplace_power;       */
        0,                         /* binaryfunc nb_inplace_lshift;       */
        0,                         /* binaryfunc nb_inplace_rshift;       */
        0,                         /* binaryfunc nb_inplace_and;          */
        0,                         /* binaryfunc nb_inplace_xor;          */
        0,                         /* binaryfunc nb_inplace_or;           */
    (binaryfunc) Pympf_floordiv,   /* binaryfunc nb_floor_divide;         */
    (binaryfunc) Pympany_truediv,  /* binaryfunc nb_true_divide;          */
        0,                         /* binaryfunc nb_inplace_floor_divide; */
        0,                         /* binaryfunc nb_inplace_true_divide;  */
        0,                         /* unaryfunc nb_index;                 */
};
#else
static PyNumberMethods mpf_number_methods =
{
    (binaryfunc) Pympf_add,
    (binaryfunc) Pympf_sub,
    (binaryfunc) Pympf_mul,
    (binaryfunc) Pympf_div,
    (binaryfunc) 0,     /* no rem */
    (binaryfunc) 0,     /* no divmod */
    (ternaryfunc) Pympf_pow,
    (unaryfunc) Pympf_neg,
    (unaryfunc) Pympf_pos,  /* nb_positive */
    (unaryfunc) Pympf_abs,
    (inquiry) Pympf_nonzero,
    (unaryfunc) 0,      /* no bit-complement */
    (binaryfunc) 0,     /* no left-shift */
    (binaryfunc) 0,     /* no right-shift */
    (binaryfunc) 0,     /* no bit-and */
    (binaryfunc) 0,     /* no bit-xor */
    (binaryfunc) 0,     /* no bit-ior */
    (coercion) 0,      /* was coerce */
    (unaryfunc) mpf2int,
    (unaryfunc) mpf2long,
    (unaryfunc) mpf2float,
    (unaryfunc) 0,      /* no oct */
    (unaryfunc) 0,      /* no hex */
        0, /* binaryfunc nb_inplace_add;        */
        0, /* binaryfunc nb_inplace_subtract;   */
        0, /* binaryfunc nb_inplace_multiply;   */
        0, /* binaryfunc nb_inplace_divide;     */
        0, /* binaryfunc nb_inplace_remainder;  */
        0, /* ternaryfunc nb_inplace_power;     */
        0, /* binaryfunc nb_inplace_lshift;     */
        0, /* binaryfunc nb_inplace_rshift;     */
        0, /* binaryfunc nb_inplace_and;        */
        0, /* binaryfunc nb_inplace_xor;        */
        0, /* binaryfunc nb_inplace_or;         */

    (binaryfunc) Pympf_floordiv,        /* binaryfunc nb_floor_divide; */
    (binaryfunc) Pympany_truediv,       /* binaryfunc nb_true_divide; */
        0, /* binaryfunc nb_inplace_floor_divide;       */
        0, /* binaryfunc nb_inplace_true_divide;        */
};
#endif

static PyMethodDef Pygmpy_methods [] =
{
    { "version", Pygmpy_get_version, 1, doc_version },
    { "_cvsid", Pygmpy_get_cvsid, 1, doc_cvsid },
    { "gmp_version", Pygmpy_get_gmp_version, 1, doc_gmp_version },
    { "mpir_version", Pygmpy_get_mpir_version, 1, doc_mpir_version },
    { "license", Pygmpy_get_license, 1, doc_license },
    { "gmp_limbsize", Pygmpy_get_gmp_limbsize, 1, doc_gmp_limbsize },
    { "set_debug", Pygmpy_set_debug, 1, doc_set_debug },
    { "set_minprec", Pygmpy_set_minprec, 1, doc_set_minprec },
    { "set_tagoff", Pygmpy_set_tagoff, 1, doc_set_tagoff },
    { "set_fcoform", Pygmpy_set_fcoform, 1, doc_set_fcoform },
    { "get_zcache", Pygmpy_get_zcache, 1, doc_get_zcache },
    { "set_zcache", Pygmpy_set_zcache, 1, doc_set_zcache },
    { "get_qcache", Pygmpy_get_qcache, 1, doc_get_qcache },
    { "set_qcache", Pygmpy_set_qcache, 1, doc_set_qcache },
    { "get_zconst", Pygmpy_get_zconst, 1, doc_get_zconst },
    { "set_zconst", Pygmpy_set_zconst, 1, doc_set_zconst },
    { "mpz", Pygmpy_mpz, 1, doc_mpz },
    { "mpq", Pygmpy_mpq, 1, doc_mpq },
    { "mpf", Pygmpy_mpf, 1, doc_mpf },
    { "gcd", Pygmpy_gcd, 1, doc_gcd },
    { "gcdext", Pygmpy_gcdext, 1, doc_gcdext },
    { "lcm", Pygmpy_lcm, 1, doc_lcm },
    { "divm", Pygmpy_divm, 1, doc_divm },
    { "fac", Pygmpy_fac, 1, doc_fac },
    { "fib", Pygmpy_fib, 1, doc_fib },
    { "pi", Pygmpy_pi, 1, doc_pi },
    { "rand", Pygmpy_rand, 1, doc_rand },
    { "sqrt", Pympz_sqrt, 1, doc_sqrtg },
    { "sqrtrem", Pympz_sqrtrem, 1, doc_sqrtremg },
    { "is_square", Pympz_is_square, 1, doc_is_squareg },
    { "is_power", Pympz_is_power, 1, doc_is_powerg },
    { "is_prime", Pympz_is_prime, 1, doc_is_primeg },
    { "next_prime", Pympz_next_prime, 1, doc_next_primeg },
    { "jacobi", Pympz_jacobi, 1, doc_jacobig },
    { "legendre", Pympz_legendre, 1, doc_legendreg },
    { "kronecker", Pympz_kronecker, 1, doc_kroneckerm },
    { "binary", Pympz_binary, 1, doc_binaryg },
    { "digits", Pympz_digits, 1, doc_digitsg },
    { "numdigits", Pympz_numdigits, 1, doc_numdigitsg },
    { "bit_length", Pympz_bit_length, 1, doc_bit_lengthg },
    { "lowbits", Pympz_lowbits, 1, doc_lowbitsg },
    { "getbit", Pympz_getbit, 1, doc_getbitg },
    { "setbit", Pympz_setbit, 1, doc_setbitg },
    { "popcount", Pympz_popcount, 1, doc_popcountg },
    { "hamdist", Pympz_hamdist, 1, doc_hamdistg },
    { "divexact", Pympz_divexact, 1, doc_divexactg },
    { "scan0", Pympz_scan0, 1, doc_scan0g },
    { "scan1", Pympz_scan1, 1, doc_scan1g },
    { "root", Pympz_root, 1, doc_rootg },
    { "bincoef", Pympz_bincoef, 1, doc_bincoefg },
    { "comb", Pympz_bincoef, 1, doc_combg },
    { "remove", Pympz_remove, 1, doc_removeg },
    { "invert", Pympz_invert, 1, doc_invertg },
    { "_copy", Pympz_copy, 1 },
    { "sign", Pympz_sign, 1, doc_signg },
    { "fsqrt", Pympf_sqrt, 1, doc_fsqrtg },
    { "qsign", Pympq_sign, 1, doc_qsigng },
    { "numer", Pympq_numer, 1, doc_numerg },
    { "denom", Pympq_denom, 1, doc_denomg },
    { "qbinary", Pympq_binary, 1, doc_qbinaryg },
    { "qdigits", Pympq_digits, 1, doc_qdigitsg },
    { "_qcopy", Pympq_copy, 1 },
    { "qsign", Pympq_sign, 1, doc_qsigng },
    { "qdiv", Pympq_qdiv, 1, doc_qdivg },
    { "reldiff", Pympf_doreldiff, 1, doc_reldiffg },
    { "fbinary", Pympf_binary, 1, doc_fbinaryg },
    { "fdigits", Pympf_digits, 1, doc_fdigitsg },
    { "fround", Pympf_round, 1, doc_froundg },
    { "getprec", Pympf_getprec, 1, doc_getprecg },
    { "getrprec", Pympf_getrprec, 1, doc_getrprecg },
    /* NOT here: { "setprec", Pympf_setprec, 1, doc_setprecg }, */
    { "_fcopy", Pympf_copy, 1 },
    { "fsign", Pympf_sign, 1, doc_fsigng },
    { "f2q", Pympf_f2q, 1, doc_f2qg },
    { "ceil", Pympf_ceil, 1, doc_ceilg },
    { "floor", Pympf_floor, 1, doc_floorg },
    { "trunc", Pympf_trunc, 1, doc_truncg },
    { "_mpmath_normalize", Pympz_mpmath_normalize, 1, doc_mpmath_normalizeg },
    { "_mpmath_create", Pympz_mpmath_create, 1, doc_mpmath_createg },

    { NULL, NULL, 1}
};

static PyMethodDef Pympz_methods [] =
{
    { "sqrt", Pympz_sqrt, 1, doc_sqrtm },
    { "sqrtrem", Pympz_sqrtrem, 1, doc_sqrtremm },
    { "is_square", Pympz_is_square, 1, doc_is_squarem },
    { "is_power", Pympz_is_power, 1, doc_is_powerm },
    { "is_prime", Pympz_is_prime, 1, doc_is_primem },
    { "next_prime", Pympz_next_prime, 1, doc_next_primem },
    { "jacobi", Pympz_jacobi, 1, doc_jacobim },
    { "legendre", Pympz_legendre, 1, doc_legendrem },
    { "kronecker", Pympz_kronecker, 1, doc_kroneckerg },
    { "binary", Pympz_binary, 1, doc_binarym },
    { "digits", Pympz_digits, 1, doc_digitsm },
    { "numdigits", Pympz_numdigits, 1, doc_numdigitsm },
    { "bit_length", Pympz_bit_length, 1, doc_bit_lengthm },
    { "lowbits", Pympz_lowbits, 1, doc_lowbitsm },
    { "getbit", Pympz_getbit, 1, doc_getbitm },
    { "setbit", Pympz_setbit, 1, doc_setbitm },
    { "popcount", Pympz_popcount, 1, doc_popcountm },
    { "hamdist", Pympz_hamdist, 1, doc_hamdistm },
    { "divexact", Pympz_divexact, 1, doc_divexactm },
    { "scan0", Pympz_scan0, 1, doc_scan0m },
    { "scan1", Pympz_scan1, 1, doc_scan1m },
    { "root", Pympz_root, 1, doc_rootm },
    { "bincoef", Pympz_bincoef, 1, doc_bincoefm },
    { "comb", Pympz_bincoef, 1, doc_combm },
    { "remove", Pympz_remove, 1, doc_removem },
    { "invert", Pympz_invert, 1, doc_invertm },
    { "_copy", Pympz_copy, 1 },
    { "sign", Pympz_sign, 1, doc_signm },
    { "qdiv", Pympq_qdiv, 1, doc_qdivm },
    { NULL, NULL, 1 }
};

static PyMethodDef Pympq_methods [] =
{
    { "sign", Pympq_sign, 1, doc_qsignm },
    { "numer", Pympq_numer, 1, doc_numerm },
    { "denom", Pympq_denom, 1, doc_denomm },
    { "_copy", Pympq_copy, 1 },
    { "binary", Pympq_binary, 1, doc_qbinarym },
    { "digits", Pympq_digits, 1, doc_qdigitsm },
    { "qdiv", Pympq_qdiv, 1, doc_qdivm },
    { NULL, NULL, 1 }
};

static PyMethodDef Pympf_methods [] =
{
    { "reldiff", Pympf_doreldiff, 1, doc_reldiffm },
    { "binary", Pympf_binary, 1, doc_fbinarym },
    { "digits", Pympf_digits, 1, doc_fdigitsm },
    { "round", Pympf_round, 1, doc_froundm },
    { "getprec", Pympf_getprec, 1, doc_getprecm },
    { "getrprec", Pympf_getrprec, 1, doc_getrprecm },
    { "setprec", Pympf_setprec, 1, doc_setprecm },
    { "_copy", Pympf_copy, 1 },
    { "sign", Pympf_sign, 1, doc_fsignm },
    { "sqrt", Pympf_sqrt, 1, doc_fsqrtm },
    { "qdiv", Pympq_qdiv, 1, doc_qdivm },
    { "f2q", Pympf_f2q, 1, doc_f2qm },
    { "ceil", Pympf_ceil, 1, doc_ceilm },
    { "floor", Pympf_floor, 1, doc_floorm },
    { "trunc", Pympf_trunc, 1, doc_truncm },
    { NULL, NULL, 1 }
};

static PyTypeObject Pympz_Type =
{
    /* PyObject_HEAD_INIT(&PyType_Type) */
#if PY_MAJOR_VERSION >= 3
    PyVarObject_HEAD_INIT(0, 0)
#else
    PyObject_HEAD_INIT(0)
        0,                          /* ob_size          */
#endif
    "mpz",                          /* tp_name          */
    sizeof(PympzObject),            /* tp_basicsize     */
        0,                          /* tp_itemsize      */
    /* methods */
    (destructor) Pympz_dealloc,     /* tp_dealloc       */
        0,                          /* tp_print         */
        0,                          /* tp_getattr       */
        0,                          /* tp_setattr       */
        0,                          /* tp_reserved      */
    (reprfunc) mpz2repr,            /* tp_repr          */
    &mpz_number_methods,            /* tp_as_number     */
        0,                          /* tp_as_sequence   */
        0,                          /* tp_as_mapping    */
    (hashfunc) Pympz_hash,          /* tp_hash          */
        0,                          /* tp_call          */
    (reprfunc) mpz2str,             /* tp_str           */
        0,                          /* tp_getattro      */
        0,                          /* tp_setattro      */
        0,                          /* tp_as_buffer     */
#if PY_MAJOR_VERSION >= 3
    Py_TPFLAGS_DEFAULT,             /* tp_flags         */
#else
    Py_TPFLAGS_HAVE_INDEX|Py_TPFLAGS_HAVE_RICHCOMPARE|Py_TPFLAGS_CHECKTYPES,      /* tp_flags */
#endif
    "GNU Multi Precision signed integer",                   /* tp_doc   */
        0,                          /* tp_traverse      */
        0,                          /* tp_clear         */
    (richcmpfunc)&mpz_richcompare,  /* tp_richcompare   */
        0,                          /* tp_weaklistoffset*/
        0,                          /* tp_iter          */
        0,                          /* tp_iternext      */
    Pympz_methods,                  /* tp_methods       */
};

static PyTypeObject Pympq_Type =
{
    /* PyObject_HEAD_INIT(&PyType_Type) */
#if PY_MAJOR_VERSION >= 3
    PyVarObject_HEAD_INIT(NULL, 0)
#else
    PyObject_HEAD_INIT(0)
        0,                                  /* ob_size          */
#endif
    "mpq",                                  /* tp_name          */
    sizeof(PympqObject),                    /* tp_basicsize     */
        0,                                  /* tp_itemsize      */
    /* methods */
    (destructor) Pympq_dealloc,             /* tp_dealloc       */
        0,                                  /* tp_print         */
        0,                                  /* tp_getattr       */
        0,                                  /* tp_setattr       */
        0,                                  /* tp_reserved      */
    (reprfunc) mpq2repr,                    /* tp_repr          */
    &mpq_number_methods,                    /* tp_as_number     */
        0,                                  /* tp_as_sequence   */
        0,                                  /* tp_as_mapping    */
    (hashfunc) Pympq_hash,                  /* tp_hash          */
        0,                                  /* tp_call          */
    (reprfunc) mpq2str,                     /* tp_str           */
    (getattrofunc) 0,                       /* tp_getattro      */
    (setattrofunc) 0,                       /* tp_setattro      */
        0,                                  /* tp_as_buffer     */
#if PY_MAJOR_VERSION >= 3
    Py_TPFLAGS_DEFAULT,                     /* tp_flags         */
#else
    Py_TPFLAGS_HAVE_RICHCOMPARE|Py_TPFLAGS_CHECKTYPES,            /* tp_flags         */
#endif
    "GNU Multi Precision rational number",  /* tp_doc           */
        0,                                  /* tp_traverse      */
        0,                                  /* tp_clear         */
    (richcmpfunc)&mpq_richcompare,          /* tp_richcompare   */
        0,                                  /* tp_weaklistoffset*/
        0,                                  /* tp_iter          */
        0,                                  /* tp_iternext      */
    Pympq_methods,                          /* tp_methods       */
};


static PyTypeObject Pympf_Type =
{
    /* PyObject_HEAD_INIT(&PyType_Type) */
#if PY_MAJOR_VERSION >= 3
    PyVarObject_HEAD_INIT(NULL, 0)
#else
    PyObject_HEAD_INIT(0)
    0,                                      /* ob_size          */
#endif
    "mpf",                                  /* tp_name          */
    sizeof(PympfObject),                    /* tp_basicsize     */
        0,                                  /* tp_itemsize      */
    /* methods */
    (destructor) Pympf_dealloc,             /* tp_dealloc       */
        0,                                  /* tp_print         */
        0,                                  /* tp_getattr       */
        0,                                  /* tp_setattr       */
        0,                                  /* tp_reserved      */
    (reprfunc) mpf2repr,                    /* tp_repr          */
    &mpf_number_methods,                    /* tp_as_number     */
        0,                                  /* tp_as_sequence   */
        0,                                  /* tp_as_mapping    */
    (hashfunc) Pympf_hash,                  /* tp_hash          */
        0,                                  /* tp_call          */
    (reprfunc) mpf2str,                     /* tp_str           */
    (getattrofunc) 0,                       /* tp_getattro      */
    (setattrofunc) 0,                       /* tp_setattro      */
        0,                                  /* tp_as_buffer     */
#if PY_MAJOR_VERSION >= 3
    Py_TPFLAGS_DEFAULT,                     /* tp_flags         */
#else
    Py_TPFLAGS_HAVE_RICHCOMPARE|Py_TPFLAGS_CHECKTYPES,            /* tp_flags         */
#endif
    "GNU Multi Precision floating point",   /* tp_doc           */
        0,                                  /* tp_traverse      */
        0,                                  /* tp_clear         */
    (richcmpfunc)&mpf_richcompare,          /* tp_richcompare   */
        0,                                  /* tp_weaklistoffset*/
        0,                                  /* tp_iter          */
        0,                                  /* tp_iternext      */
    Pympf_methods,                          /* tp_methods       */
};


static void *
gmpy_allocate(size_t size)
{
    void *res;
    size_t usize=size;
    if(usize<GMPY_ALLOC_MIN) usize=GMPY_ALLOC_MIN;

    if(options.debug)
        fprintf(stderr, "mp_allocate( %d->%d )\n", (int)size, (int)usize);
    if(!(res = malloc(usize)))
        Py_FatalError("mp_allocate failure");

    if(options.debug)
        fprintf(stderr, "mp_allocate( %d->%d ) ->%8p\n", (int)size, (int)usize, res);

    return res;
} /* mp_allocate() */


static void *
gmpy_reallocate(void *ptr, size_t old_size, size_t new_size)
{
    void *res;
    size_t uold=old_size;
    size_t unew=new_size;
    if(uold<GMPY_ALLOC_MIN) uold=GMPY_ALLOC_MIN;
    if(unew<GMPY_ALLOC_MIN) unew=GMPY_ALLOC_MIN;

    if(options.debug)
        fprintf(stderr,
            "mp_reallocate: old address %8p, old size %d(%d), new %d(%d)\n",
            ptr, (int)old_size, (int)uold, (int)new_size, (int)unew);

    if(uold==unew) {
        if(options.debug)
            fprintf(stderr, "mp_reallocate: avoided realloc for %d\n", (int)unew);
        return ptr;
    }

    if(!(res = realloc(ptr, unew)))
        Py_FatalError("mp_reallocate failure");

    if(options.debug)
        fprintf(stderr, "mp_reallocate: newob address %8p, newob size %d(%d)\n",
        res, (int)new_size, (int)unew);

    return res;
} /* mp_reallocate() */

static void
gmpy_free( void *ptr, size_t size)
{
    size_t usize=size;
    if(usize<GMPY_ALLOC_MIN) usize=GMPY_ALLOC_MIN;

    if(options.debug)
        fprintf(stderr, "mp_free      : old address %8p, old size %d(%d)\n",
            ptr, (int)size, (int)usize);

    free(ptr);
} /* mp_free() */


/* Find out how many bits are significant in a double */
static unsigned get_precision(void)
{
#if defined(DBL_MANT_BITS)
    return DBL_MANT_BITS;
#elif !defined(FLT_RADIX) || (FLT_RADIX!=2)
#   error "FLT_RADIX undefined or != 2, pls set DBL_MANT_BITS"
#elif !defined(DBL_MANT_DIG)
#   error "DBL_MANT_DIG undefined, pls set DBL_MANT_BITS"
#else
    return DBL_MANT_DIG;
#endif
#if 0
    int bit;
    double eps;
    for(bit = 0, eps = 1.0; 1.0 != (1.0 + eps); bit++) eps /= 2;
    return bit;
#endif
}

static void _PyInitGMP(void)
{
    mp_set_memory_functions(gmpy_allocate, gmpy_reallocate, gmpy_free);
    double_mantissa = get_precision();
    options.minprec = double_mantissa;
    set_zcache(options.zcache);
    set_qcache(options.qcache);
    set_zconst(options.minzco, options.maxzco);
}

static char _gmpy_docs[] = "\
gmpy 1.05 - General Multiprecision arithmetic for PYthon:\n\
exposes functionality from the GMP 4 library to Python 2.x & 3.x.\n\
\n\
Allows creation of multiprecision integer (mpz), float (mpf),\n\
and rational (mpq) numbers, conversion between them and to/from\n\
Python numbers/strings, arithmetic, bitwise, and some other\n\
higher-level mathematical operations; also, pretty good-quality\n\
linear-congruential random number generation and shuffling.\n\
\n\
mpz has comparable functionality to Python's builtin longs, but\n\
can be faster for some operations (particularly multiplication\n\
and raising-to-power) and has many further useful and speedy\n\
functions (prime testing and generation, factorial, fibonacci,\n\
binary-coefficients, gcd, lcm, square and other roots, ...).\n\
\n\
mpf and mpq only offer basic arithmetic abilities, but they\n\
do add the ability to have floating-point numbers ensuring at\n\
least a predefined number of bits' worth of precision (and with\n\
potentially-huge or extremely-tiny magnitudes), as well as\n\
unlimited-precision rationals, with reasonably-fast operations,\n\
which are not built-in features of Python.\n\
";

/* Notes on Python 3.x support: Full support for PEP-3121 has not been
 * implemented. No per-module state has been defined.
 */

#if PY_MAJOR_VERSION >= 3
#define INITERROR return NULL
static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "gmpy",
        _gmpy_docs,
        -1, /*sizeof(struct module_state) */
        Pygmpy_methods,
        NULL,
        NULL, /* gmpy_traverse */
        NULL, /* gmpy_clear */
        NULL
};

#ifdef _MSC_VER
__declspec(dllexport)
#endif
PyObject *
PyInit_gmpy(void)
#else
#define INITERROR return
DL_EXPORT(void)
initgmpy(void)
#endif
{
    //~ PyObject* decimal_module = NULL;
    PyObject* copy_reg_module = NULL;
    char *do_debug = getenv("GMPY_DEBUG");
    if (PyType_Ready(&Pympz_Type) < 0)
        INITERROR;
    if (PyType_Ready(&Pympq_Type) < 0)
        INITERROR;
    if (PyType_Ready(&Pympf_Type) < 0)
        INITERROR;

    if (do_debug)
        sscanf(do_debug, "%d", &options.debug);

    if (options.debug)
        fputs( "initgmpy() called...\n", stderr );
    _PyInitGMP();

#if PY_MAJOR_VERSION >= 3
    gmpy_module = PyModule_Create(&moduledef);
#else
    gmpy_module = Py_InitModule3("gmpy", Pygmpy_methods, _gmpy_docs);
#endif

    /* Todo: Add error checking for status of gmpy_module returned above. */

    export_gmpy(gmpy_module);

    if (options.debug)
        fprintf(stderr, "gmpy_module at %p\n", gmpy_module);

    /* Add support for pickling. */
#if PY_MAJOR_VERSION >= 3
    copy_reg_module = PyImport_ImportModule("copyreg");
    if (copy_reg_module) {
        char* enable_pickle =
            "def mpz_reducer(an_mpz): return (gmpy.mpz, (an_mpz.binary(), 256))\n"
            "def mpq_reducer(an_mpq): return (gmpy.mpq, (an_mpq.binary(), 256))\n"
            "def mpf_reducer(an_mpf): return (gmpy.mpf, (an_mpf.binary(), 0, 256))\n"
            "copyreg.pickle(type(gmpy.mpz(0)), mpz_reducer)\n"
            "copyreg.pickle(type(gmpy.mpq(0)), mpq_reducer)\n"
            "copyreg.pickle(type(gmpy.mpf(0)), mpf_reducer)\n"
        ;
        PyObject* namespace = PyDict_New();
        PyObject* result = NULL;
        if (options.debug)
            fprintf(stderr, "gmpy_module imported copyreg OK\n");
        PyDict_SetItemString(namespace, "copyreg", copy_reg_module);
        PyDict_SetItemString(namespace, "gmpy", gmpy_module);
        PyDict_SetItemString(namespace, "type", (PyObject*)&PyType_Type);
        result = PyRun_String(enable_pickle, Py_file_input,
                              namespace, namespace);
        if (result) {
            if (options.debug)
                fprintf(stderr, "gmpy_module enable pickle OK\n");
        } else {
            if (options.debug)
                fprintf(stderr, "gmpy_module could not enable pickle\n");
            PyErr_Clear();
        }
        Py_DECREF(namespace);
        Py_XDECREF(result);
    } else {
        PyErr_Clear();
        if (options.debug)
            fprintf(stderr, "gmpy_module could not import copyreg\n");
    }
#else
    copy_reg_module = PyImport_ImportModule("copy_reg");
    if (copy_reg_module) {
        char* enable_pickle =
            "def mpz_reducer(an_mpz): return (gmpy.mpz, (an_mpz.binary(), 256))\n"
            "def mpq_reducer(an_mpq): return (gmpy.mpq, (an_mpq.binary(), 256))\n"
            "def mpf_reducer(an_mpf): return (gmpy.mpf, (an_mpf.binary(), 0, 256))\n"
            "copy_reg.pickle(type(gmpy.mpz(0)), mpz_reducer)\n"
            "copy_reg.pickle(type(gmpy.mpq(0)), mpq_reducer)\n"
            "copy_reg.pickle(type(gmpy.mpf(0)), mpf_reducer)\n"
        ;
        PyObject* namespace = PyDict_New();
        PyObject* result = NULL;
        if (options.debug)
            fprintf(stderr, "gmpy_module imported copy_reg OK\n");
        PyDict_SetItemString(namespace, "copy_reg", copy_reg_module);
        PyDict_SetItemString(namespace, "gmpy", gmpy_module);
        PyDict_SetItemString(namespace, "type", (PyObject*)&PyType_Type);
        result = PyRun_String(enable_pickle, Py_file_input,
                              namespace, namespace);
        if (result) {
            if (options.debug)
                fprintf(stderr, "gmpy_module enable pickle OK\n");
        } else {
            if (options.debug)
                fprintf(stderr, "gmpy_module could not enable pickle\n");
            PyErr_Clear();
        }
        Py_DECREF(namespace);
        Py_XDECREF(result);
    } else {
        PyErr_Clear();
        if (options.debug)
            fprintf(stderr, "gmpy_module could not import copy_reg\n");
    }
#endif


#if PY_MAJOR_VERSION >= 3
    return gmpy_module;
#endif
}
