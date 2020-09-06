using System.Collections.Generic;

namespace dylanscript {
    enum TokenType {
        Parenth, Brace, Bracket, BoolOp, Comma, Semi,
        MemberOp, Sci, Float, Integer, String, AsgnOp,
        TypeOp, RefOp, CondOp, NotOp, RetDerefOp, SumOp,
        MulOp, ExpOp, SciOp, BoolVal, Keyword, Name,

        Number, Bool, Ident, ListBody, List, MapBody,
        Map, TypeName, Term, PowExp, MulExp, Expr,
        FuncCall, Declaration, Assignment, If, While, DefArgBody,
        Stmt, ScopeDec, StructBody,

        Struct, Import, FuncDef, Program
    }

    class Token {
        public TokenType Type;

        public Token(TokenType type) {
            Type = type;
        }
    }

    class SymbolToken : Token {
        public string Source;
        public int Line;
        public int Pos;

        public SymbolToken(TokenType type, string source, int line, int pos)
                : base(type) {
            Source = source;
            Line = line;
            Pos = pos;
        }
    }

    class CompoundToken : Token {
        public Token[] Children;

        public CompoundToken(TokenType type, Token[] children) : base(type) {
            // Make sure we make COPIES of children, not just add their refs
            var childs = new List<Token>();
            foreach(var child in children) {
                Token childCopy;

                if(child is SymbolToken) {
                    childCopy = new SymbolToken(
                        child.Type,
                        (child as SymbolToken).Source,
                        (child as SymbolToken).Line, (child as SymbolToken).Pos
                    );
                } else if(child is CompoundToken) {
                    childCopy = new CompoundToken(
                        child.Type, (child as CompoundToken).Children
                    );
                } else {
                    childCopy = new Token(child.Type);
                }
                childs.Add(childCopy);
            }
            Children = childs.ToArray();
        }
    }
}
