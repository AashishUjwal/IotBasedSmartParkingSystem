function updateStatus() {
    fetch('/status') // Get data from server.py
        .then(response => response.json())
        .then(data => {
            document.getElementById("entry-gate").innerText = data.entry_gate;
            document.getElementById("exit-gate").innerText = data.exit_gate;
            document.getElementById("slot1").innerText = data.slot1;
            document.getElementById("slot2").innerText = data.slot2;
            document.getElementById("slot3").innerText = data.slot3;
            document.getElementById("slot4").innerText = data.slot4;
        });
}

setInterval(updateStatus, 5000); // Update every 5 seconds
window.onload = updateStatus;