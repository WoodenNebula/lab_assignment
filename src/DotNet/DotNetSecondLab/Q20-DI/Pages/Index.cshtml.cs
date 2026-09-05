using Microsoft.AspNetCore.Mvc.RazorPages;
using Q20_DI.Services;

namespace Q20_DI.Pages {
    public class IndexModel : PageModel {
        private readonly ITransientService _transient1;
        private readonly ITransientService _transient2;
        private readonly IScopedService _scoped1;
        private readonly IScopedService _scoped2;
        private readonly ISingletonService _singleton1;
        private readonly ISingletonService _singleton2;

        public string TransientInfo1 { get; set; }
        public string TransientInfo2 { get; set; }
        public string ScopedInfo1 { get; set; }
        public string ScopedInfo2 { get; set; }
        public string SingletonInfo1 { get; set; }
        public string SingletonInfo2 { get; set; }
        public bool TransientSame { get; set; }
        public bool ScopedSame { get; set; }
        public bool SingletonSame { get; set; }

        public IndexModel(
            ITransientService transient1,
            ITransientService transient2,
            IScopedService scoped1,
            IScopedService scoped2,
            ISingletonService singleton1,
            ISingletonService singleton2) {
            _transient1 = transient1;
            _transient2 = transient2;
            _scoped1 = scoped1;
            _scoped2 = scoped2;
            _singleton1 = singleton1;
            _singleton2 = singleton2;
        }

        public void OnGet() {
            TransientInfo1 = _transient1.GetInfo();
            TransientInfo2 = _transient2.GetInfo();
            ScopedInfo1 = _scoped1.GetInfo();
            ScopedInfo2 = _scoped2.GetInfo();
            SingletonInfo1 = _singleton1.GetInfo();
            SingletonInfo2 = _singleton2.GetInfo();

            TransientSame = _transient1.ServiceId == _transient2.ServiceId;
            ScopedSame = _scoped1.ServiceId == _scoped2.ServiceId;
            SingletonSame = _singleton1.ServiceId == _singleton2.ServiceId;
        }
    }
}
