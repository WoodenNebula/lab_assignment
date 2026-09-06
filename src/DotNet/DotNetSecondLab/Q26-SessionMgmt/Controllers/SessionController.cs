using Microsoft.AspNetCore.Mvc;

namespace Q26_SessionMgmt.Controllers {
    public class SessionController : Controller {
        public IActionResult SetName() => View();

        [HttpPost]
        public IActionResult SetName(string userName) {
            HttpContext.Session.SetString("UserName", userName);
            return RedirectToAction("ShowName");
        }

        public IActionResult ShowName() {
            string? name = HttpContext.Session.GetString("UserName");
            ViewBag.Name = name;
            return View();
        }
    }
}
