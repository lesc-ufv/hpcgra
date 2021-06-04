N = int(input())
for i in range(2):
    f = open('in%d.txt' % i, 'w')
    for j in range(N):
        f.write('%d\n' % (j + 1))
    f.close()
    f = open('out%d.txt' % i, 'w')
    f.write('%d' % N)
    f.close()
