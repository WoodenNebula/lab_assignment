namespace Assignments {
    internal abstract class AssignmentBase {
        public string Title;

        public AssignmentBase(string? inTitle) {
            inTitle ??= this.GetType().Name;
            this.Title = inTitle;
        }

        // Overriden by assignment driver code
        public abstract void Body();

        public void Execute() {
            Header(Title);
            Body();
            Footer();
        }

        public static void Header(string Title) {
            Console.WriteLine("=== " + Title.ToUpper() + " ===");
        }

        public static void Footer() {
            Console.WriteLine();
            Console.WriteLine("====================");
            Console.WriteLine("Surab Parajuli");
            Console.WriteLine("Section: A, 6th-Sem");
            Console.WriteLine("Roll: 34, Symbol No.: 80010139");
            Console.WriteLine("====================");
        }

    }
}
