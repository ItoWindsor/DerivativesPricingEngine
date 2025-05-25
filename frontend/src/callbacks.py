from dash import Input, Output, State, callback
from src.enums.enums_products import ImplementedInterestRatesProducts, ImplementerEquityProducts
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
            html.Div(id="base-params", children=[
                html.Label("Start Date:"),
                dcc.Input(id="start-date", type="text", placeholder="YYYY-MM-DD"),

                html.Label("Maturity Date:"),
                dcc.Input(id="maturity-date", type="text", placeholder="YYYY-MM-DD"),
            ])
        ]

        if product_name in ImplementedInterestRatesProducts.__members__:
            product = ImplementedInterestRatesProducts[product_name]
            if product == ImplementedInterestRatesProducts.BOND:
                product_specific = [
                    html.Label("Compounding Method:"),
                    dcc.Dropdown(id="compounding", options=[
                        {"label": "Annually", "value": "annually"},
                        {"label": "Semiannually", "value": "semiannually"},
                        {"label": "Continuously", "value": "continuously"},
                    ]),

                    html.Label("Coupon Rate:"),
                    dcc.Input(id="coupon-rate", type="number", step=0.01),

                    html.Label("Spread:"),
                    dcc.Input(id="spread", type="number", step=0.01),
                ]

        elif product_name in ImplementerEquityProducts.__members__:
            product = ImplementerEquityProducts[product_name]
            product_specific = [
                html.Label("Spot Price:"),
                dcc.Input(id="spot", type="number"),

                html.Label("Strike Price:"),
                dcc.Input(id="strike", type="number"),

                html.Label("Volatility:"),
                dcc.Input(id="volatility", type="number", step=0.01),

                html.Label("Risk-Free Rate:"),
                dcc.Input(id="rate", type="number", step=0.01),

                html.Label("Exercise Style:"),
                dcc.Dropdown(id="exercise-kind", options=[
                    {"label": "European", "value": "european"},
                    {"label": "American", "value": "american"},
                ]),
            ]
        else:
            product_specific = []

        return html.Div([
            html.Div([
                html.Label("Start Date:"),
                dcc.Input(id="start-date", type="text", placeholder="YYYY-MM-DD"),

                html.Label("Maturity Date:"),
                dcc.Input(id="maturity-date", type="text", placeholder="YYYY-MM-DD"),
            ] + product_specific, style={
                "display": "grid",
                "gridTemplateColumns": "1fr 1fr",
                "gap": "1rem",
                "marginTop": "1rem"
            }),

            html.Br(),
            html.Button("Price Product", id="price-button", n_clicks=0, style={"marginTop": "1rem"})
        ])
