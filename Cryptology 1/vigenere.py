# This is a Python program I wrote for my Cryptology 1 course. It was created to break a simple Vigenere cipher.

# Set the cipher input. Hardcoding it for the purposes of this problem.
cipher = "PATGSJKGSPFPCTSSKHOIGSDHNBCUHVIHKSHVBKPBQLEGVFSHPL\
TQFLYRWSRLYBSSRPPPPGIUOTUSHVPTZSVLNBCHCIWMIZSZKPWWZ\
LZKXJWUCMWFCBCAACBKKGDBHOAPPMHVBKPBQLDXKWGPPXSZCUZH\
CNCVWGSOGRAWIVSTPHROFLBHGHVLYNQIBAEEWWGYAMJFBDDBRVV\
LKIIWAPOMXQOSHRPBHPYBEOHLZPDIZKXXCCZVJZTFHOWGIKCDAX\
ZGCMYHJFGLPATKOYSTHBCAPHTBRZKJJWQRHR"

# Find the length of the keyword
def ioc(text):
	freq = [0] * 26
	for c in text:
		freq[ord(c)-65] += 1
	sumFreq = 0
	for f in freq:
		sumFreq += f*(f-1)
	n = float(len(text))
	return sumFreq/(n*(n-1))

avg = 0
diff = []

# Check all periods from 1 to 10, and average the resulting IOCs of each period. The one with the smallest difference is the actual period.
for i in range(1,11):
	splitCipher = []
	for c in range(0,i):
		splitCipher.append(cipher[c::i])

	for j in splitCipher:
		avg += ioc(j)
	avg /= i

	diff.append(abs(avg-0.065))
	avg = 0

# Must add offset of 1 because the index will start at 0, but period starts at 1 or above. 
klen = diff.index(min(diff))+1
print("\nKeyword length is: ", klen)

# Now that the keyword length is found, find the keyword
splitCipher = []
for c in range(0,klen):
	splitCipher.append(cipher[c::klen])

# Calculates frequency distribution
def q(rowText, shift):
	shiftedText = ""
	for r in rowText:
		shiftedText += chr((ord(r)-shift-65)%26 + 65)
	freq = [0] * 26
	for s in shiftedText:
		freq[ord(s)-65] += 1

	length = float(len(shiftedText))
	freq = [x/length for x in freq]

	return freq

# Calculates distances
def d(rowText, i):
	p = [0.082, 0.015, 0.028, 0.043, 0.126, 0.022, 0.020, 0.061, 0.070, 0.002, 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091, 0.028, 0.010, 0.023, 0.001, 0.020, 0.001]

	distance = 0
	for l in range(0,26):
		distance += (p[l] - q(rowText, i)[l])**2

	return distance

key = ""

# Run it for all rows
for row in splitCipher:
	distances = []
	for k in range(0,26):
		distances.append(d(row,k))
	key += chr(distances.index(min(distances))+65)

print("\nThe key is: ", key)

# Now that the keyword is found, decrypt the text
extendedKey = key * (int(len(cipher)/len(key))+1)
decrypted = ""
for i in range(0, len(cipher)):
	decrypted += chr((ord(cipher[i]) - ord(extendedKey[i]))%26 + 65)

print("\nThe decrypted message is:\n\n" + decrypted)