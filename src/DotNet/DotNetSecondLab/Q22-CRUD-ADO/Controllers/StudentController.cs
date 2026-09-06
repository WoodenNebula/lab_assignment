using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.SqlClient;
using Q22_CRUD_ADO.Models;

namespace Q22_CRUD_ADO.Controllers {
    public class StudentController : Controller {
        private readonly IConfiguration _configuration;
        private readonly string _connectionString;

        public StudentController(IConfiguration configuration) {
            _configuration = configuration;
            _connectionString = _configuration.GetConnectionString("DefaultConnection") ?? "";
        }

        // READ - GET: Student/Index
        public IActionResult Index() {
            var students = new List<Student>();

            try {
                using (SqlConnection connection = new SqlConnection(_connectionString)) {
                    connection.Open();

                    string query = "SELECT StdID, Name, Address, Faculty, Email FROM Student ORDER BY StdID";

                    using (SqlCommand command = new SqlCommand(query, connection)) {
                        using (SqlDataReader reader = command.ExecuteReader()) {
                            while (reader.Read()) {
                                students.Add(new Student {
                                    StdID = (int)reader["StdID"],
                                    Name = reader["Name"].ToString() ?? "",
                                    Address = reader["Address"].ToString() ?? "",
                                    Faculty = reader["Faculty"].ToString() ?? "",
                                    Email = reader["Email"].ToString() ?? ""
                                });
                            }
                        }
                    }
                }
            } catch (SqlException ex) {
                ViewBag.ErrorMessage = $"Database error: {ex.Message}";
            } catch (Exception ex) {
                ViewBag.ErrorMessage = $"Error: {ex.Message}";
            }

            return View(students);
        }

        // CREATE - GET: Student/Create
        public IActionResult Create() {
            return View();
        }

        // CREATE - POST: Student/Create
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Create(Student student) {
            if (!ModelState.IsValid) {
                return View(student);
            }

            try {
                using (SqlConnection connection = new SqlConnection(_connectionString)) {
                    connection.Open();

                    string query = @"
                        INSERT INTO Student (StdID, Name, Address, Faculty, Email)
                        VALUES (@StdID, @Name, @Address, @Faculty, @Email)";

                    using (SqlCommand command = new SqlCommand(query, connection)) {
                        command.Parameters.AddWithValue("@StdID", student.StdID);
                        command.Parameters.AddWithValue("@Name", student.Name);
                        command.Parameters.AddWithValue("@Address", student.Address);
                        command.Parameters.AddWithValue("@Faculty", student.Faculty);
                        command.Parameters.AddWithValue("@Email", student.Email);

                        command.ExecuteNonQuery();
                    }
                }

                return RedirectToAction(nameof(Index));
            } catch (SqlException ex) when (ex.Number == 2627) {
                ModelState.AddModelError("StdID", "A student with this ID already exists.");
                return View(student);
            } catch (SqlException ex) {
                ModelState.AddModelError("", $"Database error: {ex.Message}");
                return View(student);
            } catch (Exception ex) {
                ModelState.AddModelError("", $"Error: {ex.Message}");
                return View(student);
            }
        }

        // READ - GET: Student/Details/5
        public IActionResult Details(int? id) {
            if (id == null) {
                return NotFound();
            }

            Student? student = null;

            try {
                using (SqlConnection connection = new SqlConnection(_connectionString)) {
                    connection.Open();

                    string query = "SELECT StdID, Name, Address, Faculty, Email FROM Student WHERE StdID = @StdID";

                    using (SqlCommand command = new SqlCommand(query, connection)) {
                        command.Parameters.AddWithValue("@StdID", id);

                        using (SqlDataReader reader = command.ExecuteReader()) {
                            if (reader.Read()) {
                                student = new Student {
                                    StdID = (int)reader["StdID"],
                                    Name = reader["Name"].ToString() ?? "",
                                    Address = reader["Address"].ToString() ?? "",
                                    Faculty = reader["Faculty"].ToString() ?? "",
                                    Email = reader["Email"].ToString() ?? ""
                                };
                            }
                        }
                    }
                }
            } catch (SqlException ex) {
                ViewBag.ErrorMessage = $"Database error: {ex.Message}";
            } catch (Exception ex) {
                ViewBag.ErrorMessage = $"Error: {ex.Message}";
            }

            if (student == null) {
                return NotFound();
            }

            return View(student);
        }

