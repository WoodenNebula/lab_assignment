import { Component, signal } from '@angular/core';
import { FormsModule } from '@angular/forms';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [FormsModule],
  styleUrl: './app.css',
  templateUrl: './app.html',
})
export class App {
  protected readonly title = signal('Q27b-angular');
  submitted = false;

  name = '';
  email = '';
  password = '';
  confirmPassword = '';
  phoneNumber = '';

  onSubmit() {
    this.submitted = true;

    if (
      this.name &&
      this.email &&
      this.password &&
      this.password === this.confirmPassword &&
      this.phoneNumber
    ) {
      alert('Registration successful!');
    }
  }
}
