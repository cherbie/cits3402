import sys
from random import randrange, random

class MatrixCreatorInt:
	def __init__(self, filename, size, seed):
		self.rows = size
		self.cols = size
		self.file = open(filename, "w")

	def populate(self, size):
		self.file.write("int\n")
		self.file.write(f"{self.rows}\n{self.cols}\n")
		for i in range(0,size):
			val = randrange(2056)
			r = randrange(2056)
			if (r % 7) == 0:
				val = 0
			self.file.write(f"{val}\x20")
		self.file.write("\n")

class MatrixCreatorFloat:
	def __init__(self, filename, size, seed):
		self.rows = size
		self.cols = size
		self.file = open(filename, "w")

	def populate(self, size):
		self.file.write("float\n")
		self.file.write(f"{self.rows}\n{self.cols}\n")
		for i in range(0,size):
			val = random() * 1000
			r = random() * 100
			if (r // 4) % 4 == 0:
				val = 0.0
			self.file.write(f"{val}\x20")
		self.file.write("\n")

def main():
	if len(sys.argv) != 4:
		print("Usage: python3 create-mtx.py filename rows cols size")
	else:
		if sys.argv[2] == "int":
			mtx = MatrixCreatorInt(sys.argv[1], sys.argv[3], 19)
			mtx.populate(int(sys.argv[3]))
		else:
			mtx = MatrixCreatorFloat(sys.argv[1], sys.argv[3], 19)
			mtx.populate(int(sys.argv[3]))
		print("... mtx file created")

main()
