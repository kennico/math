def intpairs(filename, stop=10000, times=16, per=1):
    from random import randrange as rand
    
    fmt = " ".join(["%d"] * per) + "\n"    
    r = [rand(stop) for i in range(per*times)]
    
    with open(filename, mode="w") as f:
        for i in range(0, len(r), per):
            f.write(fmt % tuple(r[i:i+per]))


def floatnums(filename, number=32, rng=(0.0, 1.0), per=1, precision=4):
    from random import uniform
    
    r = sorted([uniform(*rng) for i in range(per*number)])
    fmt = " ".join(["%." + str(precision) + "f"] * per) + "\n"
    with open(filename, mode="w") as f:
        for i in range(0, len(r), per):
            f.write(fmt % tuple(r[i:i+per]))
