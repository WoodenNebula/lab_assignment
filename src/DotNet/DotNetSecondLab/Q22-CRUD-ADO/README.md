# Question 22: ASP.NET Core MVC CRUD with ADO.NET

## Overview

A complete Student management application using **ASP.NET Core MVC** with **ADO.NET** (no Entity Framework Core). Features CRUD operations with server-side validation, parameterized SQL queries, and clean MVC architecture.

---

## SQL Server Database Setup

Execute the following SQL script in SQL Server Management Studio (SSMS) to create the database and table.

### Complete SQL Script:

```sql
-- Create Database
CREATE DATABASE StudentDB;
GO

-- Use the database
USE StudentDB;
GO

-- Create Student table
CREATE TABLE Student (
	StdID INT PRIMARY KEY,
	Name NVARCHAR(100) NOT NULL,
	Address NVARCHAR(200) NOT NULL,
	Faculty NVARCHAR(50) NOT NULL,
	Email NVARCHAR(100) NOT NULL
);
GO

-- Optional: Insert sample data for testing
INSERT INTO Student (StdID, Name, Address, Faculty, Email) VALUES
(1, 'Surab Parajuli', 'Kathmandu', 'Computer Science', 'surab@example.com'),
(2, 'John Doe', 'Pokhara', 'Information Technology', 'john@example.com'),
(3, 'Jane Smith', 'Lalitpur', 'Software Engineering', 'jane@example.com');
GO

-- Verify table and data
SELECT * FROM Student;
GO
```

**How to execute:**
1. Open SQL Server Management Studio
2. Connect to your SQL Server instance
3. Create a new query
4. Paste the entire script
5. Click Execute (F5) or use Ctrl+E

---

## Configuration

### Connection String (appsettings.json)

The connection string is configured in `appsettings.json`:

```json
{
  "ConnectionStrings": {
	"DefaultConnection": "Server=.;Database=StudentDB;Trusted_Connection=true;TrustServerCertificate=true;"
  }
}
```

**For different configurations:**

**SQL Server Authentication (Username/Password):**
```json
"DefaultConnection": "Server=.;Database=StudentDB;User Id=sa;Password=YourPassword;TrustServerCertificate=true;"
```

**Named Instance (SQLEXPRESS):**
```json
"DefaultConnection": "Server=.\\SQLEXPRESS;Database=StudentDB;Trusted_Connection=true;TrustServerCertificate=true;"
```

**Remote Server:**
```json
"DefaultConnection": "Server=your-server-name;Database=StudentDB;Trusted_Connection=true;TrustServerCertificate=true;"
```

### Program.cs Configuration

The connection string is registered in `Program.cs`:

```csharp
var builder = WebApplication.CreateBuilder(args);
builder.Services.AddControllersWithViews();
var app = builder.Build();
```

The controller accesses it via dependency injection:

```csharp
public StudentController(IConfiguration configuration) {
	_configuration = configuration;
	_connectionString = _configuration.GetConnectionString("DefaultConnection") ?? "";
}
```

---

## Project Structure

```
Q22-CRUD-ADO/
├── Controllers/
│   └── StudentController.cs      # CRUD logic with ADO.NET calls
├── Models/
│   ├── Student.cs                # Data model with validation
│   └── ErrorViewModel.cs         # Error model
├── Views/
│   ├── Student/
│   │   ├── Index.cshtml          # List all students
│   │   ├── Create.cshtml         # Create form
│   │   ├── Edit.cshtml           # Edit form
│   │   ├── Details.cshtml        # Display details
│   │   └── Delete.cshtml         # Delete confirmation
│   └── Shared/
│       ├── _Layout.cshtml        # Master layout
│       └── _ValidationScriptsPartial.cshtml
├── appsettings.json              # Connection string
└── Program.cs                    # DI & middleware config
```

---

## MVC + ADO.NET Flow Explanation

### 1. CREATE Operation Flow

**User Action → Controller → ADO.NET → Database → View**

```
1. User clicks "Add New Student"
   ↓
2. StudentController.Create() [GET] 
   → Returns empty Create.cshtml form
   ↓
3. User fills form and clicks "Create"
   ↓
4. StudentController.Create(Student model) [POST]
   → Validates model using DataAnnotations
   → Opens SqlConnection
   → Creates parameterized INSERT query
   → Executes command via SqlCommand
   → Returns RedirectToAction("Index")
   ↓
5. Browser navigates to Index
   → List displays new student
```

