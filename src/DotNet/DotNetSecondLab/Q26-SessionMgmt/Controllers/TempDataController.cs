using Microsoft.AspNetCore.Mvc;

namespace Q26_SessionMgmt.Controllers {
    public class TempDataController : Controller {
        public IActionResult Index() => View();

        [HttpPost]
        public IActionResult SetMessage(string message) {
            TempData["Message"] = message;
            return RedirectToAction("ShowMessage");
        }

        public IActionResult ShowMessage() {
            ViewBag.Message = TempData["Message"];
            return View();
        }
    }
}
