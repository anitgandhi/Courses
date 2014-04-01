# Program which tests for primes within a given range.
import sys
import math
from random import randint

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23]

# Returns true if m is prime according to trial division
def trialDivision(m):
	for p in primes:
		if p <= math.sqrt(m):
			if m % p == 0:
				return False
		else:
			primes.append(m)
			return True

# Returns true if m is probably prime with k iterations
def millerRabin(m, k):
	# Check if even
	if m % 2 == 0:
		return False

	# Calculate the prerequisites
	s = 1
	t = (m-1)/2
	while t%2 == 0:
		t /= 2
		s += 1

	t = int(t)

	for r in range (0,k):
		rand = randint(1, m-1)
		y = pow(rand, t, m)
		prime = False

		if y == 1:
			prime = True

		for i in range(0,s):
			if (y == m-1):
				prime = True
				break
			else:
				y = pow(y,2,m)

		if not prime:
			return False

	return True

start = int(input("\nEnter the start of range: "))
end = int(input("Enter the end of range: "))

# Trial Division
sumTD = 0
for i in range(start, end+1):
	if trialDivision(i):
		sumTD += 1
print("\nTrial Division: ", sumTD)

# Miller Rabin for k being 1 through 10
print("\nMiller Rabin")
for k in range(1,11):
	sumMR = 0
	for i in range(start, end+1):
		if millerRabin(i,k):
			sumMR += 1
	print("\n", k, "iterations:\t", sumMR)