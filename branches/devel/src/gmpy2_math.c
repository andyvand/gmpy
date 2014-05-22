/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * gmpy2_math.c                                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Python interface to the GMP or MPIR, MPFR, and MPC multiple precision   *
 * libraries.                                                              *
 *                                                                         *
 * Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,               *
 *           2008, 2009 Alex Martelli                                      *
 *                                                                         *
 * Copyright 2008, 2009, 2010, 2011, 2012, 2013, 2014 Case Van Horsen      *
 *                                                                         *
 * This file is part of GMPY2.                                             *
 *                                                                         *
 * GMPY2 is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the  *
 * Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * GMPY2 is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public    *
 * License for more details.                                               *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with GMPY2; if not, see <http://www.gnu.org/licenses/>    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The various math functions in this file are split into different sections
 * based on the signature of the function, the allowed argument types, and
 * whether they can create mpc results from mpfr arguments. This directly
 * impacts which macros they use.
 */

 
/* Section 1:
 * These functions accept a single argument and return either an mpfr or mpc
 * result.
 * 
 * GMPY_MPFR_MPC_UNIOP(NAME, FUNC) creates the following functions:
 *     GMPy_Real_NAME(x, context)
 *     GMPy_Complex_NAME(x, context)
 *     GMPy_Number_NAME(x, context)
 *     GMPy_Context_NAME(self, other)
 *     - called with METH_O
 */

PyDoc_STRVAR(GMPy_doc_context_sin,
"context.sin(x) -> number\n\n"
"Return sine of x; x in radians.");

PyDoc_STRVAR(GMPy_doc_function_sin,
"sin(x) -> number\n\n"
"Return sine of x; x in radians.");

GMPY_MPFR_MPC_UNIOP_EX(Sin, sin)

PyDoc_STRVAR(GMPy_doc_context_cos,
"context.cos(x) -> number\n\n"
"Return cosine of x; x in radians.");

PyDoc_STRVAR(GMPy_doc_function_cos,
"cos(x) -> number\n\n"
"Return cosine of x; x in radians.");

GMPY_MPFR_MPC_UNIOP_EX(Cos, cos)

PyDoc_STRVAR(GMPy_doc_context_tan,
"context.tan(x) -> number\n\n"
"Return tangent of x; x in radians.");

PyDoc_STRVAR(GMPy_doc_function_tan,
"tan(x) -> number\n\n"
"Return tangent of x; x in radians.");

GMPY_MPFR_MPC_UNIOP_EX(Tan, tan)

PyDoc_STRVAR(GMPy_doc_context_atan,
"context.atan(x) -> number\n\n"
"Return inverse tangent of x; result in radians.");

PyDoc_STRVAR(GMPy_doc_function_atan,
"atan(x) -> number\n\n"
"Return inverse tangent of x; result in radians.");

GMPY_MPFR_MPC_UNIOP_EX(Atan, atan)

PyDoc_STRVAR(GMPy_doc_context_sinh,
"context.sinh(x) -> number\n\n"
"Return hyperbolic sine of x.");

PyDoc_STRVAR(GMPy_doc_function_sinh,
"sinh(x) -> number\n\n"
"Return hyperbolic sine of x.");

GMPY_MPFR_MPC_UNIOP_EX(Sinh, sinh)

PyDoc_STRVAR(GMPy_doc_context_cosh,
"context.cosh(x) -> number\n\n"
"Return hyperbolic cosine of x.");

PyDoc_STRVAR(GMPy_doc_function_cosh,
"cosh(x) -> number\n\n"
"Return hyperbolic cosine of x.");

GMPY_MPFR_MPC_UNIOP_EX(Cosh, cosh)

PyDoc_STRVAR(GMPy_doc_context_tanh,
"context.tanh(x) -> number\n\n"
"Return hyperbolic tangent of x.");

PyDoc_STRVAR(GMPy_doc_function_tanh,
"tanh(x) -> number\n\n"
"Return hyperbolic tangent of x.");

GMPY_MPFR_MPC_UNIOP_EX(Tanh, tanh)

PyDoc_STRVAR(GMPy_doc_context_asinh,
"context.asinh(x) -> number\n\n"
"Return inverse hyperbolic sine of x.");

PyDoc_STRVAR(GMPy_doc_function_asinh,
"asinh(x) -> number\n\n"
"Return inverse hyperbolic sine of x.");

GMPY_MPFR_MPC_UNIOP_EX(Asinh, asinh)

PyDoc_STRVAR(GMPy_doc_context_acosh,
"context.acosh(x) -> number\n\n"
"Return inverse hyperbolic cosine of x.");

PyDoc_STRVAR(GMPy_doc_function_acosh,
"acosh(x) -> number\n\n"
"Return inverse hyperbolic cosine of x.");

GMPY_MPFR_MPC_UNIOP_EX(Acosh, acosh)

/* Section 2:
 * These functions accept a single argument and return an mpfr result.
 * 
 * GMPY_MPFR_UNIOP(NAME, FUNC) creates the following functions:
 *     GMPy_Real_NAME(x, context)
 *     GMPy_Number_NAME(x, context)
 *     GMPy_Context_NAME(self, other)
 *     - called with METH_O
 */
 
