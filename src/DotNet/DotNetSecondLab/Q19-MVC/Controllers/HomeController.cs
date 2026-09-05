using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Q19_MVC.Models;

namespace Q19_MVC.Controllers {
    public class HomeController : Controller {
        private static StudentModel currentStudent = null;

        public IActionResult Index() {
            var rollNo = 34;
            var multiplicationTableNumber = rollNo + 1;
            ViewBag.CurrentDateTime = DateTime.Now;
            ViewBag.StudentName = "Surab Parajuli";
            ViewBag.RollNumber = rollNo;
            ViewBag.MultiplicationTableNumber = multiplicationTableNumber;
            ViewBag.MultiplicationTable = GenerateMultiplicationTable(multiplicationTableNumber);
            return View();
        }

        public IActionResult StudentForm() {
            return View(new StudentModel());
        }

        [HttpPost]
        public IActionResult StudentForm(StudentModel student) {
            if (ModelState.IsValid) {
                currentStudent = student;
                return RedirectToAction("Details");
            }
            return View(student);
        }

        public IActionResult Details() {
            if (currentStudent == null) {
                return RedirectToAction("StudentForm");
            }
            return View(currentStudent);
        }

        public IActionResult Privacy() {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error() {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }

        private List<(int, int)> GenerateMultiplicationTable(int number) {
            var table = new List<(int, int)>();
            for (int i = 1; i <= 10; i++) {
                table.Add((i, i * number));
            }
            return table;
        }
    }
}
