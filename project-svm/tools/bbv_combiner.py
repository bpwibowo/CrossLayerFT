#!/usr/bin/python

##
# BBV combiner
#
# first author: Bagus Wibowo
# date created: 09/01/2015

import sys
import os
import argparse
import gzip

def DumpTable(table, fout):
	line = "T"
	for left in sorted(table):
		line += ":%d:%d   " % (left, table[left])
	line += "\n"
	fout.write(line)

# Parse arguments
parser = argparse.ArgumentParser(
	description='Combine multiple lines in BBV file.')

parser.add_argument("input", help="input BBV file")
parser.add_argument("output", help="output BBV file")
parser.add_argument("combine_size", type=int,
	help="number of lines in BBV file that will be combined into one")

args = parser.parse_args()

# check if input file exists
if not os.path.isfile(args.input):
	print "Error: could not find input file '%s'" % args.input
	sys.exit(1)

# check if combine_size value is valid
if args.combine_size < 1:
	print "Error: invalid combine_size, it should be greater than 0"
	sys.exit(1)

valid_lines_in = 0
valid_lines_out = 0

fin = None
fout = None

if args.input.endswith(".gz"): fin = gzip.open(args.input, 'rb')
else: fin = open(args.input, 'r')

if args.output.endswith(".gz"): fout = gzip.open(args.output, 'wb')
else: fout = open(args.output, 'w')

try:
	idx = 0
	table = {}
	for line in fin.read().split("\n"):
		line = line.strip()
		if len(line) == 0: continue
		if line[0] == "T": # check if line starts with "T"
			valid_lines_in += 1
			pairs = line[1:].split()
			for pair in pairs:
				[left, right] = pair[1:].split(":")
				left = int(left)
				right = int(right)
				if left not in table: table[left] = right
				else: table[left] += right

			# post line processing
			idx += 1
			if idx == args.combine_size:
				valid_lines_out += 1
				DumpTable(table, fout)
				table = {}

				# clear idx
				idx = 0
		else:
			# line does not start with "T", directly pass it to output
			fout.write("%s\n" % line)

except IOError:
	print "Error: Couldn't open gzipped file '%s'. " % args.input + \
		"The file is probably not a gzipped file or is corrupted."
	sys.exit(1)

finally:
	fin.close()
	fout.close()

print "# of valid input line = %d" % valid_lines_in
print "# of valid output line = %d" % valid_lines_out
print "BBV combiner completed!"

# end of file
