from dash import html, dcc

def get_equity_form(product_name):
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

    additional = []
    if product_name in ["CALLOPTION", "PUTOPTION"]:
        additional = [
            html.Label("Exercise Style:"),
            dcc.Dropdown(id="exercise-kind", options=[
                {"label": "European", "value": "european"},
                {"label": "American", "value": "american"},
            ]),

            html.Label("How to compute:"),
            dcc.Dropdown(id="computing-engine", options=[
                {"label": "Analytical Formulas", "value": "analytical"},
                {"label": "Monte Carlo", "value": "monte_carlo"},
                {"label": "Binomial Tree", "value": "binomial_tree"},
                {"label": "Finite Difference", "value": "finite_difference"},
            ])
        ]
    else:
        additional = [
            html.Label("How to compute:"),
            dcc.Dropdown(id="computing-engine", options=[
                {"label": "Monte Carlo", "value": "monte_carlo"},
                {"label": "Binomial Tree", "value": "binomial_tree"},
                {"label": "Finite Difference", "value": "finite_difference"},
            ])
        ]

    return common_fields + additional
