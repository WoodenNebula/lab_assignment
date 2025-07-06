import sys

def leak_from_bathroom(wet_state: dict[str, bool]) -> bool:
    return wet_state["hall"] and not(wet_state["kitchen"])

def problem_in_kitchen(wet_state: dict[str, bool]) -> bool:
    return wet_state["hall"] and not(wet_state["bathroom"])    

def leak_not_from_outside(inputs: dict[str, bool]) -> bool:
    return inputs["window_closed"] or not inputs["raining"]

def  determine_water_leakage_source() -> str|None:
    states: dict[str, bool] = {"hall" : False, "window_closed": False, "rain": False, "bathroom": False, "kitchen": False }
    print("Answer only in 'yes' or 'no' to the following:")

    states["hall"] = input("Is hall wet? ").lower().strip().__eq__("yes")
    states["kitchen"] = input("Is kitchen wet? ").lower().strip().__eq__("yes")
    
    states["bathroom"] = leak_from_bathroom(states)

    if states["bathroom"]: return "bathroom"

    is_problem_in_kitchen = problem_in_kitchen(states)

    if not is_problem_in_kitchen:
        return None
    
    print("---Problem is in kitchen---")
    
    states["window_closed"] = input("Is window closed? ").lower().strip().__eq__("yes")
    states["raining"] = input("Is it raining? ").lower().strip().__eq__("yes")

    is_leak_not_from_outside =leak_not_from_outside(states)
    if is_leak_not_from_outside:
        print("---Water is not form outside---")

    states["kitchen"] = is_leak_not_from_outside and is_problem_in_kitchen
    if states["kitchen"]:
        return "kitchen"
    else: return None

def __main__():
    leakage_source = determine_water_leakage_source()
    if leakage_source is not None:
        print(f"Leakage is in {leakage_source}")
    else:
        print("Unable to determine leakage source")

    print("\nSurab Parajuli\tSection: A\
            \nRoll: 34\tSymbol: 80010139")

__main__()
