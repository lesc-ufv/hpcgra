
ARCH=$1
ARCH_SPLIT=($(echo $ARCH | tr "_" "\n"))
ARCH_NAME=$(echo ${ARCH_SPLIT[2]} | tr "-" "_")

#cleanup
rm -rf assembly/$ARCH_NAME grid/$ARCH_NAME bitstream/$ARCH_NAME

BENCH=(
    dataflows/chebyshev_8_copy.json #
    dataflows/chebyshev.json        #
    dataflows/fir16.json            #
    dataflows/fir32.json            #
    dataflows/fir64.json            #Buffer invalid for all trying
    dataflows/kmeans_4_4.json       #
    dataflows/kmeans_8_16.json      #Routing invalid for all trying 
    dataflows/kmeans_8_8.json       #Buffer invalid for all trying
    dataflows/loopback_8.json       #
    dataflows/mibench.json          #
    dataflows/paeth.json            #
    dataflows/poly5.json            #
    dataflows/poly6.json            #
    dataflows/poly8.json            #
    dataflows/qspline.json          #
    dataflows/sgfilter.json         #
    dataflows/sobel_filter.json     #
)

for ((i = 0; i < ${#BENCH[@]}; i++)); do
     asm="assembly/$ARCH_NAME/$(basename -s .json ${BENCH[i]}).asm"
    ./create_assembly.sh $ARCH ${BENCH[i]} $ARCH_NAME
    ./create_bitstream.sh $ARCH $asm $ARCH_NAME
done


