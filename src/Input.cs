using System;
using System.IO;

namespace dylanscript {
    class SettingsManager {
        private static SettingsManager instance = null;
        private string fileName;

        private SettingsManager() {
            fileName = "";
        }

        public static SettingsManager Instance() {
            if(instance == null) {
                instance = new SettingsManager();
            }

            return instance;
        }

        public string State() {
            return fileName;
        }

        public bool SetupFromArguments(string[] args) {
            if(args.Length < 1) {
                Console.WriteLine("No filename given.");
                return false;
            } else if(args.Length > 1) {
                Console.WriteLine("Too many arguments given.");
                return false;
            } else {
                fileName = args[0];
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
        private static string[] patterns = {

        };
        
        public static SymbolToken[] Tokens(string code) {
            return new SymbolToken[0] {};
        }
    }
}
