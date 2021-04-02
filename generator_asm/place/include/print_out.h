#ifndef __PRINT_OUT_H
#define __PRINT_OUT_H

void print_inputs_outputs_json(Graph g, string path) {

    ofstream myfile;
    myfile.open(path+".map");
    myfile << "{\n";
    myfile << "\t\"input\":\n";
    myfile << "\t{\n";
    for (int i = 0; i < g.get_inputs().size(); ++i) {
        myfile << "\t\t\""<< g.get_name_node(g.get_inputs()[i]).c_str() << "\": " << g.get_inputs()[i];
        if (i == g.get_inputs().size()-1) myfile << "\n";
        else myfile << ",\n";
    }
    myfile << "\t},\n";
    myfile << "\t\"output\":\n";
    myfile << "\t{\n";
    for (int i = 0; i < g.get_outputs().size(); ++i) {
        myfile << "\t\t\""<< g.get_name_node(g.get_outputs()[i]).c_str() << "\": " << g.get_outputs()[i];
        if (i == g.get_outputs().size()-1) myfile << "\n";
        else myfile << ",\n";
    }
    myfile << "\t}\n";
    myfile << "}\n";
    myfile.close();
}

#endif