import { useState } from "react";
function RegistrationForm() {
  const [formData, setFormData] = useState({
    name: "",
    email: "",
    password: "",
    phone: "",
  });

  const [errors, setErrors] = useState({});
  const [submitted, setSubmitted] = useState(false);

  function handleChange(e) {
    const { name, value } = e.target;
    setFormData({ ...formData, [name]: value });
  }

  function validate() {
    const newErrors = {};

    if (formData.name.trim() === "") {
      newErrors.name = "Name is required.";
    }

    if (formData.email.trim() === "") {
      newErrors.email = "Email is required.";
    } else if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(formData.email)) {
      newErrors.email = "Enter a valid email address.";
    }

    if (formData.password === "" || formData.password.length < 8) {
      newErrors.password = "Password is must be at least 8 characters long.";
    }

    return newErrors;
  }

  function handleSubmit(e) {
    e.preventDefault();
    const validationErrors = validate();
    setErrors(validationErrors);
    setSubmitted(Object.keys(validationErrors).length === 0);
  }

  return (
    <div class="container">
      <h1>Registration Form</h1>

      <form onSubmit={handleSubmit}>
        <label>Name:</label>
        <input type="text" name="name" value={formData.name} onChange={handleChange} required />

        <label>Email:</label>
        <input type="email" name="email" value={formData.email} onChange={handleChange} required email />

        <label>Phone Number:</label>
        <input type="tel" name="phone" value={formData.phone} onChange={handleChange} required pattern="[0-9]{10}" />

        <label>Password:</label>
        <input type="password" name="password" value={formData.password} onChange={handleChange} required minlength="8" />

        <label>Confirm Password:</label>
        <input type="password" name="confirmPassword" value={formData.confirmPassword} onChange={handleChange} required />

        <button type="submit">Register</button>
        {submitted && <p style={{ color: "green" }}>Registration successful!</p>}
      </form>
    </div>
  );
}
function Footer() {
  return (
    <footer class="border-top text-muted">
      <div>
        <p class="text-center" style={{ fontSize: "1.3em", marginTop: "1rem !important" }}>
          <strong>Q27(c): Client Side Scripting: React</strong>
        </p>
        <p class="text-center" style={{ fontSize: "1.3em" }}>
          ====================
          <br />
          Surab Parajuli
          <br />
          Section: A, 6th-Sem
          <br />
          Roll: 34, Symbol No.: 80010139
          <br />
          ====================
        </p>
      </div>
    </footer>
  );
}

export { RegistrationForm, Footer };
