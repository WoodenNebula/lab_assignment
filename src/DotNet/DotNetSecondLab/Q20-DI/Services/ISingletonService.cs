namespace Q20_DI.Services {
    public interface ISingletonService {
        Guid ServiceId { get; }
        string GetInfo();
    }
}
