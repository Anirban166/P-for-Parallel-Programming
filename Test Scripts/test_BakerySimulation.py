# Script to check the output of the bakery program:
import sys
import re
from operator import eq

# Helper function:
def get_thread_name(line):
	p = re.compile('\[.+\]');
	match = p.match(line)
	if (match == None):
		return None
	name = match.group().replace('[','').replace(']','')
	words = name.split(" ");
	#print "WORDS=",words
	if ((not words[0] == "Cautious") and  (not words[0] == "Left-handed") and (not words[0] == "Right-handed")) or (not words[1] == "baker") or (not words[2].isdigit()):
		return None
	else:
		return name

def get_all_thread_names(lines):
	names = []
	for linenum in xrange(0,len(lines)):
  		name = get_thread_name(lines[linenum])
		if name is None:
			print "Invalid/missing thread name at line ",linenum
			print "\t --> "+lines[linenum]+"\n"
			exit(1)
		else:
			if name not in names:
				names.append(name)
	return names

def get_num_threads(lines):
	names = get_all_thread_names(lines)
	split_names = []
	for name in names:
		split_names.append(name.split(" "))

	left_handers =  sorted([int(z) for (x,y,z) in split_names if x == "Left-handed"])
	right_handers = sorted([int(z) for (x,y,z) in split_names if x == "Right-handed"])
	cautious =      sorted([int(z) for (x,y,z) in split_names if x == "Cautious"])

	if (left_handers != range(0,max(left_handers)+1)) or (right_handers != range(0,max(right_handers)+1)) or (cautious != range(0,max(cautious)+1)): 
		print "Invalid thread ids!!"
		print "\tLeft-handed baker ids: ",left_handers
		print "\tRight-handed baker ids: ",right_handers
		print "\tCautious baker ids: ",cautious
		exit(1)
	return (len(left_handers), len(right_handers), len(cautious))

# Constants:
num_iterations = 10
num_left_mitts = 3
num_right_mitts = 3
	
# Array of thread names:
threadids = []

# Get all lines from stdin:
lines = sys.stdin.readlines()

# Get number of baker threads for each type:
print "Checking that the output is well-formatted..."
(num_left_handed, num_right_handed, num_cautious) = get_num_threads(lines)
print "\tDetected", num_left_handed, "left-handed bakers"
print "\tDetected", num_right_handed, "right-handed bakers"
print "\tDetected", num_cautious, "cautious bakers"
print "Checking that every baker does its required number of operations..."
# Declare dictionaries:
working = {}
waiting = {}
cookies_baked = {}
left_wanting = {}
right_wanting = {}
left_getting = {}
right_getting = {}
left_putting_back = {}
right_putting_back = {}

# Initialize them:
for line in lines:
	name = get_thread_name(line)
	working[name] = 0
	waiting[name] = 0
	cookies_baked[name] = 0
	left_wanting[name] = 0
	right_wanting[name] = 0
	left_getting[name] = 0
	right_getting[name] = 0
	left_putting_back[name] = 0
	right_putting_back[name] = 0

# Populate them:
for line in lines:
	name = get_thread_name(line)
	if "working" in line:
		working[name] += 1
	elif "waiting" in line:
		waiting[name] += 1
	elif "taken cookies out of the oven" in line:
		cookies_baked[name] += 1
	elif "left" in line and "wants" in line:
		left_wanting[name] += 1
	elif "right" in line and "wants" in line:
		right_wanting[name] += 1
	elif "left" in line and "has got" in line:
		left_getting[name] += 1
	elif "right" in line and "has got" in line:
		right_getting[name] += 1
	elif "left" in line and "has put back" in line:
		left_putting_back[name] += 1
	elif "right" in line and "has put back" in line:
		right_putting_back[name] += 1

