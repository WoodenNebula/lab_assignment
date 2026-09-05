namespace Q20_DI.Services {
    public class TransientService : ITransientService {
        public Guid ServiceId { get; }

        public TransientService() {
            ServiceId = Guid.NewGuid();
        }

        public string GetInfo() {
            return $"Transient Service ID: {ServiceId}";
        }
    }
}
