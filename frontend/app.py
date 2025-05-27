from dash import Dash, html, dcc
from dash.dependencies import Input, Output, State
from src.callbacks import register_callbacks

app = Dash(__name__)
app.title = "Derivatives Pricing Engine"
server = app.server

app.layout = html.Div([
    
    html.Div([
        # Left panel  
        html.Div([
            html.H2("Product Selection", style={"marginBottom": "20px"}),
            
            # Product type selection
            html.Div([
                html.Label("Product Type:", style={"fontWeight": "bold", "marginBottom": "5px"}),
                dcc.Dropdown(
                    id="product-dropdown",
                    options=[
                        {"label": "Call Option", "value": "CALLOPTION"},
                        {"label": "Put Option", "value": "PUTOPTION"},
                        {"label": "Bond", "value": "BOND"},
                    ],
                    placeholder="Select a product",
                    style={"marginBottom": "20px", "width": "100%"}
                ),
                
                # Date format selection
                html.Label("Date Format:", style={"fontWeight": "bold", "marginBottom": "5px"}),
                dcc.Dropdown(
                    id="date-format",
                    options=[
                        {"label": "Date (YYYY-MM-DD)", "value": "date"},
                        {"label": "Year Fraction", "value": "year_frac"}
                    ],
                    value="date",
                    style={"marginBottom": "20px", "width": "100%"}
                ),
                
                # OK button
                html.Button("OK", id="confirm-button", n_clicks=0,
                          style={"marginBottom": "20px", "width": "100px", "height": "35px"}),
                
                # Date inputs (appear after OK)
                html.Div(id="date-inputs-container", style={"display": "none"}, children=[
                    html.Label("Valuation Date:", style={"fontWeight": "bold", "marginBottom": "5px"}),
                    dcc.Input(id="valuation-date", type="text", placeholder="2025-01-01",
                            style={"marginBottom": "15px", "width": "100%", "height": "35px"}),
                    
                    html.Label("Maturity Date:", style={"fontWeight": "bold", "marginBottom": "5px"}),
                    dcc.Input(id="maturity-date", type="text", placeholder="2026-01-01",
                            style={"marginBottom": "20px", "width": "100%", "height": "35px"}),
                ]),
                
                # Product form (appears after OK)
                html.Div(id="product-form"),
                
                # Calculate Price button
                html.Button("Calculate Price", id="submit-button", n_clicks=0, 
                          style={"marginTop": "20px", "display": "none", "width": "150px", "height": "40px"})
            ], style={"padding": "15px", "border": "1px solid #ddd", "borderRadius": "5px"})
        ], style={"width": "40%", "padding": "20px"}),

        # Right panel
        html.Div([
            html.Div(id="curve-editor-container", style={"display": "none"}, children=[
                html.H2("Interest Rate Curve", style={"marginBottom": "20px"}),
                html.Div([
                    html.Label("Select Curve:", style={"fontWeight": "bold", "marginBottom": "5px"}),
                    dcc.Dropdown(id="curve-selector", style={"marginBottom": "20px", "width": "100%"}),
                    
                    html.Label("Edit Curve Data (CSV):", style={"fontWeight": "bold", "marginBottom": "5px"}),
                    dcc.Textarea(id="curve-editor", 
                                style={"width": "100%", "height": "200px", "marginBottom": "15px",
                                      "fontFamily": "monospace"}),
                    
                    html.Button("Update Curve", id="update-curve-button", n_clicks=0, 
                              style={"marginBottom": "20px", "width": "150px", "height": "35px"}),
                    
                    dcc.Graph(id="curve-plot", style={"height": "400px"})
                ], style={"padding": "15px", "border": "1px solid #ddd", "borderRadius": "5px"})
            ])
        ], style={"width": "60%", "padding": "20px"})
    ], style={"display": "flex", "flexDirection": "row"}),

    # Output section
    html.Div(id="product-output", style={"width": "100%", "padding": "20px", "marginTop": "20px",
                                       "borderTop": "1px solid #eee"})
])

register_callbacks(app)

if __name__ == '__main__':
    app.run_server(debug=True, host="0.0.0.0", port=8050)
