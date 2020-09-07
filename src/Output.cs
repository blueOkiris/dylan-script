using System;
using System.Collections.Generic;

namespace dylanscript {
    partial class Parser {
        private static CompoundToken parseStruct(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseFuncDef(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseImport(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        public static CompoundToken BuildProgram(SymbolToken[] tokens) {
            var instructions = new List<Token>();

            for(int i = 0; i < tokens.Length; i++) {
                if(tokens[i].Type == TokenType.Keyword
                        && tokens[i].Source == "import") {
                    instructions.Add(parseImport(ref i, tokens));
                } else if(tokens[i].Type == TokenType.Keyword
                        && tokens[i].Source == "struct") {
                    instructions.Add(parseStruct(ref i, tokens));
                } else if(tokens[i].Type == TokenType.Keyword
                        && tokens[i].Source == "fn") {
                    instructions.Add(parseFuncDef(ref i, tokens));
                } else {
                    throw new Exception(
                        "Parser Error: "
                        + "Top Level token not import, struct, or fn at "
                        + "line " + tokens[i].Line + ", pos " + tokens[i].Pos
                        + "."
                    );
                }
            }
            
            return new CompoundToken(TokenType.Program, instructions.ToArray());
        }
    }

    class Compiler {
        public static string Translate(CompoundToken ast) {
            return "";
        }
    }
}