**Code Example:**
```csharp
[HttpPost]
public IActionResult Create(Student student) {
	if (!ModelState.IsValid) return View(student);

	using (SqlConnection connection = new SqlConnection(_connectionString)) {
		connection.Open();
		using (SqlCommand command = new SqlCommand(
			"INSERT INTO Student (StdID, Name, Address, Faculty, Email) " +
			"VALUES (@StdID, @Name, @Address, @Faculty, @Email)", 
			connection)) {
			command.Parameters.AddWithValue("@StdID", student.StdID);
			command.Parameters.AddWithValue("@Name", student.Name);
			// ... add other parameters
			command.ExecuteNonQuery();
		}
	}
	return RedirectToAction(nameof(Index));
}
```

---

### 2. READ Operation Flow

**User Action → Controller → ADO.NET (SqlDataReader) → Database & Memory → View**

```
1. User navigates to Students page
   ↓
2. StudentController.Index() [GET]
   → Opens SqlConnection
   → Creates SELECT query
   → Executes SqlCommand
   → SqlDataReader loops through result rows
   → Maps each row to Student object
   → Builds List<Student>
   → Returns View(studentList)
   ↓
3. Index.cshtml renders table
   → Displays all students in HTML table
```

**Code Example:**
```csharp
public IActionResult Index() {
	var students = new List<Student>();

	using (SqlConnection connection = new SqlConnection(_connectionString)) {
		connection.Open();
		using (SqlCommand command = new SqlCommand(
			"SELECT StdID, Name, Address, Faculty, Email FROM Student", 
			connection)) {
			using (SqlDataReader reader = command.ExecuteReader()) {
				while (reader.Read()) {
					students.Add(new Student {
						StdID = (int)reader["StdID"],
						Name = reader["Name"].ToString() ?? "",
						// ... map other fields
					});
				}
			}
		}
	}
	return View(students);
}
```

---

### 3. UPDATE Operation Flow

**User Action → Controller (Read) → Form → Controller (Write) → ADO.NET → Database**

```
1. User clicks Edit button on a student
   ↓
2. StudentController.Edit(id) [GET]
   → Queries database for existing student
   → Uses SqlDataReader to fetch data
   → Returns Edit.cshtml with pre-filled form
   ↓
3. User modifies fields and submits
   ↓
4. StudentController.Edit(id, Student) [POST]
   → Validates using DataAnnotations
   → Opens SqlConnection
   → Creates parameterized UPDATE query
   → Executes update via SqlCommand
   → Returns RedirectToAction("Index")
   ↓
5. List shows updated data
```

**Code Example:**
```csharp
[HttpPost]
public IActionResult Edit(int id, Student student) {
	if (!ModelState.IsValid) return View(student);

	using (SqlConnection connection = new SqlConnection(_connectionString)) {
		connection.Open();
		using (SqlCommand command = new SqlCommand(
			"UPDATE Student SET Name=@Name, Address=@Address, Faculty=@Faculty, Email=@Email WHERE StdID=@StdID",
			connection)) {
			command.Parameters.AddWithValue("@StdID", student.StdID);
			command.Parameters.AddWithValue("@Name", student.Name);
			// ... add other parameters
			command.ExecuteNonQuery();
		}
	}
	return RedirectToAction(nameof(Index));
}
```

---

### 4. DELETE Operation Flow

**User Action → Delete Confirmation → Controller → ADO.NET → Database**

```
1. User clicks Delete button
   ↓
2. StudentController.Delete(id) [GET]
   → Queries database using SqlDataReader
   → Shows confirmation view with student details
   ↓
3. User clicks "Yes, Delete Student"
   ↓
4. StudentController.DeleteConfirmed(id) [POST]
   → Creates parameterized DELETE query
   → Executes delete via SqlCommand
   → Returns RedirectToAction("Index")
   ↓
5. Student removed from database
   → List no longer shows deleted record
```

**Code Example:**
```csharp
[HttpPost, ActionName("Delete")]
public IActionResult DeleteConfirmed(int id) {
	using (SqlConnection connection = new SqlConnection(_connectionString)) {
		connection.Open();
		using (SqlCommand command = new SqlCommand(
			"DELETE FROM Student WHERE StdID = @StdID",
			connection)) {
			command.Parameters.AddWithValue("@StdID", id);
			command.ExecuteNonQuery();
		}
	}
	return RedirectToAction(nameof(Index));
}
```

