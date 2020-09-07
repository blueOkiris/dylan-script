using System;
using System.Collections.Generic;

namespace dylanscript {
    partial class Parser {
        private static CompoundToken parseWhile(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.While, new Token[0] {});
        }

        private static CompoundToken parseIf(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.If, new Token[0] {});
        }

        private static CompoundToken parseAssignment(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Assignment, new Token[0] {});
        }

        private static CompoundToken parseStmt(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Stmt, new Token[0] {});
        }

        private static CompoundToken parseScopeDec(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.ScopeDec, new Token[0] {});
        }

        private static CompoundToken parseDefArgBody(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.DefArgBody, new Token[0] {});
        }

        private static CompoundToken parseNumber(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Program, new Token[0] {});
        }

        private static CompoundToken parseFuncCall(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.FuncCall, new Token[0] {});
        }

        private static CompoundToken parseBool(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Bool, new Token[0] {});
        }

        private static CompoundToken parseMapBody(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.MapBody, new Token[0] {});
        }

        private static CompoundToken parseMap(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Map, new Token[0] {});
        }

        private static CompoundToken parseListBody(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.ListBody, new Token[0] {});
        }

        // <type-name> ::= [ <keyword> ] { <ptr-op> } <name>
        private static CompoundToken parseTypeName(
                ref int i, SymbolToken[] tokens) {
            SymbolToken refWord = null;
            if(tokens[i].Type == TokenType.Keyword
                    && tokens[i].Source == "ref") {
                refWord = tokens[i];
                i++;
                if(i >= tokens.Length) {
                    throw new Exception(
                        "Parser Error: "
                        + "Unexpected EOF at line " + tokens[i - 1].Line
                        + ", pos " + tokens[i - 1].Pos + "."
                    );
                }
            }

            var pointerOps = new List<Token>();
            while(tokens[i].Type == TokenType.RetDerefOp) {
                pointerOps.Add(tokens[i]);
                i++;
                if(i >= tokens.Length) {
                    throw new Exception(
                        "Parser Error: "
                        + "Unexpected EOF at line " + tokens[i - 1].Line
                        + ", pos " + tokens[i - 1].Pos + "."
                    );
                }
            }

            if(tokens[i].Type != TokenType.Name) {
                throw new Exception(
                    "Parser Error: "
                    + "Expected name in type line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var name = tokens[i];

            var children = new List<Token>();
            if(refWord != null) {
                children.Add(refWord);
            }
            foreach(var pointerOp in pointerOps) {
                children.Add(pointerOp);
            }
            children.Add(name);

            return new CompoundToken(TokenType.TypeName, children.ToArray());
        }

        private static CompoundToken parseList(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.List, new Token[0] {});
        }

        private static CompoundToken parseTerm(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Term, new Token[0] {});
        }

        private static CompoundToken parsePowExpr(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.PowExp, new Token[0] {});
        }

        private static CompoundToken parseMulExpr(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.MulExp, new Token[0] {});
        }

        private static CompoundToken parseExpr(
                ref int i, SymbolToken[] tokens) {
            return new CompoundToken(TokenType.Expr, new Token[0] {});
        }

        // <declaration> ::= <name> <type-op> <type-name> [ <asgn-op> <expr> ]
        private static CompoundToken parseDeclaration(
                ref int i, SymbolToken[] tokens) {
            if(tokens[i].Type != TokenType.Name) {
                throw new Exception(
                    "Parser Error: "
                    + "Expected name in declaration in struct at line "
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
            if(tokens[i].Type != TokenType.TypeOp) {
                throw new Exception(
                    "Parser Error: "
                    + "Expected ':' in declaration in struct at line "
                    + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                );
            }
            var typeOp = tokens[i];

            i++;
            if(i >= tokens.Length) {
                throw new Exception(
                    "Parser Error: "
                    + "Unexpected EOF at line " + tokens[i - 1].Line
                    + ", pos " + tokens[i - 1].Pos + "."
                );
            }
            var type = parseTypeName(ref i, tokens);

            SymbolToken asgnOp = null;
            CompoundToken asgnExpr = null;
            if(i + 1 < tokens.Length && tokens[i].Type == TokenType.AsgnOp) {
                i++;
                asgnOp = tokens[i];

                i++;
                if(i >= tokens.Length) {
                    throw new Exception(
                        "Parser Error: "
                        + "Unexpected EOF at line " + tokens[i - 1].Line
                        + ", pos " + tokens[i - 1].Pos + "."
                    );
                }
                asgnExpr = parseExpr(ref i, tokens);
            }

            var children = new List<Token>();
            children.Add(name);
            children.Add(typeOp);
            children.Add(type);

            if(asgnOp != null) {
                children.Add(asgnOp);
                children.Add(asgnExpr);
            }

            return new CompoundToken(TokenType.Declaration, children.ToArray());
        }

        // <struct-body> ::= <brace> { ( <declaration> ) <semi> } <brace>
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
                var declare = parseDeclaration(ref i, tokens);
                declares.Add(declare);
                
                i++;
                if(i >= tokens.Length) {
                    throw new Exception(
                        "Parser Error: "
                        + "Unexpected EOF at line " + tokens[i - 1].Line
                        + ", pos " + tokens[i - 1].Pos + "."
                    );
                }
                if(tokens[i].Type != TokenType.Semi) {
                    throw new Exception(
                        "Parser Error: "
                        + "Expected '{' after struct name at line "
                        + tokens[i].Line + ", pos: " + tokens[i].Pos + "."
                    );
                }
                declares.Add(tokens[i]);
                
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

        // <ident> ::= name [ <member-op> <ident> ] | <expr> <member-op> <ident>
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
