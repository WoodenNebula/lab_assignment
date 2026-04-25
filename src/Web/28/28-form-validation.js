function setError(field, message) {
  document.getElementById(`${field}Err`).textContent = message;
}

function clearErrors() {
  ["firstName", "lastName", "dob", "gender", "province", "district", "phone", "email", "username", "password", "confirmPassword"].forEach((field) => setError(field, ""));
}

function validateForm() {
  clearErrors();
  let isValid = true;

  const firstName = document.getElementById("firstName").value.trim();
  const lastName = document.getElementById("lastName").value.trim();
  const dob = document.getElementById("dob").value;
  const gender = document.getElementById("gender").value;
  const province = document.getElementById("province").value;
  const district = document.getElementById("district").value;
  const phone = document.getElementById("phone").value.trim();
  const email = document.getElementById("email").value.trim();
  const username = document.getElementById("username").value.trim();
  const password = document.getElementById("password").value;
  const confirmPassword = document.getElementById("confirmPassword").value;

  const alphaRegex = /^[A-Za-z]{1,50}$/;
  if (!firstName) {
    setError("firstName", "First name is required.");
    isValid = false;
  } else if (!alphaRegex.test(firstName)) {
    setError("firstName", "Alphabets only.");
    isValid = false;
  }

  if (lastName && !alphaRegex.test(lastName)) {
    setError("lastName", "Alphabets only.");
    isValid = false;
  }

  if (!dob) {
    setError("dob", "Date of birth is required.");
    isValid = false;
  }

  if (!gender) {
    setError("gender", "Gender is required.");
    isValid = false;
  }

  if (!province) {
    setError("province", "Province is required.");
    isValid = false;
  }

  if (!district) {
    setError("district", "District is required.");
    isValid = false;
  }

  if (!/^[0-9]{10}$/.test(phone)) {
    setError("phone", "Required. Must contain exactly 10 digits.");
    isValid = false;
  }

  if (email && !/^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$/.test(email)) {
    setError("email", "must be a valid email: example@example.com");
    isValid = false;
  }

  if (!/^[A-Za-z][A-Za-z0-9@_]{5,14}$/.test(username)) {
    setError("username", "Start with alphabet, only @ and _ special chars, length 6-15.");
    isValid = false;
  }

  if (!/^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^A-Za-z0-9]).{8,20}$/.test(password)) {
    setError("password", "8-20 chars with UPPERCASE, lowercase, digit and special character(! @ # $).");
    isValid = false;
  }

  if (confirmPassword !== password || !confirmPassword) {
    setError("confirmPassword", "Password doesn't match.");
    isValid = false;
  }

  return isValid;
}

const provinceDistricts = {
  Koshi: ["Bhojpur", "Dhankuta", "Ilam", "Jhapa", "Khotang", "Morang", "Okhaldhunga", "Panchthar", "Sankhuwasabha", "Solukhumbu", "Sunsari", "Taplejung", "Terhathum", "Udayapur"],
  Madhesh: ["Bara", "Dhanusha", "Mahottari", "Parsa", "Rautahat", "Saptari", "Sarlahi", "Siraha"],
  Bagmati: ["Bhaktapur", "Chitwan", "Dhading", "Dolakha", "Kathmandu", "Kavrepalanchok", "Lalitpur", "Makwanpur", "Nuwakot", "Ramechhap", "Rasuwa", "Sindhuli", "Sindhupalchok"],
  Gandaki: ["Baglung", "Gorkha", "Kaski", "Lamjung", "Manang", "Mustang", "Myagdi", "Nawalpur", "Parbat", "Syangja", "Tanahun"],
  Lumbini: ["Arghakhanchi", "Banke", "Bardiya", "Dang", "Eastern Rukum", "Gulmi", "Kapilvastu", "Parasi", "Palpa", "Pyuthan", "Rolpa", "Rupandehi"],
  Karnali: ["Dailekh", "Dolpa", "Humla", "Jajarkot", "Jumla", "Kalikot", "Mugu", "Salyan", "Surkhet", "Western Rukum"],
  Sudurpashchim: ["Achham", "Baitadi", "Bajhang", "Bajura", "Dadeldhura", "Darchula", "Doti", "Kailali", "Kanchanpur"],
};

function registerEventListeners() {
  const districtSelect = document.getElementById("district");
  const provinceSelect = document.getElementById("province");

  // Change district options based on selected province
  provinceSelect.addEventListener("change", () => {
    const selected = provinceSelect.value;
    districtSelect.innerHTML = '<option value="">Select district</option>';
    if (!selected) return;

    provinceDistricts[selected].forEach((district) => {
      const option = document.createElement("option");
      option.value = district;
      option.textContent = district;
      districtSelect.appendChild(option);
    });
  });

  // Highlight form on button click
  document.getElementById("formsBtn").addEventListener("click", () => {
    forms = document.forms;
    forms[0].style.backgroundColor = "#f3f3f3";
  });

  // Handle form submission
  const registrationForm = document.forms[0];
  registrationForm.addEventListener("submit", (event) => {
    event.preventDefault();
    const msg = document.getElementById("submitMessage");
    if (validateForm()) {
      msg.innerHTML = '<span class="good">Registration successful. All validation rules passed.</span>';
    } else {
      msg.innerHTML = '<span class="bad">Form has validation errors. Please fix highlighted fields.</span>';
    }
  });
}

function initializeForm() {
  document.getElementById("firstName").setAttribute("placeholder", "John");
  document.getElementById("lastName").setAttribute("placeholder", "Doe");
  document.getElementById("phone").setAttribute("placeholder", "1234567890");
  document.getElementById("email").setAttribute("placeholder", "example@example.com");
  document.getElementById("username").setAttribute("placeholder", "john_doe");

  const provinceSelect = document.getElementById("province");

  // initialize province options
  Object.keys(provinceDistricts).forEach((province) => {
    const option = document.createElement("option");
    option.value = province;
    option.textContent = province;
    provinceSelect.appendChild(option);
  });
}

initializeForm();
registerEventListeners();
