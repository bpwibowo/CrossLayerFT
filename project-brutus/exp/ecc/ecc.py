import os
import sys

class Ecc:
	val = []	# bits storage
	k = None	# data bits
	m = None	# parity bits

	def __init__(self, k):
		self.k = k
		self.m = self.compute_m(self.k)
		k_max = self.compute_maxk(self.m)
		self.val = [0 for n in xrange(k_max + self.m)]

	def get_p(self):
		p = []
		for n in xrange(self.m):
			p.append(self.val[2**n-1])
		return p

	def get_u(self):
		u = []
		next_p = 0
		for n in xrange(1,len(self.val)+1):
			if n == 2**next_p:
				next_p += 1
				continue
			else:
				u.append(self.val[n-1])
		return u

	def get_s(self):
		s = [0 for n in xrange(self.m)]
		for n in xrange(1,len(self.val)+1):
			value = self.val[n-1]
			for m in xrange(self.m-1):
				if (2**m)&n:
					s[m] ^= value
			s[-1] ^= value
		return s[:-1], s[-1]

	def update(self,data):
		u = []
		for n in xrange(self.compute_maxk(self.m)):
			u.append(1 if ((1 << n) & data) else 0)

		next_p = 0
		for n in xrange(1,len(self.val)+1):
			if n == 2**next_p:
				self.val[n-1] = 0
				next_p += 1
			else:
				self.val[n-1] = u.pop()

		p = [0 for n in xrange(self.m)]
		for n in xrange(1,len(self.val)+1):
			value = self.val[n-1]
			for m in xrange(self.m-1):
				if (2**m)&n:
					p[m] ^= value
			p[-1] ^= value

		for n in xrange(self.m-1):
			p[-1] ^= p[n]

		for m in xrange(self.m):
			self.val[2**m-1] = p[m]

		print self.val
		print self.get_p()
		print self.get_u()
		print self.get_s()

	def compute_m(self, k):
		m = 1
		while (2**m < k + m + 1):
			m += 1
		return m+1

	def compute_maxk(self, m):
		m = m-1
		k = 2**m - m - 1
		return k

	def flip(self, n):
		self.val[n-1] ^= 1

	def correct(self):
		syndrome, parity = self.get_s()

		if sum(syndrome) == 0:
			if parity == 0:
				return "Everything is correct"
			else:
				self.val[-1] ^= 1
				return "Parity bit was flipped"
		else:
			if parity == 0:
				return "Double error (uncorrectable)"
			else:
				pos = 0
				print syndrome
				for n in syndrome:
					pos += 1 << n
					print n
				self.val[pos-1] ^= 1
				return "Bit position %d was flipped" % pos

	def gen_checker(self, f):
		# declare module
		print >>f, "module ecc_checker_%d(data_in, checker_in, parity_in);" % self.m
		print >>f, ""

		# declare port
		print >>f, "input      [% 3d:0] data_in;" % (self.compute_maxk(self.m)-1)
		print >>f, "output reg [% 3d:0] checker_in;" % (self.m-1-1)
		print >>f, "output reg         parity_in;"
		print >>f, ""

		# generate checker logic
		p = [[] for n in xrange(self.m)]
		vals = [n for n in xrange(self.compute_maxk(self.m))]
		next_p = 0
		for n in xrange(1,len(self.val)+1):
			if 2**next_p == n: next_p+=1; continue

			pos = vals.pop()
			for m in xrange(self.m-1):
				if (2**m)&n and (2**m != n):
					p[m].append(pos)

		print >>f, "always @(*) begin"

		for m in xrange(self.m-1):
			print >>f, "    checker_in[%d] =" % m,
			cnt = 0
			for v in p[m]:
				cnt += 1
				if cnt > 1: print >>f, "^",
				print >>f, "data_in[%d]" % v,
			print >>f, ";"

		print >>f, "    parity_in =",
		cnt = 0
		for n in xrange(self.compute_maxk(self.m)):
			cnt += 1
			if cnt > 1: print >>f, "^",
			print >>f, "data_in[%d]" % n,
		for m in xrange(self.m-1):
			print >>f, "^ checker_in[%d]" % m,
		print >>f, ";"
		print >>f, ""

		print >>f, "end // always(*)"

		# close the module
		print >>f, "endmodule"
		print >>f, ""
		print >>f, ""

	def gen_mem(self, f):
		# declare module
		print >>f, "module data_mem_%d(clock, reset, we, data_in, data_out);" % self.m
		print >>f, ""

		# declare port
		print >>f, "input              clock;"
		print >>f, "input              reset;"
		print >>f, "input              we;"
		print >>f, "input      [% 3d:0] data_in;" % (self.compute_maxk(self.m)-1)
		print >>f, "output reg [% 3d:0] data_out;" % (self.compute_maxk(self.m)-1)
		print >>f, ""

		# body
		print >>f, "always @(posedge clock) begin"
		print >>f, "    if (reset) begin"
		print >>f, "        data_out <= 0;"
		print >>f, "    end"
		print >>f, "    else if (we) begin"
		print >>f, "        data_out <= data_in;"
		print >>f, "    end"
		print >>f, "end"

		# close the module
		print >>f, "endmodule"
		print >>f, ""
		print >>f, ""

		# declare module
		print >>f, "module ecc_mem_%d(clock, reset, we, checker_in, parity_in, checker_out, parity_out);" % self.m
		print >>f, ""

		# declare ports
		print >>f, "input              clock;"
		print >>f, "input              reset;"
		print >>f, "input              we;"
		print >>f, "input      [% 3d:0] checker_in;" % (self.m-2)
		print >>f, "input              parity_in;"
		print >>f, "output reg [% 3d:0] checker_out;" % (self.m-2)
		print >>f, "output reg         parity_out;"
		print >>f, ""

		# body
		print >>f, "always @(posedge clock) begin"
		print >>f, "    if (reset) begin"
		print >>f, "        checker_out <= 0;"
		print >>f, "        parity_out <= 0;"
		print >>f, "    end"
		print >>f, "    else if (we) begin"
		print >>f, "        checker_out <= checker_in;"
		print >>f, "        parity_out <= parity_in;"
		print >>f, "    end"
		print >>f, "end"

		# close the module
		print >>f, "endmodule"
		print >>f, ""
		print >>f, ""

	def gen_corrector(self, f):
		# declare module
		print >>f, "module ecc_corrector_%d(data_out, checker_out, parity_out, correct_data_out, error);" % self.m
		print >>f, ""

		# declare port
		print >>f, "input      [% 3d:0] data_out;" % (self.compute_maxk(self.m)-1)
		print >>f, "input      [% 3d:0] checker_out;" % (self.m-1-1)
		print >>f, "input              parity_out;"
		print >>f, "output reg [% 3d:0] correct_data_out;" % (self.compute_maxk(self.m)-1)
		print >>f, "output reg         error;"
		print >>f, ""

		# generate internal signal declarations
		print >>f, "reg        [% 3d:0] syndrome;" % (self.m-2)
		print >>f, "reg                parity;"
		print >>f, ""

		# generate syndrome logic
		p = [[] for n in xrange(self.m)]
		vals = [n for n in xrange(self.compute_maxk(self.m))]
		next_p = 0
		for n in xrange(1,len(self.val)+1):
			if 2**next_p == n: next_p+=1; continue

			pos = vals.pop()
			for m in xrange(self.m-1):
				if (2**m)&n and (2**m != n):
					p[m].append(pos)

		print >>f, "always @(*) begin"

		for m in xrange(self.m-1):
			print >>f, "    syndrome[%d] =" % m,
			cnt = 0
			for v in p[m]:
				cnt += 1
				if cnt > 1: print >>f, "^",
				print >>f, "data_out[%d]" % v,
			print >>f, "^ checker_out[%d]" % m,
			print >>f, ";"
		print >>f, ""

		# generate parity logic
		print >>f, "    parity =",
		cnt = 0
		for n in xrange(self.compute_maxk(self.m)):
			cnt += 1
			if cnt > 1: print >>f, "^",
			print >>f, "data_out[%d]" % n,
		for m in xrange(self.m-1):
			print >>f, "^ checker_out[%d]" % m,
		print >>f, "^ parity_out",
		print >>f, ";"
		print >>f, ""

		# generate error output
		print >>f, "    error = 0;"
		print >>f, "    if (syndrome == 0 && parity == 0) begin"
		print >>f, "        error = 0;"
		print >>f, "    end"
		print >>f, "    if (syndrome != 0 && parity == 0) begin"
		print >>f, "        error = 1;"
		print >>f, "    end"
		print >>f, "    if (syndrome == 0 && parity != 0) begin"
		print >>f, "        error = 0;"
		print >>f, "    end"
		print >>f, "    if (syndrome != 0 && parity != 0) begin"
		print >>f, "        error = 0;"
		print >>f, "    end"
		print >>f, ""

		# generate correct data output
		print >>f, "    correct_data_out = data_out;"
		print >>f, "    if (error == 0 && syndrome != 0) begin"
		print >>f, "        case(syndrome)"
		vals = [n for n in xrange(self.compute_maxk(self.m))]
		next_p = 0
		for n in xrange(1,len(self.val)+1):
			if n == 2**next_p: next_p += 1; continue
			pos = vals.pop()
			print >>f, "            %d: correct_data_out[%d] = ~data_out[%d];" % (n, pos, pos)
		print >>f, "        endcase"
		print >>f, "    end"

		print >>f, "end // always(*)"

		# close the module
		print >>f, "endmodule"
		print >>f, ""
		print >>f, ""

	def gen_top(self, f):
		# declare module
		print >>f, "module mem_with_ecc_%d(clock, reset, we, data_in, correct_data_out, error);" % self.m
		print >>f, ""

		# declare port
		print >>f, "input              clock;"
		print >>f, "input              reset;"
		print >>f, "input              we;"
		print >>f, "input      [% 3d:0] data_in;" % (self.compute_maxk(self.m)-1)
		print >>f, "output     [% 3d:0] correct_data_out;" % (self.compute_maxk(self.m)-1)
		print >>f, "output             error;"
		print >>f, ""

		# declare internal signals
		print >>f, "wire       [% 3d:0] data_out;" % (self.compute_maxk(self.m)-1)
		print >>f, "wire       [% 3d:0] checker_in;" % (self.m-2)
		print >>f, "wire       [% 3d:0] checker_out;" % (self.m-2)
		print >>f, "wire               parity_in;"
		print >>f, "wire               parity_out;"
		print >>f, ""

		# instantiate submodules
		print >>f, "ecc_checker_%d u_checker(data_in, checker_in, parity_in);" % self.m
		print >>f, "data_mem_%d u_datamem(clock, reset, we, data_in, data_out);" % self.m
		print >>f, "ecc_mem_%d u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);" % self.m
		print >>f, "ecc_corrector_%d u_corrector(data_out, checker_out, parity_out, correct_data_out, error);" % self.m
		print >>f, ""

		# close the module
		print >>f, "endmodule"
		print >>f, ""
		print >>f, ""

	def gen(self, fname):
		f = open(fname, "w")
		self.gen_checker(f)
		self.gen_mem(f)
		self.gen_corrector(f)
		self.gen_top(f)
		f.close()

