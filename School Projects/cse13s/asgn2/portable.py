#!/usr/bin/env python3

# ☠️  2022 Darrell Long ☠️

from math import pi as π

def sqrt(x, 𝜺=1e-15):
    """
    Calculate square root using Newton's method using the iterate:
        y + x / y
    y = ---------
            2
    """
    f = 1 # Scaling factor
    y = 1 # Initial guess
    while x > 4: # Normalize to [0, 4) by reducing the exponent in IEEE 754
        x /= 4   # Every 4 going in results in 2 going out since √4 = 2
        f *= 2   # This should not disturb the mantissa
    guess = 0
    while abs(y - guess) > 𝜺:
        guess = y;
        y = (y + x / y) / 2
    return f * y; # Rescale back to the original range

def exp(x, 𝜺=1e-15):
    """
     x
    e =
             2    3    4    5     6      7      8        9        10
            x    x    x    x     x      x      x        x        x           11
    1 + x + -- + -- + -- + --- + --- + ---- + ----- + ------ + ------- + O[x]
            2    3!   4!    5!    6!    7!     8!       9!       10!
    """
    trm = 1 # 1 for the multiplication that generates the terms
    sum = 1 # Zeroth term is always 1
    k   = 1
    while abs(trm) > 𝜺:
        trm *= abs(x) / k
        sum += trm
        k   += 1
    return 1 / sum if x < 0 else sum

def log(x, 𝜺=1e-15):
    """
    Calculate log(x) by finding the root of e - y = 0, using the iterate:
         x
    -1 + -- + y
          y
         e
    """
    f = 0
    e = 2.7182818284590455 # Remove factors of Euler's constant
    while x > e:
        f += 1
        x /= e
    y = 1 # Initial guess
    p = exp(y)
    while abs(p - x) > 𝜺:
        y = y + x / p - 1
        p = exp(y)
    return y + f

def pow(x, y, 𝜺=1e-15):
    """
     y   y log(x)
    x = e
    """
    return exp(y * log(x, 𝜺), 𝜺)

def modulus(a, b): return a - b * int(a / b)

def sin(x, 𝜺=1e-15):
    """
    sin(x) =
         3    5      7       9        11          13             15
        x    x      x       x        x           x              x              16
    x - -- + --- - ---- + ------ - -------- + ---------- - ------------- + O(x)
        3!    5!    7!      9!       11!         13!            15!
    """
    x = modulus(x, 2 * π) # Normalize to [-2π, 2π]
    val, trm = x, x
    k = 3
    while abs(trm) > 𝜺:
        trm *= -(x * x) / ((k - 1) * k)
        val += trm
        k   += 2
    return val

def cos(x, 𝜺=1e-15):
    """
    cos(x) =
         2    4    6      8        10         12           14
        x    x    x      x        x          x            x             16
    1 - -- + -- - --- + ----- - ------- + --------- - ----------- + O(x)
        2    4!   6!     8!       10!        12!          14!
    """
    x = modulus(x, 2 * π) # Normalize to [-2π, 2π]
    val, trm = 1, 1
    k = 1
    while abs(trm) > 𝜺:
        trm *= -(x * x) / ((k + 1) * k)
        val += trm;
        k += 2
    return val

def tan(x, 𝜺=1e-15):
    """
             sin(x)
    tan(x) = ------
             cos(x)
    """
    return sin(x, 𝜺) / cos(x, 𝜺)

def cot(x, 𝜺=1e-15):
    """
               1
    cot(x) = ------
             tan(x)
    """
    return 1 / tan(x, 𝜺)

def asin(x, 𝜺=1e-15):
    """
    Calculate arcsin(x) as the root of sin(x) - a = 0 using the iterate:

            sin(z) - x
    z = z - ----------
              cos(z)
    """
    t = 1
    z = 0
    if x > 31/32:
        return acos(sqrt(1 - x**2, 𝜺), 𝜺) if x > 0 else -acos(sqrt(1 - x**2, 𝜺), 𝜺)
    else:
        while abs(t - z) > 𝜺:
            t = z
            z = z - (sin(z, 𝜺) - x) / cos(z, 𝜺)
        return z

def acos(x, 𝜺=1e-15):
    """
    Calculate arcos(x) as the root of cos(x) - a = 0 using the iterate:

            cos(z) - x
    z = z - ----------
             -sin(z)
    """
    t = 0
    z = 1
    if x > 31/32:
        return π - asin(sqrt(1 - x**2, 𝜺), 𝜺) if x < 0 else asin(sqrt(1 - x**2, 𝜺), 𝜺)
    else:
        while abs(t - z) > 𝜺:
            t = z
            z = z + (cos(z, 𝜺) - x) / sin(z, 𝜺)
        return z

