using Basics;

namespace Basics {
    internal class ConstructorExample {
        public ConstructorExample() { Console.WriteLine("Default Constructor"); }
        public ConstructorExample(string Message) { Console.WriteLine("Parameterized Constructor: " + Message); }
        public ConstructorExample(ConstructorExample Other) { Console.WriteLine("Copy Constructor"); }
    }

    internal class ConstructorExampleTwo {
        private ConstructorExampleTwo() { Console.WriteLine("Private Constructor, cannot be invoked with obj = new"); }
        static ConstructorExampleTwo() { Console.WriteLine("Static Constructor"); }

        public static ConstructorExampleTwo Get() {
            if (m_instance == null)
                m_instance = new ConstructorExampleTwo();

            return m_instance;
        }

        private static ConstructorExampleTwo? m_instance;

    }
}


namespace Assignments {
    internal class Assignment1 : AssignmentBase {

        public Assignment1() : base("Constructors") { }

        public override void Body() {
            ConstructorExample defaultObj = new();
            ConstructorExample paramObj = new("Parameterized");
            ConstructorExample copiedObj = new(paramObj);

            // This is illegal
            // LanguagePreliminaries.ConstructorExampleTwo invalidObj = new();  

            ConstructorExampleTwo staticObj = ConstructorExampleTwo.Get();
        }
    }
}
