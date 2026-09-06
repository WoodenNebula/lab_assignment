using System.ComponentModel.DataAnnotations;

namespace Q22_CRUD_ADO.Models {
    public class Student {
        [Required(ErrorMessage = "Student ID is required")]
        [Range(1, int.MaxValue, ErrorMessage = "Student ID must be a positive number")]
        public int StdID { get; set; }

        [Required(ErrorMessage = "Name is required")]
        [StringLength(100, MinimumLength = 2, ErrorMessage = "Name must be between 2 and 100 characters")]
        public string Name { get; set; } = string.Empty;

        [Required(ErrorMessage = "Address is required")]
        [StringLength(200, MinimumLength = 5, ErrorMessage = "Address must be between 5 and 200 characters")]
        public string Address { get; set; } = string.Empty;

        [Required(ErrorMessage = "Faculty is required")]
        [StringLength(50, MinimumLength = 2, ErrorMessage = "Faculty must be between 2 and 50 characters")]
        public string Faculty { get; set; } = string.Empty;

        [Required(ErrorMessage = "Email is required")]
        [EmailAddress(ErrorMessage = "Invalid email address")]
        public string Email { get; set; } = string.Empty;
    }
}
