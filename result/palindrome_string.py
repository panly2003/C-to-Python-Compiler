a = [None] * 100
def main():
	global a
	num = None
	print("Input the string length: ")
	num = eval(input())
	print("Input string: ")
	a = input()
	half = (num / 2)
	i = 0
	while (i < half):
		x = (num - 1)
		a1 = a[i]
		a2 = a[(x - i)]
		if (a1 != a2):
			print("FALSE!")
			return 0
		i = (i + 1)
	print("TRUE!")
	return 0

if __name__ == '__main__':
	main()

