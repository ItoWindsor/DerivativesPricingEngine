from dash import Dash, html, dcc, Input, Output, State, ctx
import requests
import pandas as pd

import utils
from callbacks import register_callbacks

# Initialize the Dash app
app = Dash(__name__)
app.title = "Derivatives Pricing Dashboard"

CURVE_FOLDER = "backend/data/interest_rate_curves"


# Define layout
app.layout = html.Div([
    html.H1("Derivatives Pricing Engine", style={"textAlign": "center"}),

    html.Div([
        html.Label("Spot Price:"),
        dcc.Input(id="spot", type="number", placeholder="e.g. 100", debounce=True),

        html.Label("Strike Price:"),
        dcc.Input(id="strike", type="number", placeholder="e.g. 105", debounce=True),

        html.Label("Maturity (in years):"),
        dcc.Input(id="maturity", type="number", placeholder="e.g. 1.0", debounce=True),

        html.Label("Volatility (e.g. 0.2 = 20%):"),
        dcc.Input(id="volatility", type="number", step=0.01, placeholder="e.g. 0.2", debounce=True),

        html.Label("Risk-Free Rate (e.g. 0.05 = 5%):"),
        dcc.Input(id="rate", type="number", step=0.01, placeholder="e.g. 0.05", debounce=True),

        html.Br(),
        html.Button("Price Option", id="submit-button", n_clicks=0)
    ], style={"display": "grid", "gridTemplateColumns": "1fr 1fr", "gap": "1rem", "maxWidth": "600px", "margin": "auto"}),

    html.Hr(),
    html.Div(id="output-container", style={"textAlign": "center", "fontSize": "24px"}),

    html.Hr(),
    html.Div([
        html.H2("Interest Rate Curves"),
        html.Label("Choose Curve File:"),
        dcc.Dropdown(id="curve-selector", options=[{"label": f, "value": f} for f in utils.get_list_curves(CURVE_FOLDER)], value=utils.get_list_curves(CURVE_FOLDER)[0]),

        html.Label("Edit Data Points (CSV format):"),
        dcc.Textarea(id="curve-editor", style={"width": "100%", "height": 200}),

        html.Button("Update Curve", id="update-curve-button", n_clicks=0),
        dcc.Graph(id="curve-plot")
    ], style={"maxWidth": "800px", "margin": "auto"})
])

register_callbacks(app, CURVE_FOLDER)

if __name__ == "__main__":
    app.run_server(debug=True)
