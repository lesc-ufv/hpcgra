#include "kmeans.h"

int main(int argc, char *argv[]) {

    auto df = createDataFlow(0,8,8);
    df->toJSON("../kmeans.json");
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
                df->connect(inputs[i], subs[j][i], subs[j][i]->getPortA());
            }
            for (auto s:subs[j]) {
                //auto a = new Abs(idx++);
                //df->connect(s, a, a->getPortA());
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
                    auto passA = new PassA(idx++);
                    df->connect(aux[0], passA, passA->getPortA());
                    aux_reduz.push_back(passA);
                    start = 1;
                }
                for (int l = start; l < aux.size(); l += 2) {
                    auto add = new Add(idx++);
                    df->connect(aux[l], add, add->getPortA());
                    df->connect(aux[l + 1], add, add->getPortB());
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
                auto slt = new Slt(idx++);
                auto mux = new Mux(idx++);
                df->connect(aux[l], slt, slt->getPortA());
                df->connect(aux[l + 1], slt, slt->getPortB());
                df->connect(slt, mux, mux->getPortBranch());
                aux_reduz.push_back(slt);
                if (reg > 0) {
                    df->connect(mux_reduz.front(), mux, mux->getPortA());
                    mux_reduz.pop();
                    df->connect(mux_reduz.front(), mux, mux->getPortB());
                    mux_reduz.pop();
                } else {
                    auto reg1 = new PassBi(idx++, l);
                    auto reg2 = new PassBi(idx++, l + 1);
                    df->connect(reg1, mux, mux->getPortA());
                    df->connect(reg2, mux, mux->getPortB());
                }
                mux_reduz.push(mux);
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
                auto r = new PassA(idx++);
                df->connect(rr, r, r->getPortA());
                rr = r;
            }
            auto sltEnd = new Slt(idx++);
            auto muxEnd = new Mux(idx++);
            auto reg1 = new PassBi(idx++, num_clusters - 1);

            df->connect(aux[0], sltEnd, sltEnd->getPortA());
            df->connect(rr, sltEnd, sltEnd->getPortB());
            df->connect(sltEnd, muxEnd, muxEnd->getPortBranch());
            df->connect(mux_reduz.front(), muxEnd, muxEnd->getPortA());
            df->connect(reg1, muxEnd, muxEnd->getPortB());
            auto out = new OutputStream(outId,nullptr,0);
            df->connect(muxEnd, out, out->getPortA());
        } else {
            auto out = new OutputStream(outId,nullptr,0);
            df->connect(mux_reduz.front(), out, out->getPortA());
        }
    } else {
        auto in = new InputStream(idx++,nullptr,0);
        auto out = new OutputStream(idx++,nullptr,0);
        auto reg = new PassBi(idx, 0);
        df->connect(in, out, out->getPortB());
        df->connect(reg, out, out->getPortA());
    }

    return df;
}

bool compare(Operator *a, Operator *b) {
    return a->getId() < b->getId();
}
