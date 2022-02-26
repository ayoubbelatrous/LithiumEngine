using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Linq;

namespace Engine
{
    public class Compiler
    {
        public static string[] Compile(string[] files)
        {

            List<string> errorlist = null;
            CodeDomProvider provider = CodeDomProvider.CreateProvider("C#");
            CompilerParameters pars = new CompilerParameters();
            pars.GenerateExecutable = false;
            pars.OutputAssembly = "test.dll";
            
            CompilerResults results = provider.CompileAssemblyFromFile(pars, files);
            results.Errors.Cast<CompilerError>().ToList().ForEach(error => errorlist.Add(error.ErrorText +" :: " + error.ErrorNumber));

            return errorlist.ToArray();




        }

    }
}
