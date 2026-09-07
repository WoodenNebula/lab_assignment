using Microsoft.AspNetCore.Mvc;

namespace Q28_Security.Controllers {
    public class XssDemoController : Controller {
        public IActionResult Index() => View();

        [HttpPost]
        public IActionResult Index(string userInput) {
            ViewBag.UserInput = userInput;
            return View();
        }
    }
}
