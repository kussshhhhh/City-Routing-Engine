FROM python:3.9-slim
RUN apt-get update && apt-get install -y g++
WORKDIR /app
COPY . /app
RUN g++ -O3 engine.cpp -o engine
RUN pip install fastapi uvicorn
EXPOSE 8080
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8080"]