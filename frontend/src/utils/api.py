import requests

BASE_URL = "http://localhost:18080"

def send_equity_pricing_request(payload: dict) -> dict:
    product = payload.get("product", "").lower()
    endpoint = f"{BASE_URL}/price/equity/{product}"
    response = requests.post(endpoint, json=payload)
    response.raise_for_status()
    return response.json()

def send_ir_pricing_request(payload: dict) -> dict:
    product = payload.get("product", "").lower()
    endpoint = f"{BASE_URL}/price/ir/{product}"
    response = requests.post(endpoint, json=payload)
    response.raise_for_status()
    return response.json()

