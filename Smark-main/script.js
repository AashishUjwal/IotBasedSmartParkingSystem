let vehicleFare = {
    'Car': 200, // ₹200
    'Bike': 100, // ₹100
    'Auto-Rikshaw': 150, // ₹150
    'Truck': 300 // ₹300
};

let selectedVehicle = null;
let timers = {}; // Store countdown timers for each slot

function updateFare(vehicle) {
    selectedVehicle = vehicle;
    const fare = vehicleFare[vehicle];
    document.querySelectorAll('.slot-fare').forEach(fareElement => {
        fareElement.textContent = `${fare}`; // Display fare in INR
    });
}

function parkVehicle(slotId) {
    const slot = document.getElementById(slotId);
    const name = slot.querySelector('.customer-name').value;
    const time = parseInt(slot.querySelector('.parking-time').value);
    const mobile = slot.querySelector('.customer-mobile').value;

    if (!name || !time || !mobile) {
        alert('Please enter your name, parking time, and mobile number');
        return;
    }

    if (mobile.length !== 10) {
        alert('Please enter a valid 10-digit mobile number');
        return;
    }

    slot.querySelector('.status').textContent = `${selectedVehicle} parked`;
    const totalFare = vehicleFare[selectedVehicle] * time;
    slot.querySelector('.fare-info').textContent = `Fare: ₹${totalFare} for ${time} hours`; // Display fare in INR
    slot.querySelector('.park-btn').textContent = "Parked";
    slot.querySelector('.park-btn').disabled = true;

    // Start countdown timer
    let timeRemaining = time * 60; // Convert hours to minutes
    const countdownElement = slot.querySelector('.countdown');

    function updateCountdown() {
        const hours = Math.floor(timeRemaining / 60);
        const minutes = timeRemaining % 60;
        countdownElement.textContent = `Time left: ${hours}h ${minutes}m`;

        if (timeRemaining <= 0) {
            clearInterval(timers[slotId]);
            alert(`Your parking time has expired! Please vacate the slot. Contact: ${mobile}`);
            countdownElement.textContent = "Time's up!";
        }

        timeRemaining--;
    }

    timers[slotId] = setInterval(updateCountdown, 60000); // Update every minute
    updateCountdown(); // Initial countdown display
}

function showSlotStatus() {
    const slots = document.querySelectorAll('.slot');
    let available = 0;
    slots.forEach(slot => {
        if (slot.querySelector('.status').textContent === 'Available') {
            available++;
        }
    });
    alert(`There are ${available} available parking slots`);
}

function showModal(modalId) {
    document.getElementById(modalId).style.display = 'block';
}

function closeModal(modalId) {
    document.getElementById(modalId).style.display = 'none';
}

function showOtpSection() {
    document.getElementById('login-credentials').style.display = 'none';
    document.getElementById('otp-section').style.display = 'block';
}

function sendOtp() {
    const mobile = document.getElementById('login-mobile').value;
    if (mobile) {
        alert('OTP sent to ' + mobile);
    } else {
        alert('Please enter a valid mobile number');
    }
}

window.onclick = function (event) {
    const signupModal = document.getElementById('signup-modal');
    const loginModal = document.getElementById('login-modal');
    if (event.target === signupModal) {
        signupModal.style.display = 'none';
    } else if (event.target === loginModal) {
        loginModal.style.display = 'none';
    }
};

function showSignUpModal() {
    showModal('signup-modal');
}

function showLoginModal() {
    showModal('login-modal');
}
