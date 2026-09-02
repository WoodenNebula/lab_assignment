using System.Reflection;
using Assignments;

namespace Basics {
    internal class Program {
        private static bool FindAssignmentClassFromParam(string[] args, out string ClassName) {
            int parsedNum = -1;
            if (args.Length == 0 || !int.TryParse(args[0], out parsedNum)) {
                Console.WriteLine("Usage: dotnet run <assignment number>");
                ClassName = "";
                return false;
            }
            ClassName = $"Assignments.Assignment{parsedNum}";
            return true;
        }

        private static AssignmentBase? GetAssignmentObject(string ClassName) {
            // Find class by name
            Type? type = Assembly.GetExecutingAssembly().GetType(ClassName);

            if (type == null) {
                Console.WriteLine($"Class '{ClassName}' not found.");
                return null;
            }

            AssignmentBase? instance = Activator.CreateInstance(type) as AssignmentBase;

            if (instance == null) {
                Console.WriteLine($"Could not create instance of '{ClassName}'.");
                return null;
            }

            return instance;
        }

        static void Main(string[] args) {
            if (!FindAssignmentClassFromParam(args, out string className))
                return;

            AssignmentBase? assignmentOne = GetAssignmentObject(className);
            assignmentOne?.Execute();
        }
    }
}
