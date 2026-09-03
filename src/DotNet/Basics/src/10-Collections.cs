using System.Collections;

namespace Assignments {
    class Assignment10 : AssignmentBase {
        public Assignment10() : base("Collections") { }
        public override void Body() {
            Console.WriteLine("---NonGeneric Collection---");
            ArrayList nonGenericList = new ArrayList();

            // everything is wrapped and stored as object
            nonGenericList.Add(108);
            // reference type is stored directly as reference
            nonGenericList.Add("Warrior");
            nonGenericList.Add('A');

            foreach (object item in nonGenericList) {
                if (item is int) {
                    // unboxing/casting
                    int number = (int)item;
                    Console.WriteLine($" Integer (Unboxed): {number}");
                }
                else {
                    Console.WriteLine($" Object (Reference): {item}");
                }
            }
            Console.WriteLine();

            Console.WriteLine("---Generic Collection---");

            List<int> genericList = new List<int>() { 100, 200, 300 };
            genericList.Add(400);

            Console.WriteLine($"[ {string.Join(", ", genericList)} ]");
        }
    }
}