        // UPDATE - GET: Student/Edit/5
        public IActionResult Edit(int? id) {
            if (id == null) {
                return NotFound();
            }

            Student? student = null;

            try {
                using (SqlConnection connection = new SqlConnection(_connectionString)) {
                    connection.Open();

                    string query = "SELECT StdID, Name, Address, Faculty, Email FROM Student WHERE StdID = @StdID";

                    using (SqlCommand command = new SqlCommand(query, connection)) {
                        command.Parameters.AddWithValue("@StdID", id);

                        using (SqlDataReader reader = command.ExecuteReader()) {
                            if (reader.Read()) {
                                student = new Student {
                                    StdID = (int)reader["StdID"],
                                    Name = reader["Name"].ToString() ?? "",
                                    Address = reader["Address"].ToString() ?? "",
                                    Faculty = reader["Faculty"].ToString() ?? "",
                                    Email = reader["Email"].ToString() ?? ""
                                };
                            }
                        }
                    }
                }
            } catch (SqlException ex) {
                ViewBag.ErrorMessage = $"Database error: {ex.Message}";
            } catch (Exception ex) {
                ViewBag.ErrorMessage = $"Error: {ex.Message}";
            }

            if (student == null) {
                return NotFound();
            }

            return View(student);
        }

        // UPDATE - POST: Student/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Edit(int id, Student student) {
            if (id != student.StdID) {
                return NotFound();
            }

            if (!ModelState.IsValid) {
                return View(student);
            }

            try {
                using (SqlConnection connection = new SqlConnection(_connectionString)) {
                    connection.Open();

                    string query = @"
                        UPDATE Student
                        SET Name = @Name, Address = @Address, Faculty = @Faculty, Email = @Email
                        WHERE StdID = @StdID";

                    using (SqlCommand command = new SqlCommand(query, connection)) {
                        command.Parameters.AddWithValue("@StdID", student.StdID);
                        command.Parameters.AddWithValue("@Name", student.Name);
                        command.Parameters.AddWithValue("@Address", student.Address);
                        command.Parameters.AddWithValue("@Faculty", student.Faculty);
                        command.Parameters.AddWithValue("@Email", student.Email);

                        int rowsAffected = command.ExecuteNonQuery();
                        if (rowsAffected == 0) {
                            return NotFound();
                        }
                    }
                }

                return RedirectToAction(nameof(Index));
            } catch (SqlException ex) {
                ModelState.AddModelError("", $"Database error: {ex.Message}");
                return View(student);
            } catch (Exception ex) {
                ModelState.AddModelError("", $"Error: {ex.Message}");
                return View(student);
            }
        }

        // DELETE - GET: Student/Delete/5
        public IActionResult Delete(int? id) {
            if (id == null) {
                return NotFound();
            }

            Student? student = null;

            try {
                using (SqlConnection connection = new SqlConnection(_connectionString)) {
                    connection.Open();

                    string query = "SELECT StdID, Name, Address, Faculty, Email FROM Student WHERE StdID = @StdID";

                    using (SqlCommand command = new SqlCommand(query, connection)) {
                        command.Parameters.AddWithValue("@StdID", id);

                        using (SqlDataReader reader = command.ExecuteReader()) {
                            if (reader.Read()) {
                                student = new Student {
                                    StdID = (int)reader["StdID"],
                                    Name = reader["Name"].ToString() ?? "",
                                    Address = reader["Address"].ToString() ?? "",
                                    Faculty = reader["Faculty"].ToString() ?? "",
                                    Email = reader["Email"].ToString() ?? ""
                                };
                            }
                        }
                    }
                }
            } catch (SqlException ex) {
                ViewBag.ErrorMessage = $"Database error: {ex.Message}";
            } catch (Exception ex) {
                ViewBag.ErrorMessage = $"Error: {ex.Message}";
            }

            if (student == null) {
                return NotFound();
            }

            return View(student);
        }

        // DELETE - POST: Student/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public IActionResult DeleteConfirmed(int id) {
            try {
                using (SqlConnection connection = new SqlConnection(_connectionString)) {
                    connection.Open();

                    string query = "DELETE FROM Student WHERE StdID = @StdID";

                    using (SqlCommand command = new SqlCommand(query, connection)) {
                        command.Parameters.AddWithValue("@StdID", id);

                        int rowsAffected = command.ExecuteNonQuery();
                        if (rowsAffected == 0) {
                            return NotFound();
                        }
                    }
                }

                return RedirectToAction(nameof(Index));
            } catch (SqlException ex) {
                ViewBag.ErrorMessage = $"Database error: {ex.Message}";
                return RedirectToAction(nameof(Delete), new { id });
            } catch (Exception ex) {
                ViewBag.ErrorMessage = $"Error: {ex.Message}";
                return RedirectToAction(nameof(Delete), new { id });
            }
        }
    }
}
