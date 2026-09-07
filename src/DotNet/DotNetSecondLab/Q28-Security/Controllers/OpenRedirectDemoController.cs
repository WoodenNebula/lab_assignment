using Microsoft.AspNetCore.Mvc;

namespace Q28_Security.Controllers {
    public class OpenRedirectDemoController : Controller {
        public IActionResult Index() => View();

        // Intentionally vulnerable — redirects to any URL without validation, educational demo only
        public IActionResult VulnerableRedirect(string returnUrl) {
            return Redirect(returnUrl);
        }

        // Secure equivalent — only allows local URLs
        public IActionResult SafeRedirect(string returnUrl) {
            if (Url.IsLocalUrl(returnUrl)) {
                return Redirect(returnUrl);
            }

            return RedirectToAction("Index", "Home");
        }
    }
}