def atan(x, 𝜺=1e-15):
    """
                   ⎡       x      ⎤
    atan(x) = asin ⎜--------------⎟
                   ⎣sqrt(1 + x**2)⎦
    """
    if x > 1:
        return acot(1 / x, 𝜺)
    elif x < -1:
        return -acot(1 / x, 𝜺)
    else:
        return asin(x / sqrt(1 + x**2, 𝜺), 𝜺)

def acot(x, 𝜺=1e-15):
    """
                   ⎡       1      ⎤
    acot(x) = asin ⎜--------------⎟
                   ⎣sqrt(1 + x**2)⎦
    """
    if x > 1:
        return atan(1/ x, 𝜺)
    elif x < -1:
        return -atan(1 / x, 𝜺)
    else:
        return asin(1 / sqrt(1 + x**2, 𝜺), 𝜺)


# Hyperbolic functions

def sinh(x, 𝜺=1e-15):
    """
               x    -x
              e  - e
    sinh(x) = ---------
                 2
    """
    return (exp(x, 𝜺) - exp(-x, 𝜺)) / 2

def cosh(x, 𝜺=1e-15):
    """
               x    -x
              e  + e
    cosh(x) = ---------
                 2
    """
    return (exp(x, 𝜺) + exp(-x, 𝜺)) / 2

def sech(x, 𝜺=1e-15):
    """
                 1
    sech(x) = -------
              cosh(x)
    """
    return 1 / cosh(x, 𝜺)

def csch(x, 𝜺=1e-15):
    """
                 1
    csch(x) = -------
              sinh(x)
    """
    return 1 / sinh(x, 𝜺)

def tanh(x, 𝜺=1e-15):
    """
              sinh(x)
    tanh(x) = -------
              cosh(x)
    """
    return sinh(x, 𝜺) / cosh(x, 𝜺)

def coth(x, 𝜺=1e-15):
    """
              cosh(x)
    coth(x) = -------
              sinh(x)
    """
    return cosh(x, 𝜺) / sinh(x, 𝜺)

# Inverse hyperbolic functions

def asinh(x, 𝜺=1e-15):
    """
    asinh(x) = log(x + sqrt(x**2 + 1))
    """
    return log(x + sqrt(x * x + 1, 𝜺), 𝜺)

def acosh(x, 𝜺=1e-15):
    """
    acosh(x) = log(x + sqrt(x**2 - 1))
    """
    return log(x + sqrt(x * x - 1, 𝜺), 𝜺)

def atanh(x, 𝜺=1e-15):
    """
    atanh(x) = log((x + 1) / (1 - x)) / 2
    """
    return 0.5 * log((x + 1) / (1 - x), 𝜺)

def acoth(x, 𝜺=1e-15):
    """
    acoth(x) = log((x + 1) / (x - 1)) / 2
    """
    return 0.5 * log((x + 1) / (x - 1), 𝜺)

def asech(x, 𝜺=1e-15):
    """
    asech(x) = log(1 + sqrt(1 - x**2))
    """
    return log((1 + sqrt(1 - x * x, 𝜺)) / x, 𝜺)

def acsch(x, 𝜺=1e-15):
    """
    acsch(x) = log(1 / x + sqrt(1 / x**2 + 1))
    """
    return log(1 / x + sqrt(1 / (x * x) + 1, 𝜺), 𝜺)

def main():
    try:
        m = ""
        while not m in ["Quit", "quit", "Q", "q", "Exit", "exit"]:
            m = input("?? ")
            match m:
                case "exp":
                    x = 0
                    while x < 10:
                        print(f"exp({x}) = {exp(x)}")
                        x += 1
                case "log":
                    x = 1
                    while x < 11:
                        print(f"log({x}) = {log(x)}")
                        x += 1
                case "sqrt":
                    x = 0
                    while x < 10:
                        print(f"sqrt({x}) = {sqrt(x)}")
                        x += 0.25
                case "sin":
                    x = -π
                    while x <= π:
                        print(f"sin({x}) = {sin(x)}")
                        x += π / 8
                case "cos":
                    x = -π
                    while x <= π:
                        print(f"cos({x}) = {cos(x)}")
                        x += π / 8
                case "tan":
                    x = 0
                    while x < π / 2:
                        print(f"tan({x}) = {tan(x)}")
                        x += π / 16
                case "asin":
                    x = -1
                    while x <= 1:
                        print(f"asin({x}) = {asin(x)}")
                        x += 0.0625
                case "acos":
                    x = -1
                    while x <= 1:
                        print(f"acos({x}) = {acos(x)}")
                        x += 0.0625
    except:
        print("\nSo long!")

if __name__ == '__main__': main()
