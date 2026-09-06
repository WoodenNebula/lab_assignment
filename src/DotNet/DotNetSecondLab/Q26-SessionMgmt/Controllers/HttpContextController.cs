using Microsoft.AspNetCore.Mvc;

namespace Q26_SessionMgmt.Controllers {
    public class HttpContextController : Controller {
        public IActionResult Index() {
            ViewBag.Path = HttpContext.Request.Path;
            ViewBag.Method = HttpContext.Request.Method;
            ViewBag.UserAgent = HttpContext.Request.Headers["User-Agent"].ToString();
            ViewBag.RemoteIp = HttpContext.Connection.RemoteIpAddress?.ToString();
            return View();
        }
    }
}
