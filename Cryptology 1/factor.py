import math
n = 551545081
for d in range (1, 20):
	p = math.sqrt(n + d**2) - d
	# If p is an integer, we found our match.
	if (p % 1 == 0):
		q = p + 2*d
		break