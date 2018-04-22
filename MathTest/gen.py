def intpairs(filename, stop=10000, times=16, per=2):
    from random import randrange as rand
    
    fmt = " ".join(["%d"] * per) + "\n"    
    r = [rand(stop) for i in range(per*times)]
    
    with open(filename, mode="w") as f:
        for i in range(0, len(r), per):
            f.write(fmt % tuple(r[i:i+per]))
