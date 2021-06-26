ARCH="cgra_archs/cgra_one_hop_16x16_8_4.json"

BENCH=(
    chebyshev_8_copy
    chebyshev
    fir16
    fir32
    fir64
    kmeans_4_4
    #kmeans_8_16 # limitation of arch
    kmeans_8_8
    loopback_8
    mibench
    paeth
    poly5
    poly6
    poly8
    qspline
    sgfilter
    #sobel_filter # segmentation fault
)

for ((i = 0; i < ${#BENCH[@]}; i++)); do
    ./create_assembly.sh ${BENCH[i]} $ARCH
    ./create_bitstream.sh $ARCH
done


