# Question 23: ASP.NET Core MVC CRUD with EF Core (Code First)

## Project Overview

A complete Student CRUD (Create, Read, Update, Delete) application built with:
- **ASP.NET Core MVC** (Model-View-Controller)
- **Entity Framework Core 8.0** with Code First approach
- **SQL Server** via `SQLEXPRESS`
- **Data Annotations** for validation
- **Tag Helpers** for form controls
- **Async/Await** for EF Core operations

---

## Setup & Execution

### 1. Prerequisites
- .NET 10 SDK installed
- SQL Server (LocalDB or SQLEXPRESS)
- Visual Studio 2026 or Visual Studio Code with C# extension

### 2. Database Setup

#### Option A: Using EF Core Migrations (Recommended)

1. **Apply the migration to create the database:**
   ```powershell
   cd Q23-MVC-EFC
   dotnet ef database update
   ```

   This command:
   - Reads the migration file from `Migrations/` folder
   - Creates the `StudentDB` database
   - Creates the `Students` table with all defined columns

2. **Verify the database was created:**
   - Open SQL Server Management Studio (SSMS)
   - Connect to `.\SQLEXPRESS`
   - Expand "Databases" → should see `StudentDB`
   - Expand `StudentDB` → "Tables" → should see `dbo.Students`

#### Option B: Manual Migration (If needed)

If you haven't created the migration yet:

```powershell
cd Q23-MVC-EFC

# Create a new migration
dotnet ef migrations add InitialCreate

# Apply the migration
dotnet ef database update
```

### 3. Run the Application

```powershell
cd Q23-MVC-EFC
dotnet run
```

- Application will start at `https://localhost:5001` or `http://localhost:5000`
- Navigate to `/Student` to see the Students list

---

## CRUD Operations

### Create Student
1. Click **"Create New"** button on Students page
2. Fill in all required fields:
   - First Name, Last Name, Email, Phone
   - Date of Birth, Course, GPA
   - Remarks (optional), Enrollment Date
3. Click **"Create"** button
4. Student is inserted into database via `_context.Students.Add()` and `SaveChangesAsync()`

### Read/List Students
1. Navigate to `/Student` page
2. All students from database displayed in a table
3. Uses `await _context.Students.ToListAsync()` to fetch asynchronously

### Read Details
1. Click **"Details"** button on any row
2. View complete student information
3. Uses `FirstOrDefaultAsync()` to fetch single student

### Update Student
1. Click **"Edit"** button on any row
2. Modify any field
3. Click **"Save Changes"**
4. Changes updated in database via `_context.Update()` and `SaveChangesAsync()`

### Delete Student
1. Click **"Delete"** button on any row
2. Confirm deletion on confirmation page
3. Click **"Yes, Delete"**
4. Student removed from database via `_context.Students.Remove()` and `SaveChangesAsync()`

---

## Key Concepts Explained

### 1. What is Code First?

**Code First** is an EF Core development approach where:
- You write C# model classes first (e.g., `Student.cs`)
- Model defines database schema (tables, columns, constraints)
- EF Core generates migrate these models to database
- **Flow:** Model → DbContext → Migrations → Database

**Example in this project:**
```csharp
// The Student model defines the database structure
public class Student
{
	public int StudentId { get; set; }
	public string FirstName { get; set; }
	public string Email { get; set; }
	// ... other properties
}
```

When you run `dotnet ef migrations add InitialCreate`, EF Core:
1. Analyzes the `Student` model
2. Compares with existing migrations
3. Generates `InitialCreate.cs` migration file
4. Migration contains SQL to CREATE TABLE Students

**Advantages:**
- Model is source of truth
- Version control friendly
- Familiar to C# developers
- Type-safe

---

### 2. What Does DbContext Do?

**DbContext** is the bridge between your C# code and the database.

**Responsibilities:**
- Manages database connections
- Tracks entity changes (Create, Update, Delete)
- Converts LINQ queries to SQL
- Handles transactions
- Manages migrations

**In this project:**
```csharp
public class AppDbContext : DbContext
{
	public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }

	public DbSet<Student> Students { get; set; } = null!;
}
```

**What it does:**
- `DbSet<Student>` represents the `Students` table
- Constructor receives options (connection string, SQL Server provider)
- `OnModelCreating()` configures entities (keys, constraints, precision)

**Usage in Controller:**
```csharp
var students = await _context.Students.ToListAsync();  // Query database
_context.Students.Add(student);                          // Queue insert
await _context.SaveChangesAsync();                        // Execute changes
```

---

### 3. What Does DbSet Mean?

**DbSet** represents a collection of entities in the database.

```csharp
public DbSet<Student> Students { get; set; }
```

