using Microsoft.AspNetCore.Mvc;

namespace Q26_SessionMgmt.Controllers {
    public class CookieController : Controller {
        public IActionResult Index() {
            ViewBag.CookieValue = Request.Cookies["VisitorName"];
            return View();
        }

        [HttpPost]
        public IActionResult SetCookie(string visitorName) {
            var options = new CookieOptions { Expires = DateTimeOffset.Now.AddDays(7) };
            Response.Cookies.Append("VisitorName", visitorName, options);
            return RedirectToAction("Index");
        }
    }
}