---

## Key Concepts Explained

### 1. Why ADO.NET is Different from Entity Framework Core

| Aspect | ADO.NET | Entity Framework Core |
|--------|---------|---------------------|
| **Abstraction Level** | Low-level, direct SQL | High-level ORM |
| **LINQ Support** | No native LINQ | Full LINQ support |
| **SQL Writing** | Manual (parameterized queries) | Generated queries |
| **Learning Curve** | Requires SQL knowledge | Easier for beginners |
| **Performance** | Slightly faster (less overhead) | Slight overhead (mapping) |
| **Control** | Full control over queries | Less control (auto-generated) |
| **Use Case** | Complex queries, direct SQL | Standard CRUD operations |

**ADO.NET Example:**
```csharp
using (SqlConnection conn = new SqlConnection(connectionString)) {
	conn.Open();
	using (SqlCommand cmd = new SqlCommand("SELECT * FROM Student WHERE StdID = @id", conn)) {
		cmd.Parameters.AddWithValue("@id", id);
		using (SqlDataReader reader = cmd.ExecuteReader()) {
			while (reader.Read()) {
				// Manual mapping manually mapping data
			}
		}
	}
}
```

**Entity Framework Example:**
```csharp
var student = _context.Students.FirstOrDefault(s => s.StdID == id);
```

---

### 2. How SqlDataReader Works

**SqlDataReader** is a **fast, forward-only cursor** over query results.

**Key Characteristics:**
- **Forward-only**: Can only move forward through results (no backward navigation)
- **Sequential access**: Access columns by index or name
- **Memory efficient**: Loads one row at a time
- **Connected**: Requires active connection (must be disposed properly)

**Example Usage Pattern:**
```csharp
using (SqlConnection connection = new SqlConnection(connectionString)) {
	connection.Open();
	using (SqlCommand command = new SqlCommand("SELECT * FROM Student", connection)) {
		using (SqlDataReader reader = command.ExecuteReader()) {
			while (reader.Read()) {
				// Each iteration = one row
				int id = (int)reader["StdID"];           // Access by column name
				string name = (string)reader[1];          // OR by index

				// Null check
				string email = reader["Email"].ToString() ?? "";
			}
		}
	}
}
```

**SqlDataReader Lifecycle:**
1. `ExecuteReader()` → Returns SqlDataReader
2. `while (reader.Read())` → Move to next row
3. `reader["ColumnName"]` or `reader[index]` → Access column
4. Loop ends when no more rows
5. `using` statement auto-disposes reader and connection

---

### 3. Why Parameterized Queries Are Used

Parameterized queries prevent **SQL Injection attacks**.

**UNSAFE - Vulnerable to SQL Injection:**
```csharp
string query = "SELECT * FROM Student WHERE Name = '" + userInput + "'";
// If userInput = "'; DROP TABLE Student; --" 
// Query becomes: SELECT * FROM Student WHERE Name = ''; DROP TABLE Student; --'
// This DELETES the table!
```

**SAFE - Parameterized Query:**
```csharp
string query = "SELECT * FROM Student WHERE Name = @Name";
command.Parameters.AddWithValue("@Name", userInput);
// Input treated as DATA, not executable code
```

**Benefits of Parameterized Queries:**
- Prevents SQL injection
- Improves readability
- Enables query plan caching (performance)
- Type safety (automatic type conversion)
- Separates code from data

**Pattern Used Throughout Application:**
```csharp
using (SqlCommand command = new SqlCommand(query, connection)) {
	command.Parameters.AddWithValue("@StdID", student.StdID);
	command.Parameters.AddWithValue("@Name", student.Name);
	command.Parameters.AddWithValue("@Address", student.Address);
	command.Parameters.AddWithValue("@Faculty", student.Faculty);
	command.Parameters.AddWithValue("@Email", student.Email);
	command.ExecuteNonQuery();
}
```

---

### 4. Razor View to Controller Communication

**Views communicate with Controllers via:**

