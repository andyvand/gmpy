/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * gmpy_xmpz_inplace.c                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Python interface to the GMP or MPIR, MPFR, and MPC multiple precision   *
 * libraries.                                                              *
 *      Copyright 2000 - 2009 Alex Martelli                                *
 *      Copyright 2008, 2009, 2010, 2011, 2012 Case Van Horsen             *
 *                                                                         *
 * This library is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation; either version 2.1 of the License, or  *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library; if not, write to the Free Software     *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA           *
 * 02110-1301  USA                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* Provides inplace mutating operations for xmpz. */

#include <math.h>

/* Inplace xmpz addition. */

static PyObject *
Pyxmpz_inplace_add(PyObject *a, PyObject *b)
{
    mpz_t tempz;
    gmp_si temp_si;
    int overflow;

    /* Try to make mpz + small_int faster */
    if (PyIntOrLong_Check(b)) {
        TRACE("Adding (xmpz,long)\n");
        temp_si = PyLong_AsGmp_siAndOverflow(b, &overflow);
        if (overflow) {
            mpz_inoc(tempz);
            mpz_set_PyLong(tempz, b);
            mpz_add(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), tempz);
            mpz_cloc(tempz);
        }
        else if(temp_si >= 0) {
            mpz_add_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        }
        else {
            mpz_sub_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), -temp_si);
        }
        Py_INCREF(a);
        return a;
    }

    if (CHECK_MPZANY(b)) {
        mpz_add(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(b));
        Py_INCREF(a);
        return a;
    }

    TRACE("Pyxmpz_inplace_add returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace mpz subtraction.
 */

static PyObject *
Pyxmpz_inplace_sub(PyObject *a, PyObject *b)
{
    mpz_t tempz;
    gmp_si temp_si;
    int overflow;

    if (PyIntOrLong_Check(b)) {
        TRACE("Subtracting (xmpz,long)\n");
        temp_si = PyLong_AsGmp_siAndOverflow(b, &overflow);
        if (overflow) {
            mpz_inoc(tempz);
            mpz_set_PyLong(tempz, b);
            mpz_sub(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), tempz);
            mpz_cloc(tempz);
        }
        else if(temp_si >= 0) {
            mpz_sub_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        }
        else {
            mpz_add_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), -temp_si);
        }
        Py_INCREF(a);
        return a;
    }

    if (CHECK_MPZANY(b)) {
        mpz_sub(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(b));
        Py_INCREF(a);
        return a;
    }

    TRACE("Pyxmpz_inplace_sub returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace xmpz multiplication.
 */

static PyObject *
Pyxmpz_inplace_mul(PyObject *a, PyObject *b)
{
    mpz_t tempz;
    gmp_si temp_si;
    int overflow;

    if (PyIntOrLong_Check(b)) {
        TRACE("Multiplying (xmpz,long)\n");
        temp_si = PyLong_AsGmp_siAndOverflow(b, &overflow);
        if (overflow) {
            mpz_inoc(tempz);
            mpz_set_PyLong(tempz, b);
            mpz_mul(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), tempz);
            mpz_cloc(tempz);
        }
        else {
            mpz_mul_si(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        }
        Py_INCREF(a);
        return a;
    }

    if (CHECK_MPZANY(b)) {
        mpz_mul(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(b));
        Py_INCREF(a);
        return a;
    }

    TRACE("Pyxmpz_inplace_mul returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Pympany_floordiv follows the // semantics from Python 3.x. The result is
 * an mpz when the arguments are mpz or mpq, but the result is an mpf when
 * the arguments are mpf.
 */

static PyObject *
Pyxmpz_inplace_floordiv(PyObject *a, PyObject *b)
{
    mpz_t tempz;
    gmp_si temp_si;
    int overflow;

    if (PyIntOrLong_Check(b)) {
        TRACE("Floor divide (xmpz,long)\n");
        temp_si = PyLong_AsGmp_siAndOverflow(b, &overflow);
        if (overflow) {
            mpz_inoc(tempz);
            mpz_set_PyLong(tempz, b);
            mpz_fdiv_q(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), tempz);
            mpz_cloc(tempz);
        }
        else if(temp_si == 0) {
            ZERO_ERROR("xmpz division by zero");
            return NULL;
        }
        else if(temp_si > 0) {
            mpz_fdiv_q_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        }
        else {
            mpz_cdiv_q_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), -temp_si);
            mpz_neg(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a));
        }
        Py_INCREF(a);
        return a;
    }

    if (CHECK_MPZANY(b)) {
        if (mpz_sgn(Pyxmpz_AS_MPZ(b)) == 0) {
            ZERO_ERROR("xmpz division by zero");
            return NULL;
        }
        mpz_fdiv_q(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(b));
        Py_INCREF(a);
        return a;
    }

    TRACE("Pyxmpz_inplace_floordiv returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace xmpz remainder.
 */

static PyObject *
Pyxmpz_inplace_rem(PyObject *a, PyObject *b)
{
    mpz_t tempz;
    gmp_si temp_si;
    int overflow;

    if (PyIntOrLong_Check(b)) {
        TRACE("Modulo (xmpz,long)\n");
        temp_si = PyLong_AsGmp_siAndOverflow(b, &overflow);
        if (overflow) {
            mpz_inoc(tempz);
            mpz_set_PyLong(tempz, b);
            mpz_fdiv_r(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), tempz);
            mpz_cloc(tempz);
        }
        else if(temp_si > 0) {
            mpz_fdiv_r_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        }
        else if(temp_si == 0) {
            ZERO_ERROR("xmpz modulo by zero");
            return NULL;
        }
        else {
            mpz_cdiv_r_ui(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), -temp_si);
        }
        Py_INCREF(a);
        return a;
    }

    if (CHECK_MPZANY(b)) {
        TRACE("Modulo (integer,integer)\n");
        if(mpz_sgn(Pyxmpz_AS_MPZ(b)) == 0) {
            ZERO_ERROR("xmpz modulo by zero");
            return NULL;
        }
        mpz_fdiv_r(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(b));
        Py_INCREF(a);
        return a;
    }

    TRACE("Pyxmpz_inplace_rem returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace xmpz rshift.
 */

static PyObject *
Pyxmpz_inplace_rshift(PyObject *a, PyObject *b)
{
    gmp_si temp_si;
    int overflow;

    if (PyIntOrLong_Check(b)) {
        TRACE("right shift\n");
        temp_si = PyLong_AsGmp_siAndOverflow(b, &overflow);
        if (overflow) {
            OVERFLOW_ERROR("outrageous shift count");
            return NULL;
        }
        else if(temp_si >= 0) {
            mpz_fdiv_q_2exp(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
            Py_INCREF(a);
            return a;
        }
        else {
            VALUE_ERROR("negative shift count");
            return NULL;
        }
    }

    if (CHECK_MPZANY(b)) {
        if (mpz_sgn(Pyxmpz_AS_MPZ(b)) < 0) {
            VALUE_ERROR("negative shift count");
            return NULL;
        }
        if (!mpz_fits_si(Pyxmpz_AS_MPZ(b))) {
            OVERFLOW_ERROR("outrageous shift count");
            return NULL;
        }
        temp_si = mpz_get_si(Pyxmpz_AS_MPZ(b));
        mpz_fdiv_q_2exp(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        Py_INCREF(a);
        return a;
    }

    TRACE("Pyxmpz_inplace_rshift returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace xmpz lshift.
 */

static PyObject *
Pyxmpz_inplace_lshift(PyObject *a, PyObject *b)
{
    gmp_si temp_si;
    int overflow;

    if (PyIntOrLong_Check(b)) {
        TRACE("left shift\n");
        temp_si = PyLong_AsGmp_siAndOverflow(b, &overflow);
        if (overflow) {
            OVERFLOW_ERROR("outrageous shift count");
            return NULL;
        }
        else if(temp_si >= 0) {
            mpz_mul_2exp(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        }
        else {
            VALUE_ERROR("negative shift count");
            return NULL;
        }
    }

    if (CHECK_MPZANY(b)) {
        if (mpz_sgn(Pyxmpz_AS_MPZ(b)) < 0) {
            VALUE_ERROR("negative shift count");
            return NULL;
        }
        if (!mpz_fits_si(Pyxmpz_AS_MPZ(b))) {
            OVERFLOW_ERROR("outrageous shift count");
            return NULL;
        }
        temp_si = mpz_get_si(Pyxmpz_AS_MPZ(b));
        mpz_mul_2exp(Pyxmpz_AS_MPZ(a), Pyxmpz_AS_MPZ(a), temp_si);
        Py_INCREF(a);
        return a;
    }

    TRACE("Pyxmpz_inplace_lshift returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace xmpz_pow.
 */

static PyObject *
Pyxmpz_inplace_pow(PyObject *in_b, PyObject *in_e, PyObject *in_m)
{
    PympzObject *e = 0;
    gmp_ui el;

    TRACE("Pyxmpz_inplace_pow\n");

    if (!Pyxmpz_Check(in_b)) {
        PyErr_SetString(PyExc_TypeError, "base must be an Integer");
        return NULL;
    }
    if (in_m != Py_None) {
        SYSTEM_ERROR("modulo not expected");
        return NULL;
    }
    e = Pympz_From_Integer(in_e);
    if (!e) {
        TYPE_ERROR("expected an integer exponent");
        return NULL;
    }
    if (mpz_sgn(e->z) < 0) {
        VALUE_ERROR("xmpz.pow with negative power");
        Py_DECREF((PyObject*)e);
        return NULL;
    }
    if (!mpz_fits_ui(e->z)) {
        VALUE_ERROR("xmpz.pow outrageous exponent");
        Py_DECREF((PyObject*)e);
        return NULL;
    }
    el = mpz_get_ui(e->z);
    mpz_pow_ui(Pyxmpz_AS_MPZ(in_b), Pyxmpz_AS_MPZ(in_b), el);
    Py_DECREF((PyObject*)e);
    Py_INCREF((PyObject*)in_b);
    return (PyObject*)in_b;
}

/* Inplace xmpz and.
 */

static PyObject *
Pyxmpz_inplace_and(PyObject *self, PyObject *other)
{
    mpz_t tempz;

    if (CHECK_MPZANY(other)) {
        mpz_and(Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(other));
        Py_INCREF(self);
        return self;
    }

    if (PyIntOrLong_Check(other)) {
        mpz_inoc(tempz);
        mpz_set_PyLong(tempz, other);
        mpz_and(Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(self), tempz);
        mpz_cloc(tempz);
        Py_INCREF(self);
        return self;
    }

    TRACE("Pyxmpz_inplace_and returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace xmpz xor.
 */

static PyObject *
Pyxmpz_inplace_xor(PyObject *self, PyObject *other)
{
    mpz_t tempz;

    if(CHECK_MPZANY(other)) {
        mpz_xor(Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(other));
        Py_INCREF(self);
        return self;
    }

    if(PyIntOrLong_Check(other)) {
        mpz_inoc(tempz);
        mpz_set_PyLong(tempz, other);
        mpz_xor(Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(self), tempz);
        mpz_cloc(tempz);
        Py_INCREF(self);
        return self;
    }

    TRACE("Pyxmpz_inplace_xor returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

/* Inplace xmpz or.
 */

static PyObject *
Pyxmpz_inplace_ior(PyObject *self, PyObject *other)
{
    mpz_t tempz;

    if(CHECK_MPZANY(other)) {
        mpz_ior(Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(other));
        Py_INCREF(self);
        return self;
    }

    if(PyIntOrLong_Check(other)) {
        mpz_inoc(tempz);
        mpz_set_PyLong(tempz, other);
        mpz_ior(Pyxmpz_AS_MPZ(self), Pyxmpz_AS_MPZ(self), tempz);
        mpz_cloc(tempz);
        Py_INCREF(self);
        return self;
    }

    TRACE("Pyxmpz_inplace_ior returned NotImplemented\n");
    Py_RETURN_NOTIMPLEMENTED;
}

