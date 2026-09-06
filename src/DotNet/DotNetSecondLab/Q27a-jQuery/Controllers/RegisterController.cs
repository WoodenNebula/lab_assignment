using Microsoft.AspNetCore.Mvc;
using Q27a_jQuery.Models;

namespace Q27a_jQuery.Controllers {
    public class RegisterController : Controller {
        public IActionResult Index() => View(new RegisterViewModel());

        [HttpPost]
        public IActionResult Index(RegisterViewModel model) {
            if (!ModelState.IsValid) {
                return View(model);
            }

            ViewBag.Success = true;
            return View(model);
        }
    }
}
