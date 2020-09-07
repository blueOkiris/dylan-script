namespace dylanscript {
    partial class Parser {
        private static (int, CompoundToken) parseStruct(
                ref int i, Token[] tokens) {
            return (
                0,
                new CompoundToken(TokenType.Program, new CompoundToken[0] {})
            );
        }

        private static (int, CompoundToken) parseFuncDef(
                ref int i, Token[] tokens) {
            return (
                0,
                new CompoundToken(TokenType.Program, new CompoundToken[0] {})
            );
        }

        private static (int, CompoundToken) parseImport(
                ref int i, Token[] tokens) {
            return (
                0,
                new CompoundToken(TokenType.Program, new CompoundToken[0] {})
            );
        }

        public static CompoundToken BuildProgram(Token[] tokens) {
            return new CompoundToken(
                TokenType.Program, new CompoundToken[0] {}
            );
        }
    }

    class Compiler {
        public static string Translate(CompoundToken ast) {
            return "";
        }
    }
}
