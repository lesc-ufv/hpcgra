from cgra import *

def __build_one_hop_group(cgra, columns, minI, maxI, minJ, maxJ):
    for i in range(minI, maxI+1):
        for j in range(minJ, maxJ+1):
            currId = i * columns + j
            if i > minI:
                find_and_connect(cgra, currId, currId-columns)
            if i > minI+1:
                find_and_connect(cgra, currId, currId-2*columns)
            if i < maxI:
                find_and_connect(cgra, currId, currId+columns)
            if i < maxI-1:
                find_and_connect(cgra, currId, currId+2*columns)
            if j > minJ:
                find_and_connect(cgra, currId, currId-1)
            if j > minJ+1:
                find_and_connect(cgra, currId, currId-2)
            if j < maxJ:
                find_and_connect(cgra, currId, currId+1)
            if j < maxJ-1:
                find_and_connect(cgra, currId, currId+2)
    return

def create_one_hop(clusterLines, clusterColumns, groupLines, groupColums, verticalConn, horizontalConn, inputs, outputs):
    cgra = create_cgra(clusterColumns * clusterLines * groupColums * groupLines)
    columns = clusterColumns * groupColums

    # build groups
    for i in range(clusterLines):
        for j in range(clusterColumns):
            __build_one_hop_group(cgra, columns, i*groupLines, (i+1)*groupLines-1, j*groupColums, (j+1)*groupColums-1)

    # generate connections
    verticalSpace = (groupColums - verticalConn) // 2
    verticalConnections = [x+verticalSpace for x in range(verticalConn)]
    horizontalSpace = (groupLines - horizontalConn) // 2
    horizontalConnections = [x+horizontalSpace for x in range(horizontalConn)]

    # connect groups
    for i in range(clusterLines):
        for j in range(clusterColumns):
            currId = i*groupLines*columns + j*groupColums
            if i > 0:
                for c in verticalConnections:
                    find_and_connect(cgra, currId+c, currId-columns+c)
                    find_and_connect(cgra, currId-columns+c, currId+c)
            if j > 0:
                for c in horizontalConnections:
                    find_and_connect(cgra, currId+c*columns, currId+c*columns-1)
                    find_and_connect(cgra, currId+c*columns-1, currId+c*columns)

    # set inputs
    for x in inputs:
        find_and_set_type_input(cgra, x)
    
    # set outputs
    for x in outputs:
        find_and_set_type_output(cgra, x)

    # return cgra
    return cgra