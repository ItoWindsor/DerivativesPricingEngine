from dash import html, dcc

def get_interest_rate_form():
    return [
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

