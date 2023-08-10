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

    int size;

    auto cgra_acc = CgraFpga(NUM_CHANNELS, NUM_CHANNELS);
    cgra_acc.cgra_fpga_init(binaryFile, kernel_name, bitstreamFile);

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        std::stringstream ssin;
        ssin << "in" << i << ".txt";
        read_file(ssin.str(), inputs[i]);
        if (inputs[i][0] > 0)
        {
            cgra_acc.createInputQueue(i, inputs[i][0] * 2);
            auto ptr_in = (uint16_t*)cgra_acc.getInputQueue(i);
            for(int j = 0; j < inputs[i][0];j++)
            {
                ptr_in[j] = inputs[i][j+1];
            }
        }
        std::stringstream ssout;
        ssout << "out" << i << ".txt";
        read_file(ssout.str(), outputs[i]);
        if (outputs[i][0] > 0)
        {
            cgra_acc.createOutputQueue(i, outputs[i][0] * 2);
        }
    }

    cgra_acc.cgra_execute();

    for (int c = 0; c < NUM_CHANNELS; c++)
    {

        std::cout << std::endl
                  << "IN" << c << ": ";
        size = std::min((int)inputs[c][0], 10);
        auto ptr_in = (unsigned short *)cgra_acc.getInputQueue(c);
        for (int i = 0; i < size; i++)
        {
            std::cout << ptr_in[i] << " ";
        }
        std::cout << std::endl;

        std::cout << std::endl
                  << "OUT" << c << ": ";
        size = std::min((int)outputs[c][0], 10);
        auto ptr_out = (unsigned short *)cgra_acc.getOutputQueue(c);
        for (int i = 0; i < size; i++)
        {
            std::cout << ptr_out[i] << " ";
        }
        std::cout << std::endl;
    }

    cgra_acc.print_report();
    cgra_acc.cleanup();

    return 0;
}

bool read_file(std::string file, vector_u16 &data)
{
    std::string line;
    if (file.substr(0, 2) == "in")
    {
        std::ifstream MyReadFile(file);
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
        int x = std::stoul(line, nullptr, 10);
        data.push_back(x);
        MyReadFile.close();
    }
    else
    {
        std::cout << "[Error] Input file not found: " << file << std::endl;
        return false;
    }

    return true;
}
