using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Q25_WebAPI_EF.Data;
using Q25_WebAPI_EF.Models;

namespace Q23_MVC_EFC.Controllers {
    [ApiController]
    [Route("api/[controller]")]
    public class StudentController : Controller {
        private readonly AppDbContext _context;
        private readonly ILogger<StudentController> _logger;

        public StudentController(AppDbContext context, ILogger<StudentController> logger) {
            _context = context;
            _logger = logger;
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<Student>>> GetAll() {
            return Ok(await _context.Students.ToListAsync());
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<Student>> GetById(int id) {
            var student = await _context.Students.FindAsync(id);
            if (student == null)
                return NotFound();
            return Ok(student);
        }

        [HttpPost]
        public async Task<ActionResult<Student>> Create(Student student) {
            _context.Students.Add(student);
            await _context.SaveChangesAsync();
            return CreatedAtAction(nameof(GetById), new { id = student.StudentId }, student);
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> Update(int id, Student student) {
            if (id != student.StudentId)
                return BadRequest();

            bool exists = await _context.Students.AnyAsync(s => s.StudentId == id);
            if (!exists)
                return NotFound();

            _context.Entry(student).State = EntityState.Modified;
            await _context.SaveChangesAsync();
            return NoContent();
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> Delete(int id) {
            var student = await _context.Students.FindAsync(id);
            if (student == null)
                return NotFound();

            _context.Students.Remove(student);
            await _context.SaveChangesAsync();
            return NoContent();
        }
    }
}