def gen_test(sizes, module_ids, iteration, fname):
	max_size = max(sizes)

	f = open(fname, "w")

	print >>f, "// Uncomment the following to dump vcd file"
	print >>f, "`define VCD"
	print >>f, ""
	print >>f, "module ecc_test();"
	print >>f, ""
	print >>f, "reg          clock, reset;"
	print >>f, "reg  [%d:0] data_in;" % (max_size-1)
	print >>f, "reg          we;"

	for n in xrange(len(module_ids)):
		module_id = module_ids[n]
		size = sizes[n]
		print >>f, "wire [%d:0] data_out_%d;" % (size-1, module_id)
		print >>f, "wire         error_%d;" % module_id

	print >>f, ""
	for n in xrange(len(module_ids)):
		module_id = module_ids[n]
		size = sizes[n]
		print >>f, "mem_with_ecc_%d ecc_%d(clock, reset, we, data_in[%d:0], data_out_%d, error_%d);" % \
			(module_id, module_id, size-1, module_id, module_id)
	print >>f, ""
	print >>f, "always begin"
	print >>f, "    #5 clock = ~clock;"
	print >>f, "end"
	print >>f, ""
	print >>f, "`define ITERATION %d" % iteration
	print >>f, "integer i;"
	print >>f, "integer active_cnt;"
	print >>f, ""
	print >>f, "initial begin"
	print >>f, "    clock = 1;"
	print >>f, "    reset = 1;"
	print >>f, "    active_cnt = 0;"
	print >>f, "    data_in = 0;"
	print >>f, "    we = 0;"
	print >>f, ""
	print >>f, "    #25"
	print >>f, "    reset = 0;"
	print >>f, ""
	print >>f, "    for (i=0; i<`ITERATION; i=i+1) begin"
	print >>f, "        we = $random;"
	print >>f, "        if (we) begin"
	print >>f, "            active_cnt = active_cnt+1;"
	print >>f, "        #10"
	offset = 0
	while offset < max_size:
		next_offset = offset+32
		if next_offset > max_size:
			next_offset = max_size
		print >>f, "            data_in[%d:%d] = $random;" % (next_offset-1,offset)
		offset = next_offset
	print >>f, "        end"
	print >>f, "    end"
	print >>f, "    #100"
	print >>f, ""
	print >>f, "    $display(\"total_iteration = \%d\", i);"
	print >>f, "    $display(\"active_cnt = \%d\", active_cnt);"
	print >>f, "    $display(\"sim_time = %t\", $time);"
	print >>f, ""
	print >>f, "    $finish;"
	print >>f, ""
	print >>f, "end"
	print >>f, ""
	print >>f, "// Use `define VCD to allow dumping all value changes to waves.vcd"
	print >>f, "`ifdef VCD"
	print >>f, "initial begin"
	print >>f, "  $dumpfile(\"waves.vcd\");"
	print >>f, "  $dumpvars;"
	print >>f, "end"
	print >>f, "`endif"
	print >>f, ""
	print >>f, ""
	print >>f, "endmodule"

	f.close()

################################################################################

sizes = [1, 4, 11, 26, 57, 120, 247, 502]
module_ids = [3, 4, 5, 6, 7, 8, 9, 10]
iteration = 10000
ecc_test_file = "sim/ecc_test.v"

for n in xrange(len(sizes)):
	size = sizes[n]
	module_id = module_ids[n]
	c = Ecc(size)
	ecc_file = "src/rtl/mem_with_ecc_%d.sv" % module_id
	c.gen(ecc_file)

gen_test(sizes, module_ids, iteration, ecc_test_file)

# end of file
