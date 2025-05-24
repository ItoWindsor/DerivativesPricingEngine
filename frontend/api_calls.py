import requests

@app.callback(
    Output("output-container", "children"),
    Input("submit-button", "n_clicks"),
    State("spot", "value"),
    State("strike", "value"),
    State("maturity", "value"),
    State("volatility", "value"),
    State("rate", "value")
)
def update_price(n_clicks : int, spot : float, strike : float, maturity : float, volatility : float, rate : float):
    if not n_clicks:
        return ""
    if None in (spot, strike, maturity, volatility, rate):
        return "Please fill in all fields."

    try:
        response = requests.post("http://localhost:18080/price/call_option", json={
            "spot": spot,
            "strike": strike,
            "maturity": maturity,
            "volatility": volatility,
            "rate": rate
        })
        response.raise_for_status()
        result = response.json()
        return f"Call Option Price: {result.get('price', 'N/A')}"
    except requests.exceptions.RequestException as e:
        return f"API Error: {e}"
