using System;
using System.Collections.Generic;

namespace dylanscript {
    partial class Parser {
        private static CompoundToken parseWhile(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseIf(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseAssignment(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseStmt(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseScopeDec(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseDefArgBody(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseNumber(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseFuncCall(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseBool(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseMapBody(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseMap(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseListBody(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseTypeName(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseList(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseTerm(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parsePowExpr(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseMulExpr(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseExpr(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseDeclaration(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseStructBody(
                ref int i, SymbolToken[] tokens) {
            if(tokens[i].Type != TokenType.Brace
                    || tokens[i].Source != "{") {
                throw new Exception(
                    "Parser Error: "
                    + "Expected '{' after struct name at line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var lbrace = tokens[i];

            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }
            var declares = new List<Token>();
            while(tokens[i].Type != TokenType.Brace
                    || tokens[i].Source != "}") {
                i++;
                if(i >= tokens.Length) {
                    throw new Exception(
                        "Parser Error: "
                        + "Unexpected EOF at line " + tokens[i - 1].Line
                        + ", pos " + tokens[i - 1].Pos + "."
                    );
                }
            }
            var rbrace = tokens[i];

            var children = new List<Token>();
            children.Add(lbrace);
            foreach(var dec in declares) {
                children.Add(dec);
            }
            children.Add(rbrace);

            return new CompoundToken(TokenType.StructBody, children.ToArray());
        }

        private static CompoundToken parseIdent(
                ref int i, SymbolToken[] tokens) {

            var children = new List<Token>();
                       
            if(tokens[i].Type == TokenType.Name) {
                children.Add(tokens[i]);

                if(i + 1 < tokens.Length 
                        && tokens[i].Type == TokenType.MemberOp) {
                    i++;
                    children.Add(tokens[i]);
                    i++;
                    children.Add(parseIdent(ref i, tokens));
                }
            } else {
                int identI = i;
                children.Add(parseExpr(ref i, tokens));

                if(i + 1 >= tokens.Length) {
                    throw new Exception(
                        "Parser Error: "
                        + "Unexpected EOF at identifier definition at line "
                        + tokens[identI].Line + ", pos " + tokens[identI].Pos
                        + "."
                    );
                } else if(tokens[i].Type != TokenType.MemberOp) {
                    throw new Exception(
                        "Parser Error: "
                        + "Expected member op to convert expression into "
                        + "identifier at line " + tokens[identI].Line + ", pos "
                        + tokens[identI].Pos + "."
                    );
                }
                children.Add(tokens[i]);
                i++;
                children.Add(parseIdent(ref i, tokens));
            }

            return new CompoundToken(TokenType.Ident, children.ToArray());
        }
    }
}
