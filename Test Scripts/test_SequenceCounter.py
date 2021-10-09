import sys

# Get all lines from stdin:
lines = sys.stdin.readlines()

num_tid_is_zero=0
num_tid1= 0
num_tid2= 0
num_tid3= 0
num_totalseqgen= 0
num_numcorrectsequences= 0
numerrors=0;

for line in lines:
	if "My id: 1" in line:
		num_tid1+=1
	elif "My id: 2" in line:
		num_tid2+=1
	elif "My id: 3" in line:	
		num_tid3+=1
	elif "My id: 0" in line:	
		num_tid_is_zero+=1
	if "Total sequences generated" in line:
		num_totalseqgen+=1
	if "Number of correct sequences: 10" in line:
		num_numcorrectsequences+=1

print("Testing output...")
print("Num thread 1: "+str(num_tid1)) 	
print("Num thread 2: "+str(num_tid2))	
print("Num thread 3: "+str(num_tid3))	
print("Number of times total sequences generated output: "+str(num_totalseqgen))
print("Number of times the number of correct sequences is output: "+str(num_numcorrectsequences))

# Possible errors:
if (num_tid_is_zero>0):
	print("Error: Threads are supposed to be assigned ids such that none are 0")
	numerrors+=1
if (num_tid1<10 or num_tid2<10 or num_tid3<10):
	print("Error: you have less than 10 of outputs to the screen for one (or more) threads")
	numerrors+=1
if (num_totalseqgen != 1):
	print("Error: the total number of sequences generated is not output")
	numerrors+=1
if (num_numcorrectsequences != 1):
	print("Error: the number of correct sequences generated is not output (or the number is not 10)")
	numerrors+=1

# If they didnt use sleep, then the number of times a thread executes isn't as well constrained:
minval=min(num_tid1,num_tid2,num_tid3)
maxval=max(num_tid1,num_tid2,num_tid3)
if ((maxval-minval)>1):
	print("Error: The difference between the minimum number of threads outputting their ids should be 1.")
	print("Perhaps you forgot the sleep?")
	print("Max of the tids printed: "+str(maxval))
	print("Min of the tids printed: "+str(minval))
	numerrors+=1
print("Total errors detected: "+ str(numerrors))