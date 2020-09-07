using System;

namespace dylanscript {
    enum OpCode {
        
    }

    struct Instruction {
        public OpCode Operation;
        public object[] Operands;

        public Instruction(OpCode opCode, object[] args) {
            Operation = opCode;
            Operands = args;
        }
    }

    struct StackFrame {

    }
}