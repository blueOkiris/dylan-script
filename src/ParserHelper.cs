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
            return new CompoundToken(TokenType.Program, new Token[0] {});
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
