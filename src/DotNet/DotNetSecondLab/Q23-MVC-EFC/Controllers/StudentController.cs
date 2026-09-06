using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Q23_MVC_EFC.Data;
using Q23_MVC_EFC.Models;

namespace Q23_MVC_EFC.Controllers {
    public class StudentController : Controller {
        private readonly AppDbContext _context;
        private readonly ILogger<StudentController> _logger;

        public StudentController(AppDbContext context, ILogger<StudentController> logger) {
            _context = context;
            _logger = logger;
        }

        // GET: Student
        public async Task<IActionResult> Index() {
            try {
                var students = await _context.Students.ToListAsync();
                return View(students);
            }
            catch (Exception ex) {
                _logger.LogError(ex, "Error retrieving students");
                return RedirectToAction(nameof(Index), new { errorMessage = "Error retrieving students" });
            }
        }

        // GET: Student/Details/5
        public async Task<IActionResult> Details(int? id) {
            if (id == null) {
                return NotFound();
            }

            try {
                var student = await _context.Students.FirstOrDefaultAsync(m => m.StudentId == id);
                if (student == null) {
                    return NotFound();
                }

                return View(student);
            }
            catch (Exception ex) {
                _logger.LogError(ex, "Error retrieving student details");
                return RedirectToAction(nameof(Index));
            }
        }

        // GET: Student/Create
        public IActionResult Create() {
            return View();
        }

        // POST: Student/Create
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("StudentId,FirstName,LastName,Email,Phone,DateOfBirth,Course,GPA,Remarks,EnrollmentDate")] Student student) {
            if (ModelState.IsValid) {
                try {
                    _context.Add(student);
                    await _context.SaveChangesAsync();
                    return RedirectToAction(nameof(Index));
                }
                catch (Exception ex) {
                    _logger.LogError(ex, "Error creating student");
                    ModelState.AddModelError("", "Error creating student. Please try again.");
                }
            }
            return View(student);
        }

        // GET: Student/Edit/5
        public async Task<IActionResult> Edit(int? id) {
            if (id == null) {
                return NotFound();
            }

            try {
                var student = await _context.Students.FindAsync(id);
                if (student == null) {
                    return NotFound();
                }
                return View(student);
            }
            catch (Exception ex) {
                _logger.LogError(ex, "Error retrieving student for edit");
                return RedirectToAction(nameof(Index));
            }
        }

        // POST: Student/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("StudentId,FirstName,LastName,Email,Phone,DateOfBirth,Course,GPA,Remarks,EnrollmentDate")] Student student) {
            if (id != student.StudentId) {
                return NotFound();
            }

            if (ModelState.IsValid) {
                try {
                    _context.Update(student);
                    await _context.SaveChangesAsync();
                    return RedirectToAction(nameof(Index));
                }
                catch (DbUpdateConcurrencyException ex) {
                    if (!StudentExists(student.StudentId)) {
                        return NotFound();
                    }
                    else {
                        _logger.LogError(ex, "Concurrency error updating student");
                        throw;
                    }
                }
                catch (Exception ex) {
                    _logger.LogError(ex, "Error updating student");
                    ModelState.AddModelError("", "Error updating student. Please try again.");
                }
            }
            return View(student);
        }

        // GET: Student/Delete/5
        public async Task<IActionResult> Delete(int? id) {
            if (id == null) {
                return NotFound();
            }

            try {
                var student = await _context.Students.FirstOrDefaultAsync(m => m.StudentId == id);
                if (student == null) {
                    return NotFound();
                }

                return View(student);
            }
            catch (Exception ex) {
                _logger.LogError(ex, "Error retrieving student for delete");
                return RedirectToAction(nameof(Index));
            }
        }

        // POST: Student/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id) {
            try {
                var student = await _context.Students.FindAsync(id);
                if (student != null) {
                    _context.Students.Remove(student);
                    await _context.SaveChangesAsync();
                }
                return RedirectToAction(nameof(Index));
            }
            catch (Exception ex) {
                _logger.LogError(ex, "Error deleting student");
                return RedirectToAction(nameof(Index));
            }
        }

        private bool StudentExists(int id) {
            return _context.Students.Any(e => e.StudentId == id);
        }
    }
}