PyDoc_STRVAR(GMPy_doc_context_sec,
"context.sec(x) -> number\n\n"
"Return secant of x; x in radians.");

PyDoc_STRVAR(GMPy_doc_function_sec,
"sec(x) -> number\n\n"
"Return secant of x; x in radians.");

GMPY_MPFR_UNIOP_EX(Sec, sec)

PyDoc_STRVAR(GMPy_doc_context_csc,
"context.csc(x) -> number\n\n"
"Return cosecant of x; x in radians.");

PyDoc_STRVAR(GMPy_doc_function_csc,
"csc(x) -> number\n\n"
"Return cosecant of x; x in radians.");

GMPY_MPFR_UNIOP_EX(Csc, csc)

PyDoc_STRVAR(GMPy_doc_context_cot,
"context.cot(x) -> number\n\n"
"Return cotangent of x; x in radians.");

PyDoc_STRVAR(GMPy_doc_function_cot,
"cot(x) -> number\n\n"
"Return cotangent of x; x in radians.");

GMPY_MPFR_UNIOP_EX(Cot, cot)

PyDoc_STRVAR(GMPy_doc_context_sech,
"context.sech(x) -> number\n\n"
"Return hyperbolic secant of x.");

PyDoc_STRVAR(GMPy_doc_function_sech,
"sech(x) -> number\n\n"
"Return hyperbolic secant of x.");

GMPY_MPFR_UNIOP_EX(Sech, sech)

PyDoc_STRVAR(GMPy_doc_context_csch,
"context.csch(x) -> number\n\n"
"Return hyperbolic cosecant of x.");

PyDoc_STRVAR(GMPy_doc_function_csch,
"csch(x) -> number\n\n"
"Return hyperbolic cosecant of x.");

GMPY_MPFR_UNIOP_EX(Csch, csch)

PyDoc_STRVAR(GMPy_doc_context_coth,
"context.coth(x) -> number\n\n"
"Return hyperbolic cotangent of x.");

PyDoc_STRVAR(GMPy_doc_function_coth,
"coth(x) -> number\n\n"
"Return hyperbolic cotangent of x.");

GMPY_MPFR_UNIOP_EX(Coth, coth)

PyDoc_STRVAR(GMPy_doc_context_rec_sqrt,
"context.rec_sqrt(x) -> number\n\n"
"Return the reciprocal of the square root of x.");

PyDoc_STRVAR(GMPy_doc_function_rec_sqrt,
"rec_sqrt(x) -> number\n\n"
"Return the reciprocal of the square root of x.");

GMPY_MPFR_UNIOP_EX(RecSqrt, rec_sqrt)

PyDoc_STRVAR(GMPy_doc_context_rint,
"context.rint(x) -> number\n\n"
"Return x rounded to the nearest integer using the context rounding\n"
"mode.");

PyDoc_STRVAR(GMPy_doc_function_rint,
"rint(x) -> number\n\n"
"Return x rounded to the nearest integer using the current rounding\n"
"mode.");

GMPY_MPFR_UNIOP_EX(Rint, rint)

PyDoc_STRVAR(GMPy_doc_context_rint_ceil,
"context.rint_ceil(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding to the\n"
"next higher or equal integer and then, if needed, using the context\n"
"rounding mode.");

PyDoc_STRVAR(GMPy_doc_function_rint_ceil,
"rint_ceil(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding to the\n"
"next higher or equal integer and then, if needed, using the current\n"
"rounding mode.");

GMPY_MPFR_UNIOP_EX(RintCeil, rint_ceil)

PyDoc_STRVAR(GMPy_doc_context_rint_floor,
"context.rint_floor(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding to the\n"
"next lower or equal integer and then, if needed, using the context\n"
"rounding mode.");

PyDoc_STRVAR(GMPy_doc_function_rint_floor,
"rint_floor(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding to the\n"
"next lower or equal integer and then, if needed, using the current\n"
"rounding mode.");

GMPY_MPFR_UNIOP_EX(RintFloor, rint_floor)

PyDoc_STRVAR(GMPy_doc_context_rint_round,
"context.rint_round(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding to the\n"
"nearest integer (ties away from 0) and then, if needed, using\n"
"the context rounding mode.");

PyDoc_STRVAR(GMPy_doc_function_rint_round,
"rint_round(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding to the\n"
"nearest integer (ties away from 0) and then, if needed, using\n"
"the current rounding mode.");

GMPY_MPFR_UNIOP_EX(RintRound, rint_round)

PyDoc_STRVAR(GMPy_doc_context_rint_trunc,
"context.rint_trunc(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding towards\n"
"zero and then, if needed, using the context rounding mode.");

PyDoc_STRVAR(GMPy_doc_function_rint_trunc,
"rint_trunc(x) -> number\n\n"
"Return x rounded to the nearest integer by first rounding towards\n"
"zero and then, if needed, using the current rounding mode.");

GMPY_MPFR_UNIOP_EX(RintTrunc, rint_trunc)

