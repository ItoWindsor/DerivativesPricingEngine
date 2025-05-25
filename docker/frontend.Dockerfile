FROM python:3.11-slim

# Set workdir
WORKDIR /app

# Install dependencies
COPY frontend/requirements.txt .
RUN python -m venv /opt/venv && \
    . /opt/venv/bin/activate && \
    pip install --upgrade pip && \
    pip install -r requirements.txt

# Copy app code
COPY frontend/ /app/

# Activate virtualenv and run the app
ENV PATH="/opt/venv/bin:$PATH"

EXPOSE 8050
CMD ["python", "app.py"]
