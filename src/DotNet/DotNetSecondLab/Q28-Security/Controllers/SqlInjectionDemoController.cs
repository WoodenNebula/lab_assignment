using System.Data;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.SqlClient;


namespace Q28_Security.Controllers {

    public class SqlInjectionDemoController : Controller {
        private readonly string _connectionString;

        public SqlInjectionDemoController(IConfiguration configuration) {
            _connectionString = configuration.GetConnectionString("DefaultConnection")!;
        }

        public IActionResult Index() => View();

        // Intentionally vulnerable — educational demo only, isolated to this action
        [HttpPost]
        public IActionResult VulnerableSearch(string username) {
            var results = new DataTable();
            using var conn = new SqlConnection(_connectionString);
            conn.Open();

            string query = "SELECT * FROM DemoLogin WHERE Username = '" + username + "'";
            using var cmd = new SqlCommand(query, conn);
            using var reader = cmd.ExecuteReader();
            results.Load(reader);

            ViewBag.Query = query;
            ViewBag.IsVulnerable = true;
            return View("Results", results);
        }

        // Secure equivalent — parameterized query
        [HttpPost]
        public IActionResult SafeSearch(string username) {
            var results = new DataTable();
            using var conn = new SqlConnection(_connectionString);
            conn.Open();

            string query = "SELECT * FROM DemoLogin WHERE Username = @Username";
            using var cmd = new SqlCommand(query, conn);
            cmd.Parameters.Add(new SqlParameter("@Username", username));
            using var reader = cmd.ExecuteReader();
            results.Load(reader);

            ViewBag.Query = query;
            ViewBag.IsVulnerable = false;
            return View("Results", results);
        }
    }
}
