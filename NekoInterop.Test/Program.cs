using System;
using System.Collections.Generic;
using System.Text;

namespace NekoInterop.Test
{
    class Program
    {
        static void Main(string[] args)
        {
            Ruby.Initialize();
            do
            {
                Console.Write("NekoRuby> ");
                string code = Console.ReadLine();
                Console.WriteLine(Ruby.Eval(code));
            } while (true);
        }
    }
}
