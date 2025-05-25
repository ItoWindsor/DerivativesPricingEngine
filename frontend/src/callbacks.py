from dash import Input, Output, State, callback
from requests import options
from src.enums.enums_products import ImplementedInterestRatesProducts, ImplementedEquityProducts
from dash import Dash, html, dcc, Input, Output

def register_callbacks(app, curve_folder):
    @app.callback(
        Output("product-form-container", "children"),
        Input("ok-button", "n_clicks"),
        State("product-selector", "value"),
        prevent_initial_call=True
    )
    def render_product_form(n_clicks, product_name):
        if not product_name:
            return []

        base_params = [
            html.Label("Start Date:"),
            dcc.Input(id="start-date", type="text", placeholder="YYYY-MM-DD"),

            html.Label("Valuation Date:"),
            dcc.Input(id='valuation-date', type='text', placeholder="YYYY-MM-DD"),

            html.Label("Maturity Date:"),
            dcc.Input(id="maturity-date", type="text", placeholder="YYYY-MM-DD"),

        ]

        product_specific = []

        if product_name in ImplementedInterestRatesProducts.__members__:
            product = ImplementedInterestRatesProducts[product_name]
            if product == ImplementedInterestRatesProducts.BOND:
                product_specific = [
                    html.Label("Face Value:"),
                    dcc.Input(id="face-value", type="number", value=100, step=0.01),

                    html.Label("Coupon Rate:"),
                    dcc.Input(id="coupon-rate", type="number", value=0.05, step=0.01),

                    html.Label("Spread:"),
                    dcc.Input(id="spread", type="number", value=0.01, step=0.01),

                    html.Label("Compounding Method:"),
                    dcc.Dropdown(id="compounding-method", options=[
                        {"label": "Continuous", "value": "continuous"},
                        {"label": "Actuarial", "value": "actuarial"},
                    ]),

                    html.Label("Compounding Frequency:"),
                    dcc.Dropdown(id="compounding-frequency", options=[
                        {"label": "Annually", "value": "annually"},
                        {"label": "Semi-Annually", "value": "semiannually"},
                        {"label": "Quarterly", "value": "quarterly"},
                        {"label": "Monthly", "value": "monthly"},
                    ]),
                    
                    
                    html.Label("Day Count Convention:"),
                    dcc.Dropdown(id="day-count-convention", options=[
                        {"label": "Actual", "value": "actual"},
                        {"label": "Actual/360", "value": "actual360"},
                        {"label": "Actual/365", "value": "actual365"},
                    ]),

                    html.Label("Short Schedule:"),
                    dcc.Dropdown(id="short-schedule", options=[
                        {"label": "At the beginning", "value": "shortstart"},
                        {"label": "At the end", "value": "shortend"},
                    ]),
                ]

        elif product_name in ImplementedEquityProducts.__members__:
            product = ImplementedEquityProducts[product_name]

            common_fields = [
                html.Label("Spot Price:"),
                dcc.Input(id="spot", type="number", value=100, min=0.0),

                html.Label("Strike Price:"),
                dcc.Input(id="strike", type="number", value=100, min=0.0),

                html.Label("Volatility:"),
                dcc.Input(id="volatility", type="number", value=0.2, step=0.01, min=0.0),

                html.Label("Risk-Free Rate:"),
                dcc.Input(id="rate", type="number", value=0.02, step=0.01),
            ]

            if product == ImplementedEquityProducts.EUROPEANOPTION:
                product_specific = common_fields
            else:
                product_specific = common_fields + [
                    html.Label("Exercise Style:"),
                    dcc.Dropdown(id="exercise-kind", options=[
                        {"label": "European", "value": "european"},
                        {"label": "American", "value": "american"},
                    ])
                ]

        return html.Div([
            html.Div(base_params + product_specific, style={
                "display": "grid",
                "gridTemplateColumns": "1fr 1fr",
                "gap": "1rem",
                "marginTop": "1rem"
            }),
            html.Br(),
            html.Button("Price Product", id="price-button", n_clicks=0, style={"marginTop": "1rem"})
        ])
