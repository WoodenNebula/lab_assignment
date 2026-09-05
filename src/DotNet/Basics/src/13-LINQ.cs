namespace Assignments {
    class Assignment13() : AssignmentBase("LINQ") {
        public override void Body() {
            // --- Set up our Data Source ---
            int[] numbers = { 15, 22, 8, 45, 90, 13, 67, 30 };

            var res = from num in numbers
                      where num % 2 == 0
                      select num;
            var resSorted = from num in res
                            orderby num ascending
                            select num;


            Console.WriteLine($"Even selection: [ {string.Join(", ", res)} ]");
            Console.WriteLine($"Even selection(Sorted): [ {string.Join(", ", resSorted)} ]");
        }
    }
}
