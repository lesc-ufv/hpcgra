# HPCGRA - An Orthogonal Design CGRA Generator for High Performance Spatial Accelerators

## **Tools**
Here is a list of tools needed for proper synthesis and simulation:

1. Xilinx Vitis 2020.1

## **Project Dependencies**
Programs and libraries required for installation:

1. Veriloggen (To install see on https://github.com/PyHDI/veriloggen)
2. AWS FPGA Repository(https://github.com/aws/aws-fpga.git)

### Repository

Clone the HPCGRA repository

```
mkdir $HOME/workspace
cd $HOME/workspace
git clone https://github.com/lesc-ufv/hpcgra.git
```
## **Generating a homogeneous architecture JSON description**
To create homogeneous CGRAs JSON description use the hpcgra tool located in the bin folder. 
Run the command below to see the required parameters.
```
$HOME/workspace/hpcgra/bin/hpcgra -h
```
### **Usage:**
Example command to create a homogeneous CGRA 4x4 mesh JSON architecture file:
```
$HOME/workspace/hpcgra/bin/hpcgra --arch mesh --shape 4x4 --isa add sub mul or and --fifos 2 2 --inputs 0 4 8 12 --outputs 3 9 11 15 --data_width 16 --emit cgra_4x4.json
```
## Creating custom architecture description
To create a custom architecture, you must manually create the architecture description file in JSON format, following the specifications below:

1. Because the file must have a valid JSON format, it must start with "{" and end with "}". Among the keys are specified the CGRA attributes, which are just four main attributes: "data_width", "conf_bus_width", "axi_bus_data_width" and "pe". The attribute "data_width" specifies the length of the CGRA processing word, "conf_bus_width" defines the size of the configuration bus and "axi_bus_data_width" defines the width of data read and written with external memory via the AXI protocol.
2. The "pe" attribute is an array of objects that define a processing element of the architecture. Each item in the PE array has the following attributes: "id", "type", "neighbors", "routes", "elastic_queue" and "isa".
3. The "id" attribute is a unique integer for each PE and must start from 0 to N-1, where N is the total number of PEs in the architecture.
4. The "type" attribute is a string that determines whether the PE has external output/input, the possible values are: ''input" for PE that has external input, "output" for PE that has external output and "inout" for PE which has an external input and output.
5. The "neighbors" attribute is an array of integers, where each element of the array is an "id" of a neighboring PE.
6. The "routes" attribute is an integer between 0 and the number of neighbors that determines how many neighbors the PE is able to route to other neighbors.
7. The "elastic_queue" attribute is an integer array, where the number of elements in this array is given by the number of ALU entries, and the value of each element is the size of the elastic queues present in the ALU entries to perform data balancing.
8. The "isa" attribute is a string array, where each element is a logical/arithmetic instruction that the PE is able to perform. Possible operations are divided into unary, binary and ternary and are listed below:
9. Unary: "not", "abs", "pass".
10. Binary: "add", "sub", "mul", "or", "xor", "and", "slt", "sgt", "seq", "sne", "shl", "shr", "max", "min".
11. Ternary: "muladd", "mulsub", "addadd", "subsub", "addsub", "mux".