- `DbSet<Student>` = Collection of `Student` rows in database
- Like a `List<Student>` but connected to database
- Provides LINQ query methods: `Where()`, `FirstOrDefault()`, `ToList()`, etc.
- Tracks changes: knows what's inserted, updated, deleted

**Example:**
```csharp
// Query via DbSet
var allStudents = _context.Students.ToList();
var someStudent = _context.Students.FirstOrDefault(s => s.StudentId == 5);

// Modify tracked entity
someStudent.GPA = 3.8;
await _context.SaveChangesAsync();  // EF Core detects change, updates database
```

---

### 4. What is a Migration?

**Migration** is a version-controlled file that describes database schema changes.

**File location:** `Migrations/` folder

**Example migration file structure:**
```csharp
public partial class InitialCreate : Migration
{
	protected override void Up(MigrationBuilder migrationBuilder)
	{
		// SQL: CREATE TABLE, ALTER TABLE, CREATE INDEX, etc.
		migrationBuilder.CreateTable(
			name: "Students",
			columns: table => new
			{
				StudentId = table.Column<int>(nullable: false)
					.Annotation("SqlServer:Identity", "1, 1"),
				FirstName = table.Column<string>(maxLength: 50, nullable: false),
				Email = table.Column<string>(maxLength: 100, nullable: false),
				// ... more columns
			},
			constraints: table =>
			{
				table.PrimaryKey("PK_Students", x => x.StudentId);
			});
	}

	protected override void Down(MigrationBuilder migrationBuilder)
	{
		// Rollback: DROP TABLE Students
		migrationBuilder.DropTable(name: "Students");
	}
}
```

**Migration Workflow:**
1. Modify model (add property, change type, etc.)
2. Run: `dotnet ef migrations add MigrationName`
3. Review generated migration file
4. Run: `dotnet ef database update`
5. Migration is recorded in `__EFMigrationsHistory` table

**Advantages:**
- Schema is version-controlled
- Can rollback to previous schema
- Team sees what changed and why
- Deployment history preserved

---

### 5. How is the Database Generated from C# Model?

**Process:**
1. **Model Definition** (Student.cs):
   ```csharp
   public class Student
   {
	   [Key]
	   public int StudentId { get; set; }

	   [Required]
	   [StringLength(50)]
	   public string FirstName { get; set; }

	   [EmailAddress]
	   public string Email { get; set; }
   }
   ```

2. **DbContext Configuration** (AppDbContext.cs):
   ```csharp
   modelBuilder.Entity<Student>()
	   .Property(s => s.FirstName)
	   .IsRequired()
	   .HasMaxLength(50);
   ```

3. **Migration Generation** (`dotnet ef migrations add InitialCreate`):
   - EF Core analyzes model
   - Compares with previous migration (if exists)
   - Generates C# migration file with SQL builder calls

4. **Generated SQL** (simplified):
   ```sql
   CREATE TABLE [Students] (
	   [StudentId] INT NOT NULL IDENTITY(1,1),
	   [FirstName] NVARCHAR(50) NOT NULL,
	   [LastName] NVARCHAR(50) NOT NULL,
	   [Email] NVARCHAR(100) NOT NULL,
	   [Phone] NVARCHAR(20) NOT NULL,
	   [DateOfBirth] DATETIME2 NOT NULL,
	   [Course] NVARCHAR(100) NOT NULL,
	   [GPA] NUMERIC(3,2) NOT NULL,
	   [Remarks] NVARCHAR(500) NULL,
	   [EnrollmentDate] DATETIME2 NOT NULL,
	   PRIMARY KEY ([StudentId])
   );
   ```

5. **Database Update** (`dotnet ef database update`):
   - Executes migration SQL
   - Creates table in SQL Server
   - Records migration in `__EFMigrationsHistory`

---

### 6. How CRUD Works with EF Core

#### CREATE
```csharp
[HttpPost]
public async Task<IActionResult> Create(Student student)
{
	if (ModelState.IsValid)
	{
		_context.Add(student);                  // Queue for insert
		await _context.SaveChangesAsync();      // Execute INSERT
		return RedirectToAction(nameof(Index));
	}
	return View(student);
}
```
- `Add()` marks entity as "Added"
- `SaveChangesAsync()` generates & executes: `INSERT INTO Students (...)`
- Database generates `StudentId` via IDENTITY
- New record returned with ID

#### READ
```csharp
public async Task<IActionResult> Index()
{
	var students = await _context.Students.ToListAsync();
	return View(students);
}
```
- `ToListAsync()` generates & executes: `SELECT * FROM Students`
- Returns `List<Student>` with all rows
- Non-blocking (async)

