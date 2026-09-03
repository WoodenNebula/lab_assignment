using Basics;

namespace Basics {
    internal class Employee {

        public Employee(string Name) { Console.WriteLine("<Base Constructor invoked>"); this.ID = -1; this.Name = Name; }
        public virtual void IncrementID() { Console.WriteLine("<Base IncrementID method invoked>"); this.ID++; }
        public void PrintID() { Console.WriteLine("<Base PrintID method invoked>"); Console.WriteLine($"[{this.Name}]: " + this.ID); }

        protected int ID;
        protected string Name;
    }

    internal class Engineer : Employee {
        public Engineer(string Name) : base(Name) { base.ID = 10; base.PrintID(); }

        public override void IncrementID() {
            Console.WriteLine($"[{this.Name}]: Previous ID: " + base.ID);
            base.IncrementID();
            Console.WriteLine($"[{this.Name}]: After increment: " + base.ID);
        }
    }


    internal class Doctor : Employee {
        public Doctor(string Name) : base(Name) { base.ID = 20; base.PrintID(); }

        public void Register() { Console.WriteLine($"[{this.Name}]: Registered with ID: " + this.ID); }

        public override void IncrementID() {
            Console.WriteLine($"[{this.Name}]: Previous ID: " + base.ID);
            base.IncrementID();
            Console.WriteLine($"[{this.Name}]: After increment: " + base.ID);
        }
    }
}

namespace Assignments {
    internal class Assignment5 : AssignmentBase {

        public Assignment5() : base("Base Class Action") { }

        public override void Body() {

            Employee engineer = new Engineer("Engineer");
            engineer.IncrementID();

            Doctor doctor = new Doctor("Doctor");
            doctor.IncrementID();
            doctor.Register();
        }
    }
}
