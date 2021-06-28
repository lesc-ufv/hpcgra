ARCH="cgra_archs/cgra_one_hop_16x16_8_4.json"

#cleanup
rm -rf assembly grid bitstream

BENCH=(
    dataflows/chebyshev_8_copy.json
    dataflows/chebyshev.json
    dataflows/fir16.json
    dataflows/fir32.json
    dataflows/fir64.json
    dataflows/kmeans_4_4.json
    dataflows/kmeans_8_16.json # limitation of arch
    dataflows/kmeans_8_8.json
    dataflows/loopback_8.json
    dataflows/mibench.json
    dataflows/paeth.json
    dataflows/poly5.json
    dataflows/poly6.json
    dataflows/poly8.json
    dataflows/qspline.json
    dataflows/sgfilter.json
    dataflows/sobel_filter.json # segmentation fault
)

for ((i = 0; i < ${#BENCH[@]}; i++)); do
     asm="assembly/$(basename -s .json ${BENCH[i]}).asm"
    ./create_assembly.sh $ARCH ${BENCH[i]}
    ./create_bitstream.sh $ARCH $asm
done


