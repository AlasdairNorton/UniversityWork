#name1 = input("File 1:")
#name2 = input("File 2:")
name1 = "test2_goodout.txt"
name2 = "test2codeout.txt"
file1 = open(name1, 'r')
file2 = open(name2, 'r')

lines1 = []
lines2 = []

for i in file1:
    lines1.append(i)

for i in file2:
    lines2.append(i)

for i in range(len(lines1)):
    if lines1[i] != lines2[i]:
        print("error at line {}".format(i))
        print("File 1:" + lines1[i])
        print("File 2:" + lines2[i])

file1.close()
file2.close()
