using Basics;

namespace Basics {
    internal class GenericClass<T> {
        private T m_genericData;

        public GenericClass(T data) {
            m_genericData = data;
            Console.WriteLine($"Instantiated with type '{typeof(T)}'");
        }

        public T Prop {
            get => m_genericData;
            set => m_genericData = value;
        }

        public string Concat<U>(U valA, U valB) {
            return $"{valA?.ToString()}{valB?.ToString()}";
        }
    }
}

namespace Assignments {
    class Assignment11 : AssignmentBase {
        public Assignment11() : base("Templates") { }
        public override void Body() {
            GenericClass<int> intClass = new GenericClass<int>(69);
            Console.WriteLine($"Property = {intClass.Prop}");
            Console.WriteLine($"Method as string = '{intClass.Concat("foo", "bar")}'");
            Console.WriteLine($"Method as int = '{intClass.Concat(6, 9)}'");

            Console.WriteLine();
            GenericClass<string> stringClass = new GenericClass<string>("Generic String");
            Console.WriteLine($"Property = {stringClass.Prop}");
            Console.WriteLine($"Method as string = '{stringClass.Concat("foo", "bar")}'");
            Console.WriteLine($"Method as int = '{stringClass.Concat(6, 9)}'");
        }
    }
}
