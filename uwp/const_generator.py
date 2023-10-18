import math

kernels = []

for i in range(1, 16*9 + 1):
	kernels.append(int(abs(float(256) * math.sin(i))))

print(kernels)

golden_ratio = int(0.5 *(1 + math.sqrt(5)) * 1e15)
consts = []
for i in range(6):
	consts.append(golden_ratio & 0xff)
	golden_ratio >>= 8

pi = int(math.pi * 1e15)
for i in range(6):
	consts.append(pi & 0xff)
	pi >>= 8

e = int(math.exp(1) * 1e15)
for i in range(4):
	consts.append(e & 0xff)
	e >>= 8
print(consts)