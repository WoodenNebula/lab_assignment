using Basics;

namespace Basics {
    internal class Teacher : Employee {
        public Teacher(string Name) : base(Name) { base.ID = 60; base.PrintID(); }

        public override void IncrementID() {
            Console.WriteLine($"[{this.Name}]: <Child IncrementID method invoked>");
            base.IncrementID();
        }

        public new void PrintID() {
            Console.WriteLine($"[{this.Name}]: <Child PrintID method invoked>");
        }
    }
}

namespace Assignments {
    internal class Assignment6 : AssignmentBase {

        public Assignment6() : base("Polymorphism") { }

        public override void Body() {

            Teacher teacher = new Teacher("Teacher");
            Employee employee = new Teacher("Teach");

            Console.WriteLine();
            Console.WriteLine("---Method Overriding---");
            teacher.IncrementID();

            Console.WriteLine();

            Console.WriteLine("---Method Hiding---");
            Console.WriteLine("Method from parent ref:");
            employee.PrintID();
            Console.WriteLine("Method from child ref:");
            teacher.PrintID();
        }
    }
}

