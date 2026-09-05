using Basics;

namespace Basics {

    class CustomExceptionExample : Exception {
        public CustomExceptionExample(string message) : base(message) { }
    }
}
namespace Assignments {
    class Assignment15() : AssignmentBase("Exceptions") {
        public override void Body() {
            // try, catch and finally, throw custom exception
            try {
                int number = 0;
                Console.WriteLine($"Calculating: 100 / {number}");
                if (number == 0) {
                    throw new CustomExceptionExample("Division by zero");
                }
            }
            catch (CustomExceptionExample e) {
                Console.WriteLine("Caught custom exception : " + e.Message);
            }
            finally {
                Console.WriteLine("Finally block executed.");
            }
        }
    }
}
