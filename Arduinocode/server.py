from flask import Flask, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/')
def home():
    return "Server is running!"

@app.route('/status')
def status():
    return jsonify({"entry_gate": "Closed", "exit_gate": "Closed", "slot1": "Occupied", "slot2": "Occupied", "slot3": "Occupied"})

if __name__ == '_main_':
    app.run(debug=True)