PyDoc_STRVAR(GMPy_doc_context_frac,
"context.frac(x) -> number\n\n"
"Return fractional part of x.");

PyDoc_STRVAR(GMPy_doc_function_frac,
"frac(x) -> number\n\n"
"Return fractional part of x.");

GMPY_MPFR_UNIOP_EX(Frac, frac)

PyDoc_STRVAR(GMPy_doc_context_cbrt,
"context.cbrt(x) -> number\n\n"
"Return the cube root of x.");

PyDoc_STRVAR(GMPy_doc_function_cbrt,
"cbrt(x) -> number\n\n"
"Return the cube root of x.");

GMPY_MPFR_UNIOP_EX(Cbrt, cbrt)

PyDoc_STRVAR(GMPy_doc_context_log2,
"context.log2(x) -> number\n\n"
"Return base-2 logarithm of x.");

PyDoc_STRVAR(GMPy_doc_function_log2,
"log2(x) -> number\n\n"
"Return base-2 logarithm of x.");

GMPY_MPFR_UNIOP_EX(Log2, log2)

PyDoc_STRVAR(GMPy_doc_context_exp2,
"context.exp2(x) -> number\n\n"
"Return 2**x.");

PyDoc_STRVAR(GMPy_doc_function_exp2,
"exp2(x) -> number\n\n"
"Return 2**x.");

GMPY_MPFR_UNIOP_EX(Exp2, exp2)

PyDoc_STRVAR(GMPy_doc_context_exp10,
"context.exp10(x) -> number\n\n"
"Return 10**x.");

PyDoc_STRVAR(GMPy_doc_function_exp10,
"exp10(x) -> number\n\n"
"Return 10**x.");

GMPY_MPFR_UNIOP_EX(Exp10, exp10)

PyDoc_STRVAR(GMPy_doc_context_log1p,
"context.log1p(x) -> number\n\n"
"Return natural logarithm of (1+x).");

PyDoc_STRVAR(GMPy_doc_function_log1p,
"log1p(x) -> number\n\n"
"Return natural logarithm of (1+x).");

GMPY_MPFR_UNIOP_EX(Log1p, log1p)

PyDoc_STRVAR(GMPy_doc_context_expm1,
"context.expm1(x) -> number\n\n"
"Return exp(x) - 1.");

PyDoc_STRVAR(GMPy_doc_function_expm1,
"expm1(x) -> number\n\n"
"Return exp(x) - 1.");

GMPY_MPFR_UNIOP_EX(Expm1, expm1)

PyDoc_STRVAR(GMPy_doc_context_eint,
"context.eint(x) -> number\n\n"
"Return exponential integral of x.");

PyDoc_STRVAR(GMPy_doc_function_eint,
"eint(x) -> number\n\n"
"Return exponential integral of x.");

GMPY_MPFR_UNIOP_EX(Eint, eint)

PyDoc_STRVAR(GMPy_doc_context_li2,
"context.li2(x) -> number\n\n"
"Return real part of dilogarithm of x.");

PyDoc_STRVAR(GMPy_doc_function_li2,
"li2(x) -> number\n\n"
"Return real part of dilogarithm of x.");

GMPY_MPFR_UNIOP_EX(Li2, li2)

PyDoc_STRVAR(GMPy_doc_context_gamma,
"context.gamma(x) -> number\n\n"
"Return gamma of x.");

PyDoc_STRVAR(GMPy_doc_function_gamma,
"gamma(x) -> number\n\n"
"Return gamma of x.");

GMPY_MPFR_UNIOP_EX(Gamma, gamma)

PyDoc_STRVAR(GMPy_doc_context_lngamma,
"context.lngamma(x) -> number\n\n"
"Return natural logarithm of gamma(x).");

PyDoc_STRVAR(GMPy_doc_function_lngamma,
"lngamma(x) -> number\n\n"
"Return natural logarithm of gamma(x).");

GMPY_MPFR_UNIOP_EX(Lngamma, lngamma)

PyDoc_STRVAR(GMPy_doc_context_digamma,
"context.digamma(x) -> number\n\n"
"Return digamma of x.");

PyDoc_STRVAR(GMPy_doc_function_digamma,
"digamma(x) -> number\n\n"
"Return digamma of x.");

GMPY_MPFR_UNIOP_EX(Digamma, digamma)

PyDoc_STRVAR(GMPy_doc_context_zeta,
"context.zeta(x) -> number\n\n"
"Return Riemann zeta of x.");

PyDoc_STRVAR(GMPy_doc_function_zeta,
"zeta(x) -> number\n\n"
"Return Riemann zeta of x.");

GMPY_MPFR_UNIOP_EX(Zeta, zeta)

PyDoc_STRVAR(GMPy_doc_context_erf,
"context.erf(x) -> number\n\n"
"Return error function of x.");

PyDoc_STRVAR(GMPy_doc_function_erf,
"erf(x) -> number\n\n"
"Return error function of x.");

GMPY_MPFR_UNIOP_EX(Erf, erf)

PyDoc_STRVAR(GMPy_doc_context_erfc,
"context.erfc(x) -> number\n\n"
"Return complementary error function of x.");

