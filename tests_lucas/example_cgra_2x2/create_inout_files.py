M, N, MAX = map(int, input().split())
for i in range(M):
    f = open('in%d.txt' % i, 'w')
    for j in range(N):
        f.write('%d\n' % (j%MAX + 1))
    f.close()
    f = open('out%d.txt' % i, 'w')
    f.write('%d' % N)
    f.close()
