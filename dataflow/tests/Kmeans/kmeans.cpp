#include "kmeans.h"

int main(int argc, char *argv[]) {

    int num_clusters=4;
    int num_dim=4;
    auto df = createDataFlow(0,num_clusters,num_dim);
    df->toJSON("../kmeans_"+to_string(num_dim)+"_"+to_string(num_clusters)+".json");
    df->toDOT("../kmeans_"+to_string(num_dim)+"_"+to_string(num_clusters)+".dot");
    delete df;
    
    num_clusters=8;
    num_dim=8;
    df = createDataFlow(0,num_clusters,num_dim);
    df->toJSON("../kmeans_"+to_string(num_dim)+"_"+to_string(num_clusters)+".json");
    df->toDOT("../kmeans_"+to_string(num_dim)+"_"+to_string(num_clusters)+".dot");
    delete df;

    num_clusters=16;
    num_dim=8;
    df = createDataFlow(0,num_clusters,num_dim);
    df->toJSON("../kmeans_"+to_string(num_dim)+"_"+to_string(num_clusters)+".json");
    df->toDOT("../kmeans_"+to_string(num_dim)+"_"+to_string(num_clusters)+".dot");
    delete df;
    
    return 0;
}

DataFlow *createDataFlow(int id, int num_clusters, int num_dim) {
    auto df = new DataFlow(id, "kmeans");
    int idx = 0;
    std::vector<Operator *> inputs;
    std::map<int, std::vector<Operator *>> subs;
    std::map<int, std::vector<Operator *>> abs;
    std::vector<Operator *> adds_end;
    std::vector<Operator *> aux;
    std::vector<Operator *> aux_reduz;
    std::vector<Operator *> slt_reduz;
    std::queue<Operator *> mux_reduz;
    std::vector<Operator *> constants;
    int outId;
    if (num_clusters > 1) {
        for (int j = 0; j < num_clusters; ++j) {
            for (int i = 0; i < num_dim; ++i) {
                auto s = new Subi(idx++, j * num_dim + i);
                subs[j].push_back(s);
            }
        }
        inputs.reserve(static_cast<unsigned long>(num_dim));
        for (int i = 0; i < num_dim; ++i) {
            inputs.push_back(new InputStream(idx++,nullptr,0));
        }
        outId = idx++;
        for (int j = 0; j < num_clusters; ++j) {
            for (int i = 0; i < num_dim; ++i) {
                df->connect(inputs[i], subs[j][i], 0);
            }
            for (auto s:subs[j]) {
                abs[j].push_back(s);
            }
            aux.clear();
            aux_reduz.clear();
            for (auto a:abs[j]) {
                aux.push_back(a);
            }
            while (aux.size() > 1) {
                int start = 0;
                if (aux.size() % 2 != 0) {
                    auto passA = new Addi(idx++,0);
                    df->connect(aux[0], passA,0);
                    aux_reduz.push_back(passA);
                    start = 1;
                }
                for (int l = start; l < aux.size(); l += 2) {
                    auto add = new Add(idx++);
                    df->connect(aux[l], add, 0);
                    df->connect(aux[l + 1], add, 1);
                    aux_reduz.push_back(add);
                }
                aux.clear();
                for (auto a:aux_reduz) {
                    aux.push_back(a);
                }
                aux_reduz.clear();
            }
            for (auto a:aux) {
                adds_end.push_back(a);
            }
        }
        aux.clear();
        aux_reduz.clear();
        for (auto a:adds_end) {
            aux.push_back(a);
        }

        sort(aux.begin(), aux.end(), compare);

        int reg = 0;
        auto addEnd = aux[aux.size() - 1];

        while (aux.size() > 1) {
            for (int l = 0; l < aux.size() - 1; l += 2) {
                if (reg > 0) {
                    auto slt = new Slt(idx++);
                    auto mux = new Mux(idx++);
                    df->connect(aux[l], slt, 0);
                    df->connect(aux[l + 1], slt, 1);
                    df->connect(slt, mux, 0);
                    aux_reduz.push_back(slt);
                    df->connect(mux_reduz.front(), mux, 1);
                    mux_reduz.pop();
                    df->connect(mux_reduz.front(), mux,2);
                    mux_reduz.pop();
                    mux_reduz.push(mux);
                } else {
                    auto slt = new Slt(idx++);
                    auto mux = new Muxii(idx++,l,l+1);
                    df->connect(aux[l], slt, 0);
                    df->connect(aux[l + 1], slt, 1);
                    df->connect(slt, mux, 0);
                    aux_reduz.push_back(slt);
                    mux_reduz.push(mux);
                }

            }
            aux.clear();
            for (auto a:aux_reduz) {
                aux.push_back(a);
            }
            aux_reduz.clear();
            reg++;
        }
        if (num_clusters % 2 != 0) {
            auto rr = addEnd;
            for (int i = 0; i < reg; ++i) {
                auto r = new Addi(idx++,0);
                df->connect(rr, r, 0);
                rr = r;
            }
            auto sltEnd = new Slt(idx++);
            auto muxEnd = new Muxi(idx++,num_clusters - 1);

            df->connect(aux[0], sltEnd,0);
            df->connect(rr, sltEnd, 1);
            df->connect(sltEnd, muxEnd, 0);
            df->connect(mux_reduz.front(), muxEnd, 1);
            auto out = new OutputStream(outId,nullptr,0);
            df->connect(muxEnd, out, 0);
        } else {
            auto out = new OutputStream(outId,nullptr,0);
            df->connect(mux_reduz.front(), out, 0);
        }
    } else {
        auto in = new InputStream(idx++,nullptr,0);
        auto out = new OutputStream(idx++,nullptr,0);
        auto muli = new Muli(idx, 0);
        df->connect(in, muli, 0);
        df->connect(muli, out, 0);
    }

    return df;
}

bool compare(Operator *a, Operator *b) {
    return a->getId() < b->getId();
}
