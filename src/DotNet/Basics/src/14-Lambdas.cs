namespace Assignments {
    class Assignment14() : AssignmentBase("Lambda") {
        struct Student {
            public required string Name { get; set; }
            public int Marks { get; set; }
        }
        public override void Body() {
            // Expression Lambda
            Func<int, int> funcSquareOf = x => x * x;
            Console.WriteLine($"Expression Lambda - Square(8): {funcSquareOf(8)}");

            // Statement Lambda
            Func<int, bool> funcIsEven = x => {
                int remainder = x % 2;
                return (remainder == 0);
            };
            Console.WriteLine($"Statement Lambda - IsEvent(22): {funcIsEven(22)}");
            Console.WriteLine();


            // LINQ with lambda
            List<Student> students = new List<Student> {
                new Student { Name = "Ram", Marks = 78 },
                new Student { Name = "Sita", Marks = 95 },
                new Student { Name = "Hari", Marks = 42 },
                new Student { Name = "Gita", Marks = 88 }
            };

            var highScorers = students
                .Where(student => student.Marks >= 60)
                .OrderByDescending(student => student.Marks);

            Console.WriteLine("Students scoring 60 or above (Sorted):");
            foreach (var student in highScorers) {
                Console.WriteLine($"   {student.Name} -> {student.Marks}");
            }
        }
    }
}
