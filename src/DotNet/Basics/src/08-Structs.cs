using Basics;

namespace Basics {
    enum EnumExample {
        None = 0,
        First,
        Second
    }

    struct StructExample(string StringData = "", EnumExample EnumType = EnumExample.None) {
        public string Data = StringData;
        public EnumExample EnumType = EnumType;
    }


    partial class PartialClass {
        public partial void Foo();

        public StructExample StructData = new();
    }

    partial class PartialClass {
        public string ExtString = "Partial class extension";

        public void Bar(StructExample Data) {
            Console.WriteLine("Data: " + Data.Data + ", Enum: " + Data.EnumType);
        }
        public partial void Foo() {
            Console.WriteLine("Partial function implementation");
        }
    }
}

namespace Assignments {
    class Assignment8 : AssignmentBase {
        public Assignment8() : base("Struct/Enums") { }
        public override void Body() {
            PartialClass partialClass = new PartialClass();
            partialClass.Foo();

            StructExample structExample;
            structExample.Data = "Struct Data";
            structExample.EnumType = EnumExample.First;

            partialClass.Bar(structExample);
        }
    }
}

