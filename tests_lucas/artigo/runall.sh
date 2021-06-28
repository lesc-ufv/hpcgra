
ARCH=$1
#ARCH="cgra_archs/cgra_one_hop_16x16_8_4.json"

#cleanup
rm -rf assembly grid bitstream

BENCH=(
    #dataflows/chebyshev_8_copy.json #deubom
    #dataflows/chebyshev.json        #deubom
    #dataflows/fir16.json            # erro na geração do bitstream
    #dataflows/fir32.json            # erro na geração do bitstream
    #dataflows/fir64.json            # erro na geração do bitstream
    #dataflows/kmeans_4_4.json       # erro na geração do bitstream
    #dataflows/kmeans_8_16.json      # limitation of arch
    #dataflows/kmeans_8_8.json       # erro na geração do bitstream
    #dataflows/loopback_8.json       #deubom
    #dataflows/mibench.json          #deubom
    #dataflows/paeth.json             # erro na geração do bitstream
    #dataflows/poly5.json            #deuruim, dá pra olhar pelo grid!
    #dataflows/poly6.json            #deubom
    #dataflows/poly8.json            #deubom
    #dataflows/qspline.json          #deu ruim,desbalanceado!
    #dataflows/sgfilter.json          #deubom
    #dataflows/sobel_filter.json     #place segmentation fault
)

for ((i = 0; i < ${#BENCH[@]}; i++)); do
     asm="assembly/$(basename -s .json ${BENCH[i]}).asm"
    ./create_assembly.sh $ARCH ${BENCH[i]}
    ./create_bitstream.sh $ARCH $asm
done


