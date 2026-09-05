# Question 21: CRUD Operations with SQL Server and ADO.NET

## Setup Instructions

### Step 1: SQL Server Setup in SSMS

Execute the following SQL script in SQL Server Management Studio to create the database and table.

#### SQL Script:
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

**Steps to execute in SSMS:**

1. Open SQL Server Management Studio
2. Connect to your SQL Server instance (usually `(local)` or `.`)
3. Create a new query
4. Copy and paste the entire SQL script above
5. Click "Execute" (or press F5)
6. You should see messages confirming database and table creation

### Step 2: Configure Connection String

The application uses this connection string:
```
Server=.;Database=StudentDB;Trusted_Connection=true;TrustServerCertificate=true;
```

**If your setup is different, modify the `ConnectionString` in Program.cs:**

**For Windows Authentication (Trusted Connection):**
```csharp
private const string ConnectionString = "Server=.;Database=StudentDB;Trusted_Connection=true;TrustServerCertificate=true;";
```

**For SQL Server Authentication (Username/Password):**
```csharp
private const string ConnectionString = "Server=.;Database=StudentDB;User Id=sa;Password=YourPassword;TrustServerCertificate=true;";
```

**For Named Instance:**
```csharp
private const string ConnectionString = "Server=.\\SQLEXPRESS;Database=StudentDB;Trusted_Connection=true;TrustServerCertificate=true;";
```

### Step 3: Run the Console Application

1. Open the solution in Visual Studio
2. Navigate to the Q21-CRUD-Console project
3. Right-click the project → Set as Startup Project
4. Press Ctrl+F5 (Run without debugging) or F5 (Run with debugging)
5. The console application will start with the main menu

## Application Menu

```
--- Main Menu ---
1. Insert Student
2. View All Students
3. Update Student
4. Delete Student
5. Exit

Enter your choice (1-5):
```

## Testing All Four CRUD Operations

### Test 1: INSERT Operation

**Steps:**
1. Select menu option `1` (Insert Student)
2. Enter the following when prompted:
   - Student ID: `34`
   - Name: `Test Student`
   - Address: `Test Address`
   - Faculty: `Computer Science`
   - Email: `test@example.com`
3. Press Enter
4. You should see: `✓ Student inserted successfully!`

**Verify in SSMS:**
```sql
SELECT * FROM Student WHERE StdID = 34;
```

---

### Test 2: READ Operation (View All Students)

**Steps:**
1. Select menu option `2` (View All Students)
2. Press Enter
3. All students in the database will be displayed with:
   - ID
   - Name
   - Address
   - Faculty
   - Email

**Expected output:** All inserted students displayed in a formatted list

---

### Test 3: UPDATE Operation

**Steps:**
1. Select menu option `3` (Update Student)
2. Enter Student ID: `34` (or any existing ID)
3. Update the student details:
   - Enter new Name: `Updated Name`
   - Enter new Address: `Updated Address`
   - Enter new Faculty: `Updated Faculty`
   - Enter new Email: `updated@example.com`
4. Press Enter
5. You should see: `✓ Student updated successfully!`

**Verify in SSMS:**
```sql
SELECT * FROM Student WHERE StdID = 34;
```

---

### Test 4: DELETE Operation

**Steps:**
1. Select menu option `4` (Delete Student)
2. Enter Student ID: `34` (or any existing ID you wish to delete)
3. Confirm deletion: `yes` (or type `no` to cancel)
4. You should see: `✓ Student deleted successfully!`

**Verify in SSMS:**
```sql
SELECT * FROM Student WHERE StdID = 34;
```
(Should return no rows)

---

## Error Handling Tests

### Test Duplicate ID (Insert)
1. Try to insert a student with ID `1` (which already exists)
2. Expected: `✗ Database Error: Violation of PRIMARY KEY constraint`

### Test Nonexistent Student (Update/Delete)
1. Try to update or delete student with ID `999`
2. Expected: `Student with ID 999 not found.`

### Test Invalid Input
1. When asked for Student ID, enter `abc` (non-numeric)
2. Expected: `Invalid Student ID. Please enter a number.`

### Test Empty Fields
1. When asked for Name/Address/Faculty/Email, press Enter without typing
2. Expected: `[Field] cannot be empty.`

---

## Key Implementation Details

### Parameterized Queries
All SQL queries use parameter binding to prevent SQL injection:
```csharp
command.Parameters.AddWithValue("@StdID", stdId);
command.Parameters.AddWithValue("@Name", name);
```

### Using Statements
All database objects are properly disposed using `using` statements:
```csharp
using (SqlConnection connection = new SqlConnection(ConnectionString)) {
	// ... code ...
}
```

### Exception Handling
- `SqlException`: Database-specific errors
- `Exception`: General application errors
- All exceptions are caught and displayed to the user

### Technologies Used
- **Language:** C# (.NET 6)
- **Database:** SQL Server
- **ADO.NET:** Microsoft.Data.SqlClient v5.1.5
- **No ORM:** Direct SQL with parameterized commands

## Complete CRUD Summary

| Operation | Method | SQL Command | Status Check |
|-----------|--------|-------------|--------------|
| **Create** | `InsertStudent()` | `INSERT INTO` | Rows affected > 0 |
| **Read** | `ViewStudents()` | `SELECT * FROM` | `SqlDataReader` loop |
| **Update** | `UpdateStudent()` | `UPDATE ... SET WHERE` | Rows affected > 0 |
| **Delete** | `DeleteStudent()` | `DELETE FROM WHERE` | Rows affected > 0 |

## Troubleshooting

**"Cannot connect to database"**
- Verify SQL Server is running
- Check server name in connection string (`.` for default instance)
- Verify database `StudentDB` exists

**"Login failed"**
- If using SQL Authentication, verify username and password
- If using Windows Auth, verify Windows credentials

**"Invalid object name 'Student'"**
- Run the SQL script again to create the table
- Verify you're connected to `StudentDB`

**"Timeout expired"**
- Increase timeout or check if SQL Server is busy
- Close other applications consuming resources

---

**College Details:**
- Name: Surab Parajuli
- Section: A, 6th-Sem
- Roll: 34
- Symbol No.: 80010139
