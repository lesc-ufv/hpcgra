# HPCGRA - An Orthogonal Design CGRA Generator for High Performance Spatial Accelerators

## **Tools**
Here is a list of tools needed for proper synthesis and simulation:

1. Xilinx Vitis 2020.1

## **Project Dependencies**
Programs and libraries required for installation:

1. Veriloggen (To install see on https://github.com/PyHDI/veriloggen)

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
