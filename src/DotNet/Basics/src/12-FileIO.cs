namespace Assignments {
    class Assignment12 : AssignmentBase {
        public Assignment12() : base("File IO") { }
        public override void Body() {
            string filePath = "12-fileIO.txt";
            Console.WriteLine("Enter text to write to " + filePath + " ( enter `:q` to stop)");

            StreamWriter writer = new(filePath);
            string? lineInput;
            while ((lineInput = Console.ReadLine()) != null && lineInput != ":q") {
                writer.WriteLine(lineInput);
            }

            writer.Flush();
            writer.Close();
        }
    }
}
