#include "cfgPrint.h"

using namespace llvm;

namespace cfgPrint {
    
    // get label of register
    string cfgPrinterPass::getLabel(const Value *v) {
        StringRef bbName(v->getName());
        return bbName.str();
    }

    // get label of register and set on number map 
    string cfgPrinterPass::getName(const Value *v) {
        string s = getLabel(v);
        //if (opInstr.count(s) > 0)
        //    return opInstr[s];
        return s;
    }

    // get number map of all instruction and basicblock
    void cfgPrinterPass::get_map_label(Function &F) {
        for (Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb) {
            opInstr[getLabel(&(*bb))] = to_string(counter++);
            for (BasicBlock::iterator I = bb->begin(), e = bb->end(); I != e; ++I) {
                errs() << *I << "\n";
                if (!isa<SExtInst>(*I) && !isa<BranchInst>(*I) && !isa<PHINode>(*I) 
                    && !isa<ReturnInst>(*I) && !isa<StoreInst>(*I)) {
                    opInstr[getLabel(&(*I))] = to_string(counter++);
                }
            }
        }
    }

    PreservedAnalyses cfgPrinterPass::run(Function &F, FunctionAnalysisManager &FAM) {

        // Counter for Instruction and branch
        counter = 0;
        get_map_label(F);

        outFile.open(F.getName().str()+".dot");
        outFile << "digraph G {\n";
        for (Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb) {
            runBasicBlock(bb);
        }
        outFile << "}\n";
        outFile.close();
        
        return PreservedAnalyses::all();
    }

    // Executing Basic Block
    void cfgPrinterPass::runBasicBlock(Function::iterator &bb) {

        string block_name = getName(&(*bb)); 
        
        //outFile << "\tAA" << block_name <<" [shape=record, label=\"{";
        //outFile << "BB" << block_name << ":\\l\\l ";
        
        vector<string> target;
        for (BasicBlock::iterator I = bb->begin(), e = bb->end(); I != e; ++I) {
            runInstruction(I, target);
        }
        //outFile << "}\"];\n";

        /*for (vector<string>::iterator it = target.begin(), end = target.end(); it != end; ++it) {
            outFile << "\tAA" << block_name << " -> AA" << *it << "\n";
        }*/
    }

    // Executing Instruction
    void cfgPrinterPass::runInstruction(BasicBlock::iterator I, vector<string> &target) {

        // get the opcode name of each instruction
        int opcode = I->getOpcode();
        
        switch (opcode) {
            case Instruction::Add:
                AddNode(I);
                break;
            case Instruction::Ret: // Return
                RetNode(I); 
                break;
            /*case Instruction::Br: // Branch
                BrNode(I, target); 
                break;
            case Instruction::PHI: // PHINODE
                PhiNode(I); 
                break;
            case Instruction::Ret: // Return
                RetNode(I); 
                break;
            case Instruction::SExt: // SExt
                SExtNode(I);
                break;
            case Instruction::Call: // Call
                CallNode(I);
                break;
            case Instruction::Store: // Store
                StoreNode(I);
                break;
            */
            default: // other type
                //OtherNode(I); 
                break;
        }
    }

    void cfgPrinterPass::BrNode(BasicBlock::iterator I, vector<string> &target) {
        outFile << " " << I->getOpcodeName() << " ";

        if (I->getNumOperands() == 1) {
            outFile << " BB" << getName(I->getOperand(0));
            target.push_back(getName(I->getOperand(0)));
        } else {
            outFile << "%" << getName(I->getOperand(0)); 
            for (int i = I->getNumOperands()-1; i > 0; --i) {
                outFile << " BB" << getName(I->getOperand(i));
                target.push_back(getName(I->getOperand(i)));
            }
        }
        outFile << "\\l"; 
    }

    void cfgPrinterPass::AddNode(BasicBlock::iterator I) {
        for (int i = 0; i < I->getNumOperands(); ++i) {
            outFile << getName(I->getOperand(i)) << " -> " << getLabel(&(*I)) << "\n";
        }
    }

