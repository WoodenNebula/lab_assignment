using Basics;

namespace Basics {
    // ==================================================
    // Abstract Class
    // ==================================================
    internal abstract class Animal {
        public abstract void Speak();

        public void Sleep() { Console.WriteLine("Sleeping!"); }
    }

    internal class Dog : Animal {
        public override void Speak() { Console.WriteLine("Bark"); }
    }


    // ==================================================
    // Interface
    // ==================================================
    internal interface IPrintable {
        void Print();
    }

    internal class Document : IPrintable {
        public void Print() { Console.WriteLine("Invoked interface method"); }
    }


    // ==================================================
    // Multiple Inheritance using Interfaces
    // ==================================================
    internal interface ICamera {
        void TakePhoto();
    }

    internal interface IMusicPlayer {
        void PlayMusic();
    }

    internal class Phone : ICamera, IMusicPlayer {
        public void TakePhoto() { Console.WriteLine("Taking a photo"); }
        public void PlayMusic() { Console.WriteLine("Playing music"); }
    }

    internal sealed class FinalClass {
        public void Display() { Console.WriteLine("This is a sealed class, cannot create any child from it"); }
    }


}


namespace Assignments {
    class Assignment7 : AssignmentBase {
        public Assignment7() : base("More Classes") { }
        public override void Body() {
            // ERROR
            // Animal animal = new Animal();

            Console.WriteLine("---Abstract Class---");
            Animal dog = new Dog();
            dog.Speak();
            dog.Sleep();

            Console.WriteLine();

            Console.WriteLine("---Interface---");
            IPrintable document = new Document();
            document.Print();

            Console.WriteLine();

            Console.WriteLine("---Multi-Inheritance---");
            Phone phone = new Phone();
            phone.TakePhoto();
            phone.PlayMusic();

            Console.WriteLine();

            Console.WriteLine("---Sealed Class---");
            FinalClass obj = new FinalClass();
            obj.Display();
        }
    }
}
