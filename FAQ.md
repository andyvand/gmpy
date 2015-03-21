# Frequently Asked Questions #

**1. What are mutable integers?**

Normal integers in Python (_int_ and/or _long_) and **gmpy2** (_mpz_) are immutable: the value of an integer object does not change unless the object itself changes. The _xmpz_ (eXperimental MPZ) type allows the in-place operations to modify the value of an object without creating a new object.

The following code example shows the behavior of immutable and mutable integers:

```
>>> import gmpy2
>>> gmpy2.mpz(1)
>>> a=1
>>> b=a
>>> a+=1
>>> a
2
>>> b
1
>>> a=gmpy2.xmpz(1)
>>> b=a
>>> a+=1
>>> a
xmpz(2)
>>> b
xmpz(2)
```

In the first part of the example, `a+=1` creates a new object so the value of `b` is unchanged. In the second part of the example, the value of `b` changed after evaluating `a+=1`.

Python dictionaries assume the value of an object never changes so _xmpz_ objects cannot be used as keys in a dictionary.

**2. When should _xmpz_ be used?**

If you have a program that performs a sequence of in-place operations, then using _xmpz_ may be slightly faster (~10%). Certain operations, such as adding or subtracting a small value from a very large number, may be much faster.

Both _mpz_ and _xmpz_ support slice notation for accessing the individual bits of an integer. Slice assignments to individual bits of an _xmpz_ object are done in-place. This can be used to build a very efficient implementation of the Sieve of Eratosthenes.

```
>>> table=gmpy2.xbit_mask(50) # create a table of 50 bits
>>> table[0:2]=0 # remove 0 and 1 from the table
>>> bin(table)
'0b11111111111111111111111111111111111111111111111100'
>>> table[4::2]=0 # remove all even numbers beginning at 4
>>> bin(table)
'0b10101010101010101010101010101010101010101010101100'
>>> table[9::3]=0 # remove all multiples of 3 beginning at 9
>>> bin(table)
'0b10100010100010100010100010100010100010100010101100'
>>> table[25::5]=0 # remove all multiples of 5 beginning at 25
>>> bin(table)
'0b10100010100010000010100000100010100010100010101100'
>>> table[49::7]=0 # remove all multiples of 7 beginning at 49
>>> bin(table)
'0b100010100010000010100000100010100010100010101100'
```

(Historical note: In theory, _xmpz_ should provide a more substantial performance improvement since new objects do not need to be created. However, **gmpy2** and recent versions of **gmpy** include object caching that eliminate much of the penalty for creating new objects.)

**3. Why do I get inaccurate results after converting a number to an _mpfr_ object; even after increasing the precision?**

Python first converts floating point numbers into their standard internal representation which only provides 53 bits of precision. To convert a number to a high-precision value, convert it to a string first.