PyDoc_STRVAR(GMPy_doc_function_erfc,
"erfc(x) -> number\n\n"
"Return complementary error function of x.");

GMPY_MPFR_UNIOP_EX(Erfc, erfc)

PyDoc_STRVAR(GMPy_doc_context_j0,
"context.j0(x) -> number\n\n"
"Return first kind Bessel function of order 0 of x.");

PyDoc_STRVAR(GMPy_doc_function_j0,
"j0(x) -> number\n\n"
"Return first kind Bessel function of order 0 of x.");

GMPY_MPFR_UNIOP_EX(J0, j0)

PyDoc_STRVAR(GMPy_doc_context_j1,
"context.j1(x) -> number\n\n"
"Return first kind Bessel function of order 1 of x.");

PyDoc_STRVAR(GMPy_doc_function_j1,
"j1(x) -> number\n\n"
"Return first kind Bessel function of order 1 of x.");

GMPY_MPFR_UNIOP_EX(J1, j1)

PyDoc_STRVAR(GMPy_doc_context_y0,
"context.y0(x) -> number\n\n"
"Return second kind Bessel function of order 0 of x.");

PyDoc_STRVAR(GMPy_doc_function_y0,
"y0(x) -> number\n\n"
"Return second kind Bessel function of order 0 of x.");

GMPY_MPFR_UNIOP_EX(Y0, y0)

PyDoc_STRVAR(GMPy_doc_context_y1,
"context.y1(x) -> number\n\n"
"Return second kind Bessel function of order 1 of x.");

PyDoc_STRVAR(GMPy_doc_function_y1,
"y1(x) -> number\n\n"
"Return second kind Bessel function of order 1 of x.");

GMPY_MPFR_UNIOP_EX(Y1, y1)

PyDoc_STRVAR(GMPy_doc_context_ai,
"context.ai(x) -> number\n\n"
"Return Airy function of x.");

PyDoc_STRVAR(GMPy_doc_function_ai,
"ai(x) -> number\n\n"
"Return Airy function of x.");

GMPY_MPFR_UNIOP_EX(Ai, ai)

/* Section 3:
 * The following functions may return an mpc result for certain mpfr arguments.
 * Since the expectional values vary between functions, the 'Real' and 'Complex'
 * functions do not use macros. However, they do use a macro to create the
 * higher-level functions.
 * 
 * GMPY_MPFR_MPC_UNIOP_TEMPLATE(NAME, FUNC) creates the following functions:
 *     GMPy_Number_NAME(x, context)
 *     - assumes GMPy_Real_NAME & GMPy_Complex_NAME exist
 *     GMPy_Context_NAME(self, other)
 *     - called with METH_O
 */

PyDoc_STRVAR(GMPy_doc_context_acos,
"context.acos(x) -> number\n\n"
"Return inverse cosine of x; result in radians.");

PyDoc_STRVAR(GMPy_doc_function_acos,
"acos(x) -> number\n\n"
"Return inverse cosine of x; result in radians.");

