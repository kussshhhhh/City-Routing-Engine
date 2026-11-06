from fastapi import FastAPI, HTTPException
from fastapi.responses import HTMLResponse
import subprocess
import os

app = FastAPI(title="City Routing Engine API")

# Dynamically choose the executable based on the operating system
ENGINE_PATH = "./engine.exe" if os.name == "nt" else "./engine"

@app.get("/")
async def serve_frontend():
    html_content = """
    <html>
        <body style="font-family: Arial; padding: 20px;">
            <h2>City Routing Engine</h2>
            <p>Calculate the optimal path using C++ Dijkstra's Algorithm.</p>
            <button onclick="getRoute()" style="padding: 10px; background: #007bff; color: white; border: none; border-radius: 5px; cursor: pointer;">Find Route (1 to 5)</button>
            <pre id="output" style="background: #f4f4f4; padding: 15px; margin-top: 20px; border-radius: 5px;"></pre>
            <script>
                async function getRoute() {
                    document.getElementById('output').innerText = "Calculating in C++...";
                    const response = await fetch('/api/route?source=1&destination=5');
                    const data = await response.json();
                    document.getElementById('output').innerText = JSON.stringify(data, null, 2);
                }
            </script>
        </body>
    </html>
    """
    return HTMLResponse(content=html_content)

@app.get("/api/route")
async def get_optimal_route(source: int, destination: int):
    try:
        result = subprocess.run(
            [ENGINE_PATH, str(source), str(destination)],
            capture_output=True,
            text=True,
            check=True
        )
        
        output = result.stdout.strip().split()
        total_distance = float(output[0])
        path = [int(node) for node in output[1:]]

        return {
            "status": "success",
            "data": {
                "source": source,
                "destination": destination,
                "total_distance": total_distance,
                "path": path
            }
        }
    except subprocess.CalledProcessError as e:
        error_msg = e.stderr.strip() if e.stderr else "Route calculation failed."
        raise HTTPException(status_code=404, detail=error_msg)
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))