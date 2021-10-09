import sys

# Get all lines from stdin:
lines = sys.stdin.readlines()

num_tid_is_zero=0
num_tid1= 0
num_tid2= 0
num_tid3= 0
num_tid4= 0
num_tid5= 0
num_tid6= 0
num_totalseqgen_t1= 0
num_totalseqgen_t2= 0
num_numcorrectsequences_t1= 0
num_numcorrectsequences_t2= 0
numerrors=0;
one_team_correct=0
one_team_won_output=0
num_seq_t1=0
num_seq_t2=0

for line in lines:
	if "My id: 1" in line:
		num_tid1+=1
	elif "My id: 2" in line:
		num_tid2+=1
	elif "My id: 3" in line:	
		num_tid3+=1
	elif "My id: 4" in line:	
		num_tid4+=1
	elif "My id: 5" in line:	
		num_tid5+=1
	elif "My id: 6" in line:	
		num_tid6+=1			
	elif "My id: 0" in line:	
		num_tid_is_zero+=1
	if "123" in line:
		num_seq_t1+=1
	if "456" in line:
		num_seq_t2+=1	
	if "Total sequences generated team1" in line:
		num_totalseqgen_t1+=1
	if "Total sequences generated team2" in line:
		num_totalseqgen_t2+=1
	if "Number of correct sequences team1:" in line:
		num_numcorrectsequences_t1+=1
	if "Number of correct sequences team2:" in line:
		num_numcorrectsequences_t2+=1	
	if "Number of correct sequences team1: 10" in line:
		one_team_correct+=1
	if "Number of correct sequences team2: 10" in line:
		one_team_correct+=1		
	if "Team 1 won!" in line: 
		one_team_won_output+=1
	if "Team 2 won!" in line: 
		one_team_won_output+=1		

print("Testing output...")
print("Num thread 1: "+str(num_tid1)) 	
print("Num thread 2: "+str(num_tid2))	
print("Num thread 3: "+str(num_tid3))	
print("Num thread 4: "+str(num_tid4))	
print("Num thread 5: "+str(num_tid5))	
print("Num thread 6: "+str(num_tid6))	
print("Num times 123 printed: "+str(num_seq_t1))
print("Num times 456 printed: "+str(num_seq_t2))
print("Number of times total sequences generated team1 output: "+str(num_totalseqgen_t1))
print("Number of times total sequences generated team2 output: "+str(num_totalseqgen_t2))
print("Number of times correct sequences generated team1 output: "+str(num_numcorrectsequences_t1))
print("Number of times correct sequences generated team2 output: "+str(num_numcorrectsequences_t2))
print("Number of times it was output that the number of correct sequences was 10: "+str(one_team_correct))
print("Number of times it was output that Team x won: "+str(one_team_won_output))
#print("Number of times the number of correct sequences is output: "+str(num_numcorrectsequences))

# Possible errors:
if (num_tid_is_zero>0):
	print("Error: Threads are supposed to be assigned ids such that none are 0")
	numerrors+=1
if (num_tid1<10 or num_tid2<10 or num_tid3<10):
	print("Possible Error: Team 1: you have less than 10 of outputs to the screen for one (or more) threads")
	print("This can happen if the other team gets 10 correct sequences in a row, which is unlikely");
	numerrors+=1
if (num_tid4<10 or num_tid5<10 or num_tid6<10):
	print("Possible Error: Team 2: you have less than 10 of outputs to the screen for one (or more) threads")
	print("This can happen if the other team gets 10 correct sequences in a row, which is unlikely");
	numerrors+=1	
if (num_totalseqgen_t1!= 1):
	print("Error: the total number of sequences generated is not output for team 1")
	numerrors+=1
if (num_totalseqgen_t2!= 1):
	print("Error: the total number of sequences generated is not output for team 2")
	numerrors+=1	
if (num_numcorrectsequences_t1 != 1):
	print("Error: the number of correct sequences generated is not output for team 1")
	numerrors+=1
if (num_numcorrectsequences_t2 != 1):
	print("Error: the number of correct sequences generated is not output for team 2")
	numerrors+=1	
if (one_team_correct == 2):
	print("Error: both teams can't produce 10 correct sequences")
	numerrors+=1		
elif(one_team_correct < 1):
	print("Error: no team has generated 10 correct sequences")
	numerrors+=1			
if (num_seq_t1>10):
	print("Error: sequence 123 was printed more than 10 times!")
	numerrors+=1			
if (num_seq_t2>10):
	print("Error: sequence 456 was printed more than 10 times!")
	numerrors+=1
if (one_team_won_output>1):
	print("Error: it should only be output that 1 team won")	
	numerrors+=1

# If they didnt use sleep, then the number of times a thread executes isn't constrained: 

# Team 1:
minval=min(num_tid1,num_tid2,num_tid3)
maxval=max(num_tid1,num_tid2,num_tid3)
if ((maxval-minval)>1):
	print("Error: Team 1: The difference between the minimum number of threads outputting their ids should be 1.")
	print("Perhaps you forgot the sleep?")
	print("Max of the tids printed (team 2): "+str(maxval))
	print("Min of the tids printed (team 2): "+str(minval))
	numerrors+=1

# Team 2:
minval=min(num_tid4,num_tid5,num_tid6)
maxval=max(num_tid4,num_tid5,num_tid6)
if ((maxval-minval)>1):
	print("Error: Team 2: The difference between the minimum number of threads outputting their ids should be 1.")
	print("Perhaps you forgot the sleep?")
	print("Max of the tids printed (team 2): "+str(maxval))
	print("Min of the tids printed (team 2): "+str(minval))
	numerrors+=1	
print("Total errors detected: "+ str(numerrors))