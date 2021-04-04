#include <host.h>


int main(int argc, char *argv[]){

    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File> <Kernel name> <CGRA bitstream file>" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string binaryFile = argv[1];
    std::string kernel_name = argv[2];
    std::string bitstreamFile = argv[3];
    vector_u16 inputs[1];
    vector_u16 outputs[1];
    
    int size;
    int out_id = (NUM_CHANNELS % 2) == 0 ? 0 : NUM_CHANNELS-1;
       
    auto cgra_acc = CgraFpga(NUM_CHANNELS,NUM_CHANNELS);
    cgra_acc.cgra_fpga_init(binaryFile, kernel_name, bitstreamFile);
    
    read_file("in0.txt",inputs[0]);
    cgra_acc.createInputQueue(0,inputs[0].size()*2);
    auto ptr_in = cgra_acc.getInputQueue(0);
    memcpy(ptr_in,inputs[0].data(),inputs[0].size()*2); 
    
    read_file("out0.txt",outputs[0]);
    cgra_acc.createOutputQueue(out_id,outputs[0].size()*2);
    cgra_acc.cgra_execute();
    
    std::cout << std::endl << "IN0: ";
    size = inputs[0].size();
    for (int i = 0; i < size; i++) {
        std::cout << inputs[0][i] << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl << "OUT0: ";
    size = outputs[0].size();
    auto ptr_out = (short *)cgra_acc.getOutputQueue(out_id);
    for (int i = 0; i < size; i++) {
        std::cout << ptr_out[i] << " ";
    }
    std::cout << std::endl;
    
    cgra_acc.print_report();
    cgra_acc.cleanup();
    
    return 0;
}

bool read_file(std::string file, vector_u16 &data){
    std::string line;   
    if(file.substr(0,2) == "in"){
        std::ifstream MyReadFile(file);
        while(getline(MyReadFile,line)) {
            unsigned short x = std::stoul(line, nullptr, 10);
            data.push_back(x);
        }
        MyReadFile.close();
    }else if(file.substr(0,3) == "out"){
        std::ifstream MyReadFile(file);
        getline(MyReadFile,line);
        int x = std::stoul(line, nullptr, 10);
        for(int i = 0;i < x;i++){
            data.push_back(0);
        }
        MyReadFile.close();
    }else {
        std::cout << "[Error] Input file not found: " << file << std::endl;       
        return false;
    }

    return true;    
}

