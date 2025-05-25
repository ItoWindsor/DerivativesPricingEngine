import os 

def get_list_curves(CURVE_FOLDER : str) -> list[str]:
    return [f for f in os.listdir(CURVE_FOLDER) if f.endswith(".csv")]