# Check validity:
for name in get_all_thread_names(lines):
	if working[name] != num_iterations:
		print name+" isn't working "+str(num_iterations)+" times!"
		exit(1)
	if working[name] != num_iterations:
		print name+" isn't waiting "+str(num_iterations)+" times!"
		exit(1)
	if cookies_baked[name] != num_iterations:
		print name+" hasn't taken cookies out of the oven "+str(num_iterations)+" times!"
		exit(1)
	baker_type = name.split(" ")[0]
	if baker_type == "Left-handed" or baker_type == "Cautious":
		if (left_wanting[name] != num_iterations):
			print name+" isn't wanting a left mitt "+str(num_iterations)+" times!"
			exit(1)
		if (left_getting[name] != num_iterations):
			print name+" isn't getting a left mitt "+str(num_iterations)+" times!"
			exit(1)
		if (left_putting_back[name] != num_iterations):
			print name+" isn't putting back a left mitt "+str(num_iterations)+" times!"
			exit(1)
	if baker_type == "Right-handed" or baker_type == "Cautious":
		if (right_wanting[name] != num_iterations):
			print name+" isn't wanting a right mitt "+str(num_iterations)+" times!"
			exit(1)
		if (right_getting[name] != num_iterations):
			print name+" isn't getting a right mitt "+str(num_iterations)+" times!"
			exit(1)
		if (right_putting_back[name] != num_iterations):
			print name+" isn't putting back a right mitt "+str(num_iterations)+" times!"
			exit(1)
print "\tEvery thread does what it needs to do",num_iterations,"times."
print "Checking that no more mitts are taken than there are available..."
num_taken_left_mitts = 0
num_taken_right_mitts = 0
for linenum in xrange(0,len(lines)):
	line = lines[linenum]
	if "has got a left-handed mitt" in line:
		num_taken_left_mitts += 1
	if "has got a right-handed mitt" in line:
		num_taken_right_mitts += 1
	if (num_taken_left_mitts > num_left_mitts):
		print "More than "+str(num_left_mitts)+" left-handed mitts are taken!! (line "+str(linenum)+")"
		exit(1)

	if (num_taken_right_mitts > num_right_mitts):
		print "More than "+str(num_right_mitts)+" right-handed mitts are taken!! (line "+str(linenum)+")"
		exit(1)
	if "has put back a left-handed mitt" in line:
		num_taken_left_mitts -= 1
	if "has put back a right-handed mitt" in line:
		num_taken_right_mitts -= 1
print "\tNo more mitts are used than are available."
print "Checking that bakers are able to bake at the same time..."
num_left_handed_bakers_baking = 0
num_right_handed_bakers_baking = 0
num_cautious_bakers_baking = 0
max_num_left_handed_bakers_baking = 0
max_num_right_handed_bakers_baking = 0
max_num_cautious_bakers_baking = 0

for linenum in xrange(0,len(lines)):
	line = lines[linenum]
	name = get_thread_name(line)

	if "has put cookies in the oven" in line:
		if "Left-handed" in name:
			num_left_handed_bakers_baking += 1
		if "Right-handed" in name:
			num_right_handed_bakers_baking += 1
		if "Cautious" in name:
			num_cautious_bakers_baking += 1

	max_num_left_handed_bakers_baking = max(max_num_left_handed_bakers_baking,num_left_handed_bakers_baking)
	max_num_right_handed_bakers_baking = max(max_num_right_handed_bakers_baking,num_right_handed_bakers_baking)
	max_num_cautious_bakers_baking = max(max_num_cautious_bakers_baking,num_cautious_bakers_baking)

	if "has taken cookies out of the oven" in line:
		if "Left-handed" in name:
			num_left_handed_bakers_baking -= 1
		if "Right-handed" in name:
			num_right_handed_bakers_baking -= 1
		if "Cautious" in name:
			num_cautious_bakers_baking -= 1
		
if max_num_left_handed_bakers_baking == 1 and num_left_handed > 1:
	print "\tError: no more than 1 left-handed baker uses the oven at a time!"
	exit(1)
if max_num_right_handed_bakers_baking == 1 and num_right_handed > 1:
	print "\tError: no more than 1 right-handed baker uses the oven at a time!"
	exit(1)
if max_num_cautious_bakers_baking == 1 and num_cautious > 1:
	print "\tError: no more than 1 cautious baker uses the oven at a time!"
	exit(1)
print "\tBakers can bake concurrently."
print "No errors detected!"