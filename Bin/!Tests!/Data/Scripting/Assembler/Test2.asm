

#A simple preliminary assembly language test 

SetACC  0
SetData 3

[CycleStart]

	SetCNT 10

	IntAdd

	JmpNZCNT CycleStart

PushACC
