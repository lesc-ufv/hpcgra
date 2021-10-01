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

1. Because the file must have a valid JSON format, it must start with "{" and end with "}"
2. Among the keys are specified the CGRA attributes, which are just four main attributes: "data_width", "conf_bus_width", "axi_bus_data_width" and "pe". The attribute "data_width" specifies the length of the CGRA processing word, "conf_bus_width" defines the size of the configuration bus and "axi_bus_data_width" defines the width of data read and written with external memory via the AXI protocol.
3. The "pe" attribute is an array of objects that define a processing element of the architecture. Each item in the PE array has the following attributes: "id", "type", "neighbors", "routes", "isa" and "elastic_queue" 
