set -e

# Benchmarks
BENCH=(
	#sum_simple
    sum_loop
)

LLVM_INSTALL_DIR="/home/canesche/git/llvm-10/build" # </path/to/llvm/>, if you use binary installation, you can leave empty this string 
LLVM_OPT=$LLVM_INSTALL_DIR/bin/opt # </path/to/opt>
CLANG=$LLVM_INSTALL_DIR/bin/clang++ # </path/to/clang>

PATH_LIB="build/lib/libCfgPrinter.so" 

# Create the build
mkdir -p build

# Create the results
mkdir -p results

echo "Building the cfgPrinter pass"
cmake -DLLVM_INSTALL_DIR=$LLVM_INSTALL_DIR -G "Unix Makefiles" -B build/ .
cd build
cmake --build .
cd ..

for ((i = 0; i < ${#BENCH[@]}; i++)); do

    EXAMPLE=bench/${BENCH[i]}

    ../bin/generate_dataflow $EXAMPLE.cpp  

    echo "Executing the pass for bench: ${BENCH[i]}"

    $CLANG -Wno-everything -fno-discard-value-names -Xclang -disable-O0-optnone -S -emit-llvm $EXAMPLE"_to_cgra.cpp" -o $EXAMPLE".ll"
    $LLVM_OPT -S -instnamer -mem2reg $EXAMPLE".ll" -o $EXAMPLE"_opt.ll"
    
    $LLVM_OPT -load-pass-plugin $PATH_LIB -passes="cfgPrinter" $EXAMPLE"_opt.ll" -disable-output
    
    mv *.dot "results/${BENCH[i]}"
done

rm bench/*.ll
