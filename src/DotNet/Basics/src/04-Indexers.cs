using Basics;

namespace Basics {
    internal class Indexers {
        public Indexers() { Data = []; m_key = []; }

        public string this[int index] {
            get {
                if (index >= 0 && index < Data.Count)
                    return Data[index];
                return string.Empty;
            }
            set {
                if (index >= 0 && index < Data.Count)
                    Data[index] = value;
                Data.Add(value);
            }
        }

        public string this[string keyIndex] {
            get {
                if (m_key.TryGetValue(keyIndex, out int index))
                    return this[index - 1];
                return string.Empty;
            }
            set {
                if (m_key.TryGetValue(keyIndex, out int index)) {
                    this[index - 1] = value;
                }
                Data.Add(value);
                m_key.Add(keyIndex, Data.Count);
            }
        }

        public List<string> Data;
        private Dictionary<string, int> m_key;
        public Dictionary<string, int> Keys { get => m_key; }
    }
}

namespace Assignments {
    class Assignment4 : AssignmentBase {
        public Assignment4() : base("Indexers") { }
        public override void Body() {
            Indexers indx = new();
            indx[0] = "Hello";
            indx[1] = "World";

            indx["foo"] = "bar";
            indx["baz"] = "fizz";
            Console.WriteLine($"[ {String.Join(", ", indx.Data)} ]");
            Console.WriteLine();

            Console.WriteLine($"indx['foo'] = {indx["foo"]}, indx['baz'] = {indx["baz"]}");
            Console.WriteLine($"indx[0] = {indx[0]}, indx[1] = {indx[1]}, indx[3] = {indx[3]}");
        }
    }
}