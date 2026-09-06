using System.ComponentModel.DataAnnotations;
using Microsoft.AspNetCore.Mvc;

namespace Q27a_jQuery.Models {
    public class RegisterViewModel : Controller {
        [Required]
        public string Name { get; set; } = string.Empty;

        [Required]
        [EmailAddress]
        public string Email { get; set; } = string.Empty;

        [Required]
        public string Password { get; set; } = string.Empty;

        [Required]
        [RegularExpression(@"^\d{10}$", ErrorMessage = "Phone number must be exactly 10 digits.")]
        public string Phone { get; set; } = string.Empty;
    }
}
