M, N = map(int, input().split())
for i in range(M):
    f = open('inputs/in%d.txt' % i, 'w')
    for j in range(N):
        f.write('%d\n' % (j + 1))
    f.close()
    f = open('outputs/out%d.txt' % i, 'w')
    f.write('%d' % N)
    f.close()
