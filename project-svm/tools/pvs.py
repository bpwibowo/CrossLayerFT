import os
import sys
import yaml

if len(sys.argv) < 2:
	print "Usage: %s [pvs_file]" % sys.argv[0]
	exit(0)

pvs_file = sys.argv[1]

if not os.path.isfile(pvs_file):
	print "Error: File %s not found." % pvs_file
	exit(0)

pvs = yaml.load(open(pvs_file, "r").read())

imp_cnt = 0
inst_cnt = 0
for pc in pvs["inst"]:
	vals = [int(v) for v in pvs["inst"][pc].split("/")]
	imp_cnt += vals[0]
	inst_cnt += vals[1]

print "Important count: %d" % imp_cnt
print "Dynamic instruction count: %d" % inst_cnt
print "Important ratio: %f" % (float(imp_cnt)/inst_cnt)

# end of file
