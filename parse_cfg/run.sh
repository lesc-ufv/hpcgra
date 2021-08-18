set -e

# Benchmarks
BENCH=(
	sum_simple
)

LLVM_INSTALL_DIR="" # </path/to/llvm/>, if you use binary installation, you can leave empty this string 
LLVM_OPT="opt" # </path/to/opt>
CLANG="clang++" # </path/to/clang>

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

    echo "Executing the pass for bench: ${BENCH[i]}"

    EXAMPLE=bench/${BENCH[i]}
    $CLANG -Wno-everything -fno-discard-value-names -Xclang -disable-O0-optnone -S -emit-llvm $EXAMPLE".cpp" -o $EXAMPLE".ll"
    $LLVM_OPT -S -instnamer -mem2reg $EXAMPLE".ll" -o $EXAMPLE"_opt.ll"
    
    $LLVM_OPT -load-pass-plugin $PATH_LIB -passes="cfgPrinter" $EXAMPLE"_opt.ll" -disable-output
    
    mv *.dot "results/${BENCH[i]}"
done

rm bench/*.ll
