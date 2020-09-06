using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace dylanscript {
    struct Settings {
        public string FileName;
        public bool Debug;

        public Settings(string fileName = "", bool debug = false) {
            FileName = fileName;
            Debug = debug;
        }
    }

    class SettingsManager {
        private static SettingsManager instance = null;
        private Settings state;

        private SettingsManager() {
            state = new Settings();
        }

        public static SettingsManager Instance() {
            if(instance == null) {
                instance = new SettingsManager();
            }

            return instance;
        }

        public Settings State() {
            return state;
        }

        public bool SetupFromArguments(string[] args) {
            if(args.Length < 1) {
                Console.WriteLine("No filename given.");
                return false;
            } else if(args.Length > 2) {
                Console.WriteLine("Too many arguments given.");
                return false;
            } else {
                if(args.Length > 1) {
                    if(args[0] == "--debug" || args[1] == "--debug") {
                        state.Debug = true;
                    } else {
                        Console.WriteLine("Unexpected non-filename argument");
                        return false;
                    }

                    state.FileName = args[0] == "--debug" ? args[1] : args[0];
                } else {
                    state.FileName = args[0];
                }
                return true;
            }
        }
    }

    enum FileIOState {
        Okay,
        NotFound,
        ReadError
    }

    class CodeReader {
        public static (string, FileIOState) FromFile(string fileName) {
            if(!File.Exists(fileName)) {
                return ("", FileIOState.NotFound);
            }

            try {
                var code = File.ReadAllText(fileName);
                return (code, FileIOState.Okay);
            } catch(Exception e) {
                return (e.Message, FileIOState.ReadError);
            }
        }
    }

    class Lexer {
        private static Dictionary<string, TokenType> patterns = 
                new Dictionary<string, TokenType>() {
            { "[\\(\\)]", TokenType.Parenth }, { "[{}]", TokenType.Brace },
            { "[\\[\\]]", TokenType.Bracket }, { "[&|]", TokenType.BoolOp },
            { ",", TokenType.Comma }, { ";", TokenType.Semi },
            { "\\.", TokenType.MemberOp },
            { "[0-9]+(\\.[0-9]+)?e[0-9]+", TokenType.Sci },
            { "[0-9]+(\\.[0-9]+)?", TokenType.Float },
            { "[0-9]+", TokenType.Integer },
            { "'(\\\\.|[^'\\\\])*'", TokenType.String },
            { ":=", TokenType.AsgnOp }, { ":", TokenType.TypeOp },
            { "#", TokenType.RefOp },
            { "([<>]=*)|(~=)|=", TokenType.CondOp },
            { "~", TokenType.NotOp }, { "->", TokenType.RetDerefOp },
            { "[+-]", TokenType.SumOp }, { "[*\\/%]", TokenType.MulOp },
            { "\\^", TokenType.ExpOp },
            { "(true)|(false)", TokenType.BoolVal },
            { 
                "(ref)|(if)|(elif)|(else)|(while)|(import)|(fn)|(struct)",
                TokenType.Keyword
            },
            { "[A-Za-z_]+[A-Za-z0-9_]*", TokenType.Name }
        };
        
        public static SymbolToken[] Tokens(string code) {
            var tokens = new List<SymbolToken>();

            var regExes = new Dictionary<Regex, TokenType>();
            foreach(var key in patterns.Keys) {
                regExes.Add(
                    new Regex(key, RegexOptions.Compiled),
                    patterns[key]
                );
            }
            
            var len = code.Length;
            for(int ind = 0, line = 1, col = 1; ind < len; ind++, col++) {
                if(code[ind] == '\n') {
                    line++;
                    col = 0;
                    //Console.WriteLine();
                }
                //Console.Write(pos + " ");

                foreach(var key in regExes.Keys) {
                    var match = key.Match(code.Substring(ind));
                    if(match.Success && match.Index == 0) {
                        tokens.Add(
                            new SymbolToken(
                                regExes[key], match.Value,
                                line, col
                            )
                        );
                        col += match.Length - 1;
                        ind += match.Length - 1;
                        break;
                    }
                }
            }

            return tokens.ToArray();
        }
    }
}
