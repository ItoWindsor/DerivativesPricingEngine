from dash import Dash, html, dcc, Input, Output
import dash
import requests
import pandas as pd

from src import utils
from src.enums.enums_products import ImplementedInterestRatesProducts, ImplementerEquityProducts
from src.callbacks import register_callbacks

app = Dash(__name__)
app.title = "Derivatives Pricing Dashboard"

CURVE_FOLDER = "backend/data/interest_rate_curves"

# Generate dropdown options from Enums
interest_rate_options = [{"label": prod.name.title(), "value": prod.name} for prod in ImplementedInterestRatesProducts]
equity_options = [{"label": prod.name.title(), "value": prod.name} for prod in ImplementerEquityProducts]
all_product_options = interest_rate_options + equity_options

# Define layout
app.layout = html.Div([
    html.H1("Derivatives Pricing Engine", style={"textAlign": "left"}),

    html.Div([
        html.Div([
            html.Label("Select Product:"),
            html.Div([
                dcc.Dropdown(
                    id="product-selector",
                    options=all_product_options,
                    placeholder="Choose a product",
                    style={"width": "300px"}
                ),
                html.Button("OK", id="ok-button", n_clicks=0, style={"height": "40px"})
            ], style={"display": "flex", "alignItems": "center", "gap": "1rem"})
        ])
    ], style={"maxWidth": "600px", "margin": "auto"}),

    html.Br(),
    html.Div(id="product-form-container", style={"maxWidth": "600px", "margin": "auto"}),

    html.Hr(),
    html.Div(id="price-output", style={"textAlign": "center", "fontSize": "24px"}),

    html.Hr(),
    html.Div([
        html.H2("Interest Rate Curves"),
        html.Label("Choose Curve File:"),
        dcc.Dropdown(
            id="curve-selector",
            options=[{"label": f, "value": f} for f in utils.get_list_curves(CURVE_FOLDER)],
            value=utils.get_list_curves(CURVE_FOLDER)[0]
        ),

        html.Label("Edit Data Points (CSV format):"),
        dcc.Textarea(id="curve-editor", style={"width": "100%", "height": 200}),

        html.Button("Update Curve", id="update-curve-button", n_clicks=0),
        dcc.Graph(id="curve-plot")
    ], style={"maxWidth": "800px", "margin": "auto"})
])

register_callbacks(app, CURVE_FOLDER)

if __name__ == "__main__":
    app.run_server(debug=True)