#### A. **Form Tag Helpers** (for action methods)
```html
<!-- Create Form - Calls Create(Student) POST action -->
<form asp-action="Create" asp-controller="Student" method="post">
	<input asp-for="Name" />
	<button type="submit">Create</button>
</form>
```

The form automatically:
- POSTs to `/Student/Create`
- Includes CSRF token (anti-forgery)
- Model binds form data to `Student` parameter

#### B. **Anchor Tag Helpers** (for navigation)
```html
<!-- Navigate to Edit page with ID parameter -->
<a asp-action="Edit" asp-route-id="@student.StdID">Edit</a>
<!-- Generates: /Student/Edit/34 -->

<!-- Navigate to Delete with confirmation -->
<a asp-action="Delete" asp-route-id="@student.StdID">Delete</a>
<!-- Generates: /Student/Delete/34 -->
```

#### C. **Property Tag Helpers** (for form inputs)
```html
<!-- Automatically binds to model property -->
<input asp-for="Name" />
<!-- Generates: <input type="text" id="Name" name="Name" value="..." /> -->

<!-- Includes validation messages -->
<span asp-validation-for="Name" class="text-danger"></span>
<!-- Only shown if Model validation fails -->
```

#### D. **Model Validation** (server-side)
```csharp
[HttpPost]
public IActionResult Create(Student student) {
	if (!ModelState.IsValid) {
		// Validation failed, return form with errors
		return View(student); // Errors displayed via asp-validation-for
	}
	// Validation passed, process data
}
```

---

## Running the Application

1. **Ensure SQL Server is running**
   - Open SQL Server Management Studio
   - Execute the SQL script above

2. **Update connection string (if needed)**
   - Edit `appsettings.json`
   - Adjust server name or authentication method

3. **Run the application**
   - In Visual Studio: Press Ctrl+F5
   - Or: Run `dotnet run` in PowerShell
   - Navigate to `https://localhost:7XXX` (or shown URL)

4. **Access Student Management**
   - Click "Students" in navbar
   - Or navigate to `/Student`

---

## CRUD Operations Testing

### Test 1: CREATE
1. Click "Add New Student"
2. Enter values:
   - ID: 34
   - Name: Test Student
   - Address: Test Address
   - Faculty: Computer Science
   - Email: test@example.com
3. Click "Create"
4. Verify in list

### Test 2: READ
1. Go to Students page
2. View all students in table

### Test 3: UPDATE
1. Click "Edit" on a student
2. Modify fields
3. Click "Save Changes"
4. Verify changes in list

### Test 4: DELETE
1. Click "Delete" on a student
2. Review confirmation with student details
3. Click "Yes, Delete Student"
4. Verify removed from list

---

## Error Handling

The application handles:
- **Duplicate IDs** (PRIMARY KEY violation)
- **Validation errors** (displayed on form)
- **Database connection errors** (shown as alerts)
- **Not found** (404 when record doesn't exist)
- **SQL injection** (prevented by parameterized queries)

---

## Key Implementation Points

✅ **ADO.NET Used Throughout**
- `SqlConnection`, `SqlCommand`, `SqlDataReader`
- No Entity Framework Core

✅ **Parameterized Queries**
- All database access uses `@Parameter` binding

✅ **Resource Disposal**
- All `SqlConnection`, `SqlCommand`, `SqlDataReader` in `using` statements

✅ **DataAnnotations Validation**
- Model validation attributes on Student class
- Server-side validation in controller

✅ **Tag Helpers in Forms**
- `asp-for`, `asp-action`, `asp-controller`
- `asp-validation-for`, `asp-validation-summary`

✅ **MVC Architecture**
- Clean separation: Model, Controller, View
- No business logic in views
- Controllers handle ADO.NET calls

✅ **Error Messages**
- User-friendly error display
- Database error logging

---

## Summary

This application demonstrates:
1. ✅ Pure ADO.NET (no EF Core)
2. ✅ Parameterized SQL queries for safety
3. ✅ Proper resource management with `using` statements
4. ✅ Complete CRUD operations (Create, Read, Update, Delete)
5. ✅ DataAnnotations for validation
6. ✅ Tag Helpers for clean view code
7. ✅ Clean MVC architecture
8. ✅ Database error handling
9. ✅ SQL injection prevention

**College Details:**
- Name: Surab Parajuli
- Section: A, 6th-Sem
- Roll: 34
- Symbol No.: 80010139
