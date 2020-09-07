using System;

namespace dylanscript {
    class Program {
        static void Main(string[] args) {
            var settingsManager = SettingsManager.Instance();
            if(!settingsManager.SetupFromArguments(args)) {
                Environment.Exit(-1);
            }
            var settings = settingsManager.State();
            var code = CodeReader.FromFile(settings.FileName);
            switch(code.Item2) {
                case FileIOState.NotFound:
                    Console.WriteLine("File was not found.");
                    Environment.Exit(-1);
                    break;
                
                case FileIOState.ReadError:
                    Console.WriteLine("Could not read file.");
                    Environment.Exit(-1);
                    break;
                
                case FileIOState.Okay:
                    break;
            }

            //Console.WriteLine(code.Item1);
            var tokens = Lexer.Tokens(code.Item1);
            if(settings.Debug) {
                foreach(var token in tokens) {
                    Console.WriteLine(token);
                }
            }
            try {
                var ast = Parser.BuildProgram(tokens);
                if(settings.Debug) {
                    Console.WriteLine(ast);
                }

                var newCode = Compiler.Translate(ast);
                if(settings.Debug) {
                    Console.WriteLine('{');
                    foreach(var bt in newCode) {
                        Console.WriteLine(bt);
                    }
                    Console.WriteLine('}');
                }
            } catch(Exception e) {
                Console.WriteLine(e.Message);
                Environment.Exit(-1);
            }
        }
    }
}
