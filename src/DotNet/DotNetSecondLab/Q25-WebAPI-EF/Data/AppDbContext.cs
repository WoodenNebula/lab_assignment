using Microsoft.EntityFrameworkCore;
using Q25_WebAPI_EF.Models;

namespace Q25_WebAPI_EF.Data;

public partial class AppDbContext : DbContext {
    public AppDbContext() {
    }

    public AppDbContext(DbContextOptions<AppDbContext> options)
        : base(options) {
    }

    public virtual DbSet<Student> Students { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder) {
        modelBuilder.Entity<Student>(entity => {
            entity.Property(e => e.Course).HasMaxLength(100);
            entity.Property(e => e.Email).HasMaxLength(100);
            entity.Property(e => e.FirstName).HasMaxLength(50);
            entity.Property(e => e.Gpa)
                .HasColumnType("decimal(3, 2)")
                .HasColumnName("GPA");
            entity.Property(e => e.LastName).HasMaxLength(50);
            entity.Property(e => e.Phone).HasMaxLength(20);
            entity.Property(e => e.Remarks).HasMaxLength(500);
        });

        OnModelCreatingPartial(modelBuilder);
    }

    partial void OnModelCreatingPartial(ModelBuilder modelBuilder);
}
