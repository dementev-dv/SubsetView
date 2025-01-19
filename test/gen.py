def generate_subsets(n, path):
  with open(path, "w") as f:
    for mask in range(1, 1 << n):
      subset = []
      for i in range(n):
        if mask & (1 << i):
          subset.append(i + 1)
      f.write(" ".join(map(str, subset)) + "\n")

def generate_set(n, path):
  with open(path, "w") as f:
    for i in range(1, n + 1):
      f.write(f'{i} ')

n = int(input())
ntest = int(input())

generate_set(n, f'input{ntest}.txt')
generate_subsets(n, f'answer{ntest}.txt')
