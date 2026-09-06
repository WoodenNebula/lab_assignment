// Please see documentation at https://learn.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.
$(document).ready(function () {
    $("#registerForm").on("submit", function (e) {
        let isValid = true;

        // clear previous messages
        $("#nameError, #emailError, #passwordError, #phoneError").text("");

        if ($("#Name").val().trim() === "") {
            $("#nameError").text("Name is required.");
            isValid = false;
        }

        let email = $("#Email").val().trim();
        let emailPattern = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        if (email === "") {
            $("#emailError").text("Email is required.");
            isValid = false;
        } else if (!emailPattern.test(email)) {
            $("#emailError").text("Enter a valid email address.");
            isValid = false;
        }

        if ($("#Password").val() === "") {
            $("#passwordError").text("Password is required.");
            isValid = false;
        }

        let phone = $("#Phone").val().trim();
        if (phone === "") {
            $("#phoneError").text("Phone number is required.");
            isValid = false;
        } else if (!/^\d{10}$/.test(phone)) {
            $("#phoneError").text("Phone number must be exactly 10 digits.");
            isValid = false;
        }

        if (!isValid) {
            // stop form submission
            e.preventDefault();
        }
    });
});

