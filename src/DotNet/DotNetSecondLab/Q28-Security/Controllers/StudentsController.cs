using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

using Q28_Security.Models;

namespace Q28_Security.Controllers {
    [Authorize]
    public class StudentsController : Controller {
        private readonly ApplicationDbContext m_context;

        public StudentsController(ApplicationDbContext context) {
            m_context = context;
        }

        public async Task<IActionResult> Index() {
            return View(await m_context.Students.ToListAsync());
        }

        public async Task<IActionResult> Details(int id) {
            var student = await m_context.Students.FindAsync(id);
            if (student == null)
                return NotFound();
            return View(student);
        }

        [Authorize(Roles = "Admin")]
        public IActionResult Create() => View(new Student());

        [Authorize(Roles = "Admin")]
        [HttpPost]
        public async Task<IActionResult> Create(Student student) {
            if (!ModelState.IsValid)
                return View(student);
            m_context.Add(student);
            await m_context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        [Authorize(Roles = "Admin")]
        public async Task<IActionResult> Edit(int id) {
            var student = await m_context.Students.FindAsync(id);
            if (student == null)
                return NotFound();
            return View(student);
        }

        [Authorize(Roles = "Admin")]
        [HttpPost]
        public async Task<IActionResult> Edit(Student student) {
            if (!ModelState.IsValid)
                return View(student);
            m_context.Update(student);
            await m_context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        [Authorize(Roles = "Admin")]
        public async Task<IActionResult> Delete(int id) {
            var student = await m_context.Students.FindAsync(id);
            if (student == null)
                return NotFound();
            return View(student);
        }

        [Authorize(Roles = "Admin"), HttpPost, ActionName("Delete")]
        public async Task<IActionResult> DeleteConfirmed(int id) {
            var student = await m_context.Students.FindAsync(id);
            m_context.Students.Remove(student!);
            await m_context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        [Authorize(Policy = "CsitDepartmentOnly")]
        public IActionResult DepartmentReport() {
            return Content("Visible only to users with a Department=CSIT claim.");
        }
    }
}

