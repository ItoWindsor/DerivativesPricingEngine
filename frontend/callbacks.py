from dash import Input, Output, State
import pandas as pd
import plotly.graph_objs as go
import requests
import os
from io import StringIO

def register_callbacks(app, curve_folder):

    @app.callback(
        Output("output-container", "children"),
        Input("submit-button", "n_clicks"),
        State("spot", "value"),
        State("strike", "value"),
        State("maturity", "value"),
        State("volatility", "value"),
        State("rate", "value"),
    )
    def update_price(n_clicks, spot, strike, maturity, volatility, rate):
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
                "rate": rate,
            })
            response.raise_for_status()
            result = response.json()
            return f"Call Option Price: {result.get('price', 'N/A')}"
        except requests.exceptions.RequestException as e:
            return f"API Error: {e}"

    @app.callback(
        Output("curve-editor", "value"),
        Input("curve-selector", "value"),
    )
    def load_curve_data(filename):
        df = pd.read_csv(os.path.join(curve_folder, filename))
        return df.to_csv(index=False)

    @app.callback(
        Output("curve-plot", "figure"),
        Input("update-curve-button", "n_clicks"),
        State("curve-editor", "value"),
    )
    def plot_curve(_, csv_text):
        try:
            df = pd.read_csv(StringIO(csv_text))
            df["date"] = pd.to_datetime(df["date"])
            fig = go.Figure()
            fig.add_trace(go.Scatter(x=df["date"], y=df["interest_rate"], mode="lines+markers", name="Rate"))
            fig.update_layout(title="Interest Rate Curve", xaxis_title="Date", yaxis_title="Rate")
            return fig
        except Exception as e:
            return go.Figure(layout={"title": f"Error: {e}"})
