namespace Q20_DI.Services {
    public interface IScopedService {
        Guid ServiceId { get; }
        string GetInfo();
    }
}
