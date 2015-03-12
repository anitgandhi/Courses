from random import randint

# Note: Python's pow(a, b, c) function calculates a^b. If c is given, then it uses square multiply to calculate a^b mod c.

# Hardcode in the given information
n = 2051152801041163
p = 2189284635404723
m = 136203
k = 1234567

# Multiplicative inverse of k modulo (p-1) was found separately using Maple
kinv = 427810349476471

# Find the hash of the message using the publicly defined hash function
x = pow(8, m, n)

# a can be any number in Z/p-1, so it is randomly chosen here.
a = randint(0, p-1)

# Guess alpha is 42, and check to make sure that the order is 
alpha = 42
orders = [1, 2, (p-1)//2, p-1]

for o in orders:
	if pow(alpha, o, p) == 1:
		order = o

if order == p-1:
	print("The order is p-1, so alpha = 42 is a valid primitive element.")

# Now calculate the signature
beta = pow(alpha, a, p)
gamma = pow(alpha, k, p)
delta = ((x - a*gamma)*kinv) % (p-1)

print("Alpha:", alpha)
print("a:", a)
print("Beta:", beta)
print("Gamma:", gamma)
print("Delta:", delta)

# Verifcation. VL/VR means the two sides of the verification equation. VL was found by calculating the two parts separately, since these numbers are relatively large and the computer would take a long time to calculate it otherwise. 
vl1 = pow(beta, gamma, p)
vl2 = pow(gamma, delta, p)
vr = pow(alpha, x, p)
vl = (vl1*vl2) % p

if vl == vr:
	print("Signature Verified")
else:
	print("Signature NOT Verified")