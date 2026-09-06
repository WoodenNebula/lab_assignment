using System;
using System.Collections.Generic;

namespace Q24_CRUD_EF_Db.Models;

public partial class Student
{
    public int StudentId { get; set; }

    public string FirstName { get; set; } = null!;

    public string LastName { get; set; } = null!;

    public string Email { get; set; } = null!;

    public string Phone { get; set; } = null!;

    public DateTime DateOfBirth { get; set; }

    public string Course { get; set; } = null!;

    public decimal Gpa { get; set; }

    public string? Remarks { get; set; }

    public DateTime EnrollmentDate { get; set; }
}
