from flask import Flask, request

app = Flask(__name__)

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        data = request.data
        print("Received from STM32:", data)
        return "OK", 200
    else:
        return "STM32 Test Server", 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
