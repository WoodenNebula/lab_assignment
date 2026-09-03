using Basics;

namespace Basics {
    public delegate void LogDelegate(string message);

    internal class Publisher {
        public event LogDelegate? OnEventOccurred;

        public void TriggerEventExternal(string message) {
            OnEventOccurred?.Invoke(message);
        }
    }

    internal class Subscriber {
        static void LogDelegateHandlerOne(string msg) { Console.WriteLine($"[FIRST HANDLER]: '{msg}'"); }
        static void LogDelegateHandlerTwo(string msg) { Console.WriteLine($"[SECOND HANDLER]: '{msg}'"); }

        public Subscriber(Publisher publisher) {
            // Create delegates of type LogDelegate to subscribe to event
            LogDelegate HandlerOne = LogDelegateHandlerOne;
            LogDelegate HandlerTwo = LogDelegateHandlerTwo;

            LogDelegate MultiDelegate = HandlerOne;
            MultiDelegate += HandlerTwo;

            MultiDelegate += delegate (string msg) {
                Console.WriteLine($"[ANNONYMOUS HANDLER]: '{msg}'");
            };


            // Subscribe to event with the multicast delegate
            // will trigger all 3 methods
            publisher.OnEventOccurred += MultiDelegate;

            // remove handlerTwo
            publisher.OnEventOccurred -= HandlerTwo;
            publisher.OnEventOccurred += ActionHandler;
        }

        public static int FuncHandler(int x, int y) { Console.WriteLine($"[FUNC HANDLER]: Func invoked with param {x}, {y} -> {x * y}"); return x * y; }
        public static void ActionHandler(string msg) { Console.WriteLine($"[ACTION HANDLER]: '{msg}'"); }
    }
}

namespace Assignments {
    class Assignment9 : AssignmentBase {
        public Assignment9() : base("Events") { }
        public override void Body() {
            Action<string> HandleAction = Subscriber.ActionHandler;
            HandleAction("Action invoked");

            Func<int, int, int> HandleFunc = Subscriber.FuncHandler;
            HandleFunc(6, 9);

            Console.WriteLine("=========");

            Publisher publisher = new();

            Subscriber subscriber = new Subscriber(publisher);

            publisher.TriggerEventExternal("Event triggerred");
        }
    }
}
