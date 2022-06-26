#!/usr/bin/python

list_of_files = ["src/nbody0.s",
                 "src/nbody1.s",
                 "src/nbody2.s",
                 "src/nbody3.s",
                 "src/nbody4.s",
                 "src/nbody5.s",
                 "src/nbody6.s",
                 "src/nbody7.s",
                 "src/nbody8.s"]

for filename in list_of_files:
    with open(filename, 'r') as f:
        contents = f.read().splitlines()

    start = contents.index('# -- Begin  move_particles')
    end = contents.index('# -- End  move_particles')
    func = contents[start : end + 1]

    f2 = open(filename,'w')
    for element in func:
         f2.write(str(element))
         f2.write('\n')
    f2.close()