#### UPDATE
```csharp
[HttpPost]
public async Task<IActionResult> Edit(int id, Student student)
{
	_context.Update(student);               // Mark as "Modified"
	await _context.SaveChangesAsync();      // Execute UPDATE
	return RedirectToAction(nameof(Index));
}
```
- `Update()` marks entity as "Modified"
- `SaveChangesAsync()` generates & executes: `UPDATE Students SET ... WHERE StudentId = @id`
- Only changed properties are included in SQL

#### DELETE
```csharp
[HttpPost]
public async Task<IActionResult> DeleteConfirmed(int id)
{
	var student = await _context.Students.FindAsync(id);
	_context.Students.Remove(student);         // Queue for delete
	await _context.SaveChangesAsync();          // Execute DELETE
	return RedirectToAction(nameof(Index));
}
```
- `FindAsync()` retrieves entity by primary key
- `Remove()` marks as "Deleted"
- `SaveChangesAsync()` generates & executes: `DELETE FROM Students WHERE StudentId = @id`

---

### 7. Difference Between ADO.NET and EF Core

| Feature | ADO.NET | EF Core |
|---------|---------|---------|
| **Level of Abstraction** | Low-level, direct SQL | High-level, ORM |
| **Code Complexity** | Manual SQL strings, parameterization | LINQ queries, auto-generated SQL |
| **Performance** | Faster (less overhead) | Slightly slower (translation layer) |
| **Type Safety** | No compile-time checking | Full type safety |
| **Change Tracking** | Manual tracking needed | Automatic (DbContext) |
| **Migrations** | Manual SQL scripts | Auto-generated from models |
| **Learning Curve** | SQL knowledge required | C# knowledge sufficient |
| **Maintenance** | SQL and C# drift over time | Single source of truth (model) |
| **Relationships** | Manual joins in SQL | Navigation properties, auto-joins |

**ADO.NET Example:**
```csharp
using (SqlConnection conn = new SqlConnection(connectionString))
{
	SqlCommand cmd = new SqlCommand(
		"INSERT INTO Students (FirstName, Email) VALUES (@firstName, @email)", 
		conn);
	cmd.Parameters.AddWithValue("@firstName", "John");
	cmd.Parameters.AddWithValue("@email", "john@example.com");

	conn.Open();
	cmd.ExecuteNonQuery();  // Manual execution
}
```

**EF Core Example:**
```csharp
var student = new Student { FirstName = "John", Email = "john@example.com" };
_context.Students.Add(student);
await _context.SaveChangesAsync();  // Automatic
```

**In this project:** We use EF Core (Question 23), which is cleaner, more maintainable, and handles database changes automatically.

---

### 8. Exact Commands for Creating/Applying Migrations

#### Create Initial Migration
```powershell
cd Q23-MVC-EFC
dotnet ef migrations add InitialCreate
```
- Creates `Migrations/[DateTime]_InitialCreate.cs`
- Creates `Migrations/AppDbContextModelSnapshot.cs`
- Does NOT modify database yet

#### Create Additional Migrations (After Model Changes)
```powershell
# Example: Add a new property to Student model
# Then create migration:
dotnet ef migrations add AddStudentAge

# Or add a new entity:
dotnet ef migrations add AddCourseEntity
```

#### Apply Migrations to Database
```powershell
# Apply all pending migrations
dotnet ef database update

# Apply to specific migration
dotnet ef database update InitialCreate

# Rollback to previous state
dotnet ef database update [PreviousMigrationName]
```

#### View Migration Details
```powershell
# List all migrations
dotnet ef migrations list

# Show migrations with details
dotnet ef migrations list --detailed
```

#### Remove Last Migration (Before Running `database update`)
```powershell
# Only works if migration hasn't been applied to database
dotnet ef migrations remove
```

#### Drop Database (Recreate from Scratch)
```powershell
dotnet ef database drop
dotnet ef database update
```

#### Create Migration Script for Manual Application
```powershell
# Generate SQL script from InitialCreate onwards
dotnet ef migrations script --output migrations.sql

# Generate script from specific range
dotnet ef migrations script InitialCreate AddStudentAge --output changes.sql
```

---

## File Structure

```
Q23-MVC-EFC/
├── Models/
│   ├── Student.cs                  # Student entity with DataAnnotations
│   └── ErrorViewModel.cs           # Error model
├── Data/
│   └── AppDbContext.cs             # Database context
├── Controllers/
│   ├── HomeController.cs           # Home page controller
│   └── StudentController.cs        # Student CRUD controller (async)
├── Views/
│   ├── Home/
│   │   ├── Index.cshtml
│   │   └── Privacy.cshtml
│   ├── Student/
│   │   ├── Index.cshtml            # List all students
│   │   ├── Create.cshtml           # Create form
│   │   ├── Edit.cshtml             # Edit form
│   │   ├── Details.cshtml          # Read single student
│   │   └── Delete.cshtml           # Delete confirmation
│   └── Shared/
│       ├── _Layout.cshtml          # Master layout
│       └── Error.cshtml
├── Migrations/
│   ├── [DateTime]_InitialCreate.cs # Auto-generated migration
│   ├── AppDbContextModelSnapshot.cs
│   └── AppDbContext.Designer.cs
├── appsettings.json                # Connection string & logging
├── Program.cs                      # DbContext registration
├── Q23-MVC-EFC.csproj             # Project file w/ EF Core packages
└── README.md                       # This file
```

