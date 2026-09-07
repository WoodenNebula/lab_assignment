using System.ComponentModel.DataAnnotations;
namespace Q28_Security.Models {
    public class RegisterViewModel {
        [Required, EmailAddress]
        public string Email { get; set; } = string.Empty;

        [Required, DataType(DataType.Password)]
        public string Password { get; set; } = string.Empty;
    }
}

