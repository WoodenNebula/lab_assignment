namespace Assignments {
    class Assignment17() : AssignmentBase("Async") {
        public override void Body() {
            ReadFileAsync().GetAwaiter().GetResult();
        }

        private async Task ReadFileAsync() {
            string fileName = "12-fileIO.txt";

            // Create a sample file if it doesn't exist
            if (!File.Exists(fileName)) {
                await File.WriteAllTextAsync(
                    fileName,
                    "Hello from asynchronous file reading!"
                );
            }

            Console.WriteLine("Reading file asynchronously...");

            using (StreamReader reader = new StreamReader(fileName)) {
                string content = await reader.ReadToEndAsync();

                Console.WriteLine("\nFile contents:");
                Console.WriteLine(content);
            }

            Console.WriteLine("\nFile reading completed.");
        }
    }
}
