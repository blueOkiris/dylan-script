using System;
using System.Collections.Generic;

namespace dylanscript {
    partial class Parser {
        private static CompoundToken parseStruct(
                ref int i, SymbolToken[] tokens) {
            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }
            if(tokens[i].Type != TokenType.Name) {
                throw new Exception(
                    "Parser Error: "
                    + "Expected new name after keyword 'struct' at line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var name = tokens[i];
            i++;
            var body = parseStructBody(ref i, tokens);

            return new CompoundToken(
                TokenType.Struct, new Token[2] { name, body }
            );
        }

        private static CompoundToken parseFuncDef(
                ref int i, SymbolToken[] tokens) {
            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }
            if(tokens[i].Type != TokenType.Name) {
                throw new Exception(
                    "Parser Error: "
                    + "Expected new name after keyword 'struct' at line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var name = tokens[i];

            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }
            if(tokens[i].Type != TokenType.Parenth
                    || tokens[i].Source != "(") {
                throw new Exception(
                    "Parser Error: "
                    + "Expected new name after keyword 'struct' at line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var lpar = tokens[i];

            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }

            CompoundToken args = null;
            if(tokens[i].Type != TokenType.Parenth) {
                args = parseDefArgBody(ref i, tokens);
            }

            if(tokens[i].Type != TokenType.Parenth
                    || tokens[i].Source != ")") {
                throw new Exception(
                    "Parser Error: "
                    + "Expected new name after keyword 'struct' at line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var rpar = tokens[i];

            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }
            if(tokens[i].Type != TokenType.RetDerefOp) {
                throw new Exception(
                    "Parser Error: "
                    + "Expected new name after keyword 'struct' at line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var retOp = tokens[i];

            var type = parseTypeName(ref i, tokens);
            var body = parseScopeDec(ref i, tokens);

            var children = new List<Token>();
            children.Add(name);
            children.Add(lpar);
            if(args != null) {
                children.Add(args);
            }
            children.Add(rpar);
            children.Add(retOp);
            children.Add(type);
            children.Add(body);

            return new CompoundToken(TokenType.FuncDef, children.ToArray());
        }

        private static CompoundToken parseImport(
                ref int i, SymbolToken[] tokens) {
            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }
            var ident = parseIdent(ref i, tokens);
            return new CompoundToken(TokenType.Import, new Token[1] { ident });
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
