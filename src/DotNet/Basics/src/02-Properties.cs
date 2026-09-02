using Basics;

namespace Basics {
    internal class Properties {
        public Properties() {
            AutoProp = "Auto Property";
            ReadOnlyProp = "Readonly Property";
        }

        public string AutoProp { get; set; }
        public readonly string ReadOnlyProp;
    }
}

namespace Assignments {
    internal class Assignment2 : AssignmentBase {

        public Assignment2() : base("Properties") { }

        public override void Body() {
            Properties props = new();

            Console.WriteLine($"Get: {props.AutoProp}");
            props.AutoProp = "Modified";
            Console.WriteLine($"after Set: {props.AutoProp}");
            Console.WriteLine($"Get: {props.ReadOnlyProp}");
            // This is illegal
            // props.ReadOnlyProp = "Error";
        }
    }
}
