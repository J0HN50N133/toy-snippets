def isPrime(i):
    for j in range(2, i):
        if i % j == 0:
            return False
    return True


def nextPrime(i):
    while True:
        i += 1
        if i % 2 == 0:
            continue  # continuation 是自身这个循环

        if isPrime(i):
            break  # continuation 是print(i);return i
    print(i)
    return i


def nextPrimeCPS(i):
    def cps(kont):
        nonlocal i
        i += 1
        if i % 2 == 0:
            cps(kont)  # continue
        elif isPrime(i):
            kont(i)  # break
        else:
            cps(kont)

    def kont(i):
        print(i)

    cps(kont)

    return i


nextPrime(114514)
nextPrimeCPS(114514)
