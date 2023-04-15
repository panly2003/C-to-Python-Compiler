a = [None] * 100
def main():
	global a
	num = None
	print("Input the array length: ")
	num = eval(input())
	print("Input integers: ")
	i = 0
	while (i < num):
		a[i] = eval(input())
		i = (i + 1)
	i = (num - 1)
	while (i > 0):
		j = 0
		while (j < i):
			if (a[j] > a[(j + 1)]):
				tmp = a[j]
				a[j] = a[(j + 1)]
				a[(j + 1)] = tmp
			j = (j + 1)
		i = (i - 1)
	print("Result: ")
	i = 0
	while (i < num):
		print("%d " % (a[i], ))
		i = (i + 1)
	return 0

if __name__ == '__main__':
	main()

