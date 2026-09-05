namespace Q20_DI.Services {
    public interface ITransientService {
        Guid ServiceId { get; }
        string GetInfo();
    }
}
