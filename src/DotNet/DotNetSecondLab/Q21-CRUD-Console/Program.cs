using Microsoft.Data.SqlClient;

class Program {
    private const string ConnectionString = """Data Source=(localdb)\MSSQLLocalDB; Integrated Security=True; Persist Security Info=False; Pooling=False; MultipleActiveResultSets=False; Encrypt=True; TrustServerCertificate=False; Application Name="SQL Server Management Studio"; Command Timeout=0; """;

    static void Main() {
        DisplayHeader();

        // Initialize database and table before running the application
        InitializeDatabase();

        while (true) {
            DisplayMenu();
            string choice = Console.ReadLine() ?? "";

            switch (choice) {
                case "1":
                    InsertStudent();
                    break;
                case "2":
                    ViewStudents();
                    break;
                case "3":
                    UpdateStudent();
                    break;
                case "4":
                    DeleteStudent();
                    break;
                case "5":
                    Console.WriteLine("\nExiting application...");
                    DisplayFooter();
                    return;
                default:
                    Console.WriteLine("Invalid choice. Please try again.");
                    break;
            }
        }
    }

    static void InitializeDatabase() {
        try {
            // Connect to StudentDB and create the Student table if it doesn't exist
            using (SqlConnection connection = new SqlConnection(ConnectionString)) {
                connection.Open();

                // Check if Student table exists
                string checkTableQuery = "SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'Student'";
                using (SqlCommand checkCommand = new SqlCommand(checkTableQuery, connection)) {
                    int tableCount = (int)checkCommand.ExecuteScalar();

                    if (tableCount == 0) {
                        Console.WriteLine("Creating Student table...");
                        string createTableQuery = @"
                            CREATE TABLE Student (
                                StdID INT PRIMARY KEY,
                                Name NVARCHAR(100) NOT NULL,
                                Address NVARCHAR(255) NOT NULL,
                                Faculty NVARCHAR(100) NOT NULL,
                                Email NVARCHAR(100) NOT NULL
                            )";

                        using (SqlCommand createCommand = new SqlCommand(createTableQuery, connection)) {
                            createCommand.ExecuteNonQuery();
                            Console.WriteLine("Student table created successfully.");
                        }
                    }
                    else {
                        Console.WriteLine("Student table already exists.");
                    }
                }
            }

            Console.WriteLine("Database initialization completed.\n");
        }
        catch (SqlException ex) {
            Console.WriteLine($"Error during database initialization: {ex.Message}");
            Console.WriteLine("Make sure SQL Server is running and accessible.");
        }
        catch (Exception ex) {
            Console.WriteLine($"Unexpected error during initialization: {ex.Message}");
        }
    }

    static void DisplayHeader() {
        Console.Clear();
        Console.WriteLine("=== STUDENT DATABASE CRUD OPERATIONS ===");
        Console.WriteLine();
    }

    static void DisplayMenu() {
        Console.WriteLine("\n--- Main Menu ---");
        Console.WriteLine("1. Insert Student");
        Console.WriteLine("2. View All Students");
        Console.WriteLine("3. Update Student");
        Console.WriteLine("4. Delete Student");
        Console.WriteLine("5. Exit");
        Console.Write("\nEnter your choice (1-5): ");
    }

    static void InsertStudent() {
        try {
            Console.Write("Enter Student ID: ");
            if (!int.TryParse(Console.ReadLine(), out int stdId)) {
                Console.WriteLine("Invalid Student ID. Please enter a number.");
                return;
            }

            Console.Write("Enter Name: ");
            string name = Console.ReadLine() ?? "";
            if (string.IsNullOrWhiteSpace(name)) {
                Console.WriteLine("Name cannot be empty.");
                return;
            }

            Console.Write("Enter Address: ");
            string address = Console.ReadLine() ?? "";
            if (string.IsNullOrWhiteSpace(address)) {
                Console.WriteLine("Address cannot be empty.");
                return;
            }

            Console.Write("Enter Faculty: ");
            string faculty = Console.ReadLine() ?? "";
            if (string.IsNullOrWhiteSpace(faculty)) {
                Console.WriteLine("Faculty cannot be empty.");
                return;
            }

            Console.Write("Enter Email: ");
            string email = Console.ReadLine() ?? "";
            if (string.IsNullOrWhiteSpace(email)) {
                Console.WriteLine("Email cannot be empty.");
                return;
            }

            using (SqlConnection connection = new SqlConnection(ConnectionString)) {
                connection.Open();

                string query = "INSERT INTO Student (StdID, Name, Address, Faculty, Email) VALUES (@StdID, @Name, @Address, @Faculty, @Email)";

                using (SqlCommand command = new SqlCommand(query, connection)) {
                    command.Parameters.AddWithValue("@StdID", stdId);
                    command.Parameters.AddWithValue("@Name", name);
                    command.Parameters.AddWithValue("@Address", address);
                    command.Parameters.AddWithValue("@Faculty", faculty);
                    command.Parameters.AddWithValue("@Email", email);

                    int rowsAffected = command.ExecuteNonQuery();
                    if (rowsAffected > 0) {
                        Console.WriteLine($"\n Student inserted successfully!");
                    }
                }
            }
        }
        catch (SqlException ex) {
            Console.WriteLine($"\n Database Error: {ex.Message}");
        }
        catch (Exception ex) {
            Console.WriteLine($"\n Error: {ex.Message}");
        }
    }

