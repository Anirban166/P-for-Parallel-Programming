import sys

# Get all lines from stdin:
lines = sys.stdin.readlines()

total_correct=0
numerrors=0
num_tid0=0
num_tid1=0
num_tid2=0
num_tid3=0
num_tid4=0
num_tid5=0
num_tid6=0
num_tid7=0
num_tid8=0
num_tid9=0

listValues=[]

for line in lines:
	if "my num: 0" in line:
		num_tid0+=1
	if "my num: 1" in line:
		num_tid1+=1
	if "my num: 2" in line:
		num_tid2+=1
	if "my num: 3" in line:
		num_tid3+=1
	if "my num: 4" in line:
		num_tid4+=1
	if "my num: 5" in line:
		num_tid5+=1
	if "my num: 6" in line:
		num_tid6+=1
	if "my num: 7" in line:
		num_tid7+=1
	if "my num: 8" in line:
		num_tid8+=1								
	if "my num: 9" in line:
		num_tid9+=1
	if "Total: 990" in line:
		total_correct+=1
	#add the values:
	if "my num" in line:
		mylist=line.split(" ")
		listValues.append(mylist[4])		

listValues = [word.strip() for word in listValues]
value=0

print("Testing output...")
print("Num thread 0: "+str(num_tid0)) 
print("Num thread 1: "+str(num_tid1)) 	
print("Num thread 2: "+str(num_tid2))	
print("Num thread 3: "+str(num_tid3))	
print("Num thread 4: "+str(num_tid4))	
print("Num thread 5: "+str(num_tid5))	
print("Num thread 6: "+str(num_tid6))	
print("Num thread 7: "+str(num_tid7))	
print("Num thread 8: "+str(num_tid8))	
print("Num thread 9: "+str(num_tid9))	
print("The total value of 990 was output: "+str(total_correct))

# Possible errors:
if (num_tid0 !=22 or num_tid1 !=22 or num_tid2 !=22 or num_tid3 !=22 or num_tid4 !=22 or num_tid5 !=22 or num_tid6 !=22or num_tid7 !=22 or num_tid8 !=22 or num_tid9 !=22):
	print("Error: At least one of the threads 0-9 does not output exactly 22 times")
	numerrors+=1
if (total_correct!=1):
	print("Error: The correct number (990) is not output")
	numerrors+=1

numerrorsorder=0
# Adding in correct order:
for x in range (0,(22*10)):
	value+=(x%10)
	if (str(value)!=listValues[x]):
		numerrorsorder+=1
if (numerrorsorder>1):
	print("Values were not added in the correct order!")
	numerrors+=1
else: 	
	print("Values were added in the correct order :)")

print("Total errors detected: "+ str(numerrors))



