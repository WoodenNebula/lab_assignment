using Microsoft.EntityFrameworkCore;
using Q23_MVC_EFC.Models;

namespace Q23_MVC_EFC.Data
{
    public class AppDbContext : DbContext
    {
        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options)
        {
        }

        public DbSet<Student> Students { get; set; } = null!;

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            // Configure Student entity
            modelBuilder.Entity<Student>()
                .HasKey(s => s.StudentId);

            modelBuilder.Entity<Student>()
                .Property(s => s.FirstName)
                .IsRequired()
                .HasMaxLength(50);

            modelBuilder.Entity<Student>()
                .Property(s => s.LastName)
                .IsRequired()
                .HasMaxLength(50);

            modelBuilder.Entity<Student>()
                .Property(s => s.Email)
                .IsRequired()
                .HasMaxLength(100);

            modelBuilder.Entity<Student>()
                .Property(s => s.Phone)
                .IsRequired()
                .HasMaxLength(20);

            modelBuilder.Entity<Student>()
                .Property(s => s.Course)
                .IsRequired()
                .HasMaxLength(100);

            modelBuilder.Entity<Student>()
                .Property(s => s.GPA)
                .HasPrecision(3, 2);

            modelBuilder.Entity<Student>()
                .Property(s => s.Remarks)
                .HasMaxLength(500);
        }
    }
}
