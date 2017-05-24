import sys

argc		= len(sys.argv)
if argc == 1:
	filename	= "nQueens.txt"
else:
	filename 	= sys.argv[1]

file 		= open(filename, "r")
n 			= file.readline()
queens 		= file.	readline().split()
conflict 	= []

for i in queens:
	for j in queens[queens.index(i)+1::]:
		if i == j or abs(int(i)-int(j)) == abs(queens.index(i)-queens.index(j)):
			# if queens.index(i)+1 not in conflict:
			# 	conflict.append(queens.index(i)+1);
			# if queens.index(j)+1 not in conflict:
			# 	conflict.append(queens.index(j)+1);
			if i not in conflict:
				conflict.append(i);
			if j not in conflict:
				conflict.append(j);
			
if len(conflict) > 0:
	print conflict
else:
	print "Sucesso"
