namespace Assignments {
    internal class Assignment3 : AssignmentBase {

        public Assignment3() : base("Jagged Arrays") { }

        public override void Body() {
            int[][] jaggedArray = [
                [ 1, 2, 3, 4, 5, 6 ],
                [ 1, 2, 3 ],
                [ 1, 2, 3, 4, 5, 6, 7 ],
                [ 1 ],
                [ 1 ]
            ];

            Console.WriteLine("Jagged Array:");
            foreach (int[] arr in jaggedArray) {
                Console.WriteLine($"[ {string.Join(", ", arr)} ], ");
            }
        }
    }
}
