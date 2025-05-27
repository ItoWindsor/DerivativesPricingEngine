from dash import Input, Output, State, callback_context
from dash.exceptions import PreventUpdate
from dash import html, dcc
from datetime import datetime
import plotly.graph_objects as go
import pandas as pd
import os
from io import StringIO

from src.forms.equity_forms import get_equity_form
from src.forms.interest_rate_forms import get_interest_rate_form
from src.utils.api import send_equity_pricing_request, send_ir_pricing_request
from src.utils.utils import get_list_curves

def register_callbacks(app):
    CURVE_FOLDER = "/app/data/interest_rate_curves"

    @app.callback(
        Output("date-inputs-container", "style"),
        Output("product-form", "children"),
        Output("submit-button", "style"),
        Output("curve-editor-container", "style"),
        Input("confirm-button", "n_clicks"),
        State("product-dropdown", "value"),
        State("date-format", "value"),
    )
    def show_form_elements(n_clicks, product, date_format):
        if not n_clicks:
            raise PreventUpdate

        if not product or not date_format:
            return (
                {"display": "none"},
                html.Div("Please select both a product and date format", 
                         style={"color": "red", "margin": "10px 0"}),
                {"display": "none"},
                {"display": "none"}
            )

        if product in ["CALLOPTION", "PUTOPTION"]:
            form_elements = []
            equity_form = get_equity_form(product)
            for element in equity_form:
                form_elements.append(html.Div(element, style={"margin": "10px 0"}))
            return (
                {"display": "block"},
                html.Div(form_elements),
                {"margin": "20px 0", "display": "block"},
                {"display": "none"}
            )
            
        elif product in ["BOND"]:
            form_elements = []
            ir_form = get_interest_rate_form()
            for element in ir_form:
                form_elements.append(html.Div(element, style={"margin": "10px 0"}))
            return (
                {"display": "block"},
                html.Div(form_elements),
                {"margin": "20px 0", "display": "block"},
                {"display": "block"}
            )
            
        return {"display": "none"}, html.Div("Unsupported product type"), {"display": "none"}, {"display": "none"}

    @app.callback(
        Output("curve-selector", "options"),
        Output("curve-selector", "value"),
        Input("product-dropdown", "value"),
    )
    def update_curve_selector(product):
        if product == "BOND":
            curve_files = get_list_curves(CURVE_FOLDER)
            options = [{"label": f, "value": f} for f in curve_files]
            value = curve_files[0] if curve_files else None
            return options, value
        return [], None

    @app.callback(
        Output("curve-editor", "value"),
        Input("curve-selector", "value"),
    )
    def load_curve_data(filename):
        if not filename:
            return ""
        
        try:
            df = pd.read_csv(os.path.join(CURVE_FOLDER, filename))
            if not all(col in df.columns for col in ['date', 'interest_rate']):
                return "Error: CSV must contain 'date' and 'interest_rate' columns"
            return df.to_csv(index=False)
        except Exception as e:
            return f"Error loading curve: {str(e)}"

    @app.callback(
        Output("curve-plot", "figure"),
        Input("update-curve-button", "n_clicks"),
        State("curve-editor", "value"),
        prevent_initial_call=True
    )
    def plot_curve(n_clicks, csv_text):
        if not n_clicks:
            raise PreventUpdate
            
        try:
            df = pd.read_csv(StringIO(csv_text))
            if 'date' not in df.columns or 'interest_rate' not in df.columns:
                raise ValueError("CSV must contain 'date' and 'interest_rate' columns")
                
            df["date"] = pd.to_datetime(df["date"])
            fig = go.Figure()
            fig.add_trace(go.Scatter(
                x=df["date"], 
                y=df["interest_rate"], 
                mode="lines+markers",
                line=dict(color='#1f77b4', width=2),
                marker=dict(size=8, color='#ff7f0e'),
                name="Rate"
            ))
            fig.update_layout(
                title={"text": "risk free interest rate curve", "x": 0.5},
                xaxis_title="Date",
                yaxis_title="Rate (%)",
                plot_bgcolor='rgba(240,240,240,0.8)',
                margin=dict(l=40, r=40, t=60, b=40),
                hovermode="x unified"
            )
            return fig
        except Exception as e:
            return go.Figure(layout={
                "title": f"Error: {str(e)}",
                "plot_bgcolor": "rgba(240,240,240,0.8)"
            })

    @app.callback(
        Output("product-output", "children"),
        Input("submit-button", "n_clicks"),
        State("product-dropdown", "value"),
        State("date-format", "value"),
        State("valuation-date", "value"),
        State("maturity-date", "value"),
        State("curve-selector", "value"),
        State("face-value", "value"),
        State("coupon-rate", "value"),
        State("spread", "value"),
        State("compounding-method", "value"),
        State("compounding-frequency", "value"),
        State("day-count-convention", "value"),
        State("short-schedule", "value"),
        State("curve-editor", "value"),
        State("spot", "value"),
        State("strike", "value"),
        State("volatility", "value"),
        State("rate", "value"),
        State("exercise-kind", "value"),
        State("computing-engine", "value"),
        prevent_initial_call=True
    )
    def handle_submit(n_clicks, product, date_format, valuation_date, maturity_date,
                     curve_file, face_value, coupon_rate, spread, compounding_method,
                     compounding_frequency, day_count_convention, short_schedule,
                     curve_data, spot, strike, volatility, rate, exercise_kind, computing_engine):

        if not n_clicks:
            raise PreventUpdate

        # Common validation for all products
        try:
            if date_format == "date":
                valuation_date_obj = datetime.strptime(valuation_date, "%Y-%m-%d")
                maturity_date_obj = datetime.strptime(maturity_date, "%Y-%m-%d")
                val_date_str = valuation_date_obj.strftime("%Y-%m-%d")
                mat_date_str = maturity_date_obj.strftime("%Y-%m-%d")
            else:
                valuation_date_obj = float(valuation_date)
                maturity_date_obj = float(maturity_date)
                val_date_str = str(valuation_date_obj)
                mat_date_str = str(maturity_date_obj)
        except Exception as e:
            return html.Div([
                html.P("Date parsing error:", style={"color": "red", "fontWeight": "bold"}),
                html.P(str(e), style={"color": "red"})
            ])

        if product in ["CALLOPTION", "PUTOPTION"]:
            if None in [spot, strike, volatility, rate, computing_engine, exercise_kind]:
                return html.Div([
                    html.P("Missing required fields:", style={"color": "red", "fontWeight": "bold"}),
                    html.Ul([
                        html.Li("Spot Price") if spot is None else None,
                        html.Li("Strike Price") if strike is None else None,
                        html.Li("Volatility") if volatility is None else None,
                        html.Li("Risk-Free Rate") if rate is None else None,
                        html.Li("Computing Engine") if computing_engine is None else None,
                        html.Li("Exercise Style") if exercise_kind is None else None
                    ])
                ])

            payload = {
                "product": product,
                "spot": spot,
                "strike": strike,
                "volatility": volatility,
                "rate": rate,
                "valuation_date": val_date_str,
                "maturity": mat_date_str,
                "engine": computing_engine,
                "exercise": exercise_kind,
                "date_format": date_format
            }
            
            try:
                result = send_equity_pricing_request(payload)
                return html.Div([
                    html.H4("Option Pricing Result", style={"marginBottom": "10px"}),
                    html.P(f"Product: {product}"),
                    html.P(f"Price: {result['price']:.4f}", style={
                        "fontSize": "18px",
                        "fontWeight": "bold",
                        "color": "green"
                    }),
                    html.P(f"Calculation time: {result.get('calculation_time', 'N/A')}s")
                ])
            except Exception as e:
                return html.Div([
                    html.P("Calculation error:", style={"color": "red", "fontWeight": "bold"}),
                    html.P(str(e), style={"color": "red"})
                ])

        elif product in ["BOND"]:
            # Save the edited curve data if modified
            curve_saved_msg = None
            if curve_file and curve_data:
                try:
                    curve_path = os.path.join(CURVE_FOLDER, curve_file)
                    with open(curve_path, 'w') as f:
                        f.write(curve_data)
                    curve_saved_msg = html.P(
                        f"Curve '{curve_file}' successfully updated",
                        style={"color": "green", "fontSize": "12px", "marginTop": "10px"}
                    )
                except Exception as e:
                    curve_saved_msg = html.P(
                        f"Error saving curve: {str(e)}",
                        style={"color": "red", "fontSize": "12px", "marginTop": "10px"}
                    )

            if None in [face_value, coupon_rate, spread, compounding_method,
                       compounding_frequency, day_count_convention, short_schedule,
                       computing_engine, curve_file]:
                return html.Div([
                    html.P("Missing required fields:", style={"color": "red", "fontWeight": "bold"}),
                    html.Ul([
                        html.Li("Face Value") if face_value is None else None,
                        html.Li("Coupon Rate") if coupon_rate is None else None,
                        html.Li("Spread") if spread is None else None,
                        html.Li("Compounding Method") if compounding_method is None else None,
                        html.Li("Compounding Frequency") if compounding_frequency is None else None,
                        html.Li("Day Count Convention") if day_count_convention is None else None,
                        html.Li("Short Schedule") if short_schedule is None else None,
                        html.Li("Computing Engine") if computing_engine is None else None,
                        html.Li("Curve File") if curve_file is None else None
                    ])
                ])

            payload = {
                "product": product,
                "face_value": face_value,
                "coupon_rate": coupon_rate,
                "spread": spread,
                "valuation_date": val_date_str,
                "maturity": mat_date_str,
                "curve_file": curve_file,
                "compounding_method": compounding_method,
                "compounding_frequency": compounding_frequency,
                "day_count": day_count_convention,
                "short_schedule": short_schedule,
                "engine": computing_engine,
                "date_format": date_format
            }
            
            try:
                result = send_ir_pricing_request(payload)
                return html.Div([
                    html.H4("Bond Pricing Result", style={"marginBottom": "10px"}),
                    html.P(f"Face Value: {face_value}"),
                    html.P(f"Coupon Rate: {coupon_rate}%"),
                    html.P(f"Price: {result['price']:.4f}", style={
                        "fontSize": "18px",
                        "fontWeight": "bold",
                        "color": "green"
                    }),
                    html.P(f"Yield: {result.get('yield', 'N/A')}%"),
                    curve_saved_msg
                ])
            except Exception as e:
                return html.Div([
                    html.P("Calculation error:", style={"color": "red", "fontWeight": "bold"}),
                    html.P(str(e), style={"color": "red"}),
                    curve_saved_msg
                ])

        return html.Div("Unsupported product type", style={"color": "red"})