    void cfgPrinterPass::PhiNode(BasicBlock::iterator I) {
        
        PHINode *phi = dyn_cast<PHINode>(I);
        outFile << " %" << getLabel(&(*I)) << " = "  << phi->getOpcodeName() << " ";
        
        for (int i = 0; i < phi->getNumIncomingValues(); ++i) {
            outFile << "[ ";
            if (isa<ConstantInt>(phi->getIncomingValue(i))) {
                outFile << dyn_cast<ConstantInt>(phi->getIncomingValue(i))->getZExtValue();
            } else {
                outFile << " %" << getName(phi->getIncomingValue(i));
            }
            outFile << ", BB" << getName(phi->getIncomingBlock(i)) << " ]";

            if (i == 0) outFile << ", ";
        }
        outFile << "\\l";
    }

    void cfgPrinterPass::RetNode(BasicBlock::iterator I) {
        
        errs() << *I << "\n";

        if (I->getNumOperands() == 0) return;
        else if (isa<Constant>(I->getOperand(0)))
            return;
        else {
            
            outFile << getName(I->getOperand(0)) << " -> " << "output" << "\n";
        }
    }

    void cfgPrinterPass::SExtNode(BasicBlock::iterator I) {
        opInstr[getLabel(&(*I))] = getLabel(I->getOperand(0));
    }

    void cfgPrinterPass::CallNode(BasicBlock::iterator I) {

        string s;
        llvm::raw_string_ostream(s) << *I;

        if (s.find("void") != string::npos) {
            outFile << " %" << to_string(counter++) << " = ";
            outFile << "@" << getLabel(I->getOperand(I->getNumOperands()-1)) << "("; 
            for (int i = 0; i < I->getNumOperands()-1; ++i) {
                if (getLabel(I->getOperand(i)) != "") {
                    outFile << "%" << getLabel(I->getOperand(i));
                    if (i < I->getNumOperands()-2)
                        outFile << ", ";
                } else if (isa<ConstantInt>(I->getOperand(i))) {
                    outFile << dyn_cast<ConstantInt>(I->getOperand(i))->getZExtValue();
                    if (i < I->getNumOperands()-2)
                        outFile << ", ";
                }
            }
            outFile << ")";
        } else {
            outFile << " %" << getName(&(*I)) << " = ";
            outFile << I->getOpcodeName();

            if (s.find("printf") != string::npos) {
                outFile << " @printf"; 
            } else if (s.find("atoi") != string::npos) {
                outFile << " atoi";
            }
            outFile << "(";

            for (int i = I->getNumOperands()-1; i > 0; --i) {
                if (getName(I->getOperand(i)) != "printf") {
                    outFile << "%" << getName(I->getOperand(i));
                }
            }
            outFile << ")";
        }
        outFile << "\\l";
    }

    void cfgPrinterPass::StoreNode(BasicBlock::iterator I) {
        outFile << " %" << counter++ << " = " << I->getOpcodeName();
        for (int i = 0; i < I->getNumOperands(); ++i) {
            if (isa<ConstantInt>(I->getOperand(i))) {
                outFile << " " << dyn_cast<ConstantInt>(I->getOperand(i))->getZExtValue();
            } else {
                outFile << " %" << getName(I->getOperand(i));
            }
        
            if (I->getNumOperands()-1 != i) outFile << ", ";
        }
        outFile << "\\l";
    }

    void cfgPrinterPass::OtherNode(BasicBlock::iterator I) {

        outFile << " %" << getName(&(*I)) << " = " << I->getOpcodeName();
        if (ICmpInst *ICC = dyn_cast<ICmpInst>(I)) {
            llvm::ICmpInst::Predicate pr = ICC->getSignedPredicate();
            outFile << "-" << ICC->getPredicateName(pr).str();
        }

        string s;
        llvm::raw_string_ostream(s) << *I;

        if (isa<BinaryOperator>(*I)) {
            if (s.find("nsw") != string::npos) outFile << "-nsw";
            else if (s.find("nuw") != string::npos) outFile << "-nuw";
        }

        for (int i = 0; i < I->getNumOperands(); ++i) {
            if (isa<ConstantInt>(I->getOperand(i))) {
                outFile << " " << dyn_cast<ConstantInt>(I->getOperand(i))->getZExtValue();
            } else {
                outFile << " %" << getName(I->getOperand(i));
            }
        
            if (I->getNumOperands()-1 != i) outFile << ", ";
        }
        outFile << "\\l";
    }
}
