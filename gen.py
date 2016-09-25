import random

lines = []
for i in range(1, 10):
	line = [];
	for c in range(1, 11):
		line.append(str(random.randint(1, 100000)));

	lines.append(" ".join(line))


print "\n".join(lines)