    static void ViewStudents() {
        try {
            using (SqlConnection connection = new SqlConnection(ConnectionString)) {
                connection.Open();

                string query = "SELECT StdID, Name, Address, Faculty, Email FROM Student ORDER BY StdID";

                using (SqlCommand command = new SqlCommand(query, connection)) {
                    using (SqlDataReader reader = command.ExecuteReader()) {
                        Console.WriteLine("\n--- All Students ---");
                        bool hasRows = false;

                        while (reader.Read()) {
                            hasRows = true;
                            Console.WriteLine();
                            Console.WriteLine($"ID: {reader["StdID"]}");
                            Console.WriteLine($"Name: {reader["Name"]}");
                            Console.WriteLine($"Address: {reader["Address"]}");
                            Console.WriteLine($"Faculty: {reader["Faculty"]}");
                            Console.WriteLine($"Email: {reader["Email"]}");
                            Console.WriteLine(new string('-', 40));
                        }

                        if (!hasRows) {
                            Console.WriteLine("\nNo students found in the database.");
                        }
                    }
                }
            }
        }
        catch (SqlException ex) {
            Console.WriteLine($"\n Database Error: {ex.Message}");
        }
        catch (Exception ex) {
            Console.WriteLine($"\n Error: {ex.Message}");
        }
    }

    static void UpdateStudent() {
        try {
            Console.Write("Enter Student ID to update: ");
            if (!int.TryParse(Console.ReadLine(), out int stdId)) {
                Console.WriteLine("Invalid Student ID. Please enter a number.");
                return;
            }

            using (SqlConnection connection = new SqlConnection(ConnectionString)) {
                connection.Open();

                string checkQuery = "SELECT COUNT(*) FROM Student WHERE StdID = @StdID";
                using (SqlCommand checkCommand = new SqlCommand(checkQuery, connection)) {
                    checkCommand.Parameters.AddWithValue("@StdID", stdId);
                    int count = (int)checkCommand.ExecuteScalar();

                    if (count == 0) {
                        Console.WriteLine($"Student with ID {stdId} not found.");
                        return;
                    }
                }

                Console.Write("Enter new Name: ");
                string name = Console.ReadLine() ?? "";
                if (string.IsNullOrWhiteSpace(name)) {
                    Console.WriteLine("Name cannot be empty.");
                    return;
                }

                Console.Write("Enter new Address: ");
                string address = Console.ReadLine() ?? "";
                if (string.IsNullOrWhiteSpace(address)) {
                    Console.WriteLine("Address cannot be empty.");
                    return;
                }

                Console.Write("Enter new Faculty: ");
                string faculty = Console.ReadLine() ?? "";
                if (string.IsNullOrWhiteSpace(faculty)) {
                    Console.WriteLine("Faculty cannot be empty.");
                    return;
                }

                Console.Write("Enter new Email: ");
                string email = Console.ReadLine() ?? "";
                if (string.IsNullOrWhiteSpace(email)) {
                    Console.WriteLine("Email cannot be empty.");
                    return;
                }

                string updateQuery = "UPDATE Student SET Name = @Name, Address = @Address, Faculty = @Faculty, Email = @Email WHERE StdID = @StdID";

                using (SqlCommand updateCommand = new SqlCommand(updateQuery, connection)) {
                    updateCommand.Parameters.AddWithValue("@StdID", stdId);
                    updateCommand.Parameters.AddWithValue("@Name", name);
                    updateCommand.Parameters.AddWithValue("@Address", address);
                    updateCommand.Parameters.AddWithValue("@Faculty", faculty);
                    updateCommand.Parameters.AddWithValue("@Email", email);

                    int rowsAffected = updateCommand.ExecuteNonQuery();
                    if (rowsAffected > 0) {
                        Console.WriteLine($"\n Student updated successfully!");
                    }
                }
            }
        }
        catch (SqlException ex) {
            Console.WriteLine($"\n Database Error: {ex.Message}");
        }
        catch (Exception ex) {
            Console.WriteLine($"\n Error: {ex.Message}");
        }
    }

    static void DeleteStudent() {
        try {
            Console.Write("Enter Student ID to delete: ");
            if (!int.TryParse(Console.ReadLine(), out int stdId)) {
                Console.WriteLine("Invalid Student ID. Please enter a number.");
                return;
            }

            using (SqlConnection connection = new SqlConnection(ConnectionString)) {
                connection.Open();

                string checkQuery = "SELECT COUNT(*) FROM Student WHERE StdID = @StdID";
                using (SqlCommand checkCommand = new SqlCommand(checkQuery, connection)) {
                    checkCommand.Parameters.AddWithValue("@StdID", stdId);
                    int count = (int)checkCommand.ExecuteScalar();

                    if (count == 0) {
                        Console.WriteLine($"Student with ID {stdId} not found.");
                        return;
                    }
                }

                Console.Write($"Are you sure you want to delete student {stdId}? (yes/no): ");
                string confirm = Console.ReadLine()?.ToLower() ?? "";

                if (confirm != "yes") {
                    Console.WriteLine("Delete operation cancelled.");
                    return;
                }

                string deleteQuery = "DELETE FROM Student WHERE StdID = @StdID";

                using (SqlCommand deleteCommand = new SqlCommand(deleteQuery, connection)) {
                    deleteCommand.Parameters.AddWithValue("@StdID", stdId);

                    int rowsAffected = deleteCommand.ExecuteNonQuery();
                    if (rowsAffected > 0) {
                        Console.WriteLine($"\nStudent deleted successfully!");
                    }
                }
            }
        }
        catch (SqlException ex) {
            Console.WriteLine($"\nDatabase Error: {ex.Message}");
        }
        catch (Exception ex) {
            Console.WriteLine($"\nError: {ex.Message}");
        }
    }

    static void DisplayFooter() {
        Console.WriteLine();
        Console.WriteLine("====================");
        Console.WriteLine("Surab Parajuli");
        Console.WriteLine("Section: A, 6th-Sem");
        Console.WriteLine("Roll: 34, Symbol No.: 80010139");
        Console.WriteLine("====================");
    }
}

