/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * gmpy2_abs.c                                                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Python interface to the GMP or MPIR, MPFR, and MPC multiple precision   *
 * libraries.                                                              *
 *                                                                         *
 * Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,               *
 *           2008, 2009 Alex Martelli                                      *
 *                                                                         *
 * Copyright 2008, 2009, 2010, 2011, 2012, 2013 Case Van Horsen            *
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

/* This file implements abs() and context.abs().
 *
 * Private API
 * ===========
 * The Python function abs() calls the __abs__ method of a numeric type. This
 * file implements the following private functions:
 *
 *   GMPy_mpz_abs_fast; called by abs() via mpz.__abs__
 *   GMPy_mpq_abs_fast; called by abs() via mpq.__abs__
 *   GMPy_mpfr_abs_fast; called by abs() via mpfr.__abs__
 *   GMPy_mpc_abs_fast; called by abs() via mpc.__abs__
 *
 *   GMPy_Context_Abs; called by context.abs()
 *
 * Public API
 * ==========
 * The following functions are availabe as part of GMPY2's C API. A NULL value
 * for context implies the function should use the currently active context.
 * The first four functions check the type of the first argument and will set
 * an exception and return NULL if the check fails.
 *
 *   GMPy_Integer_Abs(Integer, context|NULL)
 *   GMPy_Rational_Abs(Rational, context|NULL)
 *   GMPy_Real_Abs(Real, context|NULL)
 *   GMPy_Complex_Abs(Complex, context|NULL)
 *   GMPy_Number_Abs(Number, context|NULL)
 *
 */

static PyObject *
GMPy_Integer_Abs(PyObject *x, GMPyContextObject *context)
{
    MPZ_Object *result;

    if (IS_INTEGER(x)) {
        if ((result = GMPy_MPZ_From_Integer(x))) {
            mpz_abs(result->z, result->z);
        }
        return (PyObject*)result;
    }
    else {
        TYPE_ERROR("abs(): argument is not an integer number");
        return NULL;
    }
}

static PyObject *
GMPy_mpz_abs_fast(MPZ_Object *x)
{
    MPZ_Object *result;

    if ((result = GMPy_MPZ_New())) {
        mpz_abs(result->z, x->z);
    }

    return (PyObject*)result;
}

static PyObject *
GMPy_Rational_Abs(PyObject *x, GMPyContextObject *context)
{
    MPQ_Object *result;

    if (IS_RATIONAL(x)) {
        if ((result = Pympq_From_Rational(x))) {
            mpz_abs(mpq_numref(result->q), mpq_numref(result->q));
        }
        return (PyObject*)result;
    }
    else {
        TYPE_ERROR("abs(): argument is not a rational number");
        return NULL;
    }
}

static PyObject *
GMPy_mpq_abs_fast(MPQ_Object *x)
{
    MPQ_Object *result;

    if ((result = (MPQ_Object*)Pympq_new())) {
        mpq_set(result->q, x->q);
        mpz_abs(mpq_numref(result->q), mpq_numref(result->q));
    }

    return (PyObject*)result;
}

static PyObject *
GMPy_Real_Abs(PyObject *x, GMPyContextObject *context)
{
    MPFR_Object *result;

    if (!context)
        CURRENT_CONTEXT(context);

    SET_EXPONENT(context);

    if (!(result = (MPFR_Object*)Pympfr_new_context(context)))
        return NULL;

    if (IS_REAL(x)) {
        MPFR_Object *tempx;

        if (!(tempx = GMPy_MPFR_From_Real_Temp(x, context))) {
            Py_DECREF((PyObject*)result);
            return NULL;
        }
        mpfr_clear_flags();
        result->rc = mpfr_abs(result->f, tempx->f, GET_MPFR_ROUND(context));
        Py_DECREF((PyObject*)tempx);
        MPFR_CLEANUP_2(result, context, "abs()");
        return (PyObject*)result;
    }
    else {
        TYPE_ERROR("abs(): argument is not a real number");
        Py_DECREF((PyObject*)result);
        return NULL;
    }
}

static PyObject *
GMPy_mpfr_abs_fast(MPFR_Object *x)
{
    return GMPy_Real_Abs((PyObject*)x, NULL);
}

static PyObject *
GMPy_Complex_Abs(PyObject *x, GMPyContextObject *context)
{
    MPFR_Object *result;

    if (!context)
        CURRENT_CONTEXT(context);

    SET_EXPONENT(context);

    if (!(result = (MPFR_Object*)Pympfr_new_context(context)))
        return NULL;

    if (IS_COMPLEX(x)) {
        MPC_Object *tempx;

        if (!(tempx = GMPy_MPC_From_Complex_Temp(x, context)))  {
            Py_DECREF((PyObject*)result);
            return NULL;
        }

        /* Per MPC source code, mpc_abs just calls mpfr_hypot. We will just
         * call mpfr_hypot directly.
         */
        mpfr_clear_flags();
        result->rc = mpfr_hypot(result->f, mpc_realref(tempx->c),
                                mpc_imagref(tempx->c), GET_MPFR_ROUND(context));
        Py_DECREF((PyObject*)tempx);
        MPFR_CLEANUP_2(result, context, "abs()");
        return (PyObject*)result;
    }
    else {
        TYPE_ERROR("abs(): argument is not a complex number");
        Py_DECREF((PyObject*)result);
        return NULL;
    }
}

static PyObject *
GMPy_mpc_abs_fast(MPC_Object *x)
{
    return GMPy_Complex_Abs((PyObject*)x, NULL);
}

static PyObject *
GMPy_Number_Abs(PyObject *x, GMPyContextObject *context)
{
    if (!context)
        CURRENT_CONTEXT(context);

    if (IS_INTEGER(x))
        return GMPy_Integer_Abs(x, context);

    if (IS_RATIONAL_ONLY(x))
        return GMPy_Rational_Abs(x, context);

    if (IS_REAL_ONLY(x))
        return GMPy_Real_Abs(x, context);

    if (IS_COMPLEX_ONLY(x))
        return GMPy_Complex_Abs(x, context);

    TYPE_ERROR("abs(): argument type not supported");
    return NULL;
}

/* Implement context.abs(). The following code assumes it used a as method of
 * a context. */

PyDoc_STRVAR(GMPy_doc_context_abs,
"context.abs(x) -> number\n\n"
"Return abs(x), the context is applied to the result.");

static PyObject *
GMPy_Context_Abs(PyObject *self, PyObject *args)
{
    PyObject *result;
    GMPyContextObject *context = NULL;

    if (PyTuple_GET_SIZE(args) != 1) {
        TYPE_ERROR("context.abs() requires 1 argument.");
        return NULL;
    }

    if (self && GMPyContext_Check(self)) {
        /* If we are passed a read-only context, make a copy of it before
         * proceeding. Remember to decref context when we're done.
         */

        if (((GMPyContextObject*)self)->ctx.readonly) {
            context = (GMPyContextObject*)GMPyContext_context_copy(self, NULL);
            if (!context)
                return NULL;
        }
        else {
            context = (GMPyContextObject*)self;
            Py_INCREF((PyObject*)context);
        }
    }
    else {
        SYSTEM_ERROR("function is not supported");
        return NULL;
    }

    result = GMPy_Number_Abs(PyTuple_GET_ITEM(args, 0), context);
    Py_DECREF((PyObject*)context);
    return result;
}
