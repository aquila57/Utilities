# import libraries used in this program
import sys,random
# read all of standard input into a list of lines
lst = sys.stdin.readlines()
# initialize the dictionary of lines
d = {}
# initialize the random number generator
# to current date and time
random.seed(a=None)
# line by line, add each line to the dictionary
# with a random key
for ln in lst:
	# strip the end of line character
	ln = ln[0:-1]
	# generate a random key between 0 and 1
	ky = random.random()
	# add this line to the dictionary by the random key
	# duplicate keys are not allowed
	# re-run job if duplicate key occurs
	if not ky in d:
		d[ky] = ln
	else:
		print 'A duplicate key error has occurred'
		print 'Rerun job to eliminate this error'
		sys.exit(1)
# create a sorted list of all random keys
slst = sorted(d.keys())
# for each key in sorted sequence
# print out it's corresponding line of data
for ky in slst:
	# extract the next line of data into val
	val = d[ky]
	# print the line of data just extracted
	# in sequence by its sorted key
	print val
