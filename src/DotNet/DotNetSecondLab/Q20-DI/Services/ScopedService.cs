namespace Q20_DI.Services {
    public class ScopedService : IScopedService {
        public Guid ServiceId { get; }

        public ScopedService() {
            ServiceId = Guid.NewGuid();
        }

        public string GetInfo() {
            return $"Scoped Service ID: {ServiceId}";
        }
    }
}
