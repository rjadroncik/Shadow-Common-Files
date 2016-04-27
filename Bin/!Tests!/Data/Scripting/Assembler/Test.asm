#A simple preliminary assembly language test 

SetACC  0
SetDATA 3
SetCNT 10

#This simply tests whether th use of labels prior to their declaration works..
	JmpNZCNT CycleStart

	[CycleStart]
	
	IntAdd
	
	DecCNT
	JmpNZCNT CycleStart

PushACC
