using System;

namespace dylanscript {
    class Program {
        static void Main(string[] args) {
            var settingsManager = SettingsManager.Instance();
            if(!settingsManager.SetupFromArguments(args)) {
                Environment.Exit(-1);
            }
            var settings = settingsManager.State();
            var code = CodeReader.FromFile(settings);
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

            Console.WriteLine(code.Item1);
        }
    }
}
