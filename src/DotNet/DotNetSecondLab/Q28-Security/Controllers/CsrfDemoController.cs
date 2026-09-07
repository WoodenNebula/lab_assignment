using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

namespace Q28_Security.Controllers {

    [Authorize]
    public class CsrfDemoController : Controller {
        public IActionResult Index() => View();

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult ChangeName(string newName) {
            ViewBag.Message = $"Name changed to: {newName} (request had a valid token)";
            return View("Index");
        }

        // Intentionally vulnerable — antiforgery check disabled
        [HttpPost]
        [IgnoreAntiforgeryToken]
        public IActionResult ChangeNameUnsafe(string newName) {
            ViewBag.Message = $"Name changed to: {newName} (NO token check — vulnerable demo)";
            return View("Index");
        }
    }
}