---

## Configuration Details

### appsettings.json
```json
{
  "ConnectionStrings": {
	"DefaultConnection": "Server=.\\SQLEXPRESS;Database=StudentDB;Trusted_Connection=true;Encrypt=false;"
  }
}
```
- `Server=.\SQLEXPRESS` - Local SQL Server instance
- `Database=StudentDB` - Database name
- `Trusted_Connection=true` - Windows authentication (no password)
- `Encrypt=false` - Disable encryption for development

### Program.cs DbContext Registration
```csharp
builder.Services.AddDbContext<AppDbContext>(options =>
	options.UseSqlServer(builder.Configuration.GetConnectionString("DefaultConnection")));
```
- Registers `AppDbContext` in dependency injection container
- Scoped lifetime (new instance per HTTP request)
- Reads connection string from `appsettings.json`

---

## Validation Features

### DataAnnotations Used
- `[Required]` - Field must have value
- `[StringLength(n)]` - Max string length
- `[EmailAddress]` - Valid email format
- `[Phone]` - Valid phone format
- `[Range(min, max)]` - Numeric range
- `[DataType(DataType.Date)]` - Date picker in form
- `[Key]` - Primary key

### Client-Side Validation
- HTML5 attributes generated by Tag Helpers
- Real-time validation feedback in browser

### Server-Side Validation
- EF Core DataAnnotations checked
- `ModelState.IsValid` in controller
- Error messages displayed via `asp-validation-for`

---

## Tag Helpers Used

- `asp-action` - Generate action link URLs
- `asp-route-{param}` - Pass route parameters
- `asp-for` - Bind form input to model properties
- `asp-validation-for` - Display validation errors
- `asp-validation-summary` - Display all validation errors
- `input[asp-for]` - Generate typed `<input>` with data attributes

---

## Async/Await Operations

All database operations are async:
- `ToListAsync()` - Fetch all records
- `FirstOrDefaultAsync()` - Fetch single record
- `FindAsync(id)` - Find by primary key
- `SaveChangesAsync()` - Commit changes
- `AnyAsync()` - Check existence

**Benefits:**
- Non-blocking I/O
- Better scalability
- Thread pool threads freed during I/O wait
- Responsive web application

---

## Testing the Application

1. **Create Students:**
   - Navigate to Students page
   - Click "Create New"
   - Fill form with valid data
   - Observe student added to table

2. **List Students:**
   - Refresh page
   - Verify all students appear

3. **View Details:**
   - Click "Details"
   - Verify all information displayed

4. **Update Student:**
   - Click "Edit"
   - Change GPA or course
   - Save
   - Verify changes persisted

5. **Delete Student:**
   - Click "Delete"
   - Review confirmation
   - Confirm deletion
   - Verify student removed from list

6. **Database Verification:**
   - Open SSMS
   - Query: `SELECT * FROM StudentDB.dbo.Students`
   - Verify data matches UI

---

## Common Issues & Solutions

| Issue | Solution |
|-------|----------|
| Migration tool not found | `dotnet tool install --global dotnet-ef` |
| Connection string error | Verify SQL Server is running, check connection string in appsettings.json |
| Migration already exists | `dotnet ef migrations remove` (if not applied) or `dotnet ef migrations add NameB` |
| Database locked | Close other connections, stop running app, try `dotnet ef database drop` |
| Validation errors not showing | Verify `asp-validation-for` tags in form, check `ModelState.IsValid` in controller |

---

## Summary

**Question 23** implements a complete, production-ready Student CRUD application using:
- **Code First** approach (Models → Migrations → Database)
- **Entity Framework Core 8.0** for data access
- **Async/Await** for scalable database operations
- **DataAnnotations** for validation
- **Tag Helpers** for clean, type-safe views
- **SQL Server** with `SQLEXPRESS`
- **MVC pattern** (separation of concerns)

The application demonstrates enterprise-level practices while remaining educational and maintainable.

---

## College/Institute Details

**Student:** [Your Name]  
**Course:** CSIT 6th Semester  
**Subject:** ASP.NET Core & C# Practical  
**Institution:** [Your College/University Name]  
**Date:** 2026  
**Assignment:** Question 23 - EF Core Code First CRUD Application

---
