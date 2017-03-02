def geomean(nums): # return geometric mean of values
	return (reduce(lambda x, y: x*y, nums))**(1.0/len(nums))

def mean(nums): # return average of values
	return sum(nums)/float(len(nums))

def absmean(nums): # return average of absolute values
	return sum([v if v > 0 else -v for v in nums])/float(len(nums))