static PyObject *
_GMPy_MPFR_Acos(PyObject *x, CTXT_Object *context)
{
    MPFR_Object *result;

    CHECK_CONTEXT(context);

    if (!mpfr_nan_p(MPFR(x)) &&
            (mpfr_cmp_si(MPFR(x), 1) > 0 || mpfr_cmp_si(MPFR(x), -1) < 0) &&
            context->ctx.allow_complex
       ) {
        return GMPy_Complex_Acos(x, context);
    }

    if (!(result = GMPy_MPFR_New(0, context))) {
        return NULL;
    }

    mpfr_clear_flags();
    result->rc = mpfr_acos(result->f, MPFR(x), GET_MPFR_ROUND(context));
    GMPY_MPFR_CLEANUP(result, context, "acos()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Real_Acos(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPFR_From_Real(x, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPFR_Acos(tempx, context);
    Py_DECREF(tempx);
    return result;
}

static PyObject *
_GMPy_MPC_Acos(PyObject *x, CTXT_Object *context)
{
    MPC_Object *result = NULL;

    CHECK_CONTEXT(context);

    if (!(result = GMPy_MPC_New(0, 0, context))) {
        return NULL;
    }

    result->rc = mpc_acos(result->c, MPC(x), GET_MPC_ROUND(context));
    GMPY_MPC_CLEANUP(result, context, "acos()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Complex_Acos(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPC_From_Complex(x, 1, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPC_Acos(tempx, context);
    Py_DECREF(tempx);
    return result;
}

GMPY_MPFR_MPC_UNIOP_TEMPLATE_EX(Acos, acos)

PyDoc_STRVAR(GMPy_doc_context_asin,
"context.asin(x) -> number\n\n"
"Return inverse sine of x; result in radians.");

PyDoc_STRVAR(GMPy_doc_function_asin,
"asin(x) -> number\n\n"
"Return inverse sine of x; result in radians.");

static PyObject *
_GMPy_MPFR_Asin(PyObject *x, CTXT_Object *context)
{
    MPFR_Object *result;

    CHECK_CONTEXT(context);

    if (!mpfr_nan_p(MPFR(x)) &&
            (mpfr_cmp_si(MPFR(x), 1) > 0 || mpfr_cmp_si(MPFR(x), -1) < 0) &&
            context->ctx.allow_complex
       ) {
        return GMPy_Complex_Asin(x, context);
    }

    if (!(result = GMPy_MPFR_New(0, context))) {
        return NULL;
    }

    mpfr_clear_flags();
    result->rc = mpfr_asin(result->f, MPFR(x), GET_MPFR_ROUND(context));
    GMPY_MPFR_CLEANUP(result, context, "asin()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Real_Asin(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPFR_From_Real(x, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPFR_Asin(tempx, context);
    Py_DECREF(tempx);
    return result;
}

static PyObject *
_GMPy_MPC_Asin(PyObject *x, CTXT_Object *context)
{
    MPC_Object *result;

    CHECK_CONTEXT(context);

    if (!(result = GMPy_MPC_New(0, 0, context))) {
        return NULL;
    }

    result->rc = mpc_asin(result->c, MPC(x), GET_MPC_ROUND(context));
    GMPY_MPC_CLEANUP(result, context, "asin()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Complex_Asin(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPC_From_Complex(x, 1, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPC_Asin(tempx, context);
    Py_DECREF(tempx);
    return result;
}

GMPY_MPFR_MPC_UNIOP_TEMPLATE_EX(Asin, asin)

PyDoc_STRVAR(GMPy_doc_context_atanh,
"context.atanh(x) -> number\n\n"
"Return inverse hyperbolic tanget of x.");

PyDoc_STRVAR(GMPy_doc_function_atanh,
"atanh(x) -> number\n\n"
"Return inverse hyperbolic tangent of x.");

static PyObject *
_GMPy_MPFR_Atanh(PyObject *x, CTXT_Object *context)
{
    MPFR_Object *result;

    CHECK_CONTEXT(context);

    if (!mpfr_nan_p(MPFR(x)) &&
            (mpfr_cmp_si(MPFR(x), 1) > 0 || mpfr_cmp_si(MPFR(x), -1) < 0) &&
            context->ctx.allow_complex
       ) {
        return GMPy_Complex_Atanh(x, context);
    }

    if (!(result = GMPy_MPFR_New(0, context))) {
        return NULL;
    }

    mpfr_clear_flags();
    result->rc = mpfr_atanh(result->f, MPFR(x), GET_MPFR_ROUND(context));
    GMPY_MPFR_CLEANUP(result, context, "atanh()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Real_Atanh(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPFR_From_Real(x, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPFR_Atanh(tempx, context);
    Py_DECREF(tempx);
    return result;
}

static PyObject *
_GMPy_MPC_Atanh(PyObject *x, CTXT_Object *context)
{
    MPC_Object *result;

    CHECK_CONTEXT(context);

    if (!(result = GMPy_MPC_New(0, 0, context))) {
        return NULL;
    }

    result->rc = mpc_atanh(result->c, MPC(x), GET_MPC_ROUND(context));
    GMPY_MPC_CLEANUP(result, context, "atanh()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Complex_Atanh(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPC_From_Complex(x, 1, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPC_Atanh(tempx, context);
    Py_DECREF(tempx);
    return result;
}

GMPY_MPFR_MPC_UNIOP_TEMPLATE_EX(Atanh, atanh)

PyDoc_STRVAR(GMPy_doc_function_atan2,
"atan2(y, x) -> number\n\n"
"Return arc-tangent of (y/x); result in radians.");

PyDoc_STRVAR(GMPy_doc_context_atan2,
"context.atan2(y, x) -> number\n\n"
"Return arc-tangent of (y/x); result in radians.");

GMPY_MPFR_BINOP_EX(Atan2, atan2)

PyDoc_STRVAR(GMPy_doc_function_hypot,
"hypot(x, y) -> number\n\n"
"Return square root of (x**2 + y**2).");

PyDoc_STRVAR(GMPy_doc_context_hypot,
"context.hypot(x, y) -> number\n\n"
"Return square root of (x**2 + y**2).");

GMPY_MPFR_BINOP_EX(Hypot, hypot)

static PyObject *
_GMPy_MPFR_Sin_Cos(PyObject *x, CTXT_Object *context)
{
    MPFR_Object *s, *c;
    PyObject *result;
    int code;

    CHECK_CONTEXT(context);

    s = GMPy_MPFR_New(0, context);
    c = GMPy_MPFR_New(0, context);
    result = PyTuple_New(2);
    if (!s || !c || !result) {
        Py_XDECREF((PyObject*)s);
        Py_XDECREF((PyObject*)c);
        Py_XDECREF(result);
        return NULL;
    }

    mpfr_clear_flags();
    code = mpfr_sin_cos(s->f, c->f, MPFR(x), GET_MPFR_ROUND(context));

    s->rc = code & 0x03;
    c->rc = code >> 2;
    if (s->rc == 2) s->rc = -1;
    if (c->rc == 2) c->rc = -1;

    GMPY_MPFR_CLEANUP(s, context, "sin_cos()");
    GMPY_MPFR_CLEANUP(c, context, "sin_cos()");

    if (!s || !c) {
        Py_XDECREF((PyObject*)s);
        Py_XDECREF((PyObject*)c);
        Py_DECREF(result);
        return NULL;
    }

    PyTuple_SET_ITEM(result, 0, (PyObject*)s);
    PyTuple_SET_ITEM(result, 1, (PyObject*)c);
    return result;
}

static PyObject *
GMPy_Real_Sin_Cos(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPFR_From_Real(x, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPFR_Sin_Cos(tempx, context);
    Py_DECREF(tempx);
    return result;
}

static PyObject *
_GMPy_MPC_Sin_Cos(PyObject *x, CTXT_Object *context)
{
    MPC_Object *s, *c;
    PyObject *result;
    int code;

    CHECK_CONTEXT(context);

    s = GMPy_MPC_New(0, 0, context);
    c = GMPy_MPC_New(0, 0, context);
    result = PyTuple_New(2);
    if (!s || !c || !result) {
        Py_XDECREF((PyObject*)s);
        Py_XDECREF((PyObject*)c);
        Py_XDECREF(result);
        return NULL;
    }

    code = mpc_sin_cos(s->c, c->c, MPC(x), GET_MPC_ROUND(context), GET_MPC_ROUND(context));

    s->rc = MPC_INEX1(code);
    c->rc = MPC_INEX2(code);

    GMPY_MPC_CLEANUP(s, context, "sin_cos()");
    GMPY_MPC_CLEANUP(c, context, "sin_cos()");

    if (!s || !c) {
        Py_XDECREF((PyObject*)s);
        Py_XDECREF((PyObject*)c);
        Py_XDECREF(result);
        return NULL;
    }

    PyTuple_SET_ITEM(result, 0, (PyObject*)s);
    PyTuple_SET_ITEM(result, 1, (PyObject*)c);
    return result;
}

static PyObject *
GMPy_Complex_Sin_Cos(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPC_From_Complex(x, 1, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPC_Sin_Cos(tempx, context);
    Py_DECREF(tempx);
    return result;
}

PyDoc_STRVAR(GMPy_doc_context_sin_cos,
"context.sin_cos(x) -> (number, number)\n\n"
"Return a tuple containing the sine and cosine of x; x in radians.");

PyDoc_STRVAR(GMPy_doc_function_sin_cos,
"sin_cos(x) -> (number, number)\n\n"
"Return a tuple containing the sine and cosine of x; x in radians.");

GMPY_MPFR_MPC_UNIOP_TEMPLATE_EX(Sin_Cos, sin_cos)

static PyObject *
_GMPy_MPFR_Sinh_Cosh(PyObject *x, CTXT_Object *context)
{
    MPFR_Object *s, *c;
    PyObject *result;
    int code;

    CHECK_CONTEXT(context);

    s = GMPy_MPFR_New(0, context);
    c = GMPy_MPFR_New(0, context);
    result = PyTuple_New(2);
    if (!s || !c || !result) {
        Py_XDECREF((PyObject*)s);
        Py_XDECREF((PyObject*)c);
        Py_XDECREF(result);
        return NULL;
    }

    mpfr_clear_flags();
    code = mpfr_sinh_cosh(s->f, c->f, MPFR(x), GET_MPFR_ROUND(context));

    s->rc = code & 0x03;
    c->rc = code >> 2;
    if (s->rc == 2) s->rc = -1;
    if (c->rc == 2) c->rc = -1;

    GMPY_MPFR_CLEANUP(s, context, "sinh_cosh()");
    GMPY_MPFR_CLEANUP(c, context, "sinh_cosh()");

    if (!s || !c) {
        Py_XDECREF((PyObject*)s);
        Py_XDECREF((PyObject*)c);
        Py_XDECREF(result);
        return NULL;
    }

    PyTuple_SET_ITEM(result, 0, (PyObject*)s);
    PyTuple_SET_ITEM(result, 1, (PyObject*)c);
    return result;
}

static PyObject *
GMPy_Real_Sinh_Cosh(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPFR_From_Real(x, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPFR_Sinh_Cosh(tempx, context);
    Py_DECREF(tempx);
    return result;
}

PyDoc_STRVAR(GMPy_doc_context_sinh_cosh,
"context.sinh_cosh(x) -> (number, number)\n\n"
"Return a tuple containing the hyperbolic sine and cosine of x.");

PyDoc_STRVAR(GMPy_doc_function_sinh_cosh,
"sinh_cosh(x) -> (number, number)\n\n"
"Return a tuple containing the hyperbolic sine and cosine of x.");

GMPY_MPFR_UNIOP_TEMPLATE_EX(Sinh_Cosh, sinh_cosh)

PyDoc_STRVAR(GMPy_doc_function_degrees,
"degrees(x) -> mpfr\n\n"
"Convert angle x from radians to degrees.\n"
"Note: In rare cases the result may not be correctly rounded.");

PyDoc_STRVAR(GMPy_doc_context_degrees,
"context.degrees(x) -> mpfr\n\n"
"Convert angle x from radians to degrees.\n"
"Note: In rare cases the result may not be correctly rounded.");

static PyObject *
GMPy_Context_Degrees(PyObject *self, PyObject *other)
{
    MPFR_Object *result, *tempx, *temp;
    CTXT_Object *context = NULL;

    if (self && CTXT_Check(self)) {
        context = (CTXT_Object*)self;
    }
    else {
        CHECK_CONTEXT(context);
    }

    result = GMPy_MPFR_New(0, context);
    temp = GMPy_MPFR_New(context->ctx.mpfr_prec + 100, context);
    tempx = GMPy_MPFR_From_Real(other, 1, context);
    if (!result || !temp || !tempx) {
        Py_XDECREF((PyObject*)temp);
        Py_XDECREF((PyObject*)tempx);
        Py_XDECREF((PyObject*)result);
        return NULL;
    }

    mpfr_const_pi(temp->f, MPFR_RNDN);
    mpfr_ui_div(temp->f, 180, temp->f, MPFR_RNDN);

    mpfr_clear_flags();
    mpfr_mul(result->f, temp->f, tempx->f, MPFR_RNDN);

    Py_DECREF((PyObject*)temp);
    Py_DECREF((PyObject*)tempx);
    GMPY_MPFR_CLEANUP(result, context, "degrees()");
    return (PyObject*)result;
}

PyDoc_STRVAR(GMPy_doc_function_radians,
"radians(x) -> mpfr\n\n"
"Convert angle x from degrees to radians.\n"
"Note: In rare cases the result may not be correctly rounded.");

PyDoc_STRVAR(GMPy_doc_context_radians,
"context.radians(x) -> mpfr\n\n"
"Convert angle x from degrees to radians.\n"
"Note: In rare cases the result may not be correctly rounded.");

static PyObject *
GMPy_Context_Radians(PyObject *self, PyObject *other)
{
    MPFR_Object *result, *tempx, *temp;
    CTXT_Object *context = NULL;

    if (self && CTXT_Check(self)) {
        context = (CTXT_Object*)self;
    }
    else {
        CHECK_CONTEXT(context);
    }

    result = GMPy_MPFR_New(0, context);
    temp = GMPy_MPFR_New(context->ctx.mpfr_prec + 100, context);
    tempx = GMPy_MPFR_From_Real(other, 1, context);
    if (!result || !temp || !tempx) {
        Py_XDECREF((PyObject*)temp);
        Py_XDECREF((PyObject*)tempx);
        Py_XDECREF((PyObject*)result);
        return NULL;
    }

    mpfr_const_pi(temp->f, MPFR_RNDN);
    mpfr_div_ui(temp->f, temp->f, 180, MPFR_RNDN);
    mpfr_mul(result->f, MPFR(self), temp->f, MPFR_RNDN);

    Py_DECREF((PyObject*)temp);
    Py_DECREF((PyObject*)tempx);
    GMPY_MPFR_CLEANUP(result, context, "radians()");
    return (PyObject*)result;
}

PyDoc_STRVAR(GMPy_doc_context_log10,
"context.log10(x) -> number\n\n"
"Return the base-10 logarithm of x.");

PyDoc_STRVAR(GMPy_doc_function_log10,
"log10(x) -> number\n\n"
"Return the base-10 logarithm of x.");

GMPY_MPFR_MPC_UNIOP_EX(Log10, log10)

PyDoc_STRVAR(GMPy_doc_context_log,
"context.log(x) -> number\n\n"
"Return the natural logarithm of x.");

PyDoc_STRVAR(GMPy_doc_function_log,
"log(x) -> number\n\n"
"Return the natural logarithm of x.");

GMPY_MPFR_MPC_UNIOP_EX(Log, log)

PyDoc_STRVAR(GMPy_doc_context_exp,
"context.exp(x) -> number\n\n"
"Return the exponential of x.");

PyDoc_STRVAR(GMPy_doc_function_exp,
"exp(x) -> number\n\n"
"Return the exponential of x.");

GMPY_MPFR_MPC_UNIOP_EX(Exp, exp)

PyDoc_STRVAR(GMPy_doc_context_sqrt,
"context.sqrt(x) -> number\n\n"
"Return the square root of x.");

PyDoc_STRVAR(GMPy_doc_function_sqrt,
"sqrt(x) -> number\n\n"
"Return the square root of x.");

static PyObject *
_GMPy_MPFR_Sqrt(PyObject *x, CTXT_Object *context)
{
    MPFR_Object *result;

    CHECK_CONTEXT(context);

    if (mpfr_sgn(MPFR(x)) < 0 && context->ctx.allow_complex) {
        return GMPy_Complex_Sqrt(x, context);
    }

    if (!(result = GMPy_MPFR_New(0, context))) {
        return NULL;
    }

    mpfr_clear_flags();
    result->rc = mpfr_sqrt(result->f, MPFR(x), GET_MPFR_ROUND(context));
    GMPY_MPFR_CLEANUP(result, context, "sqrt()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Real_Sqrt(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPFR_From_Real(x, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPFR_Sqrt(tempx, context);
    Py_DECREF(tempx);
    return result;
}

static PyObject *
_GMPy_MPC_Sqrt(PyObject *x, CTXT_Object *context)
{
    MPC_Object *result;

    CHECK_CONTEXT(context);

    if (!(result = GMPy_MPC_New(0, 0, context))) {
        return NULL;
    }

    result->rc = mpc_sqrt(result->c, MPC(x), GET_MPFR_ROUND(context));
    GMPY_MPC_CLEANUP(result, context, "sqrt()");
    return (PyObject*)result;
}

static PyObject *
GMPy_Complex_Sqrt(PyObject *x, CTXT_Object *context)
{
    PyObject *result, *tempx;

    CHECK_CONTEXT(context);

    if (!(tempx = (PyObject*)GMPy_MPC_From_Complex(x, 1, 1, context))) {
        return NULL;
    }

    result = _GMPy_MPC_Sqrt(tempx, context);
    Py_DECREF(tempx);
    return result;
}

GMPY_MPFR_MPC_UNIOP_TEMPLATE_EX(Sqrt, sqrt)

PyDoc_STRVAR(GMPy_doc_function_root,
"root(x, n) -> mpfr\n\n"
"Return n-th root of x. The result always an 'mpfr'.");

PyDoc_STRVAR(GMPy_doc_context_root,
"context.root(x, n) -> mpfr\n\n"
"Return n-th root of x. The result always an 'mpfr'.");

GMPY_MPFR_BINOP_REAL_ULONG(Root, root)

PyDoc_STRVAR(GMPy_doc_function_jn,
"jn(x,n) -> mpfr\n\n"
"Return the first kind Bessel function of order n of x.");

PyDoc_STRVAR(GMPy_doc_context_jn,
"context.jn(x,n) -> mpfr\n\n"
"Return the first kind Bessel function of order n of x.");

GMPY_MPFR_BINOP_REAL_LONG(Jn, jn)

PyDoc_STRVAR(GMPy_doc_function_yn,
"yn(x,n) -> mpfr\n\n"
"Return the second kind Bessel function of order n of x.");

PyDoc_STRVAR(GMPy_doc_context_yn,
"context.yn(x,n) -> mpfr\n\n"
"Return the second kind Bessel function of order n of x.");

GMPY_MPFR_BINOP_REAL_LONG(Yn, yn)

PyDoc_STRVAR(GMPy_doc_function_agm,
"agm(x, y) -> mpfr\n\n"
"Return arithmetic-geometric mean of x and y.");

PyDoc_STRVAR(GMPy_doc_context_agm,
"context.agm(x, y) -> mpfr\n\n"
"Return arithmetic-geometric mean of x and y.");

GMPY_MPFR_BINOP(AGM, agm)

PyDoc_STRVAR(GMPy_doc_function_maxnum,
"maxnum(x, y) -> mpfr\n\n"
"Return the maximum number of x and y. If x and y are not 'mpfr', they are\n"
"converted to 'mpfr'. The result is rounded to match the current context.\n"
"If only one of x or y is a number, then that number is returned.");

PyDoc_STRVAR(GMPy_doc_context_maxnum,
"context.maxnum(x, y) -> mpfr\n\n"
"Return the maximum number of x and y. If x and y are not 'mpfr', they are\n"
"converted to 'mpfr'. The result is rounded to match the specified context.\n"
"If only one of x or y is a number, then that number is returned.");

GMPY_MPFR_BINOP(Maxnum, max)

PyDoc_STRVAR(GMPy_doc_function_minnum,
"minnum(x, y) -> mpfr\n\n"
"Return the minimum number of x and y. If x and y are not 'mpfr', they are\n"
"converted to 'mpfr'. The result is rounded to match the current context.\n"
"If only one of x or y is a number, then that number is returned.");

PyDoc_STRVAR(GMPy_doc_context_minnum,
"context.minnum(x, y) -> mpfr\n\n"
"Return the minimum number of x and y. If x and y are not 'mpfr', they are\n"
"converted to 'mpfr'. The result is rounded to match the specified context.\n"
"If only one of x or y is a number, then that number is returned.");

GMPY_MPFR_BINOP(Minnum, min)

PyDoc_STRVAR(GMPy_doc_function_remainder,
"remainder(x, y) -> mpfr\n\n"
"Return x - n*y where n is the integer quotient of x/y, rounded to\n"
"the nearest integer and ties rounded to even.");

PyDoc_STRVAR(GMPy_doc_context_remainder,
"context.remainder(x, y) -> mpfr\n\n"
"Return x - n*y where n is the integer quotient of x/y, rounded to\n"
"the nearest integer and ties rounded to even.");

GMPY_MPFR_BINOP(Remainder, remainder)

PyDoc_STRVAR(GMPy_doc_function_fmod,
"fmod(x, y) -> mpfr\n\n"
"Return x - n*y where n is the integer quotient of x/y, rounded to 0.");

PyDoc_STRVAR(GMPy_doc_context_fmod,
"context.fmod(x, y) -> mpfr\n\n"
"Return x - n*y where n is the integer quotient of x/y, rounded to 0.");

GMPY_MPFR_BINOP(Fmod, fmod)
