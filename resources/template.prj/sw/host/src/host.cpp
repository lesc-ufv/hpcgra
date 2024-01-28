#include <host.h>

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File> <Kernel name> <CGRA bitstream file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string binaryFile = argv[1];
    std::string kernel_name = argv[2];
    std::string bitstreamFile = argv[3];
    vector_u16 inputs[NUM_CHANNELS];
    vector_u16 outputs[NUM_CHANNELS];

    long size;
    std::vector<long> out_size;

    auto cgra_acc = CgraFpga(NUM_CHANNELS, NUM_CHANNELS);
    cgra_acc.cgra_fpga_init(binaryFile, kernel_name, bitstreamFile);

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        std::stringstream ssin;
        ssin << "in" << i << ".txt";
        size = read_file(ssin.str(), inputs[i]);
        if (size > 0)
        {
            cgra_acc.createInputQueue(i, size*2);
            auto ptr_in = (uint16_t*)cgra_acc.getInputQueue(i);
            for(int j = 0; j < size;j++)
            {
                ptr_in[j] = inputs[i][j];
            }
        }
        std::stringstream ssout;
        ssout << "out" << i << ".txt";
        size = read_file(ssout.str(), outputs[i]);
        out_size.push_back(size);
        if (size > 0)
        {
            cgra_acc.createOutputQueue(i, size * 2);
        }
    }

    cgra_acc.cgra_execute();

    for (int c = 0; c < NUM_CHANNELS; c++)
    {
        if(inputs[c].size() > 0){
        std::cout << std::endl
                  << "IN" << c << ": ";
        size = size<10?size:10;
        auto ptr_in = (unsigned short *)cgra_acc.getInputQueue(c);
        for (int i = 0; i < size; i++)
        {
            std::cout << ptr_in[i] << " ";
        }
        std::cout << std::endl;
        }
        if(out_size[c] > 0){
        std::cout << std::endl
                  << "OUT" << c << ": ";
         size = out_size[c]<10?out_size[c]:10;
        auto ptr_out = (unsigned short *)cgra_acc.getOutputQueue(c);
        for (int i = 0; i < size; i++)
        {
            std::cout << ptr_out[i] << " ";
        }
        std::cout << std::endl;
        }
    }

    cgra_acc.print_report();
    cgra_acc.cleanup();

    return 0;
}

long read_file(std::string file, vector_u16 &data)
{
    long size = 0;
    std::string line;
    if (file.substr(0, 2) == "in")
    {
        std::ifstream MyReadFile(file);
        getline(MyReadFile, line);
	    size = std::stoul(line, nullptr, 10);
        while (getline(MyReadFile, line))
        {
            unsigned short x = std::stoul(line, nullptr, 10);
            data.push_back(x);
        }
        MyReadFile.close();
    }
    else if (file.substr(0, 3) == "out")
    {
        std::ifstream MyReadFile(file);
        getline(MyReadFile, line);
        size = std::stoul(line, nullptr, 10);
        MyReadFile.close();
    }
    else
    {
        std::cout << "[Error] Input file not found: " << file << std::endl;
        return -1;
    }

    return size;
}
