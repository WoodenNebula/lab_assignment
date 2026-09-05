namespace Q20_DI.Services {
    public class SingletonService : ISingletonService {
        public Guid ServiceId { get; }

        public SingletonService() {
            ServiceId = Guid.NewGuid();
        }

        public string GetInfo() {
            return $"Singleton Service ID: {ServiceId}";
        }
    }
}
