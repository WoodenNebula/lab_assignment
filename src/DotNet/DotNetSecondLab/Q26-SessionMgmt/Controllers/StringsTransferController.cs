using Microsoft.AspNetCore.Mvc;

namespace Q26_SessionMgmt.Controllers {
    public class StringsTransferController : Controller {
        public IActionResult PageOne() => View("PageOne");

        public IActionResult P2Query(string userName) {
            ViewBag.Name = userName;
            ViewBag.Method = "Query String";
            return View("PageTwo");
        }

        [HttpPost]
        public IActionResult P2Hidden(string userName) {
            ViewBag.Name = userName;
            ViewBag.Method = "Hidden Field";
            return View("PageTwo");
        }
    }
}

