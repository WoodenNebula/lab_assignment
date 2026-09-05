using System.ComponentModel.DataAnnotations;

namespace Q19_MVC.Models {
    public class StudentModel {
        [Required(ErrorMessage = "Student ID is required")]
        [StringLength(10, MinimumLength = 1, ErrorMessage = "Student ID must be between 1 and 10 characters")]
        public string StdID { get; set; }

        [Required(ErrorMessage = "Name is required")]
        [StringLength(100, MinimumLength = 2, ErrorMessage = "Name must be between 2 and 100 characters")]
        public string Name { get; set; }

        [Required(ErrorMessage = "Address is required")]
        [StringLength(200, MinimumLength = 5, ErrorMessage = "Address must be between 5 and 200 characters")]
        public string Address { get; set; }

        [Required(ErrorMessage = "Faculty is required")]
        [StringLength(50, MinimumLength = 2, ErrorMessage = "Faculty must be between 2 and 50 characters")]
        public string Faculty { get; set; }

        [Required(ErrorMessage = "Email is required")]
        [EmailAddress(ErrorMessage = "Invalid email format")]
        public string Email { get; set; }

        [Required(ErrorMessage = "Phone number is required")]
        [RegularExpression(@"^\d{10}$", ErrorMessage = "Phone number must be 10 digits")]
        public string PhoneNumber { get; set; }
    }
}
