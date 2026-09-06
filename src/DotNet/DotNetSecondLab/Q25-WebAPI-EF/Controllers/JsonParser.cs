using System.Text.Json;
using Microsoft.AspNetCore.Mvc;
namespace Q25_WebAPI_EF.Controllers {

    [ApiController]
    [Route("api/[controller]")]
    public class JsonParser : ControllerBase {

        [HttpGet("parse")]
        public IActionResult ParseSampleJson() {
            string rawJson = """
                {
                    "studentId": 1,
                    "firstName": "Surab",
                    "lastName": "Parajuli",
                    "email": "surabparajuli404@gmail.com",
                    "phone": "9869696969",
                    "dateOfBirth": "2005-11-05T00:00:00Z",
                    "course": "CSIT",
                    "gpa": 3.69,
                    "remarks": "WEEEEEEEEE",
                    "enrollmentDate": "2023-01-10T00:00:00Z"
                }
            """;

            using JsonDocument doc = JsonDocument.Parse(rawJson);
            JsonElement root = doc.RootElement;

            var result = new {
                studentId = root.GetProperty("studentId").GetInt32(),
                firstName = root.GetProperty("firstName").GetString(),
                lastName = root.GetProperty("lastName").GetString(),
                email = root.GetProperty("email").GetString(),
                gpa = root.GetProperty("gpa").GetDecimal()
            };

            return Ok(result);
        }
    }
}

