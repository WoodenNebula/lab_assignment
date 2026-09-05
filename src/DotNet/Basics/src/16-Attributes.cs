using Basics;

namespace Basics {
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method)]
    internal class AuthorAttribute(string name) : Attribute {
        // Positional 
        public string Name { get; } = name;
        // Named parameters
        public int Version { get; set; } = 1;
        public string Description { get; set; } = "";
    }

    [Serializable]
    [Obsolete("Use NewStudent instead.")]
    internal class OldStudent(string name) {
        public string Name { get; set; } = name;
    }

    [Author("Surab", Description = "New student class")]
    internal class NewStudent(string name) {
        public string Name { get; set; } = name;

        [Obsolete("Use DisplayNew() instead.")]
        public void Display() { Console.WriteLine($"Student: {Name}"); }
    }
}
namespace Assignments {
    class Assignment16() : AssignmentBase("Attributes") {
        public override void Body() {
            OldStudent oldStudent = new OldStudent("Deprecated");
            NewStudent newStudent = new NewStudent("Not Deprecated");
            newStudent.Display();
        }
    }